#include "graph.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

std::pair<int, double> count(Graph& graph, int n){
    // Create graph
    graph.Tarjan();
    Id* partition_scc = graph.get_partition_scc();
    std::unordered_map<Id, int> count_partition;
    // Calculate the size of cluster
    for (Id j = 0; j < n; j++){
        Id represent = partition_scc[j];
        if (count_partition.find(represent) != count_partition.end()){
            count_partition[represent]++;
        }
        else{
            count_partition[represent] = 1;
        }
    }
    // count_ : the number of clusters with size > 1
    // proportion : the number of nodes in these clusters.
    int count_ = 0;
    double proportion = 0;
    for (auto j = count_partition.begin(); j != count_partition.end(); j++){
        // j->first : represent, j->second -> the size
        if (j->second > 1) {
            count_++;
            proportion += j->second;
        }
    }
    return std::pair<int, double> (count_, proportion);
}

int main(int argc, char* argv[]){
    if ((argc != 2)){
        std::cerr<<"Usage : ./test-ER_p n"<<std::endl;
        return 1;
    }

    int n = atoi(argv[1]);
    int M = 100;

    std::string filename = "output/ER_digraph_" + (std::string) argv[1] + "n_varp.txt";

    std::ofstream output;
    output.open(filename);

    double pl = 0;
    double pr = 1;
    output<<pr<<' '<<1<<' '<<1<<std::endl;

    for (int i = 1; i < 500; i++){
        std::cerr<<i<<"/"<<500<<"\r";
        double p = (double) i / (double) (5000);
        std::pair<double, double> count_proportion = std::pair<double, double> (0., 0.);
        // Monte-Carlo
        for (int j = 0; j < M; j++){
            Graph graph = Graph(n, p);
            std::pair<int, double> count_proportion_ = count(graph, n);
            count_proportion.first += count_proportion_.first;
            count_proportion.second += count_proportion_.second;
        }
        output<<p<<' '<<count_proportion.first / M<<' '<<count_proportion.second / M / n<<std::endl;
    }

    output.close();
    return 0;

}
