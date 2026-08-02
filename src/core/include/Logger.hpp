#pragma once

#include <vector>
#include <string>
#include <chrono>

#include "UserOutput.hpp"
#include "ErrorCodes.hpp"

class UserOutput;

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

        View getView() const;
        std::chrono::system_clock::time_point getMessageTime() const;
        ErrorCode getErrorCode() const;
};

class Logger 
{
    private:
        
        const UserOutput* outputInterface;

        std::vector<LoggerMessage> Log;
        std::vector<LoggerMessage>::iterator unprocessedIterator;

        void onMessage();

    public:
        Logger();

        void setOutputInterface(const UserOutput* ob);
        bool hasOutputInterface() const;

        void log(LoggerMessage::View msgv, ErrorCode ec);
};