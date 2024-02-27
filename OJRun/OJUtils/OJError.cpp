#include "./OJError.h"

#include <map>

OJ_NAMESPACE_BEGIN

class OJErrorPrivate 
{
    friend class OJError;
private:
    OJErrorPrivate();

    static std::map<size_t, std::string> errMap;
};

std::map<size_t, std::string> OJErrorPrivate::errMap = {
    { 101, "has not found this column." }
};

OJ_NAMESPACE_END