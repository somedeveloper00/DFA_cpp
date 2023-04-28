#include "DFA.hpp"

class DFA_interactive
{
public:
    DFA_interactive(DFA* dfa);

    /// @brief starts the interactive DFA editting process
    void edit_interactive();

    /// @brief compiles commands into DFA and returns whether or not it's done
    bool compile_interactive(std::string text);

private:
    DFA* dfa;
    void print_help();
    DFA_State* resolve_get_state(std::string name);
};