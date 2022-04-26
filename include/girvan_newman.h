#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <limits>

#include "graph.h"

using std::unordered_map;
using std::pair;
using std::vector;
using std::stack;
using std::queue;
using std::numeric_limits;

// calc edge betweenness centrality
template <typename V, typename E>
unordered_map<pair<V, V>, E, custom_hash> Graph<V,E>::brandes(){
    unordered_map<pair<V, V>, E, custom_hash> cb;
    // fill Cb(edge) with 0
    for (auto it = edges.begin(); it != edges.end(); ++it){
        cb[it->first] = 0; 
    }

    //loop through each vertices:
    for (auto it = vertices.begin(); it != vertices.end(); ++it){
        stack<V> S;
        queue<V> Q;
        unordered_map<V, vector<V>> Pred;
        unordered_map<V, E> sig_st;
        sig_st[it->first] = 1.0;
        unordered_map<V, E> dist;
        dist[it->first] = 0.0;

        Q.push(it->first);
        while(!Q.empty()){
            V curr_vertex = Q.front();
            Q.pop();
            S.push(curr_vertex);

            E min_w = numeric_limits<E>::max();
            vector<V> shortest_nbr;
            // loop through neighbors of curr_vertex:
            for (pair<V, V>& neighbors : (vertices[curr_vertex]).vertex_edges){
                V nbr = neighbors.second;
                if (dist.find(nbr) == dist.end()){
                    // nbr node first discovered
                    Q.push(nbr);
                    E edge_weight = get_edge(nbr, curr_vertex).value();
                    dist[nbr] = dist[curr_vertex] + edge_weight;
                    if (edge_weight < min_w){
                        min_w = edge_weight;
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
        unordered_map<V, E> delta_prop;
        for (pair<V, E> p: dist){
            delta_prop[p.first] = 0;
        }

        while(!S.empty()){
            V curr_node = S.top();
            S.pop();
            for (V pred_nbr: Pred[curr_node]){
                E c = (sig_st[pred_nbr]/sig_st[curr_node]) * (1 + delta_prop[curr_node]);
                delta_prop[pred_nbr] += c;
                if (cb.find({curr_node, pred_nbr}) != cb.end()){
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
unordered_map<V, int> Graph<V,E>::find_communities(){
    // weighted graph: m is also weighted.
    unordered_map<V, int> membership;
    int next_grp = 0;

    for(auto it = vertices.begin(); it != vertices.end(); ++it){
        bool found_grp = false;
        for(pair<V, V> p :(it->second).vertex_edges){
            if (get_edge(p.first, p.second).has_value()){
                if (membership.find(p.second) != membership.end()){
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
E Graph<V,E>::get_m(){
    // remember to store it somewhere
    E total_m = 0.0;
    for (auto it = edges.begin(); it != edges.end(); ++it){
        total_m += it->second.e;
    }
    return total_m;
}


template <typename V, typename E>
E Graph<V,E>::get_weighted_k(const V& node){
    E sum = 0.0;
    for(std::pair<V, V> p : vertices.at(node).vertex_edges){
        sum += get_edge(p.first, p.second).value();
    }
    return sum;
}


// calc modularity
template <typename V, typename E>
E Graph<V,E>::modularity(){
    // weighted graph: m is also weighted.
    unordered_map<V, int> grp_membership = find_communities();
    E m = get_m();

    E mod = 0.0;

    for (auto it_i = vertices.begin(); it_i != vertices.end(); ++it_i){
        for (auto it_j = vertices.begin(); it_j != vertices.end(); ++it_j){
            if (it_i->first == it_j->first){break;}

            std::optional<E> optional_A = get_edge(it_i->first, it_j->first);
	    E A = optional_A.value_or(0.0);

            E s = grp_membership.at(it_i->first) == grp_membership.at(it_j->first) ? 1.0 : -1.0;
            mod += s * (A - (get_weighted_k(it_i->first) * get_weighted_k(it_j->first)/(2.0 * m)));

        }
    }
    return mod/(2.0 * m); // possibly 4m?

}


// main girvan_newman algorithm.
template <typename V, typename E>
void Graph<V,E>::girvan_newman(){
    E new_mod = 0.0;
    while (new_mod < 0.5) {
        unordered_map<pair<V, V>, E, custom_hash> brandes_map = brandes();
        pair<V, V> max_val;
        E val = -1.0;
        for (auto it = brandes_map.begin(); it != brandes_map.end(); ++it) {
            if ((it->second) > val) {
                val = it->second;
                max_val = it->first;
            }
        }
        remove_edge(max_val.first, max_val.second);
        new_mod = modularity();
    }
}
