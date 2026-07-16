#pragma once

#include "Instructions.hpp"

class Settings 
{
    public:
        virtual void setSettings(const InputCommand& cmd);
        virtual ~Settings() = default;

};

class OutputUserSettings : public Settings 
{
    private:
        bool verbose_;

    public:
        void setSettings(const InputCommand& cmd) override;
        
        bool verbose() const;
};  