#include "Logger.h"

std::string wstring_to_utf8 (const std::wstring& str) {

    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

Logger::Logger() {}

Logger::~Logger() {
    
    if (out.is_open()) {
        out.close();
    }
}

void Logger::openLogFile(const std::string& logFilePath) {

    out = std::ofstream(logFilePath, std::ios::out | std::ios::app);

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

    out  << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") 
         << " File: " << wstring_to_utf8(filePath)
         << " Hash: " << hash
         << " Verdict: " << verdict
         << std::endl;
}



