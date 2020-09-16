#include "graph.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[]){
    if ((argc != 3)){
        std::cerr<<"Usage : ./test-ER n p"<<std::endl;
        return 1;
    }

    int n = atoi(argv[1]);
    double p = atof(argv[2]);

    Graph graph = Graph(n, p);

    std::string filename = "ER_digraph_" + (std::string) argv[1] + "n" + (std::string) argv[2] + "p";
    std::string fileroot = "output/" + filename;
    std::cerr<<"Processing Tarjan algorithm on ER digraph with n = "<<n<<", p = "<<p<<"..."<<std::endl;
    graph.Tarjan();
    std::cerr<<"Outputing edge list..."<<std::endl;
    graph.output_edgelist(fileroot);
    std::cerr<<"Outputing scc..."<<std::endl;
    graph.output_scc(fileroot);
    return 0;
}
