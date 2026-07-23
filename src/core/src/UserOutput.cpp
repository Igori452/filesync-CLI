#include <ctime>
#include <sstream>
#include <iomanip>

#include "../include/UserOutput.hpp"

UserOutput::UserOutput(OutputUserSettings st) : settings(std::move(st)) {}

ConsoleOutput::ConsoleOutput(OutputUserSettings st, std::ostream& out) : UserOutput(std::move(st)), console(out) {}

void ConsoleOutput::display(const LoggerMessage& lgmsg) const
{
    
    if (!(lgmsg.getView() == LoggerMessage::MessageView::USER || settings.verbose())) 
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
        case LoggerMessage::MessageView::SYSTEM:
            formatMessage += "[ SYSTEM ] ";
            break;
        case LoggerMessage::MessageView::USER:
            formatMessage += "[  USER  ] ";
            break;
    }

    std::string_view color = Color::RESET;
    std::string_view status = "[         ]";
    switch (lgmsg.getStatus())
    {
        case LoggerMessage::MessageStatus::INFO_MSG:
            color = Color::WHITE;
            status = "[  INFO   ] ";
            break;
        case LoggerMessage::MessageStatus::SUCCESS_MSG:
            color = Color::GREEN;
            status = "[ SUCCESS ] ";
            break;

        case LoggerMessage::MessageStatus::WARNING_MSG:
            color = Color::YELLOW;
            status = "[ WARNING ] ";
            break;

        case LoggerMessage::MessageStatus::ERROR_MSG:
            color = Color::RED;
            status = "[  ERROR  ] ";
            break;
    }

    console << formatMessage << color << status << Color::RESET 
        << lgmsg.getMessage() << "\n";
}