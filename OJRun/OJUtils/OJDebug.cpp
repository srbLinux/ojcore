#include "OJDebug.h"

#include <algorithm>

OJ_NAMESPACE_BEGIN

int OJDebug::index = 0;
std::vector<std::string> OJDebug::formats;

OJDebug::OJDebug() { }

OJDebug::OJDebug(const std::string& format) { this->format=format; }

void OJDebug::putFormat(const std::string& format) {
    this->format = format;
}

void OJDebug::print() {
    std::string result = format;
    
    // 确保占位符数量不超过placeholders的大小
    size_t numPlaceholders = std::count(format.begin(), format.end(), '{');
    if (numPlaceholders > placeholders.size()) {
        std::cerr << "Error: Not enough placeholders to replace placeholders." << std::endl; return;
    }

    // 替换占位符
    size_t valueIndex = 0;
    size_t placeholderPos = result.find('{');
    while (placeholderPos != std::string::npos && valueIndex < placeholders.size()) {
        size_t endPos = result.find('}', placeholderPos);
        if (endPos != std::string::npos) {
            result.replace(placeholderPos, endPos - placeholderPos + 1, placeholders[valueIndex]);
            ++valueIndex;
        } else {
            std::cerr << "Error: Invalid format - mismatched curly braces." << std::endl; return;
        }
        placeholderPos = result.find('{', placeholderPos + 1);
    }
    std::cout << result << std::endl;
}

OJLogger::OJLogger(const std::string& file_path)
{
    infile.open(file_path, std::ios::app);
    if (!infile.is_open()) {
        error = "OJLogger::OJLogger set file failed.";
        return;
    }
}

void OJLogger::print(const std::string& context, LogLevel level) 
{
    time_t now = time(NULL);
    tm *currTm = localtime(&now);
    char time[100] = { 0 };
    strftime(time, 100, "%Y-%m-%d %H:%M:%S", currTm);
    std::string inputContent = time;
    if (level == OJLogger::ERROR) {
        inputContent += " [ERROR] ";
    } else if (level == OJLogger::INFO) {
        inputContent += " [INFO] ";
    } else if (level == OJLogger::WARNING) {
        inputContent += " [WARNING] ";
    }
    inputContent += context + "\n";
    this->infile << inputContent;
}

OJ_NAMESPACE_END