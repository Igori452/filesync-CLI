#ifndef CORE_HEADER
#define CORE_HEADER

#include <string>
#include <vector>
#include <map>
#include <algorithm>


enum class Commands 
{
    /*
     * Compare the contents of two directories 
     * and identify differences.
     */
    COMPARE, 

    /*
     * Synchronize the target directory with the 
     * source directory.
     */
    SYNC, 

    /*
     * Check that two directories are completely identical.
     */
    VERIFY, 

    /*
     * Display statistics before synchronization 
     * (number of files, changes, total data size, etc.).
     */
    STATUS, 

    /*
     * Display help on using a program or a specific command.
     */
    HELP, 

    /*
     * Display the current version of the program.
     */
    VERSION
};


enum class Options 
{
    /*
     * Traverse directories recursively.
     */
    RECURSIVE, 

    /*
     * Display detailed information during command execution.
     */
    VERBOSE, 

    /*
     * Exclude files or directories matching the specified pattern.
     */
    EXCLUDE,
    
    /*
     * Load settings from a configuration file.
     */
    CONFIG, 

    /*
     * Show the planned actions without making any changes.
     */
    DRYRUN, 
    
    /*
     * Specify the number of worker threads to use.
     */
    THREADS, 

    /*
     * Force the operation without additional confirmation.
     */
    FORCE
};


struct OptionInfo 
{

    const std::string_view optionName;
    const std::string_view commandDescription;

};

struct CommandInfo 
{

    const std::string_view commandName;
    const size_t argumentsCount;
    const std::vector<Options> options;
    const std::string_view commandDescription;

};


const std::map<Options, OptionInfo> optionRegistry 
{
    {Options::RECURSIVE,    {"--recursive", "Traverse directories recursively."}},
    {Options::VERBOSE,      {"--verbose", "Display detailed information during command execution."}},
    {Options::EXCLUDE,      {"--exclude", "Exclude files or directories matching the specified pattern."}},
    {Options::CONFIG,       {"--config", "Load settings from a configuration file."}},
    {Options::DRYRUN,       {"--dry-run", "Show the planned actions without making any changes."}},
    {Options::THREADS,      {"--threads", "Specify the number of worker threads to use."}},
    {Options::FORCE,        {"--force", "Force the operation without additional confirmation."}},
};

const std::map<Commands, CommandInfo> commandRegistry
{
    {Commands::COMPARE, { "compare", 2, {
            Options::RECURSIVE, 
            Options::VERBOSE,
            Options::EXCLUDE,
            Options::CONFIG
        },
        "Compare the contents of two directories and identify differences"
    }},

    {Commands::SYNC, { "sync", 2, {
            Options::DRYRUN, 
            Options::THREADS,
            Options::EXCLUDE,
            Options::CONFIG,
            Options::VERBOSE,
            Options::FORCE
        },
        "Synchronize the target directory with the source directory"
    }},

    {Commands::VERIFY, { "verify", 2, {
            Options::RECURSIVE, 
            Options::VERBOSE
        },
        "Check that two directories are completely identical"
    }},

    {Commands::STATUS, {"status", 2, {
        Options::VERBOSE
        },
        "Display statistics before synchronization (number of files, changes, total data size, etc.)"
    }},

    {Commands::HELP, {"help", 0, {},
        "Display help on using a program or a specific command"}},

    {Commands::VERSION, {"version", 0, {},
        "Display the current version of the program"}},
};


/* COMMAND MODEL DATA (USE CLI AND APPLICATION) */
struct InputCommand
{

    Commands command;
    std::vector<std::string> arguments;
    std::vector<Options> options;

};


#endif