#include <gtest/gtest.h>

#include "Logger.hpp"
#include "ModuleSettings.hpp"

class TestObserver : public UserOutput 
{
    private:
        mutable int cnt = 0;
        mutable const LoggerMessage* logOutput = nullptr;

    public:
        explicit TestObserver(OutputUserSettings st);

        void display(const LoggerMessage& lgmsg) const override;
        
        const LoggerMessage* getOut() const;
        int getCnt() const;
};

TestObserver::TestObserver(OutputUserSettings st) : UserOutput(std::move(st)) {}

void TestObserver::display(const LoggerMessage& lgmsg) const 
{
    logOutput = &lgmsg;
    ++cnt;
}

const LoggerMessage* TestObserver::getOut() const 
{
    return logOutput;
}

int TestObserver::getCnt() const 
{
    return cnt;
}

TEST(Logger, SingletonInstance)
{
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();

    EXPECT_EQ(&logger1, &logger2);
}

TEST(Logger, LogWithObserver)
{
    Logger& logger = Logger::instance();

    OutputUserSettings settings;
    TestObserver testObserver {settings};

    logger.subscribe(&testObserver);
    EXPECT_FALSE(logger.hasObserver());
    EXPECT_EQ(testObserver.getCnt(), 0);
    
    auto state = logger.subscribe(&testObserver);
    EXPECT_TRUE(logger.hasObserver());
    EXPECT_EQ(testObserver.getCnt(), 0);

    logger.unsubscribe();
    EXPECT_FALSE(logger.hasObserver());
    EXPECT_EQ(testObserver.getCnt(), 0);
}

TEST(Logger, LogUserAndSystemMessageWithTestObserver)
{
    std::string testMessageInfoUser = "testMessageInfoUser";
    std::string testMessageWarningUser = "testMessageWarningUser";
    std::string testMessageErrorUser = "testMessageErrorUser";
    std::string testMessageInfoSystem = "testMessageInfoSystem";
    std::string testMessageWarningSystem = "testMessageWarningSystem";
    std::string testMessageErrorSystem = "testMessageErrorSystem";

    Logger& logger = Logger::instance();

    OutputUserSettings settings;
    TestObserver testObserver {settings};
    auto state = logger.subscribe(&testObserver);

    logger.log(MessageStatus::INFO, MessageView::USER, testMessageInfoUser);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::INFO);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::USER);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageInfoUser);
    EXPECT_EQ(testObserver.getCnt(), 1);
    
    logger.log(MessageStatus::WARNING, MessageView::USER, testMessageWarningUser);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::WARNING);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::USER);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageWarningUser);
    EXPECT_EQ(testObserver.getCnt(), 2);

    logger.log(MessageStatus::ERROR, MessageView::USER, testMessageErrorUser);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::ERROR);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::USER);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageErrorUser);
    EXPECT_EQ(testObserver.getCnt(), 3);

    logger.log(MessageStatus::INFO, MessageView::SYSTEM, testMessageInfoSystem);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::INFO);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::SYSTEM);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageInfoSystem);
    EXPECT_EQ(testObserver.getCnt(), 4);

    logger.log(MessageStatus::WARNING, MessageView::SYSTEM, testMessageWarningSystem);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::WARNING);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::SYSTEM);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageWarningSystem);
    EXPECT_EQ(testObserver.getCnt(), 5);

    logger.log(MessageStatus::ERROR, MessageView::SYSTEM, testMessageErrorSystem);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::ERROR);
    EXPECT_EQ(testObserver.getOut()->getView(), MessageView::SYSTEM);
    EXPECT_EQ(testObserver.getOut()->getMessage(), testMessageErrorSystem);
    EXPECT_EQ(testObserver.getCnt(), 6);
}

TEST(Logger, LogMessageWithTestObserverSubscriveAfterLog)
{
    std::string testMessage = "testMessageInfoUser";

    Logger& logger = Logger::instance();
    OutputUserSettings settings;
    TestObserver testObserver {settings};

    logger.log(MessageStatus::INFO, MessageView::USER, testMessage);
    auto state = logger.subscribe(&testObserver);
    EXPECT_EQ(testObserver.getOut()->getStatus(), MessageStatus::INFO);
    EXPECT_EQ(testObserver.getCnt(), 1);
}

TEST(Logger, LogMessageUserWithConsoleOutputObserverWithDefaultSettings)
{
    Logger& logger = Logger::instance();
    std::ostringstream ostr;

    OutputUserSettings settings {};
    ConsoleOutput consoleOutputObserver {settings, ostr};
    auto state = logger.subscribe(&consoleOutputObserver);

    LoggerMessage testLoggerMessage1 {MessageStatus::INFO, MessageView::USER, "testMessageInfoUser"};
    LoggerMessage testLoggerMessage2 {MessageStatus::WARNING, MessageView::USER, "testMessageWarningUser"};
    LoggerMessage testLoggerMessage3 {MessageStatus::ERROR, MessageView::USER, "testMessageErrorUser"};
    LoggerMessage testLoggerMessage4 {MessageStatus::INFO, MessageView::SYSTEM, "testMessageInfoSystem"};
    LoggerMessage testLoggerMessage5 {MessageStatus::WARNING, MessageView::SYSTEM, "testMessageWarningSystem"};
    LoggerMessage testLoggerMessage6 {MessageStatus::ERROR, MessageView::SYSTEM, "testMessageErrorSystem"};

    logger.log(testLoggerMessage1.getStatus(), testLoggerMessage1.getView(), testLoggerMessage1.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage1.formatText());
    ostr.str("");

    logger.log(testLoggerMessage2.getStatus(), testLoggerMessage2.getView(), testLoggerMessage2.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage2.formatText());
    ostr.str("");

    logger.log(testLoggerMessage3.getStatus(), testLoggerMessage3.getView(), testLoggerMessage3.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage3.formatText());
    ostr.str("");

    logger.log(testLoggerMessage4.getStatus(), testLoggerMessage4.getView(), testLoggerMessage4.getMessage());
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");

    logger.log(testLoggerMessage5.getStatus(), testLoggerMessage5.getView(), testLoggerMessage5.getMessage());
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");

    logger.log(testLoggerMessage6.getStatus(), testLoggerMessage6.getView(), testLoggerMessage6.getMessage());
    EXPECT_TRUE(ostr.str().empty());
    ostr.str("");
}

TEST(Logger, LogMessageUserWithConsoleOutputObserverWithCustomSettings)
{
    InputCommand ic {Commands::COMPARE, {"source", "target"}, {Options::VERBOSE}};
    ASSERT_EQ(ic.options[0], Options::VERBOSE);

    Logger& logger = Logger::instance();
    std::ostringstream ostr;

    OutputUserSettings settings {};
    settings.setSettings(ic);

    ConsoleOutput consoleOutputObserver {settings, ostr};
    auto state = logger.subscribe(&consoleOutputObserver);

    LoggerMessage testLoggerMessage1 {MessageStatus::INFO, MessageView::USER, "testMessageInfoUser"};
    LoggerMessage testLoggerMessage2 {MessageStatus::WARNING, MessageView::USER, "testMessageWarningUser"};
    LoggerMessage testLoggerMessage3 {MessageStatus::ERROR, MessageView::USER, "testMessageErrorUser"};
    LoggerMessage testLoggerMessage4 {MessageStatus::INFO, MessageView::SYSTEM, "testMessageInfoSystem"};
    LoggerMessage testLoggerMessage5 {MessageStatus::WARNING, MessageView::SYSTEM, "testMessageWarningSystem"};
    LoggerMessage testLoggerMessage6 {MessageStatus::ERROR, MessageView::SYSTEM, "testMessageErrorSystem"};

    logger.log(testLoggerMessage1.getStatus(), testLoggerMessage1.getView(), testLoggerMessage1.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage1.formatText());
    ostr.str("");

    logger.log(testLoggerMessage2.getStatus(), testLoggerMessage2.getView(), testLoggerMessage2.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage2.formatText());
    ostr.str("");

    logger.log(testLoggerMessage3.getStatus(), testLoggerMessage3.getView(), testLoggerMessage3.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage3.formatText());
    ostr.str("");

    logger.log(testLoggerMessage4.getStatus(), testLoggerMessage4.getView(), testLoggerMessage4.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage4.formatText());
    ostr.str("");

    logger.log(testLoggerMessage5.getStatus(), testLoggerMessage5.getView(), testLoggerMessage5.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage5.formatText());
    ostr.str("");

    logger.log(testLoggerMessage6.getStatus(), testLoggerMessage6.getView(), testLoggerMessage6.getMessage());
    EXPECT_EQ(ostr.str(), testLoggerMessage6.formatText());
    ostr.str("");
}