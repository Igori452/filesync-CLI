#pragma once

#include <memory>

#include "core/include/Instructions.hpp"
#include "core/include/UserOutput.hpp"

class Application 
{

    private:
        InputCommand inputCommand;
        std::unique_ptr<UserOutput> output;

        bool compare();
        bool sync();
        bool verify();
        bool status();
        bool help();
        bool version();

        const std::map<Commands, bool (Application::*)()> applicationActions 
        {
            {Commands::COMPARE, compare},
            {Commands::SYNC, sync},
            {Commands::VERIFY, verify},
            {Commands::STATUS, status},
            {Commands::HELP, help},
            {Commands::VERSION, version},
        };

    public:
        Application() = delete;
        Application(InputCommand command, UserInterface interface = UserInterface::CONSOLE);

        // выбор нужной команды
        bool executeInputCommand();
};