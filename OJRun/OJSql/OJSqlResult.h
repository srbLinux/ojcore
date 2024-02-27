#ifndef _OJ_SQL_RESULT_H
#define _OJ_SQL_RESULT_H

#include <map>
#include <vector>
#include <string>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJVariant;
class OJSqlResultBase;

class OJSqlResult
{
public:
    OJSqlResult();
    OJSqlResult(void *res, const std::string& sql_type);
    OJSqlResult(const std::vector<std::string>& keys, const std::vector<std::vector<std::string> >& values, const std::string& sql_type);

    OJSqlResult(const OJSqlResult& result);
private:
    OJSqlResultBase *result;
    std::string sqlType;
public:
    friend std::ostream& operator<<(std::ostream& os, const OJSqlResult& result);

    int rows();
    int cols();
    bool next();
    void last();
    void first();
    bool hasNext();
    OJVariant value(int i);
    OJVariant value(const std::string& key);
};

OJ_NAMESPACE_END

#endif