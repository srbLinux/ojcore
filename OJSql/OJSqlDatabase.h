#ifndef _OJ_CORE_SQL_DATABASE_H
#define _OJ_CORE_SQL_DATABASE_H 

#include <string>

namespace oj 
{

class OJSqlConnector;

class OJSqlDatabase
{
public:
    static OJSqlConnector addConnector(const std::string& dbType);
};

}

#endif