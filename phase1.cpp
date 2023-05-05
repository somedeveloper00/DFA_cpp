#include <iostream>
#include <string>
#include "DFA.hpp"
#include "DFA_interactive.hpp"

void phase1() {

    std::cout << "\n";
    std::cout << "----Phase 1----\n";
    std::cout << "\n";
    
    DFA dfa("DFA1");
    DFA_interactive dfa_interactive(&dfa);

    std::cout << "we have a demo DFA, you can use that or you can create a custom DFA.\n"
                 "do you want to use the demo?[Y/N]";
    char useDemo;
    std::cin >> useDemo;

    if (useDemo == 'Y' || useDemo == 'y'    ) {
        std::string cmd = 
            "q0 A q1\n"
            "q0 B q2\n"
            "q1 A q2\n"
            "q1 B q2\n"
            "f q2\n"
            "q2 A q2\n"
            "q2 B q3\n"
            "q3 A q3\n"
            "q3 B q3\n"
        ;
        dfa_interactive.compile_interactive(cmd);
    } else {
        std::cout << "\ncreating custom DFA. entering interactive DFA-CLI creation:\n";
        dfa_interactive.print_help();
        dfa_interactive.edit_interactive();
    }

    std::cout << dfa.get_printable_str() << "\n";


    // 1
    {
        std::cout << "1. " << "The DFA is " << (dfa.is_empty() ? "empty" : "not empty") << "\n";
    }
    
    // 2
    {
        std::cout << "2. " << "The DFA is " << (dfa.is_infinite() ? "infinite" : "not infinite") << "\n";
        if (!dfa.is_infinite()) {
            std::cout << "   acceptable strings: [";
            auto strings = dfa.get_all_accepting_strings();
            for (size_t i = 0; i < strings.size(); i++) {
                std::cout << strings[i] << (i == strings.size()-1 ? "]" : ", ");
            }
            std::cout << "\n";
        }
    }

    // 3
    {
        std::string longest_str;
        for(auto string : dfa.get_all_accepting_strings()) {
            if (string.length() > longest_str.length()) longest_str = string;
        }
        std::cout << "3. longest string: " << longest_str << (dfa.is_infinite() ? "[...]" : "") <<  "\n";
    }
    
    // 4
    {
        std::string shortest_str = "                                                                               ";
        for(auto string : dfa.get_all_accepting_strings()) {
            if (string.length() < shortest_str.length()) shortest_str = string;
        }
        std::cout << "4. shortest string: " << shortest_str << "\n";
    }

    // 5
    {
        std::string input_str;
        std::cout << "5. type string to check for acceptance: ";
        std::cin >> input_str;
        std::cout << "   -> string \"" << input_str << "\" " << (dfa.is_accepcted(input_str) ? "is accepted" : "is not accepted") << "\n";
    }

    //6
    {
        auto strings = dfa.get_all_accepting_strings();
        auto non_strigns = dfa.find_inaccepting_strings(2);
        std::cout << "6. two accepting strings are: [" << strings[0] << ", " << strings[1] << "]\n";
        std::cout << "   two not accepting strings are: [" << non_strigns[0] << ", " << non_strigns[1] << "]\n"; 
    }

    //7
    {
        std::cout << "7. all acceptnig strings with length k. type in k: ";
        int k;
        std::cin >> k;
        auto all_possible_strings = dfa.get_all_accepting_strings_len(k);
        std::cout << "   -> [";
        for (size_t i = 0; i < all_possible_strings.size(); i++) {
            std::cout << all_possible_strings[i] << (all_possible_strings.size() - 1 == i ? "" : ", ");
        }
        std::cout << "]\n";
    }

    //8
    {
        std::cout << "8. number of all acceptnig strings with length k. type in k: ";
        int k;
        std::cin >> k;
        auto all_possible_strings = dfa.get_all_accepting_strings_len(k);
        std::cout << "   -> " << all_possible_strings.size() << "\n";
    }

    //9
    {
        std::cout << "9. complement of this DFA:\n";
        DFA cDfa(dfa);
        cDfa.complement();
        std::cout << cDfa.get_printable_str() << "\n";
        std::cout << dfa.get_printable_str() << "\n";
    }
    
}
