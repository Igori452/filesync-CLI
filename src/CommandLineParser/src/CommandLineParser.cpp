#include "CommandLineParser/include/CommandLineParser.hpp"
#include "Core/include/ErrorCodes.hpp"

CommandLineParser::ParseResult CommandLineParser::parseInputInstructions(int argc, char* argv[])
{

    if (argc <= 1) 
    {
        return {make_error_code(CommandLineParserError::COMMAND_IS_EMPTY), {}};
    }

    const auto cmdParse {parseCommand(argv[1])};

    if (!cmdParse.has_value())
    {
        return {make_error_code(CommandLineParserError::INVALID_COMMAND), {}};
    } 

    InputCommand inputCommand {};
    inputCommand.command = cmdParse.value();
    
    size_t inputArgumentsCount = commandRegistry.at(inputCommand.command).argumentsCount;
    if (inputArgumentsCount > argc - 2)
    {
        return {make_error_code(CommandLineParserError::INVALID_ARGUMENT), {}};
    }

    for (size_t i = 0; i < inputArgumentsCount; ++i) 
    {
        inputCommand.arguments.emplace_back(argv[i + 2]);
    }

    size_t it = 2 + inputArgumentsCount;
    while (it < argc)
    {
        const auto optParse {parseOption(argv[it++], inputCommand.command)};
        if(!optParse.has_value())
        {
            return {make_error_code(CommandLineParserError::INVALID_OPTION) , {}};
        }
        inputCommand.options.emplace_back(optParse.value());
    }
    
    return {make_error_code(CommandLineParserError::PARSE_SUCCESSFUL), std::move(inputCommand)};
}

std::optional<Commands> CommandLineParser::parseCommand(std::string_view command) 
{

    for (const auto& item : commandRegistry) 
    {
        if (item.second.commandName == command)
        {
            return item.first;
        }
    }

    return std::nullopt;
}

std::optional<Options> CommandLineParser::parseOption(std::string_view option, Commands cmd) 
{
    const auto& commandOptions = commandRegistry.at(cmd).options;

    for (Options opt : commandOptions) 
    {
        if (optionRegistry.at(opt).optionName == option)
        {
            return opt ;
        }
    }

    return std::nullopt;
}