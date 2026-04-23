#include "../src/obd_parser.hpp"

#include <gtest/gtest.h>

OBDParser parser = OBDParser::create();
std::map<std::string, float> mapLabel = parser.getMapLabel();

TEST(OBDParserTest, labelConversion) {
    ASSERT_EQ(mapLabel["SLOW"], 0);
    ASSERT_EQ(mapLabel["NORMAL"], 1);
    ASSERT_EQ(mapLabel["AGGRESSIVE"], 2);
}

TEST(OBDParserTest, loadError) {
    ASSERT_EQ(parser.load("wrong_file_path.csv"), -1);
}

TEST(OBDParserTest, getIndexError) {
    EXPECT_THROW(parser.getRecord(100000), std::out_of_range);
}

TEST(OBDParserTest, loadCorrect) {
    ASSERT_EQ(parser.load("../data/dataset_good.csv"), 1000);
}

TEST(OBDParserTest, loadIncorrect) {
    ASSERT_NE(parser.load("../data/dataset.csv"), 5000);
}