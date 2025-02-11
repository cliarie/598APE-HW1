#!/bin/bash

FLAMEGRAPH_DIR="$HOME/flamegraph"

echo "Starting dtrace profiling.."
sudo dtrace -n 'profile-997 /execname == "main.exe"/ { @[ustack()] = count(); }' -o out.stacks &

echo "Running main.exe.."
./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500

sleep 4

echo "Making flamegraph.."
"$FLAMEGRAPH_DIR/stackcollapse.pl" out.stacks | "$FLAMEGRAPH_DIR/flamegraph.pl" > flamegraph.svg

echo "Opening flamegraph.."
open -a "Arc" flamegraph.svg
