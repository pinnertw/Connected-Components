make test-DBSCAN_rate
./bin/test-DBSCAN_rate $1
file="${1/data/output}"
file="${file/.txt/_dbscan_find-rate.txt}"
python src/analysis_rate-similarity.py $file

