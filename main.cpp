#include <iostream>

#include "OJRun/OJRun.h"
#include "OJSql/OJSql.h"
#include "OJJson/OJJson.h"
#include "OJUtils/OJUtils.h"

#if __linux__
#include <dirent.h>
#include <sys/resource.h>
#endif

#include <sstream>

void OJRunTest();
void OJSqlTest();
void OJJsonTest();
void OJUtilsTest();

void result_register_callback(const char *codestr, int code)
{

}

int main()
{
    // OJUtilsTest();
    OJRunTest();
    return 0;
}

void OJRunTest()
{
    oj::OJServer server(10, result_register_callback);
#if __linux__ 
    std::string file, lastName;
    DIR *codeDir = opendir("./test_code");
    if (codeDir != NULL) {
        dirent *entry;
        while ((entry = readdir(codeDir)) != NULL) {
            if (entry->d_type == DT_REG) {
                file = entry->d_name;
                std::string filePath = "./test_code/";
                filePath += file;
                std::string line;
                std::vector<std::string> fileContent;
                std::ifstream outStream;
                outStream.open(filePath, std::ios::out);
                while (std::getline(outStream, line)) {
                    fileContent.push_back(line);
                }
                size_t dotIndex = file.find_last_of(".");
                if (dotIndex != std::string::npos && dotIndex < file.length() - 1) {
                    lastName = file.substr(dotIndex + 1);
                    if (lastName == ".c") {
                        server.join(fileContent, OJJudgeLanguage::C99, 1, 1);
                    }
                }
            }
        }
    }
#endif
}

void OJUtilsTest()
{
    oj::OJVariant var1=10;
    oj::print(var1);
    var1 = "API TEST";
    oj::print(var1);
    oj::OJLogger logger("./1.log");
}