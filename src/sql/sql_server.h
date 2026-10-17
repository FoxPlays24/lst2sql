#pragma once

#include "sql_query.h"

#include <iostream>
#include <mysql/jdbc.h>

struct SqlServerOptions { string database, hostname, username, password; };

class SqlServer
{
private:
  unique_ptr<sql::Connection> connection;

  void writeQueryTXT(const QueryList& queries)
  {
    wofstream fs("query.txt");
    if (!fs)
    {
      wcout << "Ошибка:  Не получилось записать query.txt" << endl;
      return;
    }

    for (auto query : queries)
      fs << query << endl;
    fs.close();
  }

public:
  void Connect(SqlServerOptions& opt)
  {
    wcout << L"Подключение к БД..."  << endl;
    try
    {
      sql::ConnectOptionsMap copt;
      copt["hostName"] = opt.hostname;
      copt["userName"] = opt.username;
      copt["password"] = opt.password;
      copt["schema"]   = opt.database;
      copt["characterSetResults"]  = "utf8";
      copt["OPT_CHARSET_NAME"]     = "utf8";
      copt["OPT_SET_CHARSET_NAME"] = "utf8";

      sql::Driver* driver = sql::mysql::get_driver_instance();
      connection = unique_ptr<sql::Connection>(driver->connect(copt));
      connection->setSchema(opt.database);
    }
    catch (const sql::SQLException& e)
    {
      wcout << L"Ошибка подключения к БД:  " << e.what() << endl;
    }
    catch (const exception& e)
    {
      wcout << L"Ошибка:  " << e.what() << endl;
    }
  }
  ~SqlServer() { if (connection != nullptr) connection->close(); }

  void Exec(const QueryList& queries)
  {
    try
    {
      if (connection == nullptr)
        throw(wstring(L"Нет подключения к БД. Убедитесь что вы перед Exec() подключились к БД через Connect() или LoadFile()."));

      unique_ptr<sql::Statement> st{connection->createStatement()};

      st->execute("SET FOREIGN_KEY_CHECKS=0;"); // Отключаем временно проверку foreign key, т.к. 
                                                // при обновлении записей БД ругается, не так безопасно как хотелось бы
      for (auto query : queries)
        st->execute(ToUTF8(query));
      writeQueryTXT(queries);
      st->execute("SET FOREIGN_KEY_CHECKS=1;");

      wcout << L"Изменения внесены в БД!" << endl;

    }
    catch (const sql::SQLException& e)
    {
      wcout << L"Ошибка выполнения SQL-запроса:\n" << e.what() << endl;
    }
    catch (const wstring& e)
    {
      wcout << L"Ошибка:  " << e << endl;
    }
  }
};