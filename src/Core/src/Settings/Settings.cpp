#include "Settings/Settings.hpp"

#include <utility>

Settings::Settings(SettingsStuff::SettingsData stData) : settingsData(std::move(stData)) {}

std::error_code SettingsManager::setFromConfig(const ConfigData& cfgd) 
{
    auto setSpecialParametr {[]<typename T>(T& key, std::string_view value)
    {
        auto b {SettingsStuff::parseConfigValue<T>(value)};
        if (b) 
        {
            key = *b;
            return true;
        }
        return false;
    }};

    const auto& configData = cfgd.getConfigLines(); 

    for (auto it = std::begin(configData); it != std::end(configData); ++it) {
        bool parse_success = false;

        if (SettingsStuff::comapareStrings(it->first, "verbose")) 
        {
            parse_success = setSpecialParametr(settingsData.verbose, it->second);
        } 
        else if (SettingsStuff::comapareStrings(it->first, "recursive")) 
        {
            parse_success = setSpecialParametr(settingsData.recursive, it->second);
        } 
        else if (SettingsStuff::comapareStrings(it->first, "pathtosavelog")) 
        {
            parse_success = setSpecialParametr(settingsData.pathToSaveLog, it->second);
        } 
        else if (SettingsStuff::comapareStrings(it->first, "savelogbarier")) 
        {
            parse_success = setSpecialParametr(settingsData.saveLogBarier, it->second);
        } 
        else 
        {
            return make_error_code(SettingsError::UNKNOWN_CONFIGURATION_PARAMETER);
        }                  
        
        if (!parse_success) 
        {
            return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
        }
    }

    return make_error_code(SettingsError::CONFIG_SUCCESSFULLY_EXTRACTED);
}

std::error_code SettingsManager::setFromOptions(const std::vector<Options>& optd) 
{
    for (const auto& opt : optd) 
    {
        switch (opt)
        {
            case Options::VERBOSE:
                settingsData.verbose = true;
                break;
            case Options::RECURSIVE:
                settingsData.recursive = true;
                break;
            default:
                return make_error_code(SettingsError::UNKNOWN_OPTION);
        }
    }

    return make_error_code(SettingsError::OPTIONS_SUCCESSFULLY_EXTRACTED);
}

Settings SettingsManager::releaseSettings() 
{
    return Settings(std::exchange(settingsData, SettingsStuff::SettingsData {}));
}

bool Settings::verbose() const 
{
    return settingsData.verbose;
}

bool Settings::recursive() const 
{
    return settingsData.recursive;
}

const std::string& Settings::pathToSaveLog() const
{
    return settingsData.pathToSaveLog;
}

size_t Settings::saveLogBarier() const
{
    return settingsData.saveLogBarier;
}