# sample code to start gluster service
# lolyu@redhat.com

from __future__ import print_function
import docker
import sys

image_name = "gluster/gluster-centos"
tag_name = 'latest'

client = docker.from_env()
image = client.images.pull(image_name, tag=tag_name)
volumes_dict = {"/etc/glusterfs": {"bind": "/etc/glusterfs", "mode": "z"},
                "/var/lib/glusterd": {"bind": "/var/lib/glusterd", "mode": "z"},
                "/var/log/glusterfs": {"bind": "/var/log/glusterfs", "mode": "z"},
                "/sys/fs/cgroup": {"bind": "/sys/fs/cgroup", "mode": "ro"}}
#volumes_dict = {"/sys/fs/cgroup": {"bind": "/sys/fs/cgroup", "mode": "ro"}}

container = client.containers.run(image.tags[0], name='gluster', detach=True,
                                  volumes=volumes_dict, privileged=True)

status, out = container.exec_run('cat /etc/hosts')
for line in out.splitlines():
    if container.short_id in line:
        ip_addr = line.split()[0]

cmd_list = ['systemctl start glusterd',
            'gluster volume create gv0 %s:/mnt/brick/ force' % ip_addr,
            'gluster volume start gv0',
            'gluster volume set gv0 server.allow-insecure on',
            'gluster volume info gv0']

for cmd in cmd_list:
    status, out = container.exec_run(cmd)
    print("cmd: %s" % cmd)
    print(out)
    if status != 0:
        print("Something wrong with %s" % cmd)
        print("exiting...")
        sys.exit()
