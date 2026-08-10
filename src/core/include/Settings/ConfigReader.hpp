#pragma once

#include "../ErrorCodes.hpp"

#include <vector>
#include <optional>

#include <memory>

class ConfigData {
    public:
        using ConfigLineData = std::pair<std::string, std::string>; 
    
    private:
        std::vector<ConfigLineData> configLines;

    public:
        ConfigData() = default;

        void addConfigLine(std::string key, std::string value);
        std::vector<ConfigLineData> getConfigLines() const;  
};

class ParsingStrategy;

class ConfigManager
{
    private:
        std::unique_ptr<ConfigData> cfg;
        std::unique_ptr<ParsingStrategy> parsingStrategy;
        
    public:
        ConfigManager() = delete;

        ConfigManager(std::unique_ptr<ParsingStrategy> ps);

        std::error_code parseCfgFrom(std::string_view pathToConfig);

        std::unique_ptr<ConfigData> releaseConfigData();
};

class ParsingStrategy 
{
    public:
        virtual std::optional<ConfigData> parse(std::string_view pathToConfig) const = 0;
        virtual ~ParsingStrategy() = default;
};

class ParsingConfigFromTxt : public ParsingStrategy 
{
    public:
        std::optional<ConfigData> parse(std::string_view pathToConfig) const override;
};