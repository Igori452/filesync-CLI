#include "Settings/ConfigReader.hpp"

#include <fstream>
#include <string>

void ConfigData::addConfigLine(std::string key, std::string value) 
{
    configLines.emplace_back(key, value);
}

std::vector<ConfigData::ConfigLineData> ConfigData::getConfigLines() const 
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

std::error_code ConfigManager::parseCfgFromFile(std::string pathToConfig) 
{
    if (!cfg)
    {
        return make_error_code(SettingsError::BAD_CONFIG_MANAGER_OPERATION);
    }

    auto parseResult = parsingStrategy->parse(FileDescriptor {std::move(pathToConfig)});

    if (!parseResult)
    {
        return make_error_code(SettingsError::INVALID_CONFIG_FILE);
    }

    *cfg = *parseResult;
    return make_error_code(SettingsError::EXTRACT_SUCCESSFUL);
}

FileDescriptor::FileDescriptor(std::string ptf) 
    : pathToFile(std::move(ptf)), file(ptf) {}

std::optional<std::reference_wrapper<std::istream>> FileDescriptor::getStream() const
{
    if (!file.is_open())
    {
        return std::nullopt;
    }

    return file;
}

FileDescriptor::~FileDescriptor() 
{
    file.close();
}

std::optional<ConfigData> ParsingConfigFromTxt::parse(const IDataSource& pd) const 
{   
    auto wrappedFile {pd.getStream()};

    if (!wrappedFile)
    {
        return std::nullopt;
    }

    std::istream& is = wrappedFile->get();

    ConfigData::ConfigLineData cfgl;
    ConfigData cfgd;

    std::string key {}, val {};
    while (std::getline(is >> std::ws, key, ':') && std::getline (is >> std::ws, val))
    {
        cfgd.addConfigLine(key, val);
    }

    return cfgd;
}