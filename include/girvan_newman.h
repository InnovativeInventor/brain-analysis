
#include "graph.h"

#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <limits>

using std::map;
using std::unordered_map;
using std::pair;
using std::vector;
using std::stack;
using std::queue;
using std::numeric_limits<double>::max()





// calc edge betweenness centrality
template <typename V, typename E>
map<pair<V, V>, double> * Graph<V, E>::brandes(Graph<V, E> & updated_g){
    map<pair<V, V>, double> cb;
    // fill Cb(edge) with 0
    for (auto it = updated_g.edges.begin(); it != updated_g.edges.end(); ++it){
        cb[it->first] = 0; 
    }

    //loop through each vertices:
    for (auto it = updated_g.vertices.begin(); it != updated_g.vertices.end(); ++it){
        stack<V> S;
        queue<V> Q;
        map<V, vector<V>> Pred;
        map<V, double> sig_st;
        sig_st[it->first] = 1.0;
        map<V, double> dist;
        dist[it->first] = 0.0;

        Q.push(it->first);
        while(!Q.empty()){
            V curr_vertex = Q.front();
            Q.pop();
            S.push(curr_vertex);

            E min_w = max();
            vector<V> shortest_nbr;
            // loop through neighbors of curr_vertex:
            for (pair<V, V>& neighbors : (updated_g.vertices[curr_vertex]).vertex_edges){
                V nbr = neighbors.second;
                if (dist.find(nbr) == dist.end()){
                    // nbr node first discovered
                    Q.push(nbr);
                    E edge_weight = updated_g.get_edge(nbr, curr_vertex);
                    dist[nbr] = dist[curr_vertex] + edge_weight;
                    if (edge_weight < min_w){
                        min_w = edge_w;
                        shortest_nbr.clear();
                        shortest_nbr.push_back(nbr);
                    } else if (edge_weight == min_w){
                        shortest_nbr.push_back(nbr);
                    }
                }
            }
            //loop through each shortest node:
            for (V & s_nbr: shortest_nbr){
                // update sig;
                if (sig_st.find(s_nbr) == sig_st.end()){
                    sig_st[s_nbr] = sig_st[curr_vertex];
                } else{
                    sig_st[s_nbr] += sig_st[curr_vertex];
                }
                // update predecessor
                if (Pred.find(s_nbr) == Pred.end()){
                    Pred[s_nbr] = vector<V>();
                }
                Pred[s_nbr].push_back(curr_vertex); 
            }
        }

        // back trace, start with filling delta with 0
        map<V, double> delta_prop;
        for (pair<V, double> p: dist){
            delta_prop[p->first] = 0;
        }

        while(!S.empty())){
            V curr_node = S.top();
            S.pop();
            for (V pred_nbr: Pred[curr_node]){
                double c = (sig_st[pred_nbr]/sig_st[curr_node]) * (1 + delta_prop[curr_node]);
                delta_prop[pred_nbr] += c;
                if (cb.find({curr_node, pred_nbr}) != cs.end()){
                    // because we have an undirected graph
                    cb[{curr_node, pred_nbr}] += c; 
                }else{
                    cb[{pred_nbr, curr_node}] += c;
                }
                
            }
        }
    }

    return cb;
}


// find communities
template <typename V, typename E>
unordered_map<V, int> Graph<V, E>::find_communities(Graph<V,E> & updated_g){
    // weighted graph: m is also weighted.
    unordered_map<typename V, int> membership;
    int next_grp = 0;

    unordered_map<V, VertexInfo>::iterator it;
    for(it = updated_g.vertices.begin(); it != updated_g.vertices.end(); ++it){
        bool found_grp = false;
        for(pair<V, V> p :(it->second).vertex_edges){
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
    return membership;
}

template <typename V, typename E>
double Graph<V, E>::get_m(){
    // remember to store it somewhere
    double total_m = 0.0;
    unordered_map<pair<V, V>, EdgeInfo>::iterator it;
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
double Graph<V, E>::modularity(Graph<V,E> & updated_g){
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
    Graph<V, E> copy_graph;
    copy_graph.vertices = vertices;
    copy_graph.edges = edges;
    double new_mod = 0.0;
    while (new_mod < 0.5) {
        map<pair<V, V>, double> brandes_map = brandes(copy_graph);
        pair<V, V> max_val;
        double val = -1;
        for (auto it = brandes_map.begin(); it != brandes_map.end(); ++it) {
            if ((it->second) > val) {
                val = it->second;
                max_val = it->first;
            }
        }
        copy_graph.remove_edge(max_val.first, max_val.second);
        new_mod = modularity(copy_graph);
    }
    return copy_graph;
}

// dump the edgelist
template <typename V, typename E>
void Graph<V, E>::write_edge_list(Graph<V,E> & updated_g){

}



