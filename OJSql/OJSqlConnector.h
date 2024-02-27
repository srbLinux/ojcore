#ifndef _OJ_CORE_SQL_CONNECTOR_H
#define _OJ_CORE_SQL_CONNECTOR_H 

#include <string>

#include "../ojdef.h"

OJ_NAMESPACE_BEGIN

class OJSqlResult;
class OJSqlConnectorBase;

class OJSqlConnector
{
public:
    OJSqlConnector() = default;
    OJSqlConnector(const std::string& dbType);
    OJSqlConnector(const OJSqlConnector& connector);

    void setHost(const std::string& host);
    void setUserName(const std::string& user);
    void setPassword(const std::string& passwd);
    void setDatabase(const std::string& database);

    bool open();
    bool isOpen();

    std::string version();
    std::string lastError();

    OJSqlResult result();
    bool exec(const std::string& sql);

private:
    std::string dbType;
    OJSqlConnectorBase *connector;
};

OJ_NAMESPACE_END

#endif