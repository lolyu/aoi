#!/bin/bash

set -x

while true
do
    echo "Current time: $(date)"
    docker exec mux supervisorctl restart linkmgrd

    sleep 20 # Pause for 1 second to prevent overwhelming the CPU

    grep "Failed to attach filter with" /var/log/syslog

    if [[ $? -eq 0 ]]
    then
        echo "Reproduced!!!!!!!!!!!"
        break
    fi
done
