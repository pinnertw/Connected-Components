#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 n"
    exit 1
fi

./bin/test-ER_p $1
FILENAME="output/ER_digraph_$1n_varp.txt"
python src/scc-p_plot.py $1
