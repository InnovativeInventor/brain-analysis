
#include <map>
#include <unordered_map>
using std::map;
using std::unordered_map;
#include "graph.h"



// calc edge betweenness centrality
// template
template <typename V, typename E>
double * Graph<V, E>::brandes(){

}


// find communities
template <typename V, typename E>
unordered_map<V, int> Graph<V, E>::find_communities(Graph<V,E> * updated_g){
    // weighted graph: m is also weighted.
    unordered_map<typename V, int> membership;
    int next_grp = 0;

    unordered_map<V, VertexInfo>::iterator it;
    for(it = vertices.begin(); it != vertices.end(); ++it){
        bool found_grp = false;
        for(std::pair<V, V> p :(it->second).vertex_edges){
            if (get_edge(p.first, p.second) != {}){
                if (membership.at(p.second) != membership.end()){
                    membership[it->first] = membership.at(p.second);
                    found_grp = true;
                    break;
                }
            }
        }
        if (!found_grp){
            membership[it->first] = next_grp;
            ++next_grp;
        }
        
    }
}

template <typename V, typename E>
double Graph<V, E>::get_m(){
    // remember to store it somewhere
    double total_m = 0.0;
    unordered_map<std::pair<V, V>, EdgeInfo>::iterator it;
    for (it = edges.begin(); it != edges.end(); ++it){
        total_m+= it->first;
    }
    return total_m;
}


template <typename V, typename E>
double Graph<V, E>::get_weighted_k(const V& node){
    double sum = 0.0;
    for(std::pair<V, V> p:node.vertex_edges){
        sum += get_edge(p.first, p.second).e;
    }
    return sum;
}


// calc modularity
template <typename V, typename E>
double Graph<V, E>::modularity(Graph<V,E> * updated_g){
    // weighted graph: m is also weighted.
    unordered_map<typename V, int> grp_membership = find_communities(updated_g);
    double m = get_m();

    double mod = 0.0;

    std::unordered_map<V, VertexInfo>::iterator it_i;
    for (it_i = updated_g.vertices.begin(); it_i != updated_g.vertices.end(); ++it_i){
        std::unordered_map<V, VertexInfo>::iterator it_j;
        for (it_j = updated_g.vertices.begin(); it_j != updated_g.vertices.end(); ++it_j){
            if (it_i->first == it_j->first){break;}

            std::optional<E> A = get_edge(it_i->first, it_j->first);
            if (A == {}){ A = 0;} // no edge then A is 0

            short s = grp_membership.at(it_i->first) == grp_membership.at(it_j->first) ? 1 : -1;
            mod += s * (A - (get_weighted_k(it_i->first) * get_weighted_k(it_j->first)/(2 * m)));

        }
    }
    return mod/(2 * m); // possibly 4m?

}


// main girvan_newman algorithm.
template <typename V, typename E>
Graph<V,E> * Graph<V,E>::girvan_newman(){

}

// dump the edgelist
template <typename V, typename E>
void Graph<V, E>::write_edge_list(Graph<V,E> * updated_g){

}



