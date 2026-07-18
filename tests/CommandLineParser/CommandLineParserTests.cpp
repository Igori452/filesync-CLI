#include <gtest/gtest.h>

#include "CommandLineParser/include/CommandLineParser.hpp"

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

    auto ic = CommandLineParser::parseInputInstructions(4, argv);
    EXPECT_TRUE(ic.has_value());

    EXPECT_EQ(ic.value().command, Commands::COMPARE);
    EXPECT_EQ(ic.value().arguments.size(), 2);
    EXPECT_EQ(ic.value().arguments[0], "source");
    EXPECT_EQ(ic.value().arguments[1], "target");
    EXPECT_TRUE(ic.value().options.empty());
}

TEST(CLI, UnknownCommand)
{
    char program[] = "program";
    char command[] = "abracadabra";

    char* argv[] = {
        program,
        command
    };

    EXPECT_FALSE(CommandLineParser::parseInputInstructions(2, argv).has_value());
}

TEST(CLI, EmptyInput)
{
    char program[] = "program";

    char* argv[] = {
        program
    };

    EXPECT_FALSE(CommandLineParser::parseInputInstructions(1, argv).has_value());
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

    EXPECT_FALSE(CommandLineParser::parseInputInstructions(3, argv).has_value());
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

    auto ic = CommandLineParser::parseInputInstructions(5, argv);

    EXPECT_TRUE(ic.has_value());

    ASSERT_EQ(ic.value().options.size(), 1);
    EXPECT_EQ(ic.value().options[0], Options::RECURSIVE);
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

    EXPECT_FALSE(CommandLineParser::parseInputInstructions(5, argv).has_value());
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

    EXPECT_FALSE(CommandLineParser::parseInputInstructions(5, argv).has_value());
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

    auto ic = CommandLineParser::parseInputInstructions(6, argv);

    EXPECT_TRUE(ic.has_value());

    ASSERT_EQ(ic.value().options.size(), 2);

    EXPECT_EQ(ic.value().options[0], Options::RECURSIVE);
    EXPECT_EQ(ic.value().options[1], Options::VERBOSE);
}