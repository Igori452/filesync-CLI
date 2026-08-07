#include "Logger/include/LoggerMessage.hpp"

LoggerMessage::LoggerMessage(View msgv, ErrorCode ec) 
: view(msgv), messageTime(std::chrono::system_clock::now()),
   errorCode(std::move(ec)) {}

LoggerMessage::LoggerMessage(View msgv, std::error_code ec) 
: view(msgv), messageTime(std::chrono::system_clock::now()),
   errorCode(std::move(ErrorCode {ec})) {}
  
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

std::ostream& operator<<(std::ostream& ostr, const LoggerMessage& lgmsg) 
{
    ostr << LoggerMessageFormater::getTextFromLoggerMessage(lgmsg);
    return ostr;
}

std::string LoggerMessageFormater::getTextFromLoggerMessage(const LoggerMessage& lgmsg) 
{

    std::time_t time_now = std::chrono::system_clock::to_time_t(lgmsg.getMessageTime());
    std::tm* local_time = std::localtime(&time_now);
    
    std::stringstream ss;
    ss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    std::string formatMessage = " " + ss.str() + " ";

    switch (lgmsg.getView())
    {
        case LoggerMessage::View::SYSTEM:
            formatMessage += "[ SYSTEM ] ";
            break;
        case LoggerMessage::View::USER:
            formatMessage += "[  USER  ] ";
            break;
    }

    std::string status = "[         ]";
    ErrorCode err = lgmsg.getErrorCode();
    switch (err.getStatus())
    {
        case ErrorStatus::SUCCESSFUL:
            status = "[ SUCCESS ] ";
            break;

        case ErrorStatus::WARNING:
            status = "[ WARNING ] ";
            break;

        case ErrorStatus::ERROR:
            status = "[  ERROR  ] ";
            break;
    }

    return formatMessage + status + err.getErrorCode().message();
}