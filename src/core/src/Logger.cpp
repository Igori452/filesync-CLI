#include "../include/Logger.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

LoggerMessage::LoggerMessage(MessageView msgv, std::string msg) 
: view(msgv), messageTime(std::chrono::system_clock::now()),
  status(MessageStatus::INFO_MSG), message(std::move(msg)) {}

LoggerMessage::MessageStatus LoggerMessage::converStatusFromError(ErrorStatus es) const 
{
    switch (es)
    {
        case ErrorStatus::WARNING: return MessageStatus::WARNING_MSG;
        case ErrorStatus::ERROR: return MessageStatus::ERROR_MSG;
        default:
            return MessageStatus::SUCCESS_MSG;
    }
}
  
LoggerMessage::MessageStatus LoggerMessage::getStatus() const 
{
    return status;
}

LoggerMessage::MessageView LoggerMessage::getView() const 
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

LoggerSubscription::LoggerSubscription(Logger* lg) : logger(lg) {}

LoggerSubscription::~LoggerSubscription() 
{
    if (logger != nullptr)
    {
        logger->unsubscribe();
    }
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

void Logger::log(LoggerMessage::MessageView msgv, std::string msg) 
{
    Log.emplace_back(msgv, msg);

    onMessage();
}