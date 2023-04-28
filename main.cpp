#include "DFA.hpp"
#include "string_utils.hpp"
#include "DFA_interactive.hpp"

#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, char const* argv[])
{
    DFA dfa;

    std::cout << "Phase 1.\n";
    std::cout << "running DFA interactive to edit the DFA\n";
    std::cout << "\n\n";
    DFA_interactive dfa_interactive(&dfa);

    std::string cmd = 
        "q0 A q1\n"
        "q0 B q2\n"
        "q1 A q1\n"
        "q1 B q2\n"
        "f q2\n"
        "q2 A q2\n"
        "q2 B q2\n"
    ;
    dfa_interactive.compile_interactive(cmd);

    std::cout << dfa.get_printable_str() << "\n";

    std::cout << "1. " << "The DFA is " << (dfa.is_empty() ? "not empty" : "empty") << "\n";
    std::cout << "2. " << "The DFA is " << (dfa.is_infinite() ? "infinite" : "not infinite") << "\n";

    return 0;
}
