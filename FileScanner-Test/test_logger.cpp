#include <gtest/gtest.h>
#include "Logger.h"
#include <fstream>
#include <string>
#include <cstdio> 


TEST(LoggerTest, LogMaliciousFile) {
    const std::string logFile = "./log_file.txt";

    std::remove(logFile.c_str());

    Logger logger;
    logger.openLogFile(logFile);
    logger.log(L"C:/malware/file.exe", "53841328e5ad7851cde95cea789dcf9d", "Exploit");
    
    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string content((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    in.close();

    EXPECT_NE(content.find("file.exe"), std::string::npos);
    EXPECT_NE(content.find("53841328e5ad7851cde95cea789dcf9d"), std::string::npos);
    EXPECT_NE(content.find("Exploit"), std::string::npos);
}

TEST(LoggerTest, LogCyrillicNameFile) {
    const std::string logFile = "./log_file.txt";

    std::remove(logFile.c_str());

    Logger logger;
    logger.openLogFile(logFile);
    logger.log(L"C:/директория/file.exe", "53841328e5ad7851cde95cea789dcf9d", "Exploit");
    
    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string content((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    in.close();

    EXPECT_NE(content.find("file.exe"), std::string::npos);
    EXPECT_NE(content.find("53841328e5ad7851cde95cea789dcf9d"), std::string::npos);
    EXPECT_NE(content.find("Exploit"), std::string::npos);
}

