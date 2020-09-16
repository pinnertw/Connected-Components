#include<iostream>
#include "graph.hpp"

int main(){
    int n=8;
    double dist=1.0;
    //DBSCAN
    double epsilon=1.5;
    int M=1;
    std::cout<<"First example"<<std::endl;
    Graph* first_exemple=new Graph(n);
    
    first_exemple->set_node_neighbor(0,1,dist);
    first_exemple->set_node_neighbor(1,2,dist);
    first_exemple->set_node_neighbor(2,0,dist);
    first_exemple->set_node_neighbor(3,1,dist);
    first_exemple->set_node_neighbor(3,2,dist);
    first_exemple->set_node_neighbor(3,4,dist);
    first_exemple->set_node_neighbor(4,3,dist);
    first_exemple->set_node_neighbor(4,5,dist);
    first_exemple->set_node_neighbor(5,2,dist);
    first_exemple->set_node_neighbor(5,6,dist);
    first_exemple->set_node_neighbor(6,5,dist);
    first_exemple->set_node_neighbor(7,4,dist);
    first_exemple->set_node_neighbor(7,6,dist);
    first_exemple->set_node_neighbor(7,7,dist);
    std::cout<<"ok_construction"<<std::endl;
    std::cout<<"Tarjan proceeding ..."<<std::endl;
    first_exemple->Tarjan();
    //display
    std::cout<<"Tarjan succeeded !"<<std::endl;
    first_exemple->affichage_Tarjan();
    std::cout<<"DBSCAN proceeding ..."<<std::endl;
    std::cout<<"paramètre epsilon = "<<epsilon<<std::endl;
    std::cout<<"paramètre M = "<<M<<std::endl;
    first_exemple->DBSCAN(epsilon, M);
    //display
    std::cout<<"DBSCAN succeeded !"<<std::endl;
    first_exemple->affichage_DBSCAN();
    //heuristiques sur les paramètres
    std::cout<<"Taux de points cohérents par rapport à epsilon = "<<first_exemple->rate_of_epsilon(epsilon)<<std::endl;
    std::cout<<"Taux de points cohérents par rapport à M en fonction de epsilon = "<<first_exemple->rate_of_M(M,epsilon)<<std::endl;

    //random digraph
    std::cout<<"Second example"<<std::endl;
    double p=0.2;
    std::cout<<"Paramètre p = "<<p<<std::endl;
    Graph* second_exemple=new Graph(n,p);
    std::cout<<"ok_construction"<<std::endl;
    std::cout<<"Tarjan proceeding ..."<<std::endl;
    second_exemple->Tarjan();
    //display
    std::cout<<"Tarjan succeeded !"<<std::endl;
    second_exemple->affichage_Tarjan();
    std::cout<<"DBSCAN proceeding ..."<<std::endl;
    std::cout<<"paramètre epsilon = "<<epsilon<<std::endl;
    std::cout<<"paramètre M = "<<M<<std::endl;
    second_exemple->DBSCAN(epsilon, M);
    //display
    std::cout<<"DBSCAN succeeded !"<<std::endl;
    second_exemple->affichage_DBSCAN();
    delete first_exemple;
    delete second_exemple;
    return 0;

}
