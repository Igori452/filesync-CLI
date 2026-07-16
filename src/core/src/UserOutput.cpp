#include "../include/UserOutput.hpp"

#include <iostream>

UserOutput::UserOutput(OutputUserSettings st) : settings(std::move(st)) {}

ConsoleOutput::ConsoleOutput(OutputUserSettings st) : UserOutput(std::move(st)) {}

void ConsoleOutput::display() const
{



}