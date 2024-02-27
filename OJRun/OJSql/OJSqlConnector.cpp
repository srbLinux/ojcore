#include "OJSqlResult.h"
#include "OJSqlConnector.h"

#include <sqlite3.h>
#include <mysql/mysql.h>

OJ_NAMESPACE_BEGIN

class OJSqlResult;

class OJSqlConnectorBase
{
    friend class OJSqlConnector;
public:
    virtual bool open() = 0;
    virtual bool isOpen() = 0;
    virtual OJSqlResult result() = 0;
    virtual std::string version() = 0;
    std::string lastErrror() { return error; }
    virtual bool exec(const std::string& sql) = 0;
protected:
    virtual bool checkRequiredParams() = 0;
protected:
    bool connect;
    std::string error;
    std::string db, user, passwd, host;
};

class OJMysqlConnector : public OJSqlConnectorBase
{
    friend class OJSqlConnector;
public:
    bool open() override;
    bool isOpen() override;
    std::string version() override;
    bool exec(const std::string& sql) override;
    OJSqlResult result() override { return res; }
    inline void setEncode(const std::string& encode) { this->encode=encode; }
protected:
    bool checkRequiredParams() override;
private:
    MYSQL connector;
    OJSqlResult res;
    std::string encode="set names utf8";
};

class OJSqliteConnector : public OJSqlConnectorBase
{
    friend class OJSqlConnector;
public:
    bool open() override;
    bool isOpen() override;
    std::string version() override;
    bool exec(const std::string& sql) override;
    OJSqlResult result() override { return res; }
protected:
    bool checkRequiredParams() override;
private:
    sqlite3 *sqlite;
    OJSqlResult res;
};

/**
 * 
 * @brief SQLITE数据库实现
 * 
*/

std::string OJSqliteConnector::version() {
    return std::string("SQLITE ") + std::string(SQLITE_VERSION);
}

bool OJSqliteConnector::exec(const std::string& sql)
{
    int nrow, ncol;
    char *errMsg=NULL;
    char **result=NULL;
    int code = sqlite3_get_table(sqlite, sql.c_str(), &result, &nrow, &ncol, &errMsg);
    if (code != SQLITE_OK) {
        error = errMsg; return false;
    }

    std::vector<std::string> keys;
    std::vector<std::vector<std::string> > values;
    for (int i = 0;i < ncol;i ++ ) {
        keys.push_back(result[i]);
    }
    for (int i = 0;i < nrow;i ++ ) {
        std::vector<std::string> value;
        for (int j = 0;j < ncol;j ++ ) {
            value.push_back(result[(nrow+1)*ncol+j]);
        }
        values.push_back(value);
    }
    res = OJSqlResult(keys, values, "OJSQLITE");
    return true;
}

bool OJSqliteConnector::isOpen()
{
    return OJSqlConnectorBase::connect;
}

bool OJSqliteConnector::open()
{
    int code = sqlite3_open(OJSqlConnectorBase::db.c_str(), &sqlite);
    if (code != SQLITE_OK) {
        error = sqlite3_errmsg(sqlite);
    }
    return true;
}

bool OJSqliteConnector::checkRequiredParams()
{
    if (!OJSqlConnectorBase::db.length()) {
        error = "sqlite cannot connect, because OJSqliteCOnnector::db is null.";
        return false;
    }
    return true;
}

/**
 * 
 * @brief MYSQL数据库实现
 * 
 */

bool OJMysqlConnector::exec(const std::string& sql) 
{
    mysql_query(&connector, encode.c_str());
    if (mysql_query(&connector, sql.c_str())) {
        error = mysql_error(&connector);
        return false;
    } else {
        res = OJSqlResult((void *)mysql_store_result(&connector), "OJMYSQL");
    }
    return true;
}

std::string OJMysqlConnector::version() {
    return std::string("MYSQL ")+std::string(LIBMYSQL_VERSION);
}

bool OJMysqlConnector::isOpen()
{
    return connect;
}

bool OJMysqlConnector::open()
{
    if (!checkRequiredParams()) {
        connect = false;
    }
    if (!mysql_init(&connector)) {
        connect = false;
        error = std::string(mysql_error(&connector));
    }

    if (mysql_real_connect(&connector, 
        host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), 0, NULL, CLIENT_FOUND_ROWS)) {
        connect = true;
    } else {
        error = std::string(mysql_error(&connector));
        connect = false;
    }

    return connect;
}

bool OJMysqlConnector::checkRequiredParams()
{

    if (!host.length()) {
        error = "mysql cannot connect, bacause OJMysqlConenctor::host is null.";
        return false;
    }

    if (!db.length()) {
        error = "mysql cannot connect, bacause OJMysqlConnector::db is null";
        return false;
    }

    if (!user.length()) {
        error = "mysql cannot connect, bacause OJMysqlConnector::user is null.";
        return false;
    }

    if (!passwd.length()) {
        error = "mysql cannot connect, bacause OJMysqlConnector::passwd is null.";
        return  false;
    }

    return true;
}



/**
 * @brief OJSqlConnector类函数实现
 * 
 * 
 * 
*/
OJSqlConnector::OJSqlConnector(const std::string& type)
    : dbType(type), connector(NULL)
{
    if (dbType == "OJMYSQL") {
        connector = new OJMysqlConnector;
    } else if (dbType == "OJSQLITE") {
        connector = new OJSqliteConnector;
    }
}

OJSqlConnector::OJSqlConnector(const OJSqlConnector& connector)
{
    this->dbType = connector.dbType;
    this->connector = connector.connector;
}

bool OJSqlConnector::open() { return connector->open(); }

bool OJSqlConnector::isOpen() { return connector->isOpen(); }

OJSqlResult OJSqlConnector::result() { return connector->result(); }

std::string OJSqlConnector::lastError() { return connector->lastErrror(); }

void OJSqlConnector::setHost(const std::string& host) { connector->host = host; }

bool OJSqlConnector::exec(const std::string& sql) { return connector->exec(sql); }

void OJSqlConnector::setUserName(const std::string& user) { connector->user = user; }

void OJSqlConnector::setPassword(const std::string& passwd) { connector->passwd = passwd; }

void OJSqlConnector::setDatabase(const std::string& database) { connector->db = database; }

OJ_NAMESPACE_END