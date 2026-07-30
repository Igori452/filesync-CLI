#pragma once

/**
 * This module is designed for structuring errors using std::error_code. 
 * Every single class function that can cause an error simply returns a std::error_code.
 *
 * At the same time, an ErrorCode wrapper was created, which implicitly takes std::error_code 
 * in its constructor and can be used to return the error status (ErrorStatus).
 *
 * Usage example:
 *   std::error_code example();
 *   ...
 *   ErrorCode err = example(); // Implicit conversion
 *
 * To systematize errors and minimize code boilerplate, an error definition scheme 
 * via static category templates is used. Meanwhile, other modules can still use raw 
 * std::error_code if they do not require a high-level status.
 *
 * The system adheres to the following classification for error numerical codes:
 * - From 100 to 199 (inclusive) — critical errors (Error)
 * - From 200 to 299 (inclusive) — warnings (Warning)
 * - From 300 to 399 (inclusive) — successful execution (Successful)
 *
 * In the getStatus method, this scheme can be easily extended if specific cases arise 
 * or if the error code structure needs to be expanded in the future (e.g., setting an error range from 1000 to 2000). 
 * To achieve this, the following code can be used inside getStatus:
 * 
 *   const auto& cat = code.category();
 *   if (cat == get_category<CommandLineParserCategory>()) { ... }
 */


#include <system_error>

enum class ErrorStatus {ERROR, WARNING, SUCCESSFUL};

enum class CommandLineParserError : int
{
    /* ERRORS */
    COMMAND_IS_EMPTY = 100,
    INVALID_COMMAND = 101,
    INVALID_ARGUMENT = 102,
    INVALID_OPTION = 103,

    /* WARNINGS */

    /* SUCCESSFUL */
    PARSE_SUCCESSFUL = 300,
    
};

enum class ModuleSettingsError : int
{
    /* WARNINGS */
    DEAFAULT_EXTRACT = 200,

    /* SUCCESSFUL */
    EXTRACT_SUCCESSFUL = 300,
    
};

namespace std 
{
    template<>
    struct is_error_code_enum<CommandLineParserError> : true_type {};

    template<>
    struct is_error_code_enum<ModuleSettingsError> : true_type {};
}

class CommandLineParserCategory : public std::error_category 
{
    public:
        const char* name() const noexcept override;
        std::string message(int cd) const override;
};

class ModuleSettingsCategory : public std::error_category 
{
    public:
        const char* name() const noexcept override;
        std::string message(int cd) const override;
};

template <typename C>
const C& get_category() 
{
    static const C category;
    return category;
}

inline std::error_code make_error_code(CommandLineParserError code) 
{
    return std::error_code(static_cast<int>(code), get_category<CommandLineParserCategory>());
}

inline std::error_code make_error_code(ModuleSettingsError code) 
{
    return std::error_code(static_cast<int>(code), get_category<ModuleSettingsCategory>());
}

/*
 * The ErrorCode module acts as a wrapper that collapses high-level 
 * errors into general categories: ERROR, WARNING, and SUCCESSFUL.
 */
class ErrorCode 
{
    private:
        std::error_code code;
        ErrorStatus status;

        ErrorStatus getErrorStatusFromCode () const;
    public:
        ErrorCode () = delete;
        ErrorCode (std::error_code cd);

        std::error_code getErrorCode() const noexcept;
        
        ErrorStatus getStatus() const;
};