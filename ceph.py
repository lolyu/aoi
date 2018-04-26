# sample code to start ceph service
# lolyu@redhat.com
from __future__ import print_function
import docker
import sys
import os
import socket
import re
import subprocess
import shlex
import time

DEVNULL = open(os.devnull, 'rw')

image_name = "ceph/daemon"
tag_name = "latest-jewel"

client = docker.from_env()
image = client.images.pull(image_name, tag=tag_name)

dir1 = "/etc/ceph"
dir2 = "/var/lib/ceph/"
os.mkdir(dir1)
os.mkdir(dir2)
volumes_dict = {dir1: {"bind": dir1, "mode": "rw"},
                dir2: {"bind": dir2, "mode": "rw"}}

# MaximumRetryCount is not needed if restart=always
restart_policy_dict = {"Name": "always"}

# get CIDR of host
ip_addr = socket.gethostbyname(socket.gethostname())
fd = os.popen("ip address")
for line in fd.read().splitlines():
    if ip_addr in line:
        cidr = re.findall(r'[0-9]+(?:\.[0-9]+){3}/[0-9]+', line)[0]
        break

env_dict = {"MON_IP": ip_addr, "CEPH_PUBLIC_NETWORK": cidr}
mon = client.containers.run(image.tags[0], "mon",
                            name="mon", detach=True,
                            volumes=volumes_dict, network_mode='host',
                            restart_policy=restart_policy_dict,
                            environment=env_dict)
mon.stop()
os.system('echo "osd pool default size = 1" >> /etc/ceph/ceph.conf')
os.system('echo "osd pool default min_size = 1" >> /etc/ceph/ceph.conf')
mon.start()

# disk size in Mb
size = 10000
cmd = shlex.split('dd if=/dev/urandom of=/home/disk.raw bs=1M count=%d' % size)
try:
    subprocess.call(cmd, stdout=DEVNULL, stderr=subprocess.STDOUT)
except Exception:
    print("dd failed")
    sys.exit()

for i in range(0, 10):
    cmd = shlex.split("losetup /dev/loop%d /home/disk.raw" % i)
    try:
        status = subprocess.call(cmd, stdout=DEVNULL, stderr=subprocess.STDOUT)
    except Exception:
        continue
    else:
        break

cmd = shlex.split('lsblk')
out = subprocess.check_output(cmd)
dev = None
for line in out.splitlines():
    if 'loop' in line:
        dev = line.split()[0]
        break
if not dev:
    print("set loop device failed")
    sys.exit()

volumes_dict['/dev/'] = {"bind": '/dev/', "mode": "rw"}
env_dict = {"OSD_DEVICE": "/dev/%s" % dev, "OSD_FORCE_ZAP": "1"}
myosd1 = client.containers.run(image.tags[0], "osd_ceph_disk",
                               name="myosd1", detach=True,
                               volumes=volumes_dict, pid_mode="host",
                               restart_policy=restart_policy_dict,
                               environment=env_dict, network_mode="host",
                               privileged=True)

cmd_list = ['ceph osd lspools',
            'ceph osd pool set rbd size 1',
            'ceph osd pool set rbd min_size 1']
for cmd in cmd_list:
    status, out = mon.exec_run(cmd)
    print("cmd: %s" % cmd)
    print(out)
    if status != 0:
        print("Something wrong with %s" % cmd)
        print("exiting...")
        sys.exit()

# wait till mon and osd finish connection
# for larger disk, may need more time to reach HEALTH_OK
print("### check ceph status after 40 seconds...")
time.sleep(40)
status, out = mon.exec_run('ceph -s')
print(out)

#def is_HEALTH_OK(container):
#    try:
#        status, out = container.exec_run('ceph -s')
#    except docker.errors.APIError:
#        return False
#    finally:
#        return True if 'HEALTH_OK' in out else False
#
#
#def wait_until(timeout, period=0.5):
#    def __wait_until(predicate):
#        def __decorator(container):
#            end = time.time() + timeout
#            while time.time() < end:
#                if predicate(container):
#                    return True
#                time.sleep(period)
#            return False
#        return __decorator
#    return __wait_until

#@wait_until(60)
#def is_HEALTH_OK(container):
#    try:
#        status, out = container.exec_run('ceph -s')
#    except docker.errors.APIError:
#        return False
#    finally:
#        return True if 'HEALTH_OK' in out else False
#
#if is_HEALTH_OK(mon):
#    print("ceph cluster is HEALTH_OK")
