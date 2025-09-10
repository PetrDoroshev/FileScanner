#include <gtest/gtest.h>
#include "FileScanner.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class ScannerFullTest : public ::testing::Test {
protected:
    fs::path testDir = "test_scan_dir";
    fs::path csvFile = "test_base.csv";
    fs::path logFile = "test_log.txt";

    void SetUp() override {
       
        fs::create_directory(testDir);

        {
            std::ofstream bad(testDir / "malicious.txt");
            bad << "some malicious content";
        }

        {
            std::ofstream good(testDir / "good.txt");
            good << "definitely not malicious";
        }

        fs::create_directory(testDir / "subdir");
        {
            std::ofstream nested(testDir / "subdir" / "nested.txt");
            nested << "hello";
        }

        {
            std::ofstream csv(csvFile);
            csv << "8c9b2badb29575ec67fd02e80e033031;Malicious\n";
        }
    }

    void TearDown() override {
        fs::remove_all(testDir);
        fs::remove(csvFile);
        fs::remove(logFile);
    }
};

TEST_F(ScannerFullTest, DetectsMaliciousAndCleanFiles) {

    FileScanner file_scanner(csvFile.string(), logFile.string());
    file_scanner.scan(testDir.string());

    EXPECT_EQ(file_scanner.getTotalFiles(), 3);        
    EXPECT_EQ(file_scanner.getMaliciousFiles(), 1);    
    EXPECT_EQ(file_scanner.getErrorFiles(), 0);

    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string content((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    in.close();

    EXPECT_NE(content.find("malicious.txt"), std::string::npos);
    EXPECT_NE(content.find("Malicious"), std::string::npos);
}

TEST_F(ScannerFullTest, EmptyDirectory) {
    fs::path emptyDir = "empty_scan_dir";
    fs::create_directory(emptyDir);

    FileScanner file_scanner(csvFile.string(), logFile.string());
    file_scanner.scan(emptyDir.string());

    EXPECT_EQ(file_scanner.getTotalFiles(), 0);
    EXPECT_EQ(file_scanner.getMaliciousFiles(), 0);
    EXPECT_EQ(file_scanner.getErrorFiles(), 0);

    fs::remove_all(emptyDir);
}

TEST_F(ScannerFullTest, NestedDirectories) {
    FileScanner file_scanner(csvFile.string(), logFile.string());
    file_scanner.scan(testDir.string());

    EXPECT_EQ(file_scanner.getTotalFiles(), 3); 
    EXPECT_GE(file_scanner.getMaliciousFiles(), 1);
}

TEST_F(ScannerFullTest, LargeFile) {
    fs::path bigFile = testDir / "big.bin";
    {
        std::ofstream out(bigFile, std::ios::binary);
        std::string block(1024 * 1024, 'A'); // 1 MB
        for (int i = 0; i < 1000; i++) out << block;
    }

    FileScanner file_scanner(csvFile.string(), logFile.string());
    file_scanner.scan(testDir.string());

    EXPECT_EQ(file_scanner.getTotalFiles(), 4); // +1 big.bin
    EXPECT_EQ(file_scanner.getErrorFiles(), 0);
}

TEST_F(ScannerFullTest, ManyFilesStressTest) {
    fs::path stressDir = testDir / "stress";
    fs::create_directory(stressDir);

    for (int i = 0; i < 200; i++) {
        std::ofstream f(stressDir / ("file" + std::to_string(i) + ".txt"));
        f << (i % 50 == 0 ? "some malicious content" : "clean");
    }

    FileScanner file_scanner(csvFile.string(), logFile.string());
    file_scanner.scan(testDir.string());

    EXPECT_GE(file_scanner.getTotalFiles(), 200);
    EXPECT_GE(file_scanner.getMaliciousFiles(), 4);
}
