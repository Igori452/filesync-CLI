#include "CommandLineParser/include/CommandLineParser.hpp"
#include "core/include/Logger.hpp"
#include "core/include/ErrorCodes.hpp"

std::optional<InputCommand> CommandLineParser::parseInputInstructions(int argc, char* argv[])
{

    InputCommand inputCommand {};

    if (argc <= 1) 
    {
        Logger::instance().log(
            LoggerMessage::MessageView::SYSTEM, 
            CommandLineParserError::COMMAND_IS_EMPTY
        );
        return {};
    }

    if (!parseCommand(argv[1], inputCommand))
    {
        Logger::instance().log(
            LoggerMessage::MessageView::SYSTEM, 
            CommandLineParserError::INVALID_COMMAND
        );
        return {};
    }

    size_t inputArgumentsCount = commandRegistry.at(inputCommand.command).argumentsCount;
    if (inputArgumentsCount > argc - 2)
    {
        Logger::instance().log(
            LoggerMessage::MessageView::SYSTEM, 
            CommandLineParserError::INVALID_ARGUMENT
        );
        return {};

    }

    for (size_t i = 0; i < inputArgumentsCount; ++i) 
    {
        parseArguments(argv[2 + i], inputCommand);
    }

    // Парсинг без аргументов опций
    size_t it = 2 + inputArgumentsCount;
    while (it < argc)
    {

        if(!parseOption(argv[it++], inputCommand))
        {
            Logger::instance().log(
                LoggerMessage::MessageView::SYSTEM, 
                CommandLineParserError::INVALID_OPTION
            );
            return {};
        }
    }
    
    Logger::instance().log(
        LoggerMessage::MessageView::SYSTEM, 
        CommandLineParserError::PARSE_SUCCESSFUL
    );
    return inputCommand;
}


bool CommandLineParser::parseCommand(std::string_view command, InputCommand& icmd) 
{

    for (const auto& item : commandRegistry) 
    {
        if (item.second.commandName == command)
        {
            icmd.command = item.first;
            return true;
        }

    }

    return false;
}

void CommandLineParser::parseArguments(std::string_view argument, InputCommand& icmd) 
{
    icmd.arguments.emplace_back(argument);
}

bool CommandLineParser::parseOption(std::string_view option, InputCommand& icmd) 
{

    const auto& commandOptions = commandRegistry.at(icmd.command).options;

    for (Options opt : commandOptions) 
    {

        if (optionRegistry.at(opt).optionName == option)
        {
            icmd.options.emplace_back(opt);
            return true;
        }

    }

    return false;
}