#ifndef CLI_HEADER
#define CLI_HEADER

#include "core/include/Instructions.hpp"

class CLI 
{

    private:

        InputCommand inputCommand;

        bool parseCommand(std::string_view command);
        void parseArguments(std::string_view argument);
        bool parseOption(std::string_view option);

    public:

        CLI();
        bool inputInstructions(int argc, char* argv[]) noexcept;
        const InputCommand releaseInputInstructions() const;

};

#endif