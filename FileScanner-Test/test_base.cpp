#include <gtest/gtest.h>
#include "HashDataBase.h"

TEST(HashBaseTest, LoadAndCheck) {
    HashDataBase base;
    base.loadDataBase("D:/Ci++/FileScanner/hashes.csv");
    EXPECT_EQ(base.getVerdict("a9963513d093ffb2bc7ceb9807771ad4"), "Exploit");
}