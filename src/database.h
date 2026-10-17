#pragma once

#include <string>
#include <stdexcept>

#include <vector>
#include <list>

#include "utils.h"

using namespace std;

using Record = list<wstring>;

enum class ColumnType
{
  integer,
  string,
  date,
  reference
};

struct Column
{
  wstring name;
  ColumnType type;
  Column* parent = nullptr;

  Column() = delete;
  Column(wstring name, ColumnType type) : name(name), type(type) {}
  virtual ~Column() {}

  bool HasReference() { return parent != nullptr; }
  void SetReference(Column* column)
  { 
    parent = column;
    type = ColumnType::reference;
  }
};

class Table
{
private:
  wstring name;
  vector<Column*> columns;
  list<Record> records;

  bool indexed = false;

public:
  Table() = delete;
  Table(wstring name) : name(name) {};
  Table(string name) : name(wstring(name.begin(), name.end())) {};
  
  int idx = 0;

  const wstring& Name() { return name; }
  const auto Indexed() { return indexed; }
  const auto Columns() { return columns; }
  const auto Records() { return records; }

  auto GetColumn(int idx) { return columns.at(idx); };

  Column* AddColumn(wstring name, ColumnType type)
  {
    auto column = new Column(name, type);
    columns.push_back(column);
    return column;
  }

  Record* AddRecord(Record& record)
  {
    if (record.size() != Columns().size())
    {
      wcout << L"Ошибка:  Размер записи должен совпадать с кол-вом столбцов таблицы \"" << name << "\"" << endl;
      return nullptr;
    }

    records.push_back(record);
    return &record;
  }

  const auto RecordsCount() { return records.size(); }

  Record* FindRecord(Record& queryRecord)
  {
    for (auto& record : records)
      if (list(next(record.begin()), record.end()) == queryRecord) return &record;
    return nullptr;
  }

  void AddIndexing()
  {
    columns.insert(columns.begin(), new Column(L"ID", ColumnType::integer));
    indexed = true;
  }

  void ToReference(Column* column, Column* parent)
  {
    if (column->HasReference())
      throw runtime_error("Столбец уже является ссылкой");
    column->SetReference(parent);
  }

  void CombineColumnNames(Column* column, wstring& name)
  {
    column->name += name;
  }

  void SetColumnType(Column* column, ColumnType type)
  {
    column->type = type;
  }

  void FormatColumns()
  {
    for (auto c : columns)
      c->name = FormatString(c->name);
  }

  void Print()
  {
    wcout << L"Таблица: \"" << name << "\"" << endl;

    if (columns.empty()) return;

    wcout << L"Столбцы (" << columns.size() << "):\n";
    for (auto column : columns)
    {
      wcout << "  \"" << column->name << "\"";

      if (column->HasReference())
      {
        wcout << L" -> \"" << column->parent->name << "\"";
      }
      else
      {
        //                                        integer, string,          date
        static const vector<wstring> typeName = { L"int",  L"varchar(255)", L"date" };
        wcout << " (" << typeName.at(int(column->type)) << ")";
      }
      wcout << "\n";
    }

    wcout << "\n";

    wcout << L"Записи (" << records.size() << "):\n";
    for (auto record : records)
    {
      for (auto cell : record)
        wcout << L"'" << cell << "' ";
      wcout << "\n";
    }

    wcout << endl;
  }
};

class Database
{
private:
  list<Table*> tables;

public:
  void AddTable(Table* tb) { tables.push_front(tb); }
  auto Tables() { return tables; };
  void Print()
  {
    wcout << L"База данных:\n";
    for (auto tb : tables) tb->Print();
  }
  void FormatTables()
  {
    for (auto table : tables)
      table->FormatColumns();
  }
};