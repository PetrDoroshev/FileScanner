#include <gtest/gtest.h>
#include "Logger.h"
#include <fstream>
#include <string>
#include <cstdio> 


TEST(LoggerTest, LogMaliciousFile) {
    const std::string logFile = "D:\\\\Ci++\\FileScanner\\log_file.txt";

    // Удаляем файл, если он остался от предыдущего запуска
    std::remove(logFile.c_str());

    Logger logger(logFile);
    logger.log("C:/malware/file.exe", "deadbeef12345678", "Exploit");

    // Проверяем, что файл создан
    std::ifstream in(logFile);
    ASSERT_TRUE(in.is_open());

    std::string content((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
    in.close();

    // Проверяем, что строка содержит все части
    EXPECT_NE(content.find("file.exe"), std::string::npos);
    EXPECT_NE(content.find("deadbeef12345678"), std::string::npos);
    EXPECT_NE(content.find("Exploit"), std::string::npos);
}