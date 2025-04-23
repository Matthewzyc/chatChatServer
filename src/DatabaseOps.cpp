#include <DatabaseOps.hpp>
#include <iostream>

static const char *db_server = "127.0.0.1";
static const char *db_user = "root";
static const char *db_password = "201531";
static const char *db_name = "db01";
static const int db_port = 3306;

MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}

MySQL::~MySQL()
{
    mysql_close(_conn);
}

bool MySQL::connect()
{
    _conn_flag = mysql_real_connect(_conn, db_server, db_user,
                                    db_password, db_name, db_port, nullptr, 0);
    if (_conn_flag == nullptr)
    {
        std::cerr << "MySQL connect failure " << mysql_errno(_conn)
                  << " : " << mysql_error(_conn) << std::endl;
        return false;
    }
    std::cout << "MySQL connect success" << std::endl;
    return true;
}

inline bool MySQL::executor(const std::string &sql, const std::string &type)
{
    _ops_flag = mysql_query(_conn, sql.c_str());
    if (_ops_flag)
    {
        std::cerr << "MySQL " << type << " failure " << mysql_errno(_conn)
                  << " : " << mysql_error(_conn) << std::endl;
        return false;
    }
    std::cout << "MySQL " << type << " success" << std::endl;
    return true;
}

bool MySQL::insert(const std::string &sql)
{
    return executor(sql, "insert");
}

bool MySQL::remove(const std::string &sql)
{
    return executor(sql, "remove");
}

bool MySQL::update(const std::string &sql)
{
    return executor(sql, "update");
}

MYSQL_RES *MySQL::query(const std::string &sql)
{
    return executor(sql, "query") ? mysql_store_result(_conn) : nullptr;
}

MYSQL *MySQL::getConnection() const
{
    return _conn;
}