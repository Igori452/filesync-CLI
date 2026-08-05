#include <gtest/gtest.h>

#include "CommandLineParser/include/CommandLineParser.hpp"
#include "Core/include/ErrorCodes.hpp"

TEST(CLI, ParseCompareCommand)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";
    char target[] = "target";

    char* argv[] = {
        program,
        command,
        source,
        target
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto[code, ic] {CommandLineParser::parseInputInstructions(argc, argv)}; 

    ErrorCode er {code};
    EXPECT_EQ(er.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(er.getErrorCode(), CommandLineParserError::PARSE_SUCCESSFUL);

    EXPECT_EQ(ic.command, Commands::COMPARE);
    EXPECT_EQ(ic.arguments.size(), 2);
    EXPECT_EQ(ic.arguments[0], source);
    EXPECT_EQ(ic.arguments[1], target);
    EXPECT_TRUE(ic.options.empty());
}

TEST(CLI, UnknownCommand)
{
    char program[] = "program";
    char command[] = "abracadabra";

    char* argv[] = {
        program,
        command
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::INVALID_COMMAND));

    EXPECT_EQ(ic.command, Commands::UNKNOWN);

    EXPECT_TRUE(ic.arguments.empty());
    EXPECT_TRUE(ic.options.empty());
}

TEST(CLI, EmptyInput)
{
    char program[] = "program";

    char* argv[] = {
        program
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::COMMAND_IS_EMPTY));

    EXPECT_EQ(ic.command, Commands::UNKNOWN);

    EXPECT_TRUE(ic.arguments.empty());
    EXPECT_TRUE(ic.options.empty());

}

TEST(CLI, MissingArguments)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";

    char* argv[] = {
        program,
        command,
        source
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::INVALID_ARGUMENT));

    EXPECT_EQ(ic.command, Commands::UNKNOWN);

    EXPECT_TRUE(ic.arguments.empty());
    EXPECT_TRUE(ic.options.empty());
}

TEST(CLI, ParseOption)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";
    char target[] = "target";
    char recursive[] = "--recursive";

    char* argv[] = {
        program,
        command,
        source,
        target,
        recursive
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::PARSE_SUCCESSFUL));

    ASSERT_EQ(ic.options.size(), 1);
    EXPECT_EQ(ic.options[0], Options::RECURSIVE);
}

TEST(CLI, UnknownOption)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";
    char target[] = "target";
    char option[] = "--unknown";

    char* argv[] = {
        program,
        command,
        source,
        target,
        option
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::INVALID_OPTION));

    EXPECT_EQ(ic.command, Commands::UNKNOWN);

    EXPECT_TRUE(ic.arguments.empty());
    EXPECT_TRUE(ic.options.empty());
}

TEST(CLI, MissedOption)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";
    char target[] = "target";
    char option[] = "--force";

    char* argv[] = {
        program,
        command,
        source,
        target,
        option
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::INVALID_OPTION));

    EXPECT_EQ(ic.command, Commands::UNKNOWN);

    EXPECT_TRUE(ic.arguments.empty());
    EXPECT_TRUE(ic.options.empty());
}

TEST(CLI, MultipleOptions)
{
    char program[] = "program";
    char command[] = "compare";
    char source[] = "source";
    char target[] = "target";
    char recursive[] = "--recursive";
    char verbose[] = "--verbose";

    char* argv[] = {
        program,
        command,
        source,
        target,
        recursive,
        verbose
    };

    int argc = sizeof(argv) / sizeof(char*);

    const auto [code, ic] {CommandLineParser::parseInputInstructions(argc, argv)};
    ErrorCode er {code};

    EXPECT_EQ(er.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(er.getErrorCode().value(), static_cast<int>(CommandLineParserError::PARSE_SUCCESSFUL));

    ASSERT_EQ(ic.options.size(), 2);

    EXPECT_EQ(ic.options[0], Options::RECURSIVE);
    EXPECT_EQ(ic.options[1], Options::VERBOSE);
}