#include "../include/Settings.hpp"

Settings::Settings(const std::vector<Options>& options)
{
    for (const auto& opt : options) 
    {
        switch (opt)
        {
            case Options::VERBOSE:
                verbose_ = true;
                break;
            case Options::RECURSIVE:
                recursive_ = true;
                break;
        }
    }
}

bool Settings::verbose() const 
{
    return verbose_;
}

bool Settings::recursive() const 
{
    return recursive_;
}