#pragma once

#include <optional>
#include <vector>

#include "Core/include/Instructions.hpp"
#include <system_error>

/* FACTORY */
class CommandLineParser 
{
    private:
        static std::optional<Commands> parseCommand(std::string_view command);
        static std::optional<Options> parseOption(std::string_view option, Commands cmd);

    public:
        CommandLineParser() = delete;

        struct ParseResult 
        {
            std::error_code code;
            InputCommand inputCommand;
        };

        static ParseResult parseInputInstructions(int argc, char* argv[]);
};