#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "core/include/Instructions.hpp"

class Application 
{

    private:

        InputCommand inputCommand;

        void compare();
        void sync();
        void verify();
        void status();
        void help();
        void version();

        const std::map<Commands, void (Application::*)()> applicationActions 
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
        Application(InputCommand command);

        // выбор нужной команды
        void executeInputCommand();

};

#endif