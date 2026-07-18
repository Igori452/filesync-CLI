#include "../include/ModuleSettings.hpp"

#include <algorithm>

OutputUserSettings::OutputUserSettings () : verbose_(false) {}

void OutputUserSettings::setSettings(const InputCommand& cmd) 
{
    const auto& opts = cmd.options;
    if (std::find(std::begin(opts), std::end(opts), Options::VERBOSE) != std::end(opts))
        verbose_ = true;
    else 
        verbose_ = false;
}

bool OutputUserSettings::verbose() const 
{
    return verbose_;
}