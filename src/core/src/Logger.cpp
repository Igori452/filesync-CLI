#include "../include/Logger.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

Logger& Logger::instance() 
{
    static Logger logger;
    return logger;
}

void Logger::log(MessageStatus st, std::string_view msg) 
{
    LoggerMessage loggerMeassage;

    loggerMeassage.status = st;
    loggerMeassage.message = msg;
    loggerMeassage.messageTime = std::chrono::system_clock::now();

    Log.emplace_back(loggerMeassage);
}

void Logger::flush(std::ostream& out, MessageFormat format) const 
{
    std::string text;
    switch (format)
    {
        case MessageFormat::TEXT:
            text = formatToText();
            break;
    }

    out << text;
}

std::string Logger::formatToText() const 
{
    std::string msg;
    for (const LoggerMessage& item : Log) 
    {
        std::string line;
        switch (item.status)
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

        std::time_t tt = std::chrono::system_clock::to_time_t(item.messageTime);
        std::tm tm = *std::localtime(&tt);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

        line += oss.str() + " : ";

        line += item.message + "\n";

        msg += line;
    }

    return msg;
}