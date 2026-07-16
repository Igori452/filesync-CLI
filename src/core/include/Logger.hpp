#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "UserOutput.hpp"

enum class MessageStatus    {INFO, WARNING, ERROR};
enum class MessageFormat    {TEXT/*, JSON, BIN*/};
enum class MessageView      {SYSTEM, USER};

struct LoggerMessage 
{
    MessageStatus status;
    std::chrono::system_clock::time_point messageTime;
    std::string message;

    MessageView view;
    bool userOutputProcessed;
};

class LoggerFormatter 
{
    public:
        LoggerFormatter() = delete;

        static std::string formatText(const LoggerMessage& lg);
        //static std::string formatJson(const LoggerMessage& lg);
        //static std::vector<std::byte> formatBinary(const LoggerMessage& lg);
};

class Logger 
{

    private:
        const UserOutput* observer = nullptr;

        size_t firstUnprocessed = 0;
        std::vector<LoggerMessage> Log;

        Logger() = default;

        void onMessage();

    public:
        static Logger& instance();

        void subscribe(const UserOutput* ob);
        void unsubscribe();
        bool hasObserver() const;

        void log(MessageStatus st, MessageView v, std::string_view msg);
        
        //void flush(std::ostream& out, MessageFormat msgf = MessageFormat::TEXT) const;

};