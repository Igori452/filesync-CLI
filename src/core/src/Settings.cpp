#include "../include/Settings.hpp"

#include <fstream>

std::error_code Settings::setSettings(const std::vector<Options>& options)
{
    for (const auto& opt : options) 
    {
        switch (opt)
        {
            case Options::VERBOSE:
                verbose_ = true;
                break;
            case Options::RECURSIVE:
                recursive_ = true;
                break;
        }
    }
}

std::error_code Settings::extractConfigFromFile() 
{
    std::fstream cfg {pathToConfigFile};

    if (!cfg.is_open())
    {
        return make_error_code(SettingsError::DEAFAULT_EXTRACT);
    }

    

    cfg.close();

    return make_error_code(SettingsError::EXTRACT_SUCCESSFUL);
}

std::error_code Settings::setParametr(std::string&& parametr, std::string&& value) 
{
    std::transform(std::begin(parametr), std::end(parametr), std::begin(parametr), [](unsigned char sym){
        return tolower(sym);
    });

    auto setSpecialParametr = [&value]<typename T>(T& param) -> std::error_code
    {
        auto b = parseConfigValue<T>(value);
        if (b) 
        {
            param = *b;
            return make_error_code(SettingsError::PARAMETER_SUCCESSFULLY_EXTRACTED);
        }
        return make_error_code(SettingsError::INVALID_CONFIGURATION_VALUE);
    };

    if (parametr == "verbose") return setSpecialParametr(verbose_);
    else if (parametr == "recursive") return setSpecialParametr(recursive_);
    else if (parametr == "pathtosavelog") pathToSaveLog_ = std::move(value);
    else if (parametr == "savelogbarier") return setSpecialParametr(saveLogBarier_);
    else return make_error_code(SettingsError::UNKNOWN_CONFIGURATION_PARAMETER);

    return make_error_code(SettingsError::PARAMETER_SUCCESSFULLY_EXTRACTED);
}

bool Settings::verbose() const 
{
    return verbose_;
}

bool Settings::recursive() const 
{
    return recursive_;
}

std::string Settings::pathToSaveLog() const
{
    return pathToSaveLog_;
}

size_t Settings::saveLogBarier() const
{
    return saveLogBarier_;
}