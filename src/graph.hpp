#pragma once
#include"node.hpp"
#include<stack>
#include<string>

class Graph{
    private:
        int n;                      // The number of nodes
        Node* nodes;               // Pointer of an array of pointer. We can't use Node* nodes because the nodes need Id to be initialized.
        Id* partition_scc;        // A vector to the representant of the partition
        Id* clusters_dbscan;      // A vector of the representant of the cluster with dbscan 
    public:
        Graph(int);
        Graph(int, double);
        Graph(const char* file);
        ~Graph();
        // Setting members of Graph.
        void set_node_neighbor(Id id1,Id id2, double distance );//sets the edge id1->id2
        void reset_partition_scc();
        void reset_clusters_dbscan();
        // Tarjan
        void parcours_Tarjan(Id id,int& num, std::stack<Id>& pile, int*& numero, int *& numero_acces); //path from id to its sons : recursive fonction
        void Tarjan();
        void affichage_Tarjan();
        // DBSCAN
        std::vector<Id> epsilon_neighborhood(Id id, double epsilon);// returns the ids of the nodes that are neighbors of id within a distance <epsilo,
        void fill_stack(std::vector<Id> sons, std::stack<Id>& pile); //pushes the elements of sons into the pile
        void extend_cluster(Id id,std::stack<Id> pile, double epsilon, int M); //extend the cluster from id to its adequate neighbors
        void DBSCAN(double, int);
        void affichage_DBSCAN();
        double rate_of_epsilon(double epsilon); //returns the rate of nodes which follow the condition(epsilon)
        double heuristic_epsilon(double); //returns an ideal value of epsilon with a certain rate
        double rate_of_M(int M, double epsilon); //returns the rate of nodes which follow the consition(M,epsilon)
        int heuristic_M(double rate, double epsilon); //returns an ideal value of M with a certain rate and epsilon
        // Getters
        int get_n();
        Id* get_partition_scc();
        Id* get_clusters_dbscan();
        // Output functions
        void output_edgelist(std::string file);
        void output_scc(std::string file);
        void output_dbscan(std::string file);
};
