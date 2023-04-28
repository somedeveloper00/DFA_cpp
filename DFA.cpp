#include "DFA.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void DFA::add_state(DFA_State* state) { this->states.push_back(state); }
void DFA::connect(DFA_State* from, DFA_State* to, char condition)
{
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
    std::string str = "";
    for (DFA_State* state : this->states) {
        str += "state: " + state->name + (state->isAccept ? " (accept)" : "") + "\n";
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

bool DFA::is_empty() {
    return this->states.size() == 0;
}

bool __is_infinite(std::vector<DFA_State*> record) {

    auto current_state = record[record.size()-1];
    bool is_failure = false; // if this is a failure state (all nodes end to itself)
    bool has_selfloop = false; // it contains at least one self-loop node 
    
    for(auto node : current_state->nodes_next) {

        auto state = node->state_to;
        if (state != current_state) {
            // check if inside record ( meaning a loop somewhere )
            if (std::find(record.begin(), record.end(), state) != record.end()) {
                return true;
            }
            is_failure = false;
        } else {
            has_selfloop = true;
        }

        // if has self-connecting node and is not failure, it makes the DFA infinite
        if (has_selfloop && !is_failure) {
            return true;
        }

        record.push_back(state);
        if (__is_infinite(record)) {
            return true;
        }
        record.erase(record.begin() + record.size()-1);
    }
    return false;
}

bool DFA::is_infinite() {
    std::vector<DFA_State*> record;
    record.push_back(this->states[0]);
    return __is_infinite(record);
}
void DFA::set_init_state(DFA_State* state) {
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

DFA_State::DFA_State(std::string name)
{
    this->name = name;
}