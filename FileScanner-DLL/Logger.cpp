#include "Logger.h"

Logger::Logger() {}

Logger::~Logger() {
    
    if (out.is_open()) {
        out.close();
    }
}

void Logger::openLogFile(const std::string& logFilePath) {

    out = std::wofstream(logFilePath, std::ios::out | std::ios::app);

    if (!out.is_open()) {

        throw std::runtime_error("Cannot open txt file: " + logFilePath);
    }

}


void Logger::log(const std::wstring &filePath, const std::string &hash, const std::string &verdict) {

    if (!out.is_open()) {
        return;
    }

    std::lock_guard<std::mutex> loack(mutex);

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    out  << std::put_time(std::localtime(&t), L"%Y-%m-%d %H:%M:%S") 
         << " File: " << filePath
         << " Hash: " << std::wstring(hash.begin(), hash.end())
         << " Verdict: " << std::wstring(verdict.begin(), verdict.end())
         << std::endl;
}



