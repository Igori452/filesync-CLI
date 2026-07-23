#pragma once

#include <string>

/* 100 <= ERRORS     <= 199 */
/* 200 <= WARNINGS   <= 299 */
/* 300 <= SUCCESSFUL <= 399 */

template<typename ErrorEnum, typename ErrorProvider>
class ErrorCode;

/*
 * 
 * An error is added by creating an `enum class` named `<module name>Error`; 
 * it is also necessary to define a provider named `<module name>ErrorProvider` 
 * with the following interface:
 * 
 * const char* name() const noexcept;
 * std::string message(<module name>Error code) const;
 * 
 * Here, the `name` function returns the name of the module associated with the
 * error, while `message` returns the error description based on the provided code.
 * 
 * 
 */


/* CODES AND PROVIDERS */

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

class CommandLineParserErrorProvider 
{
    public:
        const char* name() const noexcept;
        std::string message(CommandLineParserError code) const;
};


/* DEDUCTION GUIDS */

ErrorCode(CommandLineParserError) -> ErrorCode<CommandLineParserError, CommandLineParserErrorProvider>;


/* UNIVERSAL ERROR CODE INTERFACE */
enum class ErrorStatus {SUCCESSFUL, WARNING, ERROR};

/* TRAITS AND STATIC POLYMORPHISM */
template<typename ErrorEnum, typename ErrorProvider>
class ErrorCode
{
    private:
        ErrorEnum code;

        const ErrorProvider& getProvider() const;
    public:
        ErrorCode() = delete;
        ErrorCode(ErrorEnum cd);

        ErrorStatus getStatus() const;

        const char* name() const noexcept;
        std::string message() const;
};

template<typename ErrorEnum, typename ErrorProvider>
ErrorCode<ErrorEnum,  ErrorProvider>::ErrorCode(ErrorEnum cd) : code(cd) {}

template<typename ErrorEnum, typename ErrorProvider>
const ErrorProvider& ErrorCode<ErrorEnum,  ErrorProvider>::getProvider() const 
{
    static ErrorProvider instanceProvider;
    return instanceProvider;
}

template<typename ErrorEnum, typename ErrorProvider>
const char* ErrorCode<ErrorEnum, ErrorProvider>::name() const noexcept 
{
    return getProvider().name();
}

template<typename ErrorEnum, typename ErrorProvider>
std::string ErrorCode<ErrorEnum, ErrorProvider>::message() const 
{
    return getProvider().message(code);
}

template<typename ErrorEnum, typename ErrorProvider>
ErrorStatus ErrorCode<ErrorEnum, ErrorProvider>::getStatus() const
{
    int val = static_cast<int>(code);
    if (val >= 100 && val <= 199) return ErrorStatus::ERROR;
    else if (val >= 200 && val <= 299) return ErrorStatus::WARNING;
    else return ErrorStatus::SUCCESSFUL;
}