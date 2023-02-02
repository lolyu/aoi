import click
import json
import os.path
import requests

from tabulate import tabulate


MUX_SIMULATOR_CONFIG = "/etc/sonic/mux_simulator.json"


def collect_mux_simulator_config():

    err = RuntimeError(f"Could not collect mux simulator config from {MUX_SIMULATOR_CONFIG}, is it a valid dualtor testbed?")

    if not os.path.exists(MUX_SIMULATOR_CONFIG):
        raise err

    with open(MUX_SIMULATOR_CONFIG) as fd:
        config = json.loads(fd.read())
        server_ip = config["server_ip"]
        server_port = config["server_port"]
        vm_set = config["vm_set"]
        tor_side = config["side"]

    return server_ip, server_port, vm_set, tor_side
        

@click.group()
def main():
    pass


@main.command()
@click.option("--port_index", required=False, default=None, help="the port index")
def status(port_index):
    server_ip, server_port, vm_set, tor_side = collect_mux_simulator_config()
    mux_server_url = f"http://{server_ip}:{server_port}/mux/{vm_set}"
    headers = {'Accept': 'application/json'}
    resp = requests.get(mux_server_url, headers=headers)
    if resp.status_code != 200:
        click.echo(f"Could not interacts with mux simulator, error code {resp.status_code}")
        return

    mux_status = resp.json()
    output_headers = ["port index", "status"]
    output = []
    for port, status in mux_status.items():
        pi = port.split("-")[-1]
        ps = "active" if status["active_side"] == tor_side else "standby"
        output.append((pi, ps))

    if port_index:
        output = [item for item in output if item[0] == port_index]

    output.sort(key=lambda item: int(item[0]))
    click.echo(tabulate(output, headers=output_headers))


@main.command()
@click.option("--port_index", required=False, default=None, help="the port index")
@click.option("--status", type=click.Choice(['active', 'standby'], case_sensitive=False), required=True, help="the mux status")
def config(port_index, status):
    server_ip, server_port, vm_set, tor_side = collect_mux_simulator_config()
    mux_server_url = f"http://{server_ip}:{server_port}/mux/{vm_set}"
    if port_index:
        mux_server_url += ("/" + str(port_index))

    if status == "active":
        side = tor_side
    elif status == "standby":
        if tor_side == "upper_tor":
            side = "lower_tor"
        elif tor_side == "lower_tor":
            side = "upper_tor"

    data = {"active_side": side}
    headers = {'Accept': 'application/json', 'Content-Type': 'application/json'}
    resp = requests.post(mux_server_url, json=data, headers=headers, timeout=10)
    if resp.status_code == 200:
        click.echo("Done...")
    else:
        click.echo(f"Failed with HTTP error {resp.status_code}...")


if __name__ == "__main__":
    main()
