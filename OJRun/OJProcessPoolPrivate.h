#ifndef _OJ_CORE_PROCESS_POOL_PRIVATE_H
#define _OJ_CORE_PROCESS_POOL_PRIVATE_H

#define _USE_BASE_HEADERS_ 1

#include <queue>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJProcessPool;

struct task_t;
struct process_t;

class OJProcessPoolPrivate
{
    friend class OJProcessPool;
public:
    OJProcessPoolPrivate(int num, judge_server_result callback);

    void join(const std::vector<std::string> &code, int uid, int qid, OJJudgeLanguage lang);
private:
    int process_num;
    std::queue<task_t> task_queue;
    judge_server_result callback_func;
};

OJ_NAMESPACE_END

#undef _USE_BASE_HEADERS_

#endif