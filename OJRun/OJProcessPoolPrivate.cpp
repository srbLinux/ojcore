#include "./OJProcessPoolPrivate.h"

#if __linux__
#include <sys/wait.h>
#include <sys/types.h>
#endif

OJ_NAMESPACE_BEGIN

#if __linux__
struct task_t 
{
    int qid, uid;
};
#endif

OJProcessPoolPrivate::OJProcessPoolPrivate(int num, judge_server_result callback)
    : process_num(num), callback_func(callback)
{

}

OJ_NAMESPACE_END