#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

class Logger {

public:
 
    explicit Logger();
    ~Logger();

    void log(const std::wstring& filePath, const std::string& hash, const std::string& verdict);
    void openLogFile(const std::string& logFilePath);

private:
    std::wofstream out;      
    std::mutex mutex;       

};

#endif