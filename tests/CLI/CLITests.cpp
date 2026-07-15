#include <gtest/gtest.h>

#include "cli/include/CLI.hpp"

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

    CLI cli;

    EXPECT_TRUE(cli.inputInstructions(4, argv));

    auto input = cli.releaseInputInstructions();

    EXPECT_EQ(input.command, Commands::COMPARE);
    EXPECT_EQ(input.arguments.size(), 2);
    EXPECT_EQ(input.arguments[0], "source");
    EXPECT_EQ(input.arguments[1], "target");
    EXPECT_TRUE(input.options.empty());
}

TEST(CLI, UnknownCommand)
{
    char program[] = "program";
    char command[] = "abracadabra";

    char* argv[] = {
        program,
        command
    };

    CLI cli;

    EXPECT_FALSE(cli.inputInstructions(2, argv));
}

TEST(CLI, EmptyInput)
{
    char program[] = "program";

    char* argv[] = {
        program
    };

    CLI cli;

    EXPECT_FALSE(cli.inputInstructions(1, argv));
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

    CLI cli;

    EXPECT_FALSE(cli.inputInstructions(3, argv));
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

    CLI cli;

    EXPECT_TRUE(cli.inputInstructions(5, argv));

    auto input = cli.releaseInputInstructions();

    ASSERT_EQ(input.options.size(), 1);
    EXPECT_EQ(input.options[0], Options::RECURSIVE);
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

    CLI cli;

    EXPECT_FALSE(cli.inputInstructions(5, argv));
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

    CLI cli;

    EXPECT_TRUE(cli.inputInstructions(6, argv));

    auto input = cli.releaseInputInstructions();

    ASSERT_EQ(input.options.size(), 2);

    EXPECT_EQ(input.options[0], Options::RECURSIVE);
    EXPECT_EQ(input.options[1], Options::VERBOSE);
}