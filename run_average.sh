#!/bin/bash

runs=10
total_time=0

for ((i=1; i<=runs; i++)); do
    echo "Run $i of $runs"

    output=$( { time -p ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500; } 2>&1 )

    runtime=$(echo "$output" | awk '/^real/ {print $2}')
    echo "Run $i: $runtime seconds"

    total_time=$(echo "$total_time + $runtime" | bc -l)
done
average=$(echo "$total_time / $runs" | bc -l)
echo "Average run time over $runs runs: $average seconds"

