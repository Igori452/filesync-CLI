#pragma once

#include "Instructions.hpp"
#include "ErrorCodes.hpp"

#include <charconv>

#include <optional>
#include <algorithm>

#include <concepts>

template <typename T>
std::optional<T> parseConfigValue (std::string v) 
{
    if constexpr (std::same_as<T, bool>) 
    {
        std::transform(std::begin(v), std::end(v), std::begin(v), [](unsigned char sym){
            return tolower(sym);
        });

        if (v == "true" || v == "1") return true;
        else if  (v == "false" || v == "0") return false;
    }
    else if constexpr (std::integral<T>) 
    {
        T convert {0};
        auto [ptr, ec] = std::from_chars(v.data(), v.data() + v.size(), convert);
        if (ec == std::errc{})
        {
            return convert;
        }
        
    }
    return std::nullopt;
}

class Settings 
{
    private:
        const std::string pathToConfigFile {"../../../config.txt"};
        /*
         * Format of values ​​specified in the config file:
         * Parameter: value
         */

        bool verbose_   {false};
        bool recursive_ {false};

        std::string pathToSaveLog_ {"../../../log/"};
        size_t saveLogBarier_ {20};

        std::error_code setParametr(std::string&& parametr, std::string&& value); 

        std::error_code extractConfigFromFile();

    public:
        Settings() = default;

        std::error_code setSettings(const std::vector<Options>& opt);

        bool verbose() const;
        bool recursive() const;
        std::string pathToSaveLog() const;
        size_t saveLogBarier() const;
};