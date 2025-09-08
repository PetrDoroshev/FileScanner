#include "FileScanner.h"

FileScanner::FileScanner(const std::string& baseFile, const std::string& logFile) {

    this->baseFile = baseFile;
    this->logFile = logFile;
}

std::vector<std::wstring> FileScanner::getFilesRecursive(const std::string& rootPath) {

    std::vector<std::wstring> files;

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
                files.push_back(dir_entry.path().generic_wstring());
            }
        }    
    }

    totalFiles = files.size();

    return files;
}

void FileScanner::scan(const std::string& rootPath) {

    HashDataBase hash_base;
    Logger logger;
    ThreadPool thread_pool;

    std::vector<std::future<CheckStatus>> check_tasks;

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
           
        auto future = thread_pool.add_task([this, file, &hash_base, &logger] {
            
            return check_file(file, hash_base, logger);
        });

        check_tasks.emplace_back(std::move(future)); 
    }

    for (auto& result: check_tasks) {
        
        switch (result.get()) {

            case CheckStatus::ERR:
                errorFiles++;
                break;

            case CheckStatus::MALICIOUS:
                maliciousFiles++;
                break;

            default:
                break;
        }
    }

    std::wcout << errorFiles << " ," << maliciousFiles << std::endl;

}

CheckStatus FileScanner::check_file(const std::wstring& filePath, HashDataBase& hashBase, Logger& logger) {

    std::string hash;

    try {

        hash = MD5(filePath);
    }
    catch (std::runtime_error& ex) {

        std::cerr << ex.what() << std::endl;
        return CheckStatus::ERR;
    }
    
    std::string verdict = hashBase.getVerdict(hash);

    if (!verdict.empty()) {

        logger.log(filePath, hash, verdict);
        return CheckStatus::MALICIOUS;
    }

    return CheckStatus::OK;
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