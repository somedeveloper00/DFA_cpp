#pragma once

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
    bool is_accepcted(std::string str, bool verbose);

    /// @brief returns whether or not this DFA is empty
    bool is_empty();

    /// @brief returns whether or not this DFA is infinite
    bool is_infinite();

    /// @brief sets the given state as the init state
    void set_init_state(DFA_State* state);

    /// @brief all states inside this DFA
    std::vector<DFA_State*> states;

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