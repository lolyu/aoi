#!/usr/bin/env bash
#
# validate_vpp_mux_neigh_consistency.sh
#
# Validate VPP dual-ToR mux neighbor/route consistency on a SONiC-VPP DUT.
#
# Invariant checked, per mux port and per address family (v4 /32 and v6 /128):
#   * mux port ACTIVE   -> server neighbor is PRESENT on bvi1000  AND the server
#                          host route is programmed as an ATTACHED route via bvi1000
#                          (NO tunnel).
#   * mux port STANDBY  -> server neighbor is ABSENT              AND the server
#                          /32 (/128) is programmed as an IPinIP TUNNEL route
#                          (via the peer ToR over ipipN), NOT a bvi1000 host route
#                          and NOT falling through to the 192.168.0.0/21 glean.
#
# Background: on a mux transition MuxOrch programs the same server prefix from two
# independent libsaivs writers (neighbor host route + mux tunnel route). A trailing
# whole-prefix delete used to clobber the just-installed route, leaving standby with
# 0 tunnel routes (falls through to the /21 glean -> ARP -> drop). This script makes
# that failure (and the reverse) observable directly from the VPP FIB.
#
# Usage:
#   Run locally on the DUT (recommended):
#       ./validate_vpp_mux_neigh_consistency.sh
#
#   Run remotely over SSH from a dev box:
#       ./validate_vpp_mux_neigh_consistency.sh -H 10.250.0.134            # key-based ssh
#       SSHPASS=password ./validate_vpp_mux_neigh_consistency.sh -H 10.250.0.134 -e
#       ./validate_vpp_mux_neigh_consistency.sh -H 10.250.0.134 -u admin -p password
#
# Options:
#   -H HOST     SSH into HOST and run the collection there (default: run locally).
#   -u USER     SSH user (default: admin).
#   -p PASS     SSH password (uses sshpass). Alternatively export SSHPASS and pass -e.
#   -e          Use $SSHPASS from the environment as the SSH password (with sshpass).
#   -c NAME     syncd container name (default: syncd).
#   -4          Validate IPv4 (/32) only.
#   -6          Validate IPv6 (/128) only.
#              (default: validate both families)
#   -v          Verbose: also print rows that PASS.
#   -h          Show this help.
#
# Exit status: 0 if every checked (port, family) is consistent, 1 if any FAIL,
# 2 on collection/usage error.
#
set -u -o pipefail

SSHUSER="admin"
HOST=""
PW=""
USE_ENV_PW=0
CNAME="syncd"
DO4=1
DO6=1
VERBOSE=0
SSHOPTS="-o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o ConnectTimeout=8 -o LogLevel=ERROR"

usage() { sed -n '2,45p' "$0" | sed 's/^#\{0,1\} \{0,1\}//'; }

only4=0; only6=0
while getopts ":H:u:p:ec:46vh" opt; do
  case "$opt" in
    H) HOST="$OPTARG" ;;
    u) SSHUSER="$OPTARG" ;;
    p) PW="$OPTARG" ;;
    e) USE_ENV_PW=1 ;;
    c) CNAME="$OPTARG" ;;
    4) only4=1 ;;
    6) only6=1 ;;
    v) VERBOSE=1 ;;
    h) usage; exit 0 ;;
    \?) echo "Unknown option: -$OPTARG" >&2; usage; exit 2 ;;
    :) echo "Option -$OPTARG requires an argument" >&2; exit 2 ;;
  esac
done
# -4 / -6 select a single family; giving neither (or both) means both.
if [ "$only4" = 1 ] && [ "$only6" = 0 ]; then DO6=0; fi
if [ "$only6" = 1 ] && [ "$only4" = 0 ]; then DO4=0; fi
if [ "$USE_ENV_PW" = 1 ]; then PW="${SSHPASS:-}"; fi

# ---- remote/local collection script (evaluated on the DUT) ---------------------
# NOTE: single-quoted literal so $CNAME and the loop vars are evaluated on the DUT,
# not by the shell that launches this. CNAME is injected as a leading assignment.
COLLECT_BODY='
set -o pipefail
vpp() { docker exec "$CNAME" vppctl "$@" 2>/dev/null; }
red() { docker exec "$CNAME" redis-cli -n 4 "$@" 2>/dev/null; }
echo "@@@MUX"
show mux status 2>/dev/null
echo "@@@NBR4"
vpp show ip neighbors
echo "@@@NBR6"
vpp show ip6 neighbors
keys=$(red --scan --pattern "MUX_CABLE|Ethernet*" | tr -d "\r" | sort -V)
for k in $keys; do
  port=${k#MUX_CABLE|}
  v4=$(red hget "$k" server_ipv4 | tr -d "\r")
  v6=$(red hget "$k" server_ipv6 | tr -d "\r")
  echo "@@@PORT $port ${v4:-none} ${v6:-none}"
  if [ -n "$v4" ]; then echo "@@@FIB4 $port $v4"; vpp show ip  fib "$v4"; fi
  if [ -n "$v6" ]; then echo "@@@FIB6 $port $v6"; vpp show ip6 fib "$v6"; fi
done
echo "@@@END"
'
COLLECT="CNAME=$(printf '%q' "$CNAME")
$COLLECT_BODY"

collect() {
  if [ -n "$HOST" ]; then
    if ! command -v ssh >/dev/null 2>&1; then echo "ssh not found" >&2; exit 2; fi
    if [ -n "$PW" ]; then
      command -v sshpass >/dev/null 2>&1 || { echo "sshpass not found (needed for -p/-e)" >&2; exit 2; }
      sshpass -p "$PW" ssh $SSHOPTS "$SSHUSER@$HOST" bash -s <<<"$COLLECT"
    else
      ssh $SSHOPTS "$SSHUSER@$HOST" bash -s <<<"$COLLECT"
    fi
  else
    bash -s <<<"$COLLECT"
  fi
}

RAW="$(collect | tr -d '\r')" || { echo "collection failed" >&2; exit 2; }
if ! printf '%s\n' "$RAW" | grep -q '@@@END'; then
  echo "ERROR: collection incomplete (no @@@END marker). Raw head:" >&2
  printf '%s\n' "$RAW" | head -20 >&2
  exit 2
fi

# ---- parse + evaluate ----------------------------------------------------------
printf '%s\n' "$RAW" | awk -v want4="$DO4" -v want6="$DO6" -v verbose="$VERBOSE" '
function ipof(p){ sub(/\/.*/,"",p); return p }
BEGIN{ sect=""; np=0 }
/^@@@MUX/  { sect="MUX"; next }
/^@@@NBR4/ { sect="NBR"; next }
/^@@@NBR6/ { sect="NBR"; next }
/^@@@PORT/ { sect="PORT"; p=$2; ports[++np]=p; v4[p]=$3; v6[p]=$4; next }
/^@@@FIB4/ { sect="FIB"; fk=$2 SUBSEP "4"; fpref=$3; seen[fk]=1; hdr=""; next }
/^@@@FIB6/ { sect="FIB"; fk=$2 SUBSEP "6"; fpref=$3; seen[fk]=1; hdr=""; next }
/^@@@END/  { sect=""; next }
{
  if (sect=="MUX") {
    if ($1 ~ /^Ethernet[0-9]+$/) mux[$1]=$2
    next
  }
  if (sect=="NBR") {
    # columns: Age  IP  Flags  Ethernet  Interface
    if (NF>=5 && $2 ~ /^[0-9A-Fa-f:.]+$/) nbr_if[$2]=$NF
    next
  }
  if (sect=="FIB") {
    if (hdr=="" && $0 ~ /fib:0 index:/) { hdr=$1; if (hdr==fpref) spec[fk]=1 }
    if ($0 ~ /via[^\n]* bvi1000/) bvi[fk]=1
    if ($0 ~ /ipip/)             tun[fk]=1
    next
  }
}
function route_kind(fk,   k){
  if (!(fk in seen))      return "NO-FIB"
  if (!spec[fk])          return "MISSING"          # fell through to covering (e.g. /21 glean)
  if (bvi[fk] && tun[fk]) return "BOTH"             # anomaly: host + tunnel on same prefix
  if (bvi[fk])            return "HOST"
  if (tun[fk])            return "TUNNEL"
  return "OTHER"
}
function check(port, af, pfx,    sip, st, present, kind, exp_n, exp_r, an, ok, fk){
  if (pfx=="none" || pfx=="") return
  fk = port SUBSEP af
  sip = ipof(pfx)
  st  = (port in mux) ? mux[port] : "unknown"
  present = (sip in nbr_if) ? 1 : 0
  kind = route_kind(fk)
  an = present ? "PRESENT" : "absent"
  if (st=="active")      { exp_n="PRESENT"; exp_r="HOST" }
  else if (st=="standby"){ exp_n="absent";  exp_r="TUNNEL" }
  else                   { exp_n="?";        exp_r="?" }

  ok = 0
  if (st=="active"  && present && kind=="HOST")             ok=1
  else if (st=="standby" && !present && kind=="TUNNEL")     ok=1

  total++; tot_af[af]++
  if (ok) { pass++; }
  else    { fail++; fail_af[af]++ }

  res = ok ? "PASS" : "FAIL"
  # neighbor sub-verdict and route sub-verdict for the detail column
  nsub = (present == (exp_n=="PRESENT")) ? "ok" : "BAD"
  rsub = (kind==exp_r) ? "ok" : "BAD"

  if (!ok || verbose) {
    printf "  %-11s  %-7s  v%s  %-18s  neigh=%-8s(exp %-8s)[%s]  route=%-8s(exp %-8s)[%s]  => %s\n",
           port, st, af, sip, an, exp_n, nsub, kind, exp_r, rsub, res
  }
}
END{
  # @@@PORT rows were emitted in `sort -V` order, so iterate emission order
  # (avoids a gawk-only asort dependency; works on mawk too).
  print "VPP mux neighbor/route consistency"
  print "----------------------------------------------------------------------------------------------------"
  for (i=1;i<=np;i++){
    p=ports[i]
    if (want4=="1") check(p,"4",v4[p])
    if (want6=="1") check(p,"6",v6[p])
  }
  print "----------------------------------------------------------------------------------------------------"
  printf "TOTAL: %d checked, %d PASS, %d FAIL", total+0, pass+0, fail+0
  if (want4=="1") printf "   | v4: %d/%d ok", (tot_af["4"]-fail_af["4"])+0, tot_af["4"]+0
  if (want6=="1") printf "   | v6: %d/%d ok", (tot_af["6"]-fail_af["6"])+0, tot_af["6"]+0
  print ""
  if (fail>0) { print "RESULT: FAIL"; exit 1 }
  print "RESULT: PASS"; exit 0
}
'
rc=$?
exit $rc
