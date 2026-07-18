#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "UserOutput.hpp"

class UserOutput;
class Logger;

enum class MessageStatus    {INFO, WARNING, ERROR};
//enum class MessageFormat    {TEXT/*, JSON, BIN*/};
enum class MessageView      {SYSTEM, USER};

class LoggerMessage 
{
    private:
        MessageStatus status;
        MessageView view;

        std::string message;

        std::chrono::system_clock::time_point messageTime;

    public:
        LoggerMessage() = delete;
        explicit LoggerMessage(MessageStatus st, MessageView msgv, std::string msg);

        MessageStatus getStatus() const;
        MessageView getView() const;
        std::chrono::system_clock::time_point getMessageTime() const;
        std::string getMessage() const;

        std::string formatText() const;
        //static std::string formatJson() const;
        //static std::vector<std::byte> formatBinary() const;
};

/* RAII SUBSCRIPTION */
class LoggerSubscription 
{
    private:
        /* No one other than Logger can create a subscription. */
        friend class Logger;

        Logger* logger;

        LoggerSubscription() = delete;
        LoggerSubscription(Logger* lg);

    public:
        LoggerSubscription(const LoggerSubscription&) = delete;
        LoggerSubscription& operator=(const LoggerSubscription&) = delete;

        ~LoggerSubscription();
};

class Logger 
{
    private:
        const UserOutput* observer;

        size_t firstUnprocessed;
        std::vector<LoggerMessage> Log;

        Logger();

        void onMessage();

    public:
        static Logger& instance();

        /* Returns the subscription state that the user is required to save. */
        LoggerSubscription subscribe(const UserOutput* ob);
        void unsubscribe();
        bool hasObserver() const;

        void log(MessageStatus st, MessageView v, std::string_view msg);
        
        //void flush(std::ostream& out, MessageFormat msgf = MessageFormat::TEXT) const;

};