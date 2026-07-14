#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "core/include/core.hpp"

class Application 
{

    private:

        InputCommand inputCommand;

        // описание шагов отдельных команд
        void compareCommand();
        void syncCommand();

    public:

        // выбор нужной команды
        void executeInputCommand();

};

#endif