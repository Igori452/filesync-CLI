#pragma once

#include "ModuleSettings.hpp"

enum class UserInterface {CONSOLE /*,UI, WEB...*/};

class UserOutput 
{
    protected:
        OutputUserSettings settings;

    public:
        UserOutput() = delete;
        explicit UserOutput(OutputUserSettings st);

        virtual void display() const = 0;
        virtual ~UserOutput() = default;

};

class ConsoleOutput : public UserOutput 
{
    public:

        explicit ConsoleOutput(OutputUserSettings st);

        void display() const override;

};