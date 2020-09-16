#include "graph.hpp"
#include "ConfusionMatrix.hpp"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <unordered_map>
#include <vector>

std::vector<Id> true_value(Id* partition, int n){
    std::vector<Id> res (n);
    std::unordered_map<Id, std::vector<Id> > count_partition;
    // Calculate the clusters
    for (Id j = 0; j < n; j++){
        // Init res[j]
        res[j] = 0;
        Id represent = partition[j];
        if (count_partition.find(represent) != count_partition.end()){
            count_partition[represent].push_back(j);
        }
        else{
            count_partition[represent] = {j};
        }
    }
    for (auto j = count_partition.begin(); j != count_partition.end(); j++){
        if (j->second.size() > 1){
            for (auto k : j->second){
                res[k] = 1;
            }
        }
    }
    return res;
}


int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr<<"Usage : ./test-io input_file"<<std::endl;
        return 1;
    }
    std::string filename = (std::string) (argv[1]);
    std::string fileroot = "output/" + filename.substr(0, filename.find(".")).substr(filename.find("/") + 1) + "_dbscan_find-rate.txt";

    std::ofstream output;
    output.open(fileroot);
    std::chrono::time_point<std::chrono::system_clock> start, end;

    Graph graph = Graph(argv[1]);
    // Tarjan for comparison
    //
    start = std::chrono::system_clock::now();
    std::cerr << "Processing Tarjan...";
    graph.Tarjan();
    std::vector<Id> true_values = true_value(graph.get_partition_scc(), graph.get_n());

    end = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    std::cerr << "elapsed time: " << elapsed_seconds << "ms\n";



    start = std::chrono::system_clock::now();
    std::cerr << "Processing DBSCAN..."<<std::endl;;
    for (int i = 1; i < 100; i++){
        double rate = (double) i / 100.;
        double epsilon = graph.heuristic_epsilon(rate);
        int M = graph.heuristic_M(rate, epsilon);
        std::cerr<<"Processing DBSCAN algorithm with rate = "<<rate<<"\r";
        graph.DBSCAN(epsilon, M);
        std::vector<Id> prediction = true_value(graph.get_clusters_dbscan(), graph.get_n());
        ConfusionMatrix cfmatrix = ConfusionMatrix();
        for (Id j = 0; j < graph.get_n(); j ++){
            cfmatrix.AddPrediction(true_values[j], prediction[j]);
        }
        output<<rate<<' '<<cfmatrix.error_rate()<<' '<<cfmatrix.false_alarm_rate()<<' '<<cfmatrix.detection_rate()<<' '<<cfmatrix.f_score()<<' '<<cfmatrix.precision()<<std::endl;
    }

    end = std::chrono::system_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    std::cerr << "elapsed time: " << elapsed_seconds << "ms\n";
    output.close();
    return 0;
}
