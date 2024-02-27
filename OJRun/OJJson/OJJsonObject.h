#ifndef _OJ_CORE_JSON_OBJECT_H
#define _OJ_CORE_JSON_OBJECT_H 

#include "../ojdef.h"

#include <map>

OJ_NAMESPACE_BEGIN

class OJJsonObject 
{
public:
    OJJsonObject() = default;
    OJJsonObject(const std::map<std::string, std::string>& data);

    
};

OJ_NAMESPACE_END


#endif