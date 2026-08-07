#pragma once

#include "Core/include/ErrorCodes.hpp"

#include <chrono>

class LoggerMessage 
{
    public:
        enum class View {SYSTEM, USER};

    private:
        View view;
        std::chrono::system_clock::time_point messageTime;
        ErrorCode errorCode;

    public:
        LoggerMessage() = delete;

        explicit LoggerMessage(View msgv, ErrorCode ec);
        explicit LoggerMessage(View msgv, std::error_code ec);

        View getView() const;
        std::chrono::system_clock::time_point getMessageTime() const;
        ErrorCode getErrorCode() const;

        friend std::ostream& operator<<(std::ostream& ostr, const LoggerMessage& lgmsg);
};

/* Adding support for message writing (serialization) */
std::ostream& operator<<(std::ostream& ostr, const LoggerMessage& lgmsg);

/* FACTORY */
class LoggerMessageFormater 
{
    public:
        static std::string getTextFromLoggerMessage(const LoggerMessage& lgmsg);
};