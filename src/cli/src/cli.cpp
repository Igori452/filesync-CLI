#include "cli/include/CLI.hpp"

CLI::CLI() {}

bool CLI::inputInstructions(int argc, char* argv[]) 
{

    if (argc <= 1) 
    {
        // Запись в логгер, комманда отсуствует
        return false;
    }

    if (!parseCommand(argv[1]))
    {
        // Запись в логгер, неверная комманда
        return false;
    }

    size_t inputArgumentsCount = commandRegistry.at(inputCommand.command).argumentsCount;
    if (inputArgumentsCount > argc - 2)
    {
        // Запись в логгер, отсутсвуют аргументы для данной команды
        return false;

    }

    for (size_t i = 0; i < inputArgumentsCount; ++i) 
    {
        parseArguments(argv[2 + i]);
    }

    // Парсинг без аргументов опций
    size_t it = 2 + inputArgumentsCount;
    while (it < argc)
    {

        if(!parseOption(argv[it++]))
        {
            // Запись в логгер, опция для данного логера отсутсвует
            return false;
        }
    }
    
    // Запись в логгер, об успешной операции
    return true;
}


bool CLI::parseCommand(std::string_view command) 
{

    for (const auto& item : commandRegistry) 
    {
        if (item.second.commandName == command)
        {
            inputCommand.command = item.first;
            return true;
        }

    }

    return false;
}

void CLI::parseArguments(std::string_view argument) 
{
    inputCommand.arguments.emplace_back(argument);
}

bool CLI::parseOption(std::string_view option) 
{

    const auto& commandOptions = commandRegistry.at(inputCommand.command).options;

    for (Options opt : commandOptions) 
    {

        if (optionRegistry.at(opt).optionName == option)
        {
            inputCommand.options.emplace_back(opt);
            return true;
        }

    }

    return false;
}

const InputCommand CLI::releaseInputInstructions() const 
{
    return std::move(inputCommand);
}