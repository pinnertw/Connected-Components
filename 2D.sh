#!/bin/sh

if [ "$#" -ne 2 ] && [ "$#" -ne 3 ] && [ "$#" -ne 4 ]; then
    echo "Usage: $0 n p (epsilon) (M)"
    echo "Usage: $0 n p (rate)"
    exit 1
fi

python src/graph_2D_output_coords_et_format.py $1 $2
make test-TDB
./bin/test-TDB data/graph_2D.txt $3 $4
python src/graph_2D_plot.py data/graph_2D_coordinates.txt output/graph_2D_edges.txt
