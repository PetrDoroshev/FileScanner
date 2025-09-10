#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <stack>
#include <vector>
#include <future>
#include <mutex>
#include "MD5.h"
#include "HashDataBase.h"
#include "Logger.h"
#include "ThreadPool.h"
#include "ThreadPool.cpp"

namespace fs = std::filesystem;

enum class CheckStatus {

    ERR,
    MALICIOUS,
    OK

};

/**
 * @class FileScanner
 * @brief Performs recursive scanning of files in a directory to detect malicious content.
 *
 * The FileScanner is the main class responsible for:
 * - Traversing directories recursively.
 * - Comparing file hashes against a known malicious hash database (CSV).
 * - Logging detections into a report file.
 * - Collecting scan statistics (total files, malicious files, errors, elapsed time).
 *
 * Usage:
 * @code
 * FileScanner file_scanner("base.csv", "report.log");
 * file_scanner.scan("C:/folder");
 * @endcode
 */

class FileScanner {
public:
    FileScanner(const std::string& baseFile, const std::string& logFile);
    void scan(const std::string& rootPath);
    
    size_t getTotalFiles() const;
    size_t getMaliciousFiles() const;
    size_t getErrorFiles() const;
 
private:

    CheckStatus checkFile(const std::wstring& filePath, HashDataBase& hashBase, Logger& logger);
    std::vector<std::wstring> getFilesRecursive(const std::string& rootPath);

    std::string baseFile;
    std::string logFile;

    std::mutex mutex;

    size_t totalFiles = 0;
    size_t maliciousFiles = 0;
    size_t errorFiles = 0;
};

#endif