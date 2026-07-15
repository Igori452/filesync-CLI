#include <gtest/gtest.h>

#include <regex>

#include "Logger.hpp"

TEST(Logger, SingletonInstance)
{
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();

    EXPECT_EQ(&logger1, &logger2);
}

TEST(Logger, LogInfoMessage)
{
    Logger& logger = Logger::instance();

    logger.log(MessageStatus::INFO, "Test message");

    std::ostringstream out;
    logger.flush(out);

    EXPECT_NE(out.str().find("Test message"), std::string::npos);
}

TEST(Logger, LogDifferentStatuses)
{
    Logger& logger = Logger::instance();

    logger.log(MessageStatus::INFO, "Info");
    logger.log(MessageStatus::WARNING, "Warning");
    logger.log(MessageStatus::ERROR, "Error");

    std::ostringstream out;
    logger.flush(out);

    std::string text = out.str();

    EXPECT_NE(text.find("[INFO]"), std::string::npos);
    EXPECT_NE(text.find("[WARNING]"), std::string::npos);
    EXPECT_NE(text.find("[ERROR]"), std::string::npos);

    std::regex datePattern(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");

    EXPECT_TRUE(std::regex_search(text, datePattern));
}

TEST(Logger, FlushToStream)
{
    Logger& logger = Logger::instance();

    logger.log(MessageStatus::INFO, "Flush works");

    std::ostringstream out;

    logger.flush(out);

    EXPECT_FALSE(out.str().empty());
}