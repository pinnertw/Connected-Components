#!/bin/sh

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 n p"
    exit 1
fi

./bin/test-ER $1 $2
FILENAME="output/ER_digraph_$1n$2p_edges.txt"
python src/plot.py $FILENAME scc
