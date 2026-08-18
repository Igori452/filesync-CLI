#include "Settings/ConfigReader.hpp"

#include <fstream>
#include <string>

void ConfigData::addConfigLine(std::string key, std::string value) 
{
    configLines.emplace_back(std::move(key), std::move(value));
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
        return std::make_unique<ConfigData>();
    } 

    return std::move(cfg);
}

std::error_code ConfigManager::parseCfgFrom(const IDataSource& ids) 
{
    if (!cfg)
    {
        return make_error_code(SettingsError::BAD_CONFIG_MANAGER_OPERATION);
    }

    auto parseResult = parsingStrategy->parse(ids);

    if (!parseResult)
    {
        return make_error_code(SettingsError::EXTRACT_ERROR);
    }

    cfg = std::make_unique<ConfigData>(std::move(*parseResult));
    return make_error_code(SettingsError::EXTRACT_SUCCESSFUL);
}

std::error_code ConfigManager::parseCfgFromFile(std::string pathToConfig) 
{
    return parseCfgFrom(FileDescriptor {std::move(pathToConfig)});
}

FileDescriptor::FileDescriptor(std::string ptf) 
    : pathToFile(std::move(ptf)), file(pathToFile) {}

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

std::optional<ConfigData> ParsingConfigFromTxt::parse(const IDataSource& ids) const 
{   
    auto wrappedFile {ids.getStream()};

    if (!wrappedFile)
    {
        return std::nullopt;
    }

    std::istream& is = wrappedFile->get();

    ConfigData cfgd;
    std::string key {}, val {};

    const std::string rmSyms {" \t\n"};
    auto filterStr = [&rmSyms](std::string_view strw) -> std::string_view { 
        const size_t indStart = strw.find_first_not_of(rmSyms);

        if (indStart == std::string::npos)
        {
            return "";
        }
        
        const size_t indEnd = strw.find_last_not_of(rmSyms);
        return strw.substr(indStart, (indEnd - indStart + 1));
    };

    while (std::getline(is, key, ':') && std::getline (is, val))
    {
        cfgd.addConfigLine(std::string {filterStr(key)}, std::string {filterStr(val)});
    }

    return cfgd;
}