#!/usr/bin/env bash
SCRIPT_DIR=$(dirname $(readlink -f "$0")) 
cd ${SCRIPT_DIR}/1brc/
SECONDS=0
./calculate_average_thomaswue.sh | tee ../example-output.txt
echo "$SECONDS" > ../example-time
