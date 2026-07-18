#pragma once

#include <iostream>

#include "ModuleSettings.hpp"
#include "Logger.hpp"

class LoggerMessage;

enum class UserInterface {CONSOLE /*,UI, WEB...*/};

class UserOutput 
{
    protected:
        OutputUserSettings settings;

    public:
        UserOutput() = delete;
        explicit UserOutput(OutputUserSettings st);

        virtual void display(const LoggerMessage& lgmsg) const = 0;
        virtual ~UserOutput() = default;

};

/*
 * The class is designed to allow for easy testing of console output.
 *
 * Console output logger.
 * out - Stream reference. Must outlive this object.
 */
class ConsoleOutput : public UserOutput 
{
    private:
        std::ostream& console;

    public:
        explicit ConsoleOutput(OutputUserSettings st, std::ostream& out = std::cout);

        void display(const LoggerMessage& lgmsg) const override;

};