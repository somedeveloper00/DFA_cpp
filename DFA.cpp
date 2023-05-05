#include "DFA.hpp"
#include "string_utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int _get_route_search_length_limit(DFA* dfa) { return dfa->states.size() * 2 + 20; }

const int _get_max_accept_route_count(DFA* dfa) { return dfa->states.size() * dfa->get_all_conditions().size() + 20; }

DFA::DFA(DFA& other) {
    this->name = other.name;
    for (size_t i = 0; i < other.states.size(); i++) {
        this->states.push_back(new DFA_State(*other.states[i]));
    }
}

void DFA::add_state(DFA_State* state) { this->states.push_back(state); }

void DFA::connect(DFA_State* from, DFA_State* to, char condition) {
    this->invalidate_caches();
    DFA_Node* node = new DFA_Node();
    node->condition = condition;
    node->state_from = from;
    node->state_to = to;

    // remove previous matching nodes
    int c = from->nodes_next.size();
    for (size_t i = 0; i < c; i++) {
        if (from->nodes_next[i]->condition == condition) {
            from->nodes_next.erase(from->nodes_next.begin() + i);
        }
    }
    c = to->nodes_before.size();
    for (size_t i = 0; i < c; i++) {
        if (to->nodes_before[i]->condition == condition) {
            to->nodes_before.erase(to->nodes_before.begin() + i);
        }
    }
    
    from->nodes_next.push_back(node);
    to->nodes_before.push_back(node);
}

void DFA::disconnect(DFA_State* from, DFA_State* to, char condition) {
    this->invalidate_caches();
    // remove node from from-state
    for (size_t i = 0; i < from->nodes_next.size(); i++) {
        if (from->nodes_next[i]->condition == condition) {
            if (from->nodes_next[i]->state_to == to) {
                from->nodes_next.erase(from->nodes_next.begin() + i);
                i--;
            }
        }
    }
    // remove node from to-state
    for (size_t i = 0; i < to->nodes_before.size(); i++) {
        if (to->nodes_before[i]->condition == condition) {
            if (to->nodes_before[i]->state_from == from) {
                to->nodes_before.erase(to->nodes_before.begin() + i);
                i--;
            }
        }
    }
}

std::string DFA::get_printable_str() {
    std::string str = "\n--:" + this->name + "\n";
    size_t c = this->states.size();
    for (size_t i = 0; i < c; i++) {
        auto state = this->states[i];
        str += "state: " + state->name 
            + (state->isAccept ? " (accept)" : "") 
            + (i == 0 ? " (init)" : "") 
            + "\n";
        for (DFA_Node* node : state->nodes_next) {
            str += "    (";
            str += node->condition;
            str += ") => " + node->state_to->name + "\n";
        }
    }
    return str;
}

bool DFA::is_accepcted(std::string str, bool verbose) {
    DFA_State* current_state = this->states[0];
    for (char c : str) {
        if (verbose) std::cout <<  "-> " << current_state->name << " ";
        bool found = false;
        for (DFA_Node* node : current_state->nodes_next) {
            if (node->condition == c) {
                current_state = node->state_to;
                if (verbose) std::cout << "(" << c << ")";
                found = true;
                break;
            }
        }
        if (!found) {
            if (verbose) std::cout << "[not found]\n";
            return false;
        }
    }
    if (verbose) std::cout << " " << current_state->name << "\n";
    return current_state->isAccept;
}

bool DFA::is_empty() { return this->states.size() == 0; }

bool DFA::is_infinite() {
    if (!this->all_accept_routes_are_cahced()) 
        this->cache_all_accept_routes();
    return this->all_accept_routes_cached.size() == _get_max_accept_route_count(this);
}

std::string get_nodes_list_string(std::vector<DFA_Node*>& nodes) {
    std::string r;
    int c = nodes.size();
    for (auto node : nodes) r += node->condition;
    return r;
}

std::vector<std::string> DFA::get_all_accepting_strings() {
    if (!this->all_accept_routes_are_cahced())
        this->cache_all_accept_routes(); 
    
    std::vector<std::string> results;
    for(auto route : this->all_accept_routes_cached)
        results.push_back(get_nodes_list_string(route));
    return results;
}

std::vector<std::string> DFA::get_all_accepting_strings_len(int length){
    if (!this->all_conditions_are_cahced())
        this->cache_all_conditions();

    // use cached accept routes if length is sufficient
    if (length < this->all_accept_routes_cached[this->all_accept_routes_cached.size()-1].size()) {
        // get all accept routs of specified length
        std::vector<std::vector<DFA_Node*>> routes(this->all_accept_routes_cached);
        for (size_t i = 0; i < routes.size(); i++) 
            if (routes[i].size() != length) routes.erase(routes.begin()+i--);

        // convert routes to string
        std::vector<std::string> results;
        for (auto route : routes) results.push_back(get_nodes_list_string(route));
        return results;
    }

    // get all strings
    std::string str;
    for (auto c : this->all_conditions_cached) str += c;
    auto all_combs = string_utils::all_possible_strings(str, length);

    // remove those that are not accepted
    for (size_t i = 0; i < all_combs.size(); i++) {
        if (!this->is_accepcted(all_combs[i]))
            all_combs.erase(all_combs.begin() + i--);
    }
    return all_combs;
}

void DFA::set_init_state(DFA_State* state) {
    this->invalidate_caches();
    int ind = 0;
    int c = this->states.size();
    for (size_t i = 0; i < c; i++) {
        if (this->states[i] == state) {
            ind = i;
            break;
        }
    }
    auto tmp = this->states[0];
    this->states[0] = this->states[ind];
    this->states[ind] = tmp;
}

std::vector<std::string> DFA::find_inaccepting_strings(int count) {
    if (!this->all_accept_routes_are_cahced())
        this->cache_all_accept_routes();
    
    
    std::vector<std::string> results;
    std::vector<char> conditions = this->get_all_conditions();
    while (results.size() < count) {
        int rCount = this->all_accept_routes_cached.size();
        for (size_t i = 0; i < rCount; i++) {
            auto route = this->all_accept_routes_cached[i];
            std::string str = get_nodes_list_string(route);
            // trying to insert a condition character to all possible positions 
            for (auto c : conditions) {
                int route_count = route.size();
                for (size_t j = 0; j < route_count; j++) {
                    str.insert(str.begin() + j, c);
                    if (!is_accepcted(str)) {
                        results.push_back(str);
                        if (results.size() >= count) return results;
                    }
                    str.erase(str.begin() + j);
                }
            }
        }
    }
    return results;
}

std::vector<char> DFA::get_all_conditions() {
    if (!this->all_conditions_are_cahced())
        this->cache_all_conditions();
    return this->all_conditions_cached;
}

void DFA::complement() {
    for (size_t i = 0; i < this->states.size(); i++) {
        this->states[i]->isAccept = !this->states[i]->isAccept;
    }
}

void DFA::cache_all_accept_routes() {
    this->all_accept_routes_cached.clear();
    
    std::vector<std::vector<DFA_Node*>> searching_routes;
    int max_accept_routes = _get_max_accept_route_count(this);
    int max_route_length = _get_route_search_length_limit(this);

    // initialize search
    for (auto node : this->states[0]->nodes_next)
        searching_routes.push_back(std::vector<DFA_Node*>({ node }));


    while(searching_routes.size() > 0) {
        int s_count = searching_routes.size();
        while (s_count > 0) { // we'll check index 0 and delete it until s_count is finished
            // process this route
            auto head_state = searching_routes[0][searching_routes[0].size() - 1]->state_to;

            // accept check
            if (head_state->isAccept) {
                // save a copy (because we'll modify the original later on)
                this->all_accept_routes_cached.push_back(std::vector<DFA_Node*>(searching_routes[0]));
                if (this->all_accept_routes_cached.size() >= max_accept_routes) {
                    return;
                }
            }

            // continue search deeper if possible
            if (searching_routes[0].size() < max_route_length) {
                for(auto node : head_state->nodes_next) {
                    // add new routes to the list
                    std::vector<DFA_Node*> route = std::vector<DFA_Node*>(searching_routes[0]); // copy
                    route.push_back(node);
                    searching_routes.push_back(route);
                }
            }


            // remove this route
            searching_routes.erase(searching_routes.begin());
            s_count--;
        }
    }
}

void DFA::cache_all_conditions() {
    this->all_conditions_cached.clear();
    for (auto node : this->states[0]->nodes_next) {
        // add if not already added
        if (std::find(this->all_conditions_cached.begin(), this->all_conditions_cached.end(), node->condition) == this->all_conditions_cached.end()) {
            this->all_conditions_cached.push_back(node->condition);
        }
    }
}

DFA_State::DFA_State(std::string name) { this->name = name; }