#include "Settings/Settings.hpp"

#include <fstream>

#include <set>

std::error_code SettingsManager::setFromConfig(const ConfigData& cfgd) 
{
    if (!st) 
    {
        return make_error_code(SettingsError::BAD_SETTINGS_MANAGER_OPERATION);
    }

    auto configData = cfgd.getConfigLines(); 

    bool visit {false};
    auto setSpecialParametr = [this, &visit]<typename T>(T& key, std::string value)
    {
        visit = true;
        auto b = parseConfigValue<T>(value);
        if (b) 
        {
            key = *b;
            return false;
        }
        return true;
    };

    for (auto it = std::begin(configData); it != std::end(configData); ++it) {
        std::transform(std::begin(it->first), std::end(it->first), std::begin(it->first), [](unsigned char sym){
            return tolower(sym);
        });

        if (it->first == "verbose" && setSpecialParametr(st->verbose_, it->second))                     return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        else if (it->first == "recursive" && setSpecialParametr(st->recursive_, it->second))            return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        else if (it->first == "pathtosavelog" && setSpecialParametr(st->pathToSaveLog_, it->second))    return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        else if (it->first == "savelogbarier" && setSpecialParametr(st->saveLogBarier_, it->second))    return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        
        if (!visit) return make_error_code(SettingsError::UNKNOWN_CONFIGURATION_PARAMETER);
        visit = false;
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
        return {};
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