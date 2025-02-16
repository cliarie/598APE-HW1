#!/bin/bash

FLAMEGRAPH_DIR="$HOME/flamegraph"

rm -f out.stacks out.folded flamegraph.svg

sudo -v || { echo "Sudo access is required. Exiting."; exit 1; }

echo "Starting dtrace profiling.."

# first input
# sudo dtrace -c './main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500' -o out.stacks -n 'profile-997 /execname == "main.exe"/ { @[ustack(100)] = count(); }' &

# second input
# sudo dtrace -c './main.exe -i inputs/globe.ray --ppm -a inputs/globe.animate --movie -F 24' -o out.stacks -n 'profile-997 /execname == "main.exe"/ { @[ustack(100)] = count(); }' &

# third input
# sudo dtrace -c './main.exe -i inputs/elephant.ray --ppm  -a inputs/elephant.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4' -o out.stacks -n 'profile-997 /execname == "main.exe"/ { @[ustack(100)] = count(); }' &


sudo dtrace -n 'profile-997 /execname == "main.exe"/ { @[ustack(100)] = count(); }' -o out.stacks &
DTRACE_PID=$!
echo "DTrace started with PID: $DTRACE_PID"

sleep 2

echo "Running main.exe..."
# first input
# ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500

# second input 
# ./main.exe -i inputs/globe.ray --ppm -a inputs/globe.animate --movie -F 24
#
# third input
./main.exe -i inputs/elephant.ray --ppm  -a inputs/elephant.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4 
#
echo "Waiting for main.exe to finish..."
while pgrep -x "main.exe" >/dev/null; do
    sleep 1
done

echo "main.exe has finished."

sleep 10
echo "Stopping DTrace..."
sudo kill $DTRACE_PID 2>/dev/null

if [ ! -f out.stacks ]; then
    echo "Error: out.stacks was not created. Profiling data is missing."
    exit 1
fi
#
echo "Making flamegraph.."
"$FLAMEGRAPH_DIR/stackcollapse.pl" out.stacks > out.folded
"$FLAMEGRAPH_DIR/flamegraph.pl" out.folded > flamegraph.svg

open -a "Arc" flamegraph.svg
