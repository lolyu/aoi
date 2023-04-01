import argparse
import docker
import logging
import json
import subprocess
import sys
import time

from sonic_py_common import daemon_base
from swsscommon import swsscommon


DOCKER_CLIENT = docker.DockerClient(base_url='unix://var/run/docker.sock')
RUNNING = "running"


def get_active_active_mux_ports(config_db):
    mux_cable_table = swsscommon.Table(config_db, "MUX_CABLE")
    active_active_mux_ports = []
    for mux_cable in mux_cable_table.getKeys():
        res, configs = mux_cable_table.get(mux_cable)
        if not res:
            continue
        configs = dict(configs)
        if configs.get("cable_type", "active-standby") == "active-active":
            active_active_mux_ports.append(mux_cable)
    return active_active_mux_ports


def is_mux_running():
    container = DOCKER_CLIENT.containers.get("mux")
    container_state = container.attrs['State']
    container_is_running = container_state['Status'] == RUNNING
    return container_is_running


def verify_active_active_mux_status(active_active_ports):
    show_result = run_command("show mux status --json")
    if not show_result:
        logging.debug("not output from show mux status")

    show_result = json.loads(show_result)
    if not show_result or "MUX_CABLE" not in show_result:
        logging.debug("not output from show mux status")

    show_result = show_result["MUX_CABLE"]
    active_active_mux_status = {}
    for mux_port in active_active_ports:
        if mux_port not in show_result:
            logging.debug("mux port %s not present in show mux status", mux_port)
            return False
        active_active_mux_status[mux_port] = show_result[mux_port]

    logging.debug("mux status:\n%s", json.dumps(active_active_mux_status, indent=4))
    for mux_port, port_status in active_active_mux_status.items():
        if port_status["STATUS"] != "active" or port_status["SERVER_STATUS"] != "active":
            logging.info("mux port %s is not active", mux_port)
            return False
    return True


def run_command(cmd, check=True):
    """Run a command."""
    logging.debug("COMMAND: %s", cmd)
    process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True,
    )
    out, err = process.communicate()
    logging.debug("COMMAND STDOUT:\n%s\n", out)
    logging.debug("COMMAND STDERR:\n%s\n", err)
    if check:
        if process.returncode != 0:
            raise RuntimeError
    return out


def config_logging():
    log_format = "%(asctime)s %(funcName)-20.20s %(levelname)-5.5s #%(lineno)-.4d| %(message)s"
    root = logging.getLogger()
    root.setLevel(logging.DEBUG)
    handler = logging.StreamHandler(sys.stdout)
    handler.setLevel(logging.DEBUG)
    formatter = logging.Formatter(log_format)
    handler.setFormatter(formatter)
    root.addHandler(handler)


def wait_for(func, timeout, first=0.0, step=1.0, text=None, args=None, kwargs=None):
    if args is None:
        args = []
    if kwargs is None:
        kwargs = {}
    start_time = time.monotonic()
    end_time = start_time + timeout

    time.sleep(first)

    while time.monotonic() < end_time:
        if text:
            logging.debug("%s (%.9f secs)", text, (time.monotonic() - start_time))

        output = func(*args, **kwargs)
        if output:
            return output

        time.sleep(step)

    return None


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Do config reload and check the mux status"
    )

    parser.add_argument(
        "-c",
        "--count",
        type=int,
        required=True,
        default=10,
        dest="count",
        help="the check loop count"
    )

    config_logging()
    args = parser.parse_args()
    config_db = daemon_base.db_connect("CONFIG_DB")
    loop_count = args.count
    mux_ports = get_active_active_mux_ports(config_db)

    for loop in range(loop_count):
        # do config reload
        logging.warning("=========================================\n")
        logging.warning("%sth check", loop)
        logging.info("config reload the device")
        run_command("config reload -y -f")

        logging.info("sleep 60s waiting for mux running")
        time.sleep(30)

        # wait till mux is running
        if not wait_for(is_mux_running, 120, 10):
            logging.error("mux is not running")
            sys.exit(1)

        if not wait_for(verify_active_active_mux_status, 60, 10, text=None, args=(mux_ports,)):
            logging.error("mux ports not all reach active")
            sys.exit(1)

        logging.info("%sth check passed!!!!!!", loop)
        logging.warning("\n=========================================\n")
