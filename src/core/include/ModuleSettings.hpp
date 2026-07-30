#pragma once

#include "Instructions.hpp"

class Settings 
{
    private:
        bool verbose_ = false;
        bool recursive_ = false;

    public:
        Settings() = delete;
        Settings(const std::vector<Options>& opt);

        bool verbose() const;
        bool recursive() const;

};
