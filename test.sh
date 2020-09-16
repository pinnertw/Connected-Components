#!/bin/sh

if [ "$#" -ne 1 ] && [ "$#" -ne 2 ] && [ "$#" -ne 3]; then
    echo "Usage: $0 data/dataname (lambda) (M)"
    echo "Or   : $0 data/dataname (rate)"
    exit 1
fi

make test-TDB
echo "Processing with Tarjan algorithm and DBSCAN algorithm..."
./bin/test-TDB $1 $2 $3
output_file="${1/data/output}"
output_file="${output_file/.txt/_edges.txt}"
python src/plot.py $output_file
