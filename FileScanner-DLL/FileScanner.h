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

class FileScanner {
public:
    FileScanner(const std::string& baseFile, const std::string& logFile);
    void scan(const std::string& rootPath);
    
    size_t getTotalFiles() const;
    size_t getMaliciousFiles() const;
    size_t getErrorFiles() const;
 
private:

    CheckStatus check_file(const std::wstring& filePath, HashDataBase& hashBase, Logger& logger);
    std::vector<std::wstring> getFilesRecursive(const std::string& rootPath);

    std::string baseFile;
    std::string logFile;

    std::mutex mutex;

    size_t totalFiles = 0;
    size_t maliciousFiles = 0;
    size_t errorFiles = 0;
};

#endif