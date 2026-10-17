#include "sys_tweaks.h"
#include "database.h"

#include "lst_parser.h"
#include "sql_server.h"
#include "sql_query.h"

#include <iostream>

int main(int argc, char *argv[])
{
  InitTerminal();

  if (argc < 4)
  {
    std::wcout << L"lst2sql -r <бд:адрес:логин:пароль> <.lst файл>" << std::endl;
    return 1;
  }

  // Чтение LST файла
  Database lstDb;

  // r - remote
  if (string(argv[1]) != "-r")
    return 1;

  // Берем аргумент с данными подключения к БД и заносим в программу
  vector<string> connData;
  char* token = strtok(argv[2], ":");
  while (token != nullptr)
  {
    connData.push_back(token);
    token = strtok(nullptr, ":");
  }

  if (connData.size() != 4)
  {
    wcout << "Ошибка:  Неверное кол-во значений для подключения, пример: -c <бд:адрес:логин:пароль>" << endl;
    return 1;
  }

  // Парсим LST файл
  LstParser lstParser(argv[3], &lstDb);

  Table mainTable(lstParser.filename);
  lstDb.AddTable(&mainTable);

  while(!lstParser.EOHeader())
    lstParser.ReadColumn(mainTable);
  
  while(!lstParser.EOFile())
    lstParser.ReadRecord(&mainTable);

  // Создаем SQL-запрос для создания базы данных из LST файла
  SqlQuery query(&lstDb);

  // Чтение SQL базы данных
  SqlServer sqlServer;

  SqlServerOptions opt;
  opt.database = connData[0]; // "sql12836007";
  opt.hostname = connData[1]; // "sql12.freesqldatabase.com";
  opt.username = connData[2]; // "sql12836007";
  opt.password = connData[3]; // "yeWETXhJuE";

  sqlServer.Connect(opt);
  sqlServer.Exec(query.Data());

  wcout << L"\nНажмите любую клавишу чтобы завершить работу программы...";
  getchar();
  return 0;
}
