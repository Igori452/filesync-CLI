#pragma once

#include <optional>

#include "core/include/Instructions.hpp"

/* FACTORY */
class CommandLineParser 
{

    private:
        static bool parseCommand(std::string_view command, InputCommand& icmd);
        static void parseArguments(std::string_view argument, InputCommand& icmd);
        static bool parseOption(std::string_view option, InputCommand& icmd);

    public:
        CommandLineParser() = delete;
        static std::optional<InputCommand> parseInputInstructions(int argc, char* argv[]);
};