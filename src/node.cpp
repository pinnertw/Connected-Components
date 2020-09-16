#include "node.hpp"
#include <cfloat>

// constructor
Node::Node(){
    cluster = -1;
    out_deg = 0;
    neighbors = std::unordered_map<Id, double>();
    deja_vu = false;
    nearest_neighbor=-1;
    nearest_dis = DBL_MAX;
}

void Node::push(Id node_, double distance_){
    // push : renew nearest neighbor, then add node_ to neighbors
    if (distance_ < nearest_dis) {
        nearest_neighbor = node_;
        nearest_dis = distance_;
    }
    neighbors[node_] = distance_;
    out_deg++;
}

void Node::pop(Id node_){
    // pop a element from neighbors.
    std::unordered_map<int, double>::const_iterator find = neighbors.find(node_);
    if (find != neighbors.end()) {
        out_deg--;
        neighbors.erase(find);
    }
    else return;
}

bool Node::in(Id node_){
    // Check if a node is in the neighborhood.
    return neighbors.count(node_) > 0;
}

// Getters : nearest_neighbor, out_deg, neighbors, weight of edge
Id Node::get_nearest_neighbor(){
    return nearest_neighbor;
}

int Node::get_out_deg(){
    return out_deg;
}

double Node::get_nearest_dis(){
    return nearest_dis;
}

std::vector<Id> Node::get_neighbors(){
    int size= neighbors.size();
    std::vector<Id> _neighbors=std::vector<Id>(size);
    int index=0;
    for( const auto& n : neighbors) {
        _neighbors[index]=n.first;
        index++;
    }
    return _neighbors;
}

double Node::edge(Id id){
    if (in(id)) return neighbors[id];
    else return DBL_MAX;
}
