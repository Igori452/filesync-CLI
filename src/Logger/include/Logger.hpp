#pragma once

#include <vector>
#include <string>
#include <optional>

#include <filesystem>

#include "OutputInterface/include/OutputInterface.hpp"
#include "LoggerMessage.hpp"

class Logger 
{
    private:
        const size_t saveLogBarier = 20;
        const std::string pathToSaveLog {"../../../log/"};

        std::optional<std::string> nameLogFile;
        
        const OutputInterface* outputInterface;

        std::vector<LoggerMessage> Log;
        size_t unprocessed;

        void onMessage();

        /* Save and clear Log */
        std::error_code saveLogToFile(); 

    public:
        Logger();
        Logger(const OutputInterface* ob);

        void setOutputInterface(const OutputInterface* ob);
        bool hasOutputInterface() const;

        std::error_code log(LoggerMessage::View msgv, ErrorCode ec);
        std::error_code log(LoggerMessage::View msgv, std::error_code ec);
};