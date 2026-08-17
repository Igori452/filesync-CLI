#pragma once

#include "Instructions.hpp"
#include "ErrorCodes.hpp"
#include "ConfigReader.hpp"

#include <charconv>

#include <optional>
#include <algorithm>

#include <concepts>

#include <memory>

class SettingsManager;

/* DATA (SETTINGS) STORAGE */
class Settings 
{
    private:
        friend class SettingsManager;

        bool verbose_   {false};
        bool recursive_ {false};

        std::string pathToSaveLog_ {"../../../log/"};
        size_t saveLogBarier_ {20};

        Settings() = default;

    public:
        Settings(const Settings&) = delete;
        Settings& operator=(Settings&) = delete;

        bool verbose() const;
        bool recursive() const;
        const std::string& pathToSaveLog() const;
        size_t saveLogBarier() const;
};

/* BUILDER SETTINGS STORAGE */
class SettingsManager
{
    private:
        std::unique_ptr<Settings> st;

    public:
        SettingsManager();

        std::error_code setFromConfig(const ConfigData& cfgd);
        std::error_code setFromOptions(const std::vector<Options>& optd);

        std::unique_ptr<Settings> releaseSettings();
};

namespace SettingsManagerStuff 
{

template <typename T>
std::optional<T> parseConfigValue(std::string v) 
{
    if constexpr (std::same_as<T, std::string>) 
    {
        return v;
    }
    else if constexpr (std::same_as<T, bool>) 
    {
        std::transform(std::begin(v), std::end(v), std::begin(v), [](unsigned char sym){
            return tolower(sym);
        });

        if (v == "true" || v == "1") return true;
        else if  (v == "false" || v == "0") return false;
    }
    else if constexpr (std::integral<T>) 
    {
        std::string_view sw {v};
        if (!sw.empty() && sw[0] == '+') 
        {
            sw.remove_prefix(1);
        }

        T convert {0};
        auto [ptr, ec] = std::from_chars(sw.data(), sw.data() + sw.size(), convert);
        if (ec == std::errc{})
        {
            return convert;
        }
        
    }
    return std::nullopt;
}

} /* namespace SettingsManagerStuff */