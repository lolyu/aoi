#!/usr/bin/env python
import os
import sys
import pip
import imp
import pexpect
import subprocess
import time
#import pdb

image = "registry.access.redhat.com/rhgs3/rhgs-server-rhel7"

print "****** GlusterFS container bootstrap ******"
print "[0] check docker installation"
FNULL = open(os.devnull, 'w')
try:
    subprocess.call(['docker'], stdout=FNULL, stderr=FNULL)
except OSError as e:
    if e.errno == os.errno.ENOENT:
        print 'not installed, install then'
        subprocess.call(['yum', 'install', 'docker', '-y'])
    else:
        print str(e)
        print 'unknown error, exiting...'
        sys.exit()
else:
    print 'docker version'
    subprocess.call(['docker', 'version'])

print "\n[1] pull the image off"
out = subprocess.call(['docker', 'pull', image])
if out != 0:
    print 'something went wrong pulling images, exiting...'
    sys.exit()

print
print "\n[2] create a GlusterFS container"
#rm existing container named gluster if possible
if 'gluster' in subprocess.check_output(['docker', 'ps']):
    subprocess.call(['docker', 'stop', 'gluster'])
    subprocess.call(['docker', 'rm', 'gluster'])

cmd = 'docker run --privileged=true -it --name gluster %s /bin/bash'
child = pexpect.spawn(cmd % image)
child.sendline('/usr/sbin/glusterd')

fmt = '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}'
try:
    ip = subprocess.check_output(['docker', 'inspect', '-f', fmt, 'gluster'])
except subprocess.CalledProcessError as e:
    print str(e)
    print 'something went wrong with container, exiting...'
    sys.exit()
ip = ip.strip()

child.sendline('gluster volume create gv0 %s:/mnt/brick/ force' % ip)
child.sendline('gluster volume start gv0')

# detach the child process
# child.interact()

print "\n[3] GlusterFS details"
fmt = '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}'
print "ip address: %s" % ip
print "volume: gv0"
print "container name: gluster"

print "\n[4] check volume connection"
try:
    imp.find_module('gluster')
except ImportError:
    pip.main(['install', 'gfapi'])

from gluster import gfapi
volume = gfapi.Volume(host=ip, volname='gv0', log_level=0)

#pdb.set_trace()

time.sleep(5)

try:
    volume.mount()
except Exception as e:
    print str(e)
    print 'volume seems not exist, exiting...'

if volume.mounted:
    volume.umount()

print 'server is working well...'
