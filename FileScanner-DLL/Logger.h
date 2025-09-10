#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <codecvt>

/**
 * @class Logger
 * @brief Thread-safe logging utility for writing scan results to a file.
 *
 * Logger writes messages in UTF-8 encoding to ensure compatibility
 * with international file paths (including Cyrillic).
 *
 * Logged information includes:
 * - Timestamp
 * - File path
 * - MD5 hash
 * - Verdict (if malicious)
 */


class Logger {

public:
 
    explicit Logger();
    ~Logger();

    void log(const std::wstring& filePath, const std::string& hash, const std::string& verdict);
    void openLogFile(const std::string& logFilePath);

private:
    std::ofstream out;      
    std::mutex mutex;       

};

#endif