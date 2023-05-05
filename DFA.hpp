#pragma once

#include <iostream>
#include <string>
#include <vector>

class DFA;
class DFA_Node;
class DFA_State;

class DFA {
public:

    /// @brief adds a state to the DFA.
    void add_state(DFA_State* state);
    
    /// @brief connects the two states with the given condition
    void connect(DFA_State* from, DFA_State* to, char condition);

    /// @brief disconnects the two states with the given condition
    void disconnect(DFA_State* from, DFA_State* to, char condition);
    
    /// @brief returns a printable form of the current DFA
    std::string get_printable_str();

    /// @brief runs the string through the automata and returns whether or not the string is accepted
    bool is_accepcted(std::string str, bool verbose = false);

    /// @brief returns whether or not this DFA is empty
    bool is_empty();

    /// @brief returns whether or not this DFA is infinite
    bool is_infinite();

    /// @brief returns all the accepting strings in this DFA. in an infinite DFA, this will only include some accepting strings
    std::vector<std::string> get_all_accepting_strings();

    /// @brief returns all the accepting strings in this DFA with the specified length
    std::vector<std::string> get_all_accepting_strings_len(int length);

    /// @brief sets the given state as the init state
    void set_init_state(DFA_State* state);

    /// @brief finds inaccaptable strings for this DFA
    std::vector<std::string> find_inaccepting_strings(int count);

    /// @brief returns all condition characters in this DFA
    std::vector<char> get_all_conditions();

    /// @brief complements this DFA
    void complement();


    /// @brief all states inside this DFA
    std::vector<DFA_State*> states;

private:
    void invalidate_caches() { 
        this->all_accept_routes_cached.clear(); 
        this->all_conditions_cached.clear(); 
    }
    void cache_all_accept_routes();
    bool all_accept_routes_are_cahced() { return all_accept_routes_cached.size() != 0; }
    void cache_all_conditions();
    bool all_conditions_are_cahced() { return all_conditions_cached.size() != 0; }
    
    std::vector<std::vector<DFA_Node*>> all_accept_routes_cached; // empty means not yet cached
    std::vector<char> all_conditions_cached;
};

class DFA_State {
public:
    DFA_State(std::string name);
    bool isAccept = false;
    std::string name;
    std::vector<DFA_Node*> nodes_next, nodes_before;
};

class DFA_Node {
public:
    DFA_State* state_from;
    DFA_State* state_to;
    char condition;
};