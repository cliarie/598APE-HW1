#!/bin/bash

FLAMEGRAPH_DIR="$HOME/flamegraph"

rm -f out.stacks out.folded flamegraph.svg

sudo -v || { echo "Sudo access is required. Exiting."; exit 1; }

echo "Starting dtrace profiling.."
sudo dtrace -c './main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500' -o out.stacks -n 'profile-997 /execname == "main.exe"/ { @[ustack(100)] = count(); }' &

sleep 5

if [ ! -f out.stacks ]; then
    echo "Error: out.stacks was not created. Profiling data is missing."
    exit 1
fi
#
echo "Making flamegraph.."
"$FLAMEGRAPH_DIR/stackcollapse.pl" out.stacks > out.folded
"$FLAMEGRAPH_DIR/flamegraph.pl" out.folded > flamegraph.svg

open -a "Arc" flamegraph.svg
