#include <gtest/gtest.h>

#include "../src/obd_parser.hpp"

OBDParser parser = OBDParser::create();

TEST(OBDParserTest, loadError) {
    ASSERT_EQ(parser.load("wrong_file_path.csv"), -1);
}

TEST(OBDParserTest, getIndexError) {
    EXPECT_THROW(parser.getRecord(100000), std::out_of_range);
}

TEST(OBDParserTest, loadCorrect) {
    ASSERT_EQ(parser.load("../data/dataset_good.csv"), 500);
}

TEST(OBDParserTest, loadIncorrect) {
    ASSERT_NE(parser.load("../data/dataset_bad.csv"), 5000);
}