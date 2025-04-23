#include <DatabaseOps.hpp>
#include <iostream>

int main()
{
    MySQL mysql;
    mysql.connect();
    mysql.insert("INSERT INTO user(name, age) VALUES('Dlice', 40)");
    mysql.remove("DELETE FROM user WHERE age = 30");
    mysql.update("UPDATE user SET age = 50 WHERE name = 'Alice'");
    MYSQL_RES *res = mysql.query("SELECT * FROM user");
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        for (int i = 0; i < 3; i++)
        {
            if (i != 0)
            {
                std::cout << '\t';
            }
            std::cout << row[i];
        }
        std::cout << '\n';
    }
    return 0;
}