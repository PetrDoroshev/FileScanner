#include <gtest/gtest.h>
#include "HashDataBase.h"

TEST(HashBaseTest, LoadAndCheck) {

    const char* testFile = "test_base.csv";

    {
        std::ofstream out(testFile);
        out << "53841328e5ad7851cde95cea789dcf9d;Exploit\n";
        out << "f3f0c6e992b7562598d9865b6fe8b3a6;Dropper\n";
    }

    HashDataBase base;
    base.loadDataBase(testFile);
    
    EXPECT_EQ(base.getVerdict("53841328e5ad7851cde95cea789dcf9d"), "Exploit");
    EXPECT_EQ(base.getVerdict("f3f0c6e992b7562598d9865b6fe8b3a6"), "Dropper");
    EXPECT_EQ(base.getVerdict("not_in_base"), "");

    std::remove(testFile);
}

TEST(HashBaseTest, CorruptedFile) {

    const char* testFile = "test_base.csv";

    {
        std::ofstream out(testFile);
        out << "53841328e5ad7851cde95cea789dcf9d;Exploit\n";
        out << "f3f0c6e992b7562598d9865b6fe8b3a6\n";
    }

    HashDataBase base;
    base.loadDataBase(testFile);
    
    EXPECT_EQ(base.getVerdict("53841328e5ad7851cde95cea789dcf9d"), "Exploit");
    EXPECT_EQ(base.getVerdict("f3f0c6e992b7562598d9865b6fe8b3a6"), "");
    
    std::remove(testFile);
}