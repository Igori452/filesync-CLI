#include "Logger/include/Logger.hpp"

#include <algorithm>

#include <iterator>

#include <fstream>
#include <sstream>

Logger::Logger() : outputInterface(nullptr), unprocessed(0)
{
    Log.emplace_back(LoggerMessage::View::SYSTEM, ErrorCode {LoggerError::LOGGER_INIT_SUCCESSFUL});
}

Logger::Logger(const OutputInterface* ob) : Logger()
{
    outputInterface = ob;
}

void Logger::onMessage() 
{
    if (outputInterface == nullptr)
        return;

    
    for(;unprocessed < Log.size(); ++unprocessed)
    {
        outputInterface->display(Log[unprocessed]);
    }
}

std::error_code Logger::saveLogToFile()  
{
    if (!nameLogFile.has_value()) 
    {
        auto nowDateTime {std::chrono::system_clock::now()};
        std::time_t time {std::chrono::system_clock::to_time_t(nowDateTime)};

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "log_%Y-%m-%d_%H-%M-%S.txt");
        nameLogFile = ss.str();
    }
    
    std::ofstream logFile {pathToSaveLog + nameLogFile.value(), std::ios::app};

    if (!logFile.is_open()) 
    {
        return make_error_code(LoggerError::FILE_CREATION_ERROR);
    }

    std::copy(std::begin(Log), std::end(Log), std::ostream_iterator<LoggerMessage>{logFile});

    logFile.close();

    Log.clear();
    unprocessed = 0;

    return make_error_code(LoggerError::SUCCESSFUL_SAVE_LOG);
}

void Logger::setOutputInterface(const OutputInterface* ob) 
{
    outputInterface = ob;

    if (unprocessed != Log.size()) 
    {
        onMessage();
    }
}

bool Logger::hasOutputInterface() const 
{
    return (outputInterface == nullptr ? false : true);
}

std::error_code Logger::log(LoggerMessage::View msgv, ErrorCode ec) 
{
    Log.emplace_back(msgv, ec);

    onMessage();

    if (Log.size() >= 20) {
        return saveLogToFile();
    }

    return make_error_code(LoggerError::SUCCESSFUL_SAVE_LOG);
}

 std::error_code Logger::log(LoggerMessage::View msgv, std::error_code ec) 
 {
    return log(msgv, ErrorCode {ec});
 }