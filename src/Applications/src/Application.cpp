#include "Applications/include/Application.hpp"
#include "core/include/ModuleSettings.hpp"
#include "Scanner/include/Scanner.hpp"
#include "Index/include/Index.hpp"

Application::Application(InputCommand command, UserInterface interface) 
    : inputCommand(std::move(command)) 
    {
       switch (interface)
       {
            case UserInterface::CONSOLE:
                OutputUserSettings settings {};
                settings.setSettings(command);

                output = std::make_unique<ConsoleOutput>(std::move(settings));
                break;
       }
    }

bool Application::executeInputCommand() 
{
    const auto action = applicationActions.at(inputCommand.command);
    return (this->*action)();
}

bool Application::compare() 
{
    Scanner scanner;

    std::filesystem::path path {inputCommand.arguments.at(0)};

    if (!scanner.setPathData(path)) 
    {
        // 
    }

    Index index(scanner.getPathData());

    // index1 == index2?
}

//bool Application::sync();
//bool Application::verify();
//bool Application::status();
//bool Application::help();
//bool Application::version();