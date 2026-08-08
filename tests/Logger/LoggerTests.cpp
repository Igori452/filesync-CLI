#include <gtest/gtest.h>

#include "Logger/include/Logger.hpp"

/* MOCK */
class TestOutput : public OutputInterface  
{
    private:
        mutable const LoggerMessage* logOutput = nullptr;

    public:
        explicit TestOutput(const Settings& st);

        void display(const LoggerMessage& lgmsg) const override;
        
        const LoggerMessage* getOut() const;
};

TestOutput::TestOutput(const Settings& st) : OutputInterface(st) {}

void TestOutput::display(const LoggerMessage& lgmsg) const 
{
    logOutput = &lgmsg;
}

const LoggerMessage* TestOutput::getOut() const 
{
    return logOutput;
}

TEST(Logger, LogWithInterface)
{
    Settings st;
    TestOutput out {st};

    Logger lg1 {&out};

    EXPECT_TRUE(lg1.hasOutputInterface());

    Logger lg2 {};
    
    lg2.setOutputInterface(&out);
    EXPECT_TRUE(lg2.hasOutputInterface());

    Logger lg3 {};
    
    EXPECT_FALSE(lg3.hasOutputInterface());
}

TEST(Logger, LogSuccessfulErrorCode)
{
    Settings st;
    TestOutput out {st};

    Logger lg {&out};

    lg.log(LoggerMessage::View::USER, CommandLineParserError::PARSE_SUCCESSFUL);
    const LoggerMessage* lgmsg = out.getOut();
    ErrorCode err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::USER);
    EXPECT_FALSE(err.getErrorCode().message().empty());

    lg.log(LoggerMessage::View::SYSTEM, CommandLineParserError::PARSE_SUCCESSFUL);
    lgmsg = out.getOut();
    err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::SYSTEM);
    EXPECT_FALSE(err.getErrorCode().message().empty());
}

TEST(Logger, LogWarningErrorCode)
{
    Settings st;
    TestOutput out {st};

    Logger lg {&out};

    lg.log(LoggerMessage::View::USER, SettingsError::DEAFAULT_EXTRACT);
    const LoggerMessage* lgmsg = out.getOut();
    ErrorCode err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::WARNING);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::USER);
    EXPECT_FALSE(err.getErrorCode().message().empty());

    lg.log(LoggerMessage::View::SYSTEM, SettingsError::DEAFAULT_EXTRACT);
    lgmsg = out.getOut();
    err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::WARNING);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::SYSTEM);
    EXPECT_FALSE(err.getErrorCode().message().empty());
}

TEST(Logger, LogErrorErrorCode)
{
    Settings st;
    TestOutput out {st};

    Logger lg {&out};

    lg.log(LoggerMessage::View::USER, LoggerError::FILE_CREATION_ERROR);
    const LoggerMessage* lgmsg = out.getOut();
    ErrorCode err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::ERROR);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::USER);
    EXPECT_FALSE(err.getErrorCode().message().empty());

    lg.log(LoggerMessage::View::SYSTEM, SettingsError::DEAFAULT_EXTRACT);
    lgmsg = out.getOut();
    err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::WARNING);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::SYSTEM);
    EXPECT_FALSE(err.getErrorCode().message().empty());
}

TEST(Logger, LogMessageWithSetOutputInterfaceAfterLog)
{
    Logger lg;
    lg.log(LoggerMessage::View::USER, CommandLineParserError::PARSE_SUCCESSFUL);

    Settings st;
    TestOutput out {st};

    lg.setOutputInterface(&out);

    const LoggerMessage* lgmsg = out.getOut();
    ErrorCode err = lgmsg->getErrorCode();

    EXPECT_EQ(err.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(lgmsg->getView(), LoggerMessage::View::USER);
    EXPECT_FALSE(err.getErrorCode().message().empty());
}

TEST(Logger, LogMessageUserWithConsoleOutputInterfaceWithDefaultSettings)
{
    Logger lg;
    std::ostringstream ostr;

    Settings st {};
    ConsoleOutput consoleOutputInterface {st, ostr};
    lg.setOutputInterface(&consoleOutputInterface);

    lg.log(LoggerMessage::View::USER, CommandLineParserError::PARSE_SUCCESSFUL);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::USER, SettingsError::DEAFAULT_EXTRACT);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::USER, LoggerError::FILE_CREATION_ERROR);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, CommandLineParserError::PARSE_SUCCESSFUL);
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, SettingsError::DEAFAULT_EXTRACT);
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, LoggerError::FILE_CREATION_ERROR);
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");
}

TEST(Logger, LogMessageUserWithConsoleOutputInterfaceWithCustomSettings)
{
    Logger lg;
    std::ostringstream ostr;

    std::vector<Options> opt{Options::VERBOSE};

    Settings st {opt};
    ConsoleOutput consoleOutputInterface {st, ostr};
    lg.setOutputInterface(&consoleOutputInterface);

    lg.log(LoggerMessage::View::USER, CommandLineParserError::PARSE_SUCCESSFUL);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::USER, SettingsError::DEAFAULT_EXTRACT);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::USER, LoggerError::FILE_CREATION_ERROR);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, CommandLineParserError::PARSE_SUCCESSFUL);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, SettingsError::DEAFAULT_EXTRACT);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");

    lg.log(LoggerMessage::View::SYSTEM, LoggerError::FILE_CREATION_ERROR);
    EXPECT_FALSE(ostr.str().empty());
    ostr.str("");
}

/*
TEST(Logger, SaveLog) 
{
    Settings st;
    TestOutput out {st};

    Logger lg {&out};
    
    for (size_t i = 0; i < 30; ++i)
    {
        ErrorCode err {lg.log(LoggerMessage::View::USER, CommandLineParserError::PARSE_SUCCESSFUL)};

        EXPECT_EQ(err.getStatus(), ErrorStatus::SUCCESSFUL);
        EXPECT_EQ(err.getErrorCode().value(), static_cast<int>(LoggerError::SUCCESSFUL_SAVE_LOG));
    }

    std::vector<std::string> strLog;

    std::ifstream ifile {"../../"};

    std::copy(std::istream_iterator<std::string>{})

}*/