#pragma once

#include "Instructions.hpp"

class Settings 
{
    public:
        virtual void setSettings(const InputCommand& cmd) = 0;
        virtual ~Settings() = default;

};

class OutputUserSettings : public Settings 
{
    private:
        bool verbose_;

    public:
        /* Default settings */
        OutputUserSettings();

        /* Custom settings */
        void setSettings(const InputCommand& cmd) override;
        
        bool verbose() const;
};  