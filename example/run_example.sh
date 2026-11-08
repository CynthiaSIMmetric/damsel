#!/usr/bin/env bash
SCRIPT_DIR=$(dirname $(readlink -f "$0")) 
cd ${SCRIPT_DIR}/1brc/
./calculate_average_thomaswue.sh
