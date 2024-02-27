#ifndef _OJ_CORE_RUN_JUDGE_PROCESS_H
#define _OJ_CORE_RUN_JUDGE_PROCESS_H

#include <string>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJJudgeProcess 
{
public:
    OJJudgeProcess();

    void join(const std::string& code, const std::string& lang, int uid, int qid);
};

OJ_NAMESPACE_END

#endif