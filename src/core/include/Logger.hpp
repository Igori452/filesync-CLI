#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <vector>
#include <string>
#include <chrono>

enum class MessageStatus {INFO, WARNING, ERROR};
enum class MessageFormat {TEXT/*, JSON, BIN*/};

struct LoggerMessage 
{
    MessageStatus status;
    std::chrono::system_clock::time_point messageTime;
    std::string message;
};

class Logger 
{

    private:    

        std::vector<LoggerMessage> Log;

        Logger() = default;

        std::string formatToText() const;
        //void formatToJson() const;
        //void formatToBin() const;

    public:
        static Logger& instance();

        void log(MessageStatus st, std::string_view msg);
        
        void flush(std::ostream& out, MessageFormat format = MessageFormat::TEXT) const;

};

#endif