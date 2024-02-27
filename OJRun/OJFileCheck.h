#ifndef OJ_CORE_RUN_LIMITS_H
#define OJ_CORE_RUN_LIMITS_H 

#include "../ojdef.h"

#include <vector>
#include <string>

OJ_NAMESPACE_BEGIN

/**
 * @brief 检测代码中是否有禁止使用的头文件和API
*/
class OJFileCheck 
{
public:
    OJFileCheck() = default;
    OJFileCheck(const std::string& lang) { language = lang; }

    void setLanguage(const std::string& lang) { language = lang; }
    std::string lastError() { return error; }
public:
    enum ErrorType {
        API_ERROR, IMPORT_ERROR, CHECK_SUCCESS, NOT_SET_LANG,
    };

    ErrorType check(const std::vector<std::string>& fileContent);
private:
    std::string language;
    std::string error;
};

OJ_NAMESPACE_END

#endif
