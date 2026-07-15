#include "../include/Application.hpp"

Application::Application(InputCommand command) 
    : inputCommand(std::move(command)) {}

void Application::executeInputCommand() 
{

    const auto action = applicationActions.at(inputCommand.command);
    (this->*action)();

}

void Application::compare() 
{

    

}

//void Application::sync();
//void Application::verify();
//void Application::status();
//void Application::help();
//void Application::version();