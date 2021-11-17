from __future__ import print_function

#import pdb
import sys
import time
import string
import json
import random
import requests
import argparse
import datetime
import pexpect


def generate_username(length=6):
    username = []
    for i in range(length):
        username.append(random.choice(string.ascii_lowercase))
    d = datetime.date.today()
    username.extend(map(str, [d.year, d.month, d.day]))
    return "".join(username)


parser = argparse.ArgumentParser()
helpstr = ("'manager': install rhvm needed repos\n"
           "'host': install host needed repos")
parser.add_argument("-t", "--type", type=str,
                    choices=['manager', 'host'],
                    help=helpstr, required=True)
args = parser.parse_args()

# https://docs.engineering.redhat.com/display/ETHEL/API+Documentation
payload = {
    'username': generate_username(),
    'password': 'kvmautotest',
    'first_name': 'autotest',
    'last_name': 'kvm',
    'sku': ['ES0113909', 'ESA0002', 'RV00045'],
    'quantity': 1,
}

request_parameters = {
    'create_account': ('/account/new',
                       ['username', 'password', 'first_name', 'last_name']),
    'activate_account': ('/account/activate',
                         ['username', 'password']),
    'attach_subscriptions': ('/account/attach',
                             ['username', 'password', 'sku', 'quantity']),
    'view_account': ('/account/get',
                     ['username', 'password']),
}

base_url = 'http://account-manager-stage.app.eng.rdu2.redhat.com%s'
sep = "#" * 10 + " %s"
err_msg = "something wrong in %s"

for operation in ['create_account',
                  'attach_subscriptions',
                  'activate_account']:
    print(sep % operation)
    url = base_url % request_parameters[operation][0]
    pl = {k: v for k, v in payload.items()
          if k in request_parameters[operation][1]}
    for key, value in pl.items():
        print("%s: %s" % (key, value))
    r = requests.post(url, json=pl)
    if r.ok:
        print("%s succeeded\n" % operation)
    else:
        print(err_msg % operation)
        print(r.content)
        print("exiting...")
        sys.exit(-1)

print("\nAccepting Policies and Conditions...\n")
# view_account first to check if conditions are accepted
url = base_url % request_parameters['view_account'][0]
pl = {k: v for k, v in payload.items()
      if k in request_parameters['view_account'][1]}
r = requests.get(url, params=pl)
d = json.loads(r.text)
print(d['msg'])


def ensure_pools_existed():
    r = requests.get(url, params=pl)
    d = json.loads(r.text)
    if d.get('data') and d['data'].get('pools'):
        return False
    return True


animation = "|/-\\"
idx = 0
while ensure_pools_existed():
    print(animation[idx % len(animation)], end='\r')
    idx += 1
    time.sleep(0.1)


print("\nAccount is ready to work!!!!!\n")
print("install relevant repos...")

# unregister first
cmd = "subscription-manager unregister"
pexpect.run(cmd)

# register
cmd = ('subscription-manager register --serverurl '
       'subscription.rhsm.stage.redhat.com')
child = pexpect.spawn(cmd)
# http://pexpect.readthedocs.io/en/stable/commonissues.html
# wait enough time before sending to child
child.delaybeforesend = 5
child.sendline(payload['username'])
child.sendline(payload['password'])
child.wait()

# list pools available
cmd = "subscription-manager list --available"
out = pexpect.run(cmd, timeout=30)
pools = [line.split()[-1] for line in out.splitlines() if 'Pool ID' in line]

# attach to those pools
cmd = "subscription-manager attach --pool=%s"
map(pexpect.run, [cmd % pool for pool in pools])

# disable all repos
cmd = "subscription-manager repos --disable=*"
pexpect.run(cmd)

repos_manager = [
    'rhel-7-server-rpms',
    'rhel-7-server-supplementary-rpms',
    'rhel-7-server-rhv-4.2-manager-rpms',
    'rhel-7-server-rhv-4-manager-tools-rpms',
    'rhel-7-server-ansible-2-rpms',
    'jb-eap-7-for-rhel-7-server-rpms',
]

repos_host = [
    'rhel-7-server-rpms',
    'rhel-7-server-rhv-4-mgmt-agent-rpms',
    'rhel-7-server-ansible-2-rpms',
]

# install repos for manager or host
cmd = "subscription-manager repos --enable=%s"
if args.type == 'manager':
    map(pexpect.run, [cmd % repo for repo in repos_manager])
elif args.type == 'host':
    map(pexpect.run, [cmd % repo for repo in repos_host])

print('\nDONE...\n')
