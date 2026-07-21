#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <variant>

#include "UserOutput.hpp"
#include "ErrorCodes.hpp"

class UserOutput;
class Logger;


/**
 * 
 * std::string message          ErrorCode<typename ErrorEnum, typename ErrorProvider>                        
 *        |                                                  |
 *        |                                                  |
 *        |                                                  |
 *        ————————————————————————————————————————————————————
 *                                  |
 *                              LoggerMessage
 * 
 * The class combines standard messages and application errors designed within the ErrorCode class.
 * 
 */
class LoggerMessage 
{
    public:
        enum class MessageView      {SYSTEM, USER};
        enum class MessageStatus    {INFO_MSG, SUCCESS_MSG, WARNING_MSG, ERROR_MSG};

    private:
        MessageView view;
        MessageStatus status;

        std::string message;

        std::chrono::system_clock::time_point messageTime;

        MessageStatus converStatusFromError(ErrorStatus es) const;

    public:
        LoggerMessage() = delete;
        explicit LoggerMessage(MessageView msgv, std::string msg);

        template<typename ErrorEnum, typename ErrorProvider>
        explicit LoggerMessage(MessageView msgv, const ErrorCode<ErrorEnum, ErrorProvider>& ec);

        MessageStatus getStatus() const;
        MessageView getView() const;
        std::chrono::system_clock::time_point getMessageTime() const;
        std::string getMessage() const;
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
        /*Uses an observer to which all logger records are sent. */
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

        void log(LoggerMessage::MessageView msgv, std::string msg);

        template<typename ErrorEnum, typename ErrorProvider>
        void log(LoggerMessage::MessageView msgv, const ErrorCode<ErrorEnum, ErrorProvider>& ec);
};

template<typename ErrorEnum, typename ErrorProvider>
LoggerMessage::LoggerMessage(MessageView msgv, const ErrorCode<ErrorEnum, ErrorProvider>& ec) 
: view(msgv), status(this->converStatusFromError(ec.getStatus())), message(ec.message()), 
  messageTime(std::chrono::system_clock::now()) {}

template<typename ErrorEnum, typename ErrorProvider>
void Logger::log(LoggerMessage::MessageView msgv, const ErrorCode<ErrorEnum, ErrorProvider>& ec) 
{
    Log.emplace_back(msgv, ec);

    onMessage();
}