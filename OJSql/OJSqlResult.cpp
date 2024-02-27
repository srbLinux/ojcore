#include "OJSqlResult.h"

#include <vector>
#include <mysql/mysql.h>

#include "../OJUtils/OJVariant.h"

OJ_NAMESPACE_BEGIN

class OJSqlResult;

class OJSqlResultBase
{
    friend class OJSqlResult;
public:
    virtual int rows() = 0;
    virtual int cols() = 0;
    virtual bool next() = 0;
    virtual bool hasNext() = 0;
    virtual OJVariant value(int i) = 0;
    virtual OJVariant value(const std::string& key) = 0;
    inline void first() { record_ptr = 0; }
    inline void last() { record_ptr = values.size() - 1; }
protected:
    // void *res;
    int record_ptr;
public:
    size_t col, row;
    std::vector<std::string> keys;
    std::vector<std::vector<std::string>> values;

    friend std::ostream& operator<<(std::ostream& os, const OJSqlResultBase& result);
};

std::ostream& operator<<(std::ostream& os, const OJSqlResultBase& result)
{
    os << "The result of the SQL query is as follows: \n";
    for (int i = 0;i < result.col;i ++ ) {
        os << result.keys[i] << "\t";
    }
    os << "\n";
    if (result.row < 8) {
        for (int i = 0;i < result.row;i ++ ) {
            for (int j = 0;j < result.col;j ++ ) {
                os << result.values[i][j] << "\t";
            }
            os << "\n";
        }
    } else {
        for (int i = 0;i < 4;i ++ ) {
            for (int j = 0;j < result.col;j ++ ) {
                os << result.values[i][j] << "\t";
            }
            os << "\n";
        }

        for (int i = result.row-4;i < result.row;i ++ ) {
            for (int j = 0;j < result.col;j ++ ) {
                os << result.values[i][j] << "\t";
            }
            os << "\n";
        }
    }
    os << "cols: " << result.col << " rows: " << result.row << "\n";
    return os;
}

class OJMysqlResult : public OJSqlResultBase
{
    friend class OJSqlResult;
public:
    OJMysqlResult() = default;
    OJMysqlResult(MYSQL_RES *res);
    int rows() override;
    int cols() override;
    bool next() override;
    bool hasNext() override;
    OJVariant value(int i) override;
    OJVariant value(const std::string& key) override;

private:
    MYSQL_RES *res;
    MYSQL_ROW resultRow;
};

class OJSqliteResult : public OJSqlResultBase
{
    friend class OJSqlResult;
public:
    OJSqliteResult() = default;
    int rows() override;
    int cols() override;
    bool next() override;
    bool hasNext() override;
    OJVariant value(int i) override;
    OJVariant value(const std::string& key) override;
private:

};

OJMysqlResult::OJMysqlResult(MYSQL_RES *res) 
    : res(res) { 
    OJSqlResultBase::record_ptr = 0;
    this->row = mysql_num_rows(res);
    this->col = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_field(res);
    for (int i=0;i<col;i++) {
        keys.push_back(fields[i].name);
    }
    for (int i=0;i<row;i++) {
        std::vector<std::string> value;
        resultRow = mysql_fetch_row(res);
        for (int j=0;j<col;j++) {
            value.push_back(resultRow[j]);
        }
        values.push_back(value);
    }
}

int OJMysqlResult::rows() {
    return row;
}

int OJMysqlResult::cols() {
    return col;
}

bool OJMysqlResult::hasNext() {
    return (record_ptr <= (mysql_num_rows(res)-1));
}

bool OJMysqlResult::next() {
    if (record_ptr <= (mysql_num_rows(res)-1)) {
        record_ptr++;
        return true;
    }
    return false;
}

OJVariant OJMysqlResult::value(int i) {
    if (i > col || i < 0) {
        return OJVariant("FIND ERROR");
    }
    return OJVariant(values[record_ptr][i]);
}

OJVariant OJMysqlResult::value(const std::string& key) {
    int coli = -1;
    for (int i=0;i<mysql_num_fields(res);i++) {
        if (keys[i]==key) {
            coli = i; break;
        }
    }
    if (coli==-1) {
        return OJVariant("FIND ERROR");
    }
    return OJVariant(values[record_ptr][coli]);
}

int OJSqliteResult::cols() { return col; }

int OJSqliteResult::rows() { return row; }

bool OJSqliteResult::hasNext() {
    if (this->record_ptr < values.size() - 1) {
        return true;
    }
    return false;
}

bool OJSqliteResult::next() {
    if (this->record_ptr > values.size() - 1) {
        return false;
    }
    this->record_ptr ++ ;
    return true;
}

OJVariant OJSqliteResult::value(int i) { return values[record_ptr][i]; }

OJVariant OJSqliteResult::value(const std::string& key) {
    return values[record_ptr][1];
}

OJSqlResult::OJSqlResult() {  }

OJSqlResult::OJSqlResult(const OJSqlResult& other) 
{
    sqlType = other.sqlType;
    result = other.result;
}

OJSqlResult::OJSqlResult(void *res, const std::string& sql_type)
{
    if (sql_type == "OJMYSQL") {
        result = new OJMysqlResult((MYSQL_RES *)res);
    }
}

OJSqlResult::OJSqlResult(const std::vector<std::string>& keys, const std::vector<std::vector<std::string> >& values, const std::string& sql_type) 
{
    if (sql_type == "OJSQLITE") {
        result = new OJSqliteResult();
    } else if (sql_type == "OJMYSQL") {
        result = new OJMysqlResult();
    }
}

void OJSqlResult::first() { return result->first(); }

int OJSqlResult::cols() { return result->cols(); }

int OJSqlResult::rows() { return result->rows(); }

bool OJSqlResult::next() { return result->next(); }

bool OJSqlResult::hasNext() { return result->hasNext(); }

OJVariant OJSqlResult::value(const std::string& key) {
    return result->value(key);
}

std::ostream& operator<<(std::ostream& os, const OJSqlResult& result) 
{
    os << (*result.result);
    return os;
}

OJ_NAMESPACE_END

