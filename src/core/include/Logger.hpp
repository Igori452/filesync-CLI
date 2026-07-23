#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <variant>
#include <type_traits>

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

template<typename ErrorEnum>
concept isEnum = std::is_enum_v<ErrorEnum>;

class LoggerMessage 
{
    public:
        enum class MessageView      {SYSTEM, USER};
        enum class MessageStatus    {INFO_MSG, SUCCESS_MSG, WARNING_MSG, ERROR_MSG};

    private:
        MessageView view;

        std::chrono::system_clock::time_point messageTime;

        MessageStatus status;
        std::string message;

        MessageStatus converStatusFromError(ErrorStatus es) const;

    public:
        LoggerMessage() = delete;
        explicit LoggerMessage(MessageView msgv, std::string msg);

        template<isEnum ErrorEnum>
        explicit LoggerMessage(MessageView msgv, ErrorEnum ec);

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

        template<isEnum ErrorEnum>
        void log(LoggerMessage::MessageView msgv, ErrorEnum ec);
};

template<isEnum ErrorEnum>
LoggerMessage::LoggerMessage(MessageView msgv, ErrorEnum ec) 
: view(msgv), messageTime(std::chrono::system_clock::now()) 
{
    ErrorCode error {ec};

    message = error.message();
    status = this->converStatusFromError(error.getStatus());
}

template<isEnum ErrorEnum>
void Logger::log(LoggerMessage::MessageView msgv, ErrorEnum ec) 
{
    Log.emplace_back(msgv, ec);

    onMessage();
}