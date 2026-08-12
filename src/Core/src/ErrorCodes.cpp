#include "ErrorCodes.hpp"

const char* CommandLineParserCategory::name() const noexcept 
{
    return "CommandLineParser";
}

std::string CommandLineParserCategory::message(int cd) const 
{
    switch (cd)
    {
        case 100: return "Command line is missing";
        case 101: return "Invalid command";
        case 102: return "Incorrect number of arguments";
        case 103: return "Invalid option (flag)";
        case 300: return "String successfully recognized";
    }
    return "Unknown";
}

const char* SettingsCategory::name() const noexcept 
{
    return "ModuleSettings";
}

std::string SettingsCategory::message(int cd) const 
{
    switch (cd)
    {
        case 100: return "Unknown parameter in the configuration file";
        case 101: return "Incorrect configuration parameter type";
        case 200: return "Failed to open config.txt; using default system settings";
        case 201: return "Unknown data in the configuration file; some lines were ignored";
        case 300: return "Config extracted successfully";
        case 301: return "Parameter and value successfully extracted.";
    }
    return "Unknown";
}

const char* LoggerCategory::name() const noexcept 
{
    return "Logger";
}

std::string LoggerCategory::message(int cd) const 
{
    switch (cd)
    {
        case 100: return "Error creating the file for automatic log saving";
        case 300: return "Logger successfuly initialisation";
        case 301: return "Log successfully saved to file";
    }
    return "Unknown";
}

ErrorCode::ErrorCode(std::error_code cd) : code(cd) 
{
    status = getErrorStatusFromCode();
    /* can be extended to any category */
}

std::error_code ErrorCode::getErrorCode() const noexcept 
{
    return code;
}

ErrorStatus ErrorCode::getErrorStatusFromCode () const 
{
    int codeVal = code.value();
    if (codeVal >= 100 && codeVal <= 199) return ErrorStatus::ERROR;
    else if (codeVal >= 200 && codeVal <= 299) return ErrorStatus::WARNING;
    else return ErrorStatus::SUCCESSFUL;
}

ErrorStatus ErrorCode::getStatus() const 
{
    return status;
}