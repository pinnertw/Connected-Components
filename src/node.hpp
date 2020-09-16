#pragma once
#include<unordered_map>
#include<vector>

typedef int Id;

// We can use template here, but every method should be template, too. It's far less readable.
// Example : template<typename T> node with T=int, double then replace every double below by T.
// Then in the node.cpp file we should add the same thing to every function.
class Node {
    private:
        Id cluster;                 // Id cluster
        int out_deg;                // Out degree.
        std::unordered_map<Id, double> neighbors;     // The first int : The Id of the neighbor. The second one is the distance.
        Id nearest_neighbor;        // nearest neighbor
        double nearest_dis;         // The distance to the nearest neighbor

    public:
        bool deja_vu;               // For the altorigthm
        Node();
        void push(Id, double);      // O(out_deg) if we need to resize the hash table. O(1) in average.
        void pop(Id);               // O(out_deg) if the hash table is not balanced (a lots of key get the same hash value). O(1) in average.
        bool in(Id);                // O(out_deg) if the hash table is not balanced. O(1) in average.
        std::vector<Id> get_neighbors(); // o(out_deg), returns only the ids of node's neighbors
        Id get_nearest_neighbor();  // O(1), return the id of the nearest neighbor
        double get_nearest_dis();
        int get_out_deg();          // O(1), get out degree.
        double edge(Id id);         // O(out_deg), get the length of the edge Node ->Node(id). O(1) in average.
};
