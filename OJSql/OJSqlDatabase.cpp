#include "OJSqlDatabase.h"
#include "OJSqlConnector.h"

namespace oj 
{

OJSqlConnector OJSqlDatabase::addConnector(const std::string& dbType)
{
    return OJSqlConnector(dbType);
}

}