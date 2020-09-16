#include"graph.hpp"
#include"node.hpp"
// For random number generators
#include <random>
#include <chrono>
// For Tarjan and DBSCAN
#include <stack> 
#include <algorithm>
#include <cfloat>
// Read file
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Graph::Graph(int n_){
    n = n_;
    nodes = new Node[n_];
    partition_scc = new Id[n_];
    clusters_dbscan = new Id[n_];
    // init clusters : everyone is in a cluster with size 1, represent is themself.
    for (Id i = 0; i < n_; i++){
        partition_scc[i]=i;
        clusters_dbscan[i] = i;
    }
}

Graph::Graph(int n_, double p_){
    n = n_;
    nodes = new Node[n_];
    partition_scc = new Id[n_];
    clusters_dbscan = new Id[n_];
    // init clusters : everyone is in a cluster with size 1.
    for (Id i = 0; i < n_; i++){
        partition_scc[i]=i;
        clusters_dbscan[i] = i;
    }
    // Distribution declaration
    std::bernoulli_distribution distribution(p_);
    std::default_random_engine generator;
    // Random number generator initialization
    // generator.seed(0);
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    // Add edges
    for (int i=0; i<n_; i++){
        for (int j=0; j<n_; j++){
            if (distribution(generator)){
                nodes[i].push(j,1); // no distance implemented for random digraphs
            }
        }
    }
}

Graph::Graph(const char* file){
    std::ifstream fin(file);
    if (fin.fail()) {
        std::cout<<"Cannot read from file "<<file<<" !"<<std::endl;
        exit(1);
    }
    // Read buffer
    std::cerr<<"Reading file..."<<std::endl;
    std::string line;
    getline(fin, line);
    std::stringstream s(line);

    // Read if the graph is weighted or not
    bool weighted; // 0 -> every distance = 1
                   // 1 -> there are three numbers for every edge.
    bool directed; // 0 -> undirected
                   // 1 -> directed
    s >> weighted >> directed;

    // Read the number of the nodes/edges
    getline(fin, line);
    s = std::stringstream (line);
    int n_, m_;   // The number total of nodes, the number total of edge.
    s >> n_ >> m_;

    // Init nodes, partition, clusters
    n = n_;
    nodes = new Node[n_];
    partition_scc = new Id[n_];
    clusters_dbscan = new Id[n_];
    // init clusters : everyone is in a cluster with size 1.
    for (Id i = 0; i < n_; i++){
        partition_scc[i]=i;
        clusters_dbscan[i] = i;
    }

    std::unordered_map<int, Id> number_to_Id; // Nodes in data are not from 0 to n - 1. So we create a map to change theses numbers to 0 - n - 1.
    Id Id_temp = 0;

    // Read edges
    int from, to;
    double weight;

    // If the graph is undirected : we use bernoulli distribution to determine the direction.
    // Distribution declaration
    std::bernoulli_distribution bernoulli(0.5);
    std::default_random_engine generator;
    // Random number generator initialization
    //generator.seed(0);
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::cerr<<"Reading edges..."<<std::endl;

    for (int edge = 0; edge < m_; edge++){
        // Print processing edges
        std::cerr<<edge<<"/"<<m_<<"\r";
        if ((edge + 1) % 10000000 == 0) std::cerr<<std::endl;
        // Read from buffer
        getline(fin, line);
        s = std::stringstream (line);
        weight = 1.;
        s >> from >> to;
        if (weighted) s>>weight;

        // Map number to node.
        if (number_to_Id.find(from) == number_to_Id.end()){
            number_to_Id[from] = Id_temp;
            Id_temp++;
        }
        if (number_to_Id.find(to) == number_to_Id.end()){
            number_to_Id[to] = Id_temp;
            Id_temp++;
        }
        if (!directed && !bernoulli(generator)) nodes[number_to_Id[to]].push(number_to_Id[from], weight);
        else nodes[number_to_Id[from]].push(number_to_Id[to], weight);
    }
    fin.close();
}

Graph::~Graph(){
    delete[] nodes;
    delete[] partition_scc;
    delete[] clusters_dbscan;
}

// Reset/set member of Graph
void Graph::set_node_neighbor(Id id1,Id id2, double distance ){
    nodes[id1].push(id2,distance);
}

void Graph::reset_partition_scc(){
    for(int i=0; i<n; i++){
        partition_scc[i]=i;
    }
}

void Graph::reset_clusters_dbscan(){
    for(int i=0; i<n; i++){
        clusters_dbscan[i]=i;
    }
}

// Tarjan : processing one of Id
void Graph::parcours_Tarjan(Id id, int & num, std::stack<Id> & pile, int* & numero, int* & numero_acces ){
    // Set cluster, add Id to processing stack.
    numero[id]=num;
    numero_acces[id]=num;
    num++;
    pile.push(id);
    nodes[id].deja_vu=true;

    //depth path from id
    std::vector <Id> sons=nodes[id].get_neighbors();
    int length_sons=sons.size();
    for (int i=0; i<length_sons; i++){
        Id son=sons[i];
        if (numero[son]==-1){
            //recursive
            parcours_Tarjan(son, num, pile, numero, numero_acces);
            numero_acces[id]=std::min(numero_acces[id],numero_acces[son]);
        }
        else if (nodes[son].deja_vu) {
            numero_acces[id]=std::min(numero_acces[id],numero[son]);
        }
    }

    if (numero_acces[id]==numero[id]){ //means id is a root -> calcul of associate SCC
        Id sortie;
        do {
            sortie=pile.top();
            pile.pop();
            nodes[sortie].deja_vu=false;
            partition_scc[sortie]=id;
        }
        while (sortie!=id);
    }
}

void Graph::Tarjan(){ //fills partitition_scc without returning anything
    int num=0;
    std::stack<Id> pile;
    int* numero=new int [n];
    for (Id index=0; index<n;index++){ //initialisation numero
        numero[index]=-1;              // we expect num>=0 if there is any change
    }
    int* numero_acces=new int[n];

    for (Id id=0; id<n;id++){
        if (numero[id]==-1){//numero[id] never modified
            parcours_Tarjan(id,num,pile,numero,numero_acces);
        }
    }

    //cleaning
    delete[] numero;
    delete[] numero_acces;
}

void Graph::affichage_Tarjan(){
    for (int i=0; i<n;i++){
        std::cout<<"sommet "<<i<<" dans la SCC de racine "<<partition_scc[i]<<std::endl;
    }
}

// Get the neighborhood of id with distance < epsilon
std::vector<Id> Graph::epsilon_neighborhood(Id id, double epsilon){
    std::vector <Id> sons=nodes[id].get_neighbors();
    std::vector<Id> epsilon_neighbors;
    int num_neighbors=sons.size();
    Id son;
    for (int i=0; i<num_neighbors; i++){
        son=sons[i];
        if(nodes[id].edge(son)<=epsilon){
            epsilon_neighbors.push_back(son);
        }
    }
    return epsilon_neighbors;
}

// add sons to stack
void Graph::fill_stack(std::vector<Id> sons, std::stack<Id>& pile){
    int size=sons.size();
    for (int i=0; i<size; i++){
        pile.push(sons[i]);
    }
}

void Graph::extend_cluster(Id id,std::stack<Id> pile, double epsilon, int M){
    Id son;
    while(!pile.empty()){
        son=pile.top();
        pile.pop();
        if (!nodes[son].deja_vu){
            nodes[son].deja_vu=true;
            std::vector<Id> sons_of_son=epsilon_neighborhood(son,epsilon);
            int size=sons_of_son.size();
            if(size>=M){  
                //depth path                    // a node is in a cluster if and only if core point
                fill_stack(sons_of_son,pile);  // ie at least M points around itself with distance <=epsilon
                if (clusters_dbscan[son]==son){
                    clusters_dbscan[son]=id;
                }
            }
        }
    }
}

void Graph::DBSCAN(double epsilon, int M){
    // Initialization
    for (Id id = 0; id<n; id++){
        clusters_dbscan[id] = id;
    }
    for (Id id=0;id<n;id++){
        nodes[id].deja_vu=true;
        std::vector<Id> epsilon_sons=epsilon_neighborhood(id,epsilon);
        int size_eps_sons=epsilon_sons.size();
        if (size_eps_sons>=M){
            std::stack<Id> pile;
            fill_stack(epsilon_sons,pile);
            extend_cluster(id,pile,epsilon,M);
        }
    }
    for (Id id=0;id<n;id++){
        nodes[id].deja_vu=false;
    }
}

void Graph::affichage_DBSCAN(){
    for (int i=0; i<n;i++){
        std::cout<<"sommet "<<i<<" dans le cluster de racine "<<clusters_dbscan[i]<<std::endl;
    }
}
double Graph::rate_of_epsilon(double epsilon){
    double rate =0;
    Node point;
    for (Id i=0; i<n; i++){
        point=nodes[i];
        if (point.edge(point.get_nearest_neighbor()) <epsilon){
            rate+=1;
        }
    }
    return rate/n;

}

double Graph::heuristic_epsilon(double rate){//really depends on the edges in the graph...
    std::vector<double> epsilon_canditates (n);
    double maximum = 0;
    for (Id i = 0; i < n; i++){
        if (nodes[i].get_nearest_neighbor() != -1) maximum = std::max(maximum, nodes[i].get_nearest_dis());
    }
    for (Id i = 0; i < n; i++){
        if (nodes[i].get_nearest_neighbor() == -1) epsilon_canditates[i] = maximum;
        else epsilon_canditates[i] = nodes[i].get_nearest_dis();
    }
    std::nth_element(epsilon_canditates.begin(), epsilon_canditates.begin() + (int) (rate * n), epsilon_canditates.end());
    return epsilon_canditates[(int) (rate * n)];
}

double Graph::rate_of_M(int M, double epsilon){
    double rate =0;
    Node point;
    for (Id i=0; i<n; i++){
        point=nodes[i];
        if (epsilon_neighborhood(i,epsilon).size() >=M){
            rate+=1;
        }
    }
    return rate/n;
}

int Graph::heuristic_M(double rate, double epsilon){ //rate generally >0.9
    std::vector<int> M_canditates (n);
    for (Id i = 0; i < n; i++){
        M_canditates[i] = epsilon_neighborhood(i, epsilon).size();
    }
    std::nth_element(M_canditates.begin(), M_canditates.begin() + (int) (rate * n), M_canditates.end());
    return M_canditates[(int) (rate * n)];
}

// Getter partition_scc
int Graph::get_n(){
    return n;
}

Id* Graph::get_partition_scc(){
    return partition_scc;
}

Id* Graph::get_clusters_dbscan(){
    return clusters_dbscan;
}

// Output functions
void Graph::output_edgelist(std::string file){
    std::ofstream output;
    output.open(file + "_edges.txt");
    for (Id i = 0; i < n; i++){
        for (auto j: nodes[i].get_neighbors()){
            output<<i<<' '<<j<<std::endl;
        }
    }
}

void Graph::output_scc(std::string file){
    std::ofstream output;
    output.open(file + "_scc.txt");
    for (Id i = 0; i < n; i++){
        output << partition_scc[i]<<std::endl;
    }
    output.close();
}

void Graph::output_dbscan(std::string file){
    std::ofstream output;
    output.open(file + "_dbscan.txt");
    for (Id i = 0; i < n; i++){
        output << clusters_dbscan[i]<<std::endl;
    }
    output.close();
}

