#include "graph.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char* argv[]){
    if ((argc < 2) || (argc > 4)){
        std::cerr<<"Usage : ./test-io input_file (epsilon) (M)"<<std::endl;
        std::cerr<<"Or : ./test-io input_file (rate)"<<std::endl;
        return 1;
    }
    Graph graph = Graph(argv[1]);
    double epsilon = graph.heuristic_epsilon(0.9);
    int M = graph.heuristic_M(0.9, epsilon);

    if (argc == 3){
        double rate = atof(argv[2]);
        epsilon = graph.heuristic_epsilon(rate);
        M = graph.heuristic_M(rate, epsilon);
    }

    if (argc == 4){
        epsilon = atof(argv[2]);
        M = atoi(argv[3]);
    }
    std::chrono::time_point<std::chrono::system_clock> start, end;


    std::cerr<<"Using epsilon = "<<epsilon<<", M = "<<M<<std::endl;
    std::string filename = (std::string) (argv[1]);
    std::string fileroot = "output/" + filename.substr(0, filename.find(".")).substr(filename.find("/") + 1);
    std::cerr<<"Processing Tarjan algorithm...";
    start = std::chrono::system_clock::now();
    graph.Tarjan();
    end = std::chrono::system_clock::now();
    int elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
    std::cerr<<"elapsed time: " <<elapsed_time << "ms\n"<<std::endl;
    std::cerr<<"Processing DBSCAN algorithm...";
    start = std::chrono::system_clock::now();
    graph.DBSCAN(epsilon, M);
    end = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
    std::cerr<<"elapsed time: " <<elapsed_time << "ms\n"<<std::endl;
    std::cerr<<"Outputing edge list..."<<std::endl;
    graph.output_edgelist(fileroot);
    std::cerr<<"Outputing scc..."<<std::endl;
    graph.output_scc(fileroot);
    std::cerr<<"Outputing DBSCAN..."<<std::endl;
    graph.output_dbscan(fileroot);
    return 0;
}
