#include "ErrorCodes.hpp"

const char* CommandLineParserErrorProvider::name() const noexcept 
{
    return "CommandLineParser";
}

std::string CommandLineParserErrorProvider::message(CommandLineParserError code) const 
{
    switch (static_cast<int>(code))
    {
        case 100: return "Command line is missing";
        case 101: return "Invalid command";
        case 102: return "Incorrect number of arguments";
        case 103: return "Invalid option (flag)";
        case 300: return "String successfully recognized";
    }
    return "Unknown";
}