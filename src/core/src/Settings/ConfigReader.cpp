#include "Settings/ConfigReader.hpp"

#include <fstream>
#include <string>

void ConfigData::addConfigLine(std::string key, std::string value) 
{
    configLines.emplace_back(key, value);
}

const std::vector<ConfigData::ConfigLineData>& ConfigData::getConfigLines() const 
{
    return configLines;
}

ConfigManager::ConfigManager(std::unique_ptr<ParsingStrategy> ps) 
    : cfg(std::make_unique<ConfigData>()), parsingStrategy(std::move(ps)) {}

std::unique_ptr<ConfigData> ConfigManager::releaseConfigData() 
{
    if (!cfg) 
    {
        return {};
    } 

    return std::move(cfg);
}

std::error_code ConfigManager::parseCfgFrom(std::string_view pathToConfig) 
{
    if (!cfg)
    {
        return make_error_code(SettingsError::BAD_CONFIG_MANAGER_OPERATION);
    }

    auto parseResult = parsingStrategy->parse(pathToConfig);

    if (!parseResult)
    {
        return make_error_code(SettingsError::INVALID_CONFIG_FILE);
    }

    *cfg = *parseResult;
    return make_error_code(SettingsError::EXTRACT_SUCCESSFUL);
}

std::optional<ConfigData> ParsingConfigFromTxt::parse(std::string_view pathToConfig) const
{
    std::ifstream cfgFile {pathToConfig.data()};

    if (!cfgFile.is_open())
    {
        return std::nullopt;
    }

    ConfigData::ConfigLineData cfgl;
    ConfigData cfgd;

    std::string key {}, val {};
    while (std::getline(cfgFile >> std::ws, key, ':') && std::getline (cfgFile >> std::ws, val))
    {
        cfgd.addConfigLine(key, val);
    }

    cfgFile.close();

    return  cfgd;
}