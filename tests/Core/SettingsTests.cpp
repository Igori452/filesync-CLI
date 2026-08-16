#include <gtest/gtest.h>

#include "Settings/ConfigReader.hpp"

/* MOCK */
class StreamStringDescriptor : public IDataSource 
{
    private:
        mutable std::stringstream iss;

    public:
        StreamStringDescriptor() = default;

        void setData(std::string&& data);

        std::optional<std::reference_wrapper<std::istream>> getStream() const override;
};

void StreamStringDescriptor::setData(std::string&& data) 
{
    iss << data;
}

std::optional<std::reference_wrapper<std::istream>> StreamStringDescriptor::getStream() const 
{
    return iss;
}

TEST(Parsing, CheckParsingFromStream) 
{
    std::vector<ConfigData::ConfigLineData> data 
    {
        {"key1", "val1"},
        {"key2", "val2"},
        {"key3", "val3"},
        {"key4", "val4"},
    };

    ConfigData cfgd {};
    StreamStringDescriptor ssd {};

    for (auto it = std::begin(data); it != std::end(data); ++it)
    {
        cfgd.addConfigLine(it->first, it->second);
        ssd.setData(it->first + ": " + it->second + "\n");
    }

    ParsingConfigFromTxt parsingTxt {};
    auto parseRes = parsingTxt.parse(ssd);

    ASSERT_TRUE(parseRes.has_value());

    std::vector<ConfigData::ConfigLineData> parseCfgd = parseRes.value().getConfigLines();

    for (auto it1 = std::begin(parseCfgd), it2 = std::begin(data); it1 != std::end(parseCfgd) && it2 != std::end(data); ++it1, ++it2) 
    {
        EXPECT_EQ(it1->first, it2->first);
        EXPECT_EQ(it1->second, it2->second);
    }
}

TEST(Parsing, CheckParsingFromStreamWithTabs) 
{
    std::vector<ConfigData::ConfigLineData> data 
    {
        {"key1", "val1"},
        {"key2", "val2"},
        {"key3", "val3"},
        {"key4", "val4"},
    };

    ConfigData cfgd {};
    StreamStringDescriptor ssd {};

    for (auto it = std::begin(data); it != std::end(data); ++it)
    {
        cfgd.addConfigLine(it->first, it->second);
        ssd.setData("\t    " + it->first + "               \t\n\n:\t" + it->second + "\t       \t \n\n\n");
    }

    ParsingConfigFromTxt parsingTxt {};
    auto parseRes = parsingTxt.parse(ssd);

    ASSERT_TRUE(parseRes.has_value());

    std::vector<ConfigData::ConfigLineData> parseCfgd = parseRes.value().getConfigLines();

    for (auto it1 = std::begin(parseCfgd), it2 = std::begin(data); it1 != std::end(parseCfgd) && it2 != std::end(data); ++it1, ++it2) 
    {
        EXPECT_EQ(it1->first, it2->first);
        EXPECT_EQ(it1->second, it2->second);
    }
}

TEST(ConfigManager, ParsingFromFileWithHelpOfConfigManager) 
{    
    std::vector<ConfigData::ConfigLineData> data 
    {
        {"key1", "val1"},
        {"key2", "val2"},
        {"key3", "val3"},
        {"key4", "val4"},
    };

    StreamStringDescriptor ssd {};
    for (auto it = std::begin(data); it != std::end(data); ++it)
    {
        ssd.setData("\t    " + it->first + "               \t\n\n:\t" + it->second + "\t       \t \n\n\n");
    }

    ConfigManager cfgManager {std::make_unique<ParsingConfigFromTxt>()};

    ErrorCode err {cfgManager.parseCfgFrom(ssd)};

    EXPECT_EQ(err.getStatus(), ErrorStatus::SUCCESSFUL);
    EXPECT_EQ(err.getErrorCode().value(), static_cast<int>(SettingsError::EXTRACT_SUCCESSFUL));

    std::vector<ConfigData::ConfigLineData> cfgd = cfgManager.releaseConfigData()->getConfigLines();

    for (auto it1 = std::begin(cfgd), it2 = std::begin(data); it1 != std::end(cfgd) && it2 != std::end(data); ++it1, ++it2) 
    {
        EXPECT_EQ(it1->first, it2->first);
        EXPECT_EQ(it1->second, it2->second);
    }

    err = cfgManager.parseCfgFrom(ssd);

    EXPECT_EQ(err.getStatus(), ErrorStatus::WARNING);
    EXPECT_EQ(err.getErrorCode().value(), static_cast<int>(SettingsError::BAD_CONFIG_MANAGER_OPERATION));

    std::vector<ConfigData::ConfigLineData> cfgdEmpty = cfgManager.releaseConfigData()->getConfigLines();

    EXPECT_TRUE(cfgdEmpty.empty());
}