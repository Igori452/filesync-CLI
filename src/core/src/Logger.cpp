#include "../include/Logger.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

LoggerMessage::LoggerMessage(MessageStatus st, MessageView msgv, std::string msg) 
: status(st), view(msgv), message(std::move(msg)),
  messageTime(std::chrono::system_clock::now()) {}

MessageStatus LoggerMessage::getStatus() const 
{
    return status;
}

MessageView LoggerMessage::getView() const 
{
    return view;
}

std::chrono::system_clock::time_point LoggerMessage::getMessageTime() const 
{
    return messageTime;
}

std::string LoggerMessage::getMessage() const 
{
    return message;
}

std::string LoggerMessage::formatText() const
{
    std::string line;
    switch (status)
    {
        case MessageStatus::INFO:
            line = "[INFO] - ";
            break;
        case MessageStatus::WARNING:
            line = "[WARNING] - ";   
            break;
        case MessageStatus::ERROR:
            line = "[ERROR] - ";
            break;
    }

    std::time_t tt = std::chrono::system_clock::to_time_t(messageTime);
    std::tm tm = *std::localtime(&tt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    line += oss.str() + " : ";

    line += message + "\n";

    return line;
}

Logger::Logger() : observer(nullptr), firstUnprocessed(0) {}

Logger& Logger::instance() 
{
    static Logger logger;
    return logger;
}

void Logger::onMessage() 
{
    if (observer == nullptr)
        return;
    
    if (firstUnprocessed != Log.size()) 
    {
        for (; firstUnprocessed < Log.size(); ++firstUnprocessed) 
        {
            observer->display(Log[firstUnprocessed]);
        }
    }
}

LoggerSubscription Logger::subscribe(const UserOutput* ob) 
{
    observer = ob;

    if (firstUnprocessed != Log.size()) 
    {
        onMessage();
    }

    return LoggerSubscription(this);
}

void Logger::unsubscribe() 
{
    if (observer != nullptr)
    {
        observer = nullptr;
    }
}

bool Logger::hasObserver() const 
{
    return (observer == nullptr ? false : true);
}

void Logger::log(MessageStatus st, MessageView v, std::string_view msg) 
{
    LoggerMessage loggerMeassage(st, v, std::string(msg));

    Log.emplace_back(std::move(loggerMeassage));

    onMessage();
}

LoggerSubscription::LoggerSubscription(Logger* lg) : logger(lg) {}

LoggerSubscription::~LoggerSubscription() 
{
    if (logger != nullptr)
    {
        logger->unsubscribe();
    }
}