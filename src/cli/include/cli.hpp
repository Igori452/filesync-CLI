#ifndef CLI_HEADER
#define CLI_HEADER

#include "cli/include/Instructions.hpp"


class CLI {
    private:
    
        Commands command;
        std::vector<std::string> arguments;
        std::vector<Options> options;

        //parseCommand();
        //parse Arguments();
        //parse Options();
        // validate()

    public:

        CLI() = delete;
        CLI(int argc, char* argv[]);
};

#endif