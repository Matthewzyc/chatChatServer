#ifndef _DATABASE_OPS_H_
#define _DATABASE_OPS_H_

#include <mysql/mysql.h>
#include <string>

class MySQL
{
public:
    MySQL();
    ~MySQL();
    
    bool connect();
    inline bool executor(const std::string &sql, const std::string &type);
    bool insert(const std::string &sql);
    bool remove(const std::string &sql);
    bool update(const std::string &sql);
    MYSQL_RES* query(const std::string &sql);
    MYSQL* getConnection() const;

private:
    MYSQL* _conn;
    MYSQL* _conn_flag;
    bool _ops_flag;
};

#endif