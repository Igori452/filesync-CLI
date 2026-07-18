#include "../include/UserOutput.hpp"

UserOutput::UserOutput(OutputUserSettings st) : settings(std::move(st)) {}

ConsoleOutput::ConsoleOutput(OutputUserSettings st, std::ostream& out) : UserOutput(std::move(st)), console(out) {}

void ConsoleOutput::display(const LoggerMessage& lgmsg) const
{
    if (settings.verbose())
    {
        console << lgmsg.formatText();
    }
    else if (lgmsg.getView() == MessageView::USER)
    {
        console << lgmsg.formatText();
    }
}