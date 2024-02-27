#ifndef _OJ_CORE_RUN_PROCESS_POOL_H
#define _OJ_CORE_RUN_PROCESS_POOL_H

#define __OJ_USE_SEM__ 1

#include <vector>
#include <string>

#include "../ojdef.h"
#include "./OJProcessPool.h"

OJ_NAMESPACE_BEGIN

class OJProcessPoolPrivate;

class OJProcessPool 
{
public:
    OJProcessPool() = default;
    OJProcessPool(int num, judge_server_result register_func);

    void join(const std::vector<std::string>& code, int uid, int pid, OJJudgeLanguage lang);
private:
    OJProcessPoolPrivate *vptr;
};

OJ_NAMESPACE_END

#endif