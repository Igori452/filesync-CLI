#pragma once

#include "Instructions.hpp"
#include "ErrorCodes.hpp"
#include "ConfigReader.hpp"

#include <charconv>

#include <optional>
#include <algorithm>

#include <concepts>

namespace SettingsStuff 
{

struct SettingsData {
    bool verbose        {false};
    bool recursive      {false};

    std::string pathToSaveLog {"../../../log/"};

    size_t saveLogBarier {20};
};

inline bool comapareStrings(std::string_view sw1, std::string_view sw2) 
{
    if (sw1.size() != sw2.size())
    {
        return false;
    }

    return std::equal(std::begin(sw1), std::end(sw1), std::begin(sw2), [](char sym1, char sym2){
        return std::tolower(static_cast<unsigned char>(sym1)) == std::tolower(static_cast<unsigned char>(sym2));
    });
}

template <typename T>
std::optional<T> parseConfigValue(std::string_view v) 
{
    if constexpr (std::same_as<T, std::string>) 
    {
        return std::string(v);
    }
    else if constexpr (std::same_as<T, bool>) 
    {
        if (comapareStrings(v, "true") || comapareStrings(v, "1")) return true;
        else if (comapareStrings(v, "false") || comapareStrings(v, "0")) return false;
    }
    else if constexpr (std::integral<T>) 
    {
        if (!v.empty() && v[0] == '+') 
        {
            v.remove_prefix(1);
        }

        T convert {0};
        auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), convert);
        if (ec == std::errc{})
        {
            return convert;
        }
        
    }
    return std::nullopt;
}

} /* namespace SettingsStuff */

class Settings 
{
    private:
        SettingsStuff::SettingsData settingsData;

    public:
        Settings() = delete;
        explicit Settings(SettingsStuff::SettingsData stData);

        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;

        Settings(Settings&&) = default;
        Settings& operator=(Settings&&) = default;

        bool verbose() const;
        bool recursive() const;
        const std::string& pathToSaveLog() const;
        size_t saveLogBarier() const;
};

class SettingsManager
{
    private:
        SettingsStuff::SettingsData settingsData;

    public:
        SettingsManager() = default;

        std::error_code setFromConfig(const ConfigData& cfgd);
        std::error_code setFromOptions(const std::vector<Options>& optd);

        Settings releaseSettings();
};