#include <ctime>
#include <sstream>
#include <iomanip>

#include "../include/OutputInterface.hpp"

OutputInterface::OutputInterface(const Settings& st) : settings(st) {}

ConsoleOutput::ConsoleOutput(const Settings& st, std::ostream& out) : OutputInterface(st), console(out) {}

void ConsoleOutput::display(const LoggerMessage& lgmsg) const
{
    
    if (!(lgmsg.getView() == LoggerMessage::View::USER || settings.verbose())) 
        return;
    
    // 14:20:01 [ SYSTEM ] [  INFO   ] Message.
    // 14:20:02 [  USER  ] [ SUCCESS ] Message.

    std::time_t time_now = std::chrono::system_clock::to_time_t(lgmsg.getMessageTime());
    
    std::tm* local_time = std::localtime(&time_now);
    
    std::stringstream ss;
    ss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    std::string formatMessage = " " + ss.str() + " ";

    switch (lgmsg.getView())
    {
        case LoggerMessage::View::SYSTEM:
            formatMessage += "[ SYSTEM ] ";
            break;
        case LoggerMessage::View::USER:
            formatMessage += "[  USER  ] ";
            break;
    }

    ErrorCode err {lgmsg.getErrorCode()};

    std::string_view color = Color::RESET;
    std::string_view status = "[         ]";
    switch (err.getStatus())
    {
        case ErrorStatus::SUCCESSFUL:
            color = Color::GREEN;
            status = "[ SUCCESS ] ";
            break;

        case ErrorStatus::WARNING:
            color = Color::YELLOW;
            status = "[ WARNING ] ";
            break;

        case ErrorStatus::ERROR:
            color = Color::RED;
            status = "[  ERROR  ] ";
            break;
    }

    console << formatMessage << color << status << Color::RESET 
        << err.getErrorCode().message() << "\n";
}