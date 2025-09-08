#include "FileScanner.h"

FileScanner::FileScanner(const std::string& baseFile, const std::string& logFile) {

    this->baseFile = baseFile;
    this->logFile = logFile;
}

std::vector<std::string> FileScanner::getFilesRecursive(const std::string& rootPath) {

    std::vector<std::string> files;

    if (!fs::exists(rootPath) || !fs::is_directory(rootPath)) {
        
        std::cerr << "Directory " << rootPath << " does not exist or is not a directory." << std::endl;
        return files;
    }

    std::stack<fs::path> directory_stack;
    directory_stack.push(rootPath);

    while (!directory_stack.empty()) {

        auto path = directory_stack.top();
        directory_stack.pop();

        fs::directory_iterator di;

        try {
            di = fs::directory_iterator(path);
        }
        catch (fs::filesystem_error& ex) {
            continue;
        }

        for (const auto& dir_entry: di) {

            if (dir_entry.is_directory()) {
                directory_stack.push(dir_entry.path());
            }
            else {
                files.push_back(dir_entry.path().generic_string());
            }
        }    
    }

    return files;
}

void FileScanner::scan(const std::string& rootPath) {

    HashDataBase hash_base;
    Logger logger;

    try {

        hash_base.loadDataBase(baseFile);
        logger.openLogFile(logFile);
    }
    catch(std::runtime_error& ex) {
        
        std::cerr << ex.what() << std::endl;
        return;
    }

    auto files = getFilesRecursive(rootPath);

    for (auto& file: files) {

        check_file(file, hash_base, logger);

    }

}

void FileScanner::check_file(const std::string& filePath, HashDataBase& hashBase, Logger& logger) {

    std::string hash;

    try {
        hash = MD5(filePath);
    }
    catch (std::runtime_error& ex) {
        
    }
    
    std::string verdict = hashBase.getVerdict(hash);

    std::cout << filePath << " " << hash << " " << verdict << std::endl;

    if (!verdict.empty()) {
        logger.log(filePath, hash, verdict);
    }
}


size_t FileScanner::getTotalFiles() const {
    return totalFiles;
}

size_t FileScanner::getMaliciousFiles() const {
    return maliciousFiles;
}

size_t FileScanner::getErrorFiles() const {
    return errorFiles;
}