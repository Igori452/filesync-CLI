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
 * 
 * out — a reference to the stream. 
 * The lifetime of the stream must exceed the lifetime of this object; 
 * otherwise, undefined behavior occurs.
 */

class ConsoleOutput : public UserOutput 
{
    private:
        struct Color {
            static constexpr std::string_view RESET   = "\033[0m";
            static constexpr std::string_view RED     = "\033[31m";
            static constexpr std::string_view GREEN   = "\033[32m";
            static constexpr std::string_view YELLOW  = "\033[33m";
            static constexpr std::string_view WHITE   = "\033[37m";
        };
    
        std::ostream& console;

    public:
        explicit ConsoleOutput(OutputUserSettings st, std::ostream& out = std::cout);

        void display(const LoggerMessage& lgmsg) const override;

};