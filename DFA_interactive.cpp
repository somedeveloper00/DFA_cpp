#include "DFA_interactive.hpp"
#include "string_utils.hpp"

#include <iostream>
#include <string>

DFA_interactive::DFA_interactive(DFA* dfa) {
    this->dfa = dfa;
}

void DFA_interactive::edit_interactive()
{
    while (1) {
        std::cout << "command (h: help): ";
        std::string cmd;
        std::getline(std::cin, cmd);
        if (compile_interactive(cmd) == true) {
            break;
        }
    }

    std::cout << "\n";
    std::cout << "ALRIGHT!\n";
}

bool DFA_interactive::compile_interactive(std::string text) {

    std::vector<std::string> lines = string_utils::split(text, "\n");

    for (std::string cmd : lines) {

        if (cmd.size() == 0) {
            continue;
        }
        
        std::vector<std::string> cmds = string_utils::split(cmd, " ");

        if (cmds.size() == 1 && cmds[0] == "d") {
            return true;
        }

        if (cmds.size() == 1 && cmd == "h") {
            this->print_help();
            continue;
        }
        if (cmds.size() == 1 && cmd == "p") {
            std::cout << dfa->get_printable_str() << "\n";
            continue;
        }
        if (cmds.size() == 2 && cmds[0] == "f") {
            DFA_State* state = this->resolve_get_state(cmds[1]);
            state->isAccept = true;
            continue;
        }
        if (cmds.size() == 2 && cmds[0] == "-f") {
            DFA_State* state = this->resolve_get_state(cmds[1]);
            state->isAccept = false;
            continue;
        }
        if (cmds.size() == 2 && cmds[0] == "s") {
            DFA_State* state = this->resolve_get_state(cmds[1]);
            dfa->set_init_state(state);
            continue;
        }
        if (cmds.size() == 4 && cmds[0] == "r" && cmds[2].length() == 1) {
            DFA_State* state1 = this->resolve_get_state(cmds[1]);
            DFA_State* state2 = this->resolve_get_state(cmds[3]);

            // disconnect
            char condition = cmds[2].c_str()[0];
            dfa->disconnect(state1, state2, condition);
            
            // std::cout << "disconnected " << state1->name << " to " << state2->name << " by \"" << condition << "\n";
            continue;
        }

        if (cmds.size() == 3 && cmds[1].length() == 1) {
            DFA_State* state1 = this->resolve_get_state(cmds[0]);
            DFA_State* state2 = this->resolve_get_state(cmds[2]);

            // connect
            char condition = cmds[1].c_str()[0];
            dfa->connect(state1, state2, condition);
            // std::cout << "connected " << state1->name << " to " << state2->name << " by \'" << condition << "\'\n";
            continue;
        }

        std::cout << "wrong input at \"" << cmd << "\"\n";
        break;
    }
    return false;
}

void DFA_interactive::print_help() {
    std::cout 
        << "\n"
        << "  commands:\n"
        << "     d: finishes the interactive DFA\n"
        << "     h: prints this help\n"
        << "     p: prints the current DFA\n"
        << "     f [State]: marks the given state as an accapring state\n" 
        << "     -f [State]: unmarks the given state as an accapring state\n" 
        << "     s [State]: marks the given state as the inital state (by default the first\n" 
        << "                created state is the initial state)\n"
        << "     [State] [Condition] [State]: selects the first state and connects it to the\n" 
        << "                                  second state with the condition\n"
        << "     r [State] [Condition] [State]: removes the node/connection between the two\n" 
        << "                                    states with thespecified condition\n"
        << "  definitions:\n" 
        << "     [State]: selects a state if already exists, otherwise creates a new state.\n"
        << "     [Condition]: a character that is used to transition from one state to another\n"
        << "\n"
    ;
}

DFA_State* DFA_interactive::resolve_get_state(std::string name) {
    DFA_State* rstate = nullptr;
    for (DFA_State* state : dfa->states) {
        if (state->name == name) {
            rstate = state;
            break;
        }
    }
    if (rstate == nullptr) {
        rstate = new DFA_State(name);
        dfa->add_state(rstate);
    }
    return rstate;
}
