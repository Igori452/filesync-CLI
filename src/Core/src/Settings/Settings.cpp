#include "Settings/Settings.hpp"

#include <fstream>

#include <set>

SettingsManager::SettingsManager() : st(std::unique_ptr<Settings>(new Settings())) {}

std::error_code SettingsManager::setFromConfig(const ConfigData& cfgd) 
{
    if (!st) 
    {
        return make_error_code(SettingsError::BAD_SETTINGS_MANAGER_OPERATION);
    }

    auto configData = cfgd.getConfigLines(); 

    auto setSpecialParametr = [this]<typename T>(T& key, const std::string& value)
    {
        auto b {SettingsManagerStuff::parseConfigValue<T>(std::move(value))};
        if (b) 
        {
            key = *b;
            return true;
        }
        return false;
    };

    for (auto it = std::begin(configData); it != std::end(configData); ++it) {
        std::transform(std::begin(it->first), std::end(it->first), std::begin(it->first), [](unsigned char sym){
            return tolower(sym);
        });

        bool parse_failed = false;

        if (it->first == "verbose") 
        {
            parse_failed = setSpecialParametr(st->verbose_, it->second);
        } 
        else if (it->first == "recursive") 
        {
            parse_failed = setSpecialParametr(st->recursive_, it->second);
        } 
        else if (it->first == "pathtosavelog") 
        {
            parse_failed = setSpecialParametr(st->pathToSaveLog_, it->second);
        } 
        else if (it->first == "savelogbarier") 
        {
            parse_failed = setSpecialParametr(st->saveLogBarier_, it->second);
        } 
        else 
        {
            return make_error_code(SettingsError::UNKNOWN_CONFIGURATION_PARAMETER);
        }                  
        
        if (!parse_failed) 
        {
            return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        }
    }

    return make_error_code(SettingsError::PARAMETER_SUCCESSFULLY_EXTRACTED);
}

std::error_code SettingsManager::setFromOptions(const std::vector<Options>& optd) 
{
    if (!st) 
    {
        return make_error_code(SettingsError::BAD_SETTINGS_MANAGER_OPERATION);
    }

    for (const auto& opt : optd) 
    {
        switch (opt)
        {
            case Options::VERBOSE:
                st->verbose_ = true;
                break;
            case Options::RECURSIVE:
                st->recursive_ = true;
                break;
            default:
                return make_error_code(SettingsError::UNKNOWN_OPTION);
        }
    }

    return make_error_code(SettingsError::OPTIONS_SUCCESSFULLY_EXTRACTED);
}

std::unique_ptr<Settings> SettingsManager::releaseSettings() 
{
    if (!st) 
    {
        return std::unique_ptr<Settings>(new Settings());
    }    

    return std::move(st);
}

bool Settings::verbose() const 
{
    return verbose_;
}

bool Settings::recursive() const 
{
    return recursive_;
}

const std::string& Settings::pathToSaveLog() const
{
    return pathToSaveLog_;
}

size_t Settings::saveLogBarier() const
{
    return saveLogBarier_;
}