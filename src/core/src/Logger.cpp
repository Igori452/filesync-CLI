#include "../include/Logger.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

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
            observer->display(/*DATA*/);
        }
    }
}

void Logger::subscribe(const UserOutput* ob) 
{
    observer = ob;

    if (firstUnprocessed != Log.size() - 1) 
    {
        onMessage();
    }
}

void Logger::unsubscribe() 
{
    observer = nullptr;
}

bool Logger::hasObserver() const 
{
    return (observer == nullptr ? false : true);
}

void Logger::log(MessageStatus st, MessageView v, std::string_view msg) 
{
    LoggerMessage loggerMeassage {};

    loggerMeassage.status = st;
    loggerMeassage.message = msg;
    loggerMeassage.messageTime = std::chrono::system_clock::now();

    loggerMeassage.view = v;
    loggerMeassage.userOutputProcessed = false;

    Log.emplace_back(std::move(loggerMeassage));
}

std::string LoggerFormatter::formatText(const LoggerMessage& lg) 
{
    std::string line;
    switch (lg.status)
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

    std::time_t tt = std::chrono::system_clock::to_time_t(lg.messageTime);
    std::tm tm = *std::localtime(&tt);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    line += oss.str() + " : ";

    line += lg.message + "\n";

    return line;
}