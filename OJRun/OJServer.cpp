#include "OJServer.h"

#include "../OJSql/OJSql.h"

#if __linux__

#endif

OJ_NAMESPACE_BEGIN

class OJServerPrivate
{
    friend class OJServer;
public:
    OJServerPrivate(int num, judge_server_result register_func);
private:
    OJSqlConnector connector;
};

OJServerPrivate::OJServerPrivate(int num, judge_server_result register_callback)
{
    connector = OJSqlConnector("OJMYSQL");
    connector.setPassword("123"); connector.setUserName("oj_manager");
    connector.setDatabase("onlinejudge"); connector.setHost("localhost");
}

OJServer::OJServer(int num, judge_server_result resultRegisterFunc)
    : vptr(new OJServerPrivate(num, resultRegisterFunc))
{

}

void OJServer::join(const std::vector<std::string>& code, OJJudgeLanguage lang, int qid, int uid)
{
    
}

OJ_NAMESPACE_END