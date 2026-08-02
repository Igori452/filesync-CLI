#include "../include/Logger.hpp"

#include <iomanip>
#include <sstream>
#include <ctime>

LoggerMessage::LoggerMessage(View msgv, ErrorCode ec) 
: view(msgv), messageTime(std::chrono::system_clock::now()),
   errorCode(std::move(ec)) {}
  
ErrorCode LoggerMessage::getErrorCode() const 
{
    return errorCode;
}

LoggerMessage::View LoggerMessage::getView() const 
{
    return view;
}

std::chrono::system_clock::time_point LoggerMessage::getMessageTime() const 
{
    return messageTime;
}

Logger::Logger() : outputInterface(nullptr) 
{
    Log.emplace_back(LoggerMessage::View::SYSTEM, ErrorCode {LoggerError::LOGGER_INIT_SUCCESSFUL});
    unprocessedIterator = std::begin(Log);
}

void Logger::onMessage() 
{
    if (outputInterface == nullptr)
        return;

    size_t ind = 0;    
    std::for_each(unprocessedIterator, std::end(Log), [&]() mutable {
        outputInterface->display(*(unprocessedIterator + ind++));
    });
    
}

void Logger::setOutputInterface(const UserOutput* ob) 
{
    outputInterface = ob;

    if (unprocessedIterator != std::end(Log)) 
    {
        onMessage();
    }
}

bool Logger::hasOutputInterface() const 
{
    return (outputInterface == nullptr ? false : true);
}

void Logger::log(LoggerMessage::View msgv, ErrorCode ec) 
{
    Log.emplace_back(msgv, ec);

    onMessage();
}