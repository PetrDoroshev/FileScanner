#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <stack>
#include <vector>
#include "MD5.h"
#include "HashDataBase.h"
#include "Logger.h"

namespace fs = std::filesystem;

class FileScanner {
public:
    FileScanner(const std::string& baseFile, const std::string& logFile);
    void scan(const std::string& rootPath);
    
    size_t getTotalFiles() const;
    size_t getMaliciousFiles() const;
    size_t getErrorFiles() const;
 
private:

    void check_file(const std::string& filePath, HashDataBase& hashBase, Logger& logger);
    std::vector<std::string> getFilesRecursive(const std::string& rootPath);

    std::string baseFile;
    std::string logFile;
    size_t totalFiles = 0;
    size_t maliciousFiles = 0;
    size_t errorFiles = 0;
};

#endif