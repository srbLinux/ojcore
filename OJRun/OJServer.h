#ifndef _OJ_CORE_SERVER_H
#define _OJ_CORE_SERVER_H 

#include <vector>
#include <string>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJJsonObject;

class OJServerPrivate;

class OJServer 
{
public:
    OJServer(int num, judge_server_result resultRegisterFunc);

    void join(const std::vector<std::string>& code, OJJudgeLanguage lang, int questionID, int userID);

    OJJudgeState result(int questionID, int userID);
private:
    OJServerPrivate *vptr;
};

OJ_NAMESPACE_END

/**
 * @brief   项目编译成动态库(so, dll)时需要被导出的C语言函数，
 *          因为C++有着二进制不兼容的特性，很难直接编译成库被其他语言包装调用
*/
__OJ_C_EXPORT_API__ void judge_server_run();

__OJ_C_EXPORT_API__ OJJudgeState judge_server_join_task(int qid, int uid, const char *code, OJJudgeLanguage lang);


#endif