#pragma once

#include "../ErrorCodes.hpp"

#include <vector>
#include <optional>
#include <variant>

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

        std::error_code parseCfgFromFile(std::string pathToConfig);

        std::unique_ptr<ConfigData> releaseConfigData();
};

class IDataSource {
    public:
        virtual std::optional<std::reference_wrapper<std::istream>> getStream() const = 0;
        virtual ~IDataSource() = default;
};

class FileDescriptor : public IDataSource {
    private:
        std::string pathToFile;
        mutable std::ifstream file;

    public:
        FileDescriptor() = delete;

        FileDescriptor(std::string ptf);

        std::optional<std::reference_wrapper<std::istream>> getStream() const override;

        FileDescriptor(const FileDescriptor&) = delete;
        FileDescriptor& operator=(const FileDescriptor&) = delete;

        FileDescriptor(FileDescriptor&& other) noexcept = default;
        FileDescriptor& operator=(FileDescriptor&& other) noexcept = default;

        ~FileDescriptor();
};

class ParsingStrategy 
{
    public:
        virtual std::optional<ConfigData> parse(const IDataSource& pd) const = 0;
        virtual ~ParsingStrategy() = default;
};

class ParsingConfigFromTxt : public ParsingStrategy 
{
    public:
        ParsingConfigFromTxt() = default;

        std::optional<ConfigData> parse(const IDataSource& pd) const override;
};