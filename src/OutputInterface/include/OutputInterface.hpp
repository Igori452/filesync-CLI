#pragma once

#include <iostream>

#include "Core/include/Settings/Settings.hpp"
#include "Logger/include/LoggerMessage.hpp"

enum class UserInterface {CONSOLE /*,UI, WEB*/};

class OutputInterface 
{
    protected:
        const Settings& settings;

    public:
        OutputInterface() = delete;
        explicit OutputInterface(const Settings& st);

        virtual void display(const LoggerMessage& lgmsg) const = 0;
        virtual ~OutputInterface() = default;

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

class ConsoleOutput : public OutputInterface 
{
    private:
        struct Color {
            static constexpr std::string_view RESET   = "\033[0m";
            static constexpr std::string_view RED     = "\033[31m";
            static constexpr std::string_view GREEN   = "\033[32m";
            static constexpr std::string_view YELLOW  = "\033[33m";
        };
    
        std::ostream& console;

    public:
        explicit ConsoleOutput(const Settings& st, std::ostream& out = std::cout);

        void display(const LoggerMessage& lgmsg) const override;

};