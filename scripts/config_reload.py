import argparse
import logging
import shlex
import subprocess
import sys
import time
import traceback


def parse_args():
    parser = argparse.ArgumentParser(
        description="Verify neighbors state is consistent with mux state."
    )
    parser.add_argument(
        "-c",
        "--retry-count",
        type=int,
        default=1,
        help="config reload count"
    )
    parser.add_argument(
        "-l",
        "--log-level",
        choices=["ERROR", "WARNING", "INFO", "DEBUG"],
        default=None,
        help="stdout log level"
    )
    args = parser.parse_args()
    if args.log_level is None:
        args.log_level = logging.INFO
    else:
        args.log_level = logging.getLevelName(args.log_level)
    return args


def config_logging(args):
    """Configures logging based on arguments."""
    logging.basicConfig(
        stream=sys.stdout,
        level=args.log_level,
        format="%(message)s"
    )


def run_command(cmd):
    """Runs a command and returns its output."""
    logging.debug("Running command: %s", cmd)
    try:
        p = subprocess.Popen(shlex.split(cmd), stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (output, _) = p.communicate()
    except Exception as details:
        raise RuntimeError("Failed to run command: %s", details)
    logging.debug("Command output: %s", output)
    logging.debug("Command return code: %s", p.returncode)
    if p.returncode != 0:
        raise RuntimeError("Command failed with return code %s: %s" % (p.returncode, output))
    return output.decode()


def wait_until(timeout, interval, delay, condition, *args, **kwargs):
    logging.info("Wait until %s is True, timeout is %s seconds, checking interval is %s, delay is %s seconds" %
                 (condition.__name__, timeout, interval, delay))

    if delay > 0:
        logging.debug("Delay for %s seconds first" % delay)
        time.sleep(delay)

    start_time = time.time()
    elapsed_time = 0
    while elapsed_time < timeout:
        logging.debug("Time elapsed: %f seconds" % elapsed_time)

        try:
            check_result = condition(*args, **kwargs)
        except Exception as e:
            exc_info = sys.exc_info()
            details = traceback.format_exception(*exc_info)
            logging.error(
                "Exception caught while checking {}:{}, error:{}".format(
                    condition.__name__, "".join(details), e
                )
            )
            check_result = False

        if check_result:
            logging.info("%s is True, exit early with True" % condition.__name__)
            return True
        else:
            logging.debug("%s is False, wait %d seconds and check again" % (condition.__name__, interval))
            time.sleep(interval)
            elapsed_time = time.time() - start_time

    if elapsed_time >= timeout:
        logging.info("%s is still False after %d seconds, exit with False" % (condition.__name__, timeout))
        return False


def _per_namespace_swss_ready():
    out = run_command('systemctl show swss --property ActiveState --value')
    if out.strip() != "active":
        return False
    out = run_command('systemctl show swss --property ActiveEnterTimestampMonotonic --value')
    swss_up_time = float(out.strip())/1000000
    now =  time.monotonic()
    if (now - swss_up_time > 120):
        return True
    else:
        return False


def check_services():
    services = run_command("docker ps --format \{\{.Names\}\}").split()
    results = {_: False for _ in services}

    # Check and update service status
    try:
        output = run_command(r"docker ps --filter status=running --format \{\{.Names\}\}")
        for service in services:
            if service in output:
                results[service] = True
    except Exception as e:
        logging.info("Critical service status: {}".format(output))
        logging.info("Get critical service status failed with error {}".format(repr(e)))

    return all(results.values()) and _per_namespace_swss_ready()


def config_reload(retry_count):
    for i in range(retry_count):
        logging.warning("config reload %s times", i)
        run_command("config reload -y")
        time.sleep(60)
        if not wait_until(120, 10, 0, check_services):
            logging.error("service health check failed, exit")
            break


if __name__ == "__main__":
    args = parse_args()
    config_logging(args)
    config_reload(args.retry_count)
