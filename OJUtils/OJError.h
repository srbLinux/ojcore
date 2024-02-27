#ifndef _OJ_CORE_UTILS_ERROR_H
#define _OJ_CORE_UTILS_ERROR_H

#include <string>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJErrorPrivate;

class OJError 
{
public:
    OJError();

    size_t lastErrno();
    std::string lastError();
private:
    OJErrorPrivate *vptr;
};

OJ_NAMESPACE_END

#endif