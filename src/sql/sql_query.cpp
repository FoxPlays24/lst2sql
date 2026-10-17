#include "sql_query.h"

wstring SqlQuery::parseRecord(const Table& table, Record& record)
{
  wstring query;

  for (auto value : record)
    query += L"\"" + value + L"\",";
  query.pop_back();

  return query;
}

void SqlQuery::ParseTable(Table& table)
{
  wstring query = L"CREATE TABLE IF NOT EXISTS `" + table.Name() + L"` (\n";
  wstring foreign_keys;

  for (auto column : table.Columns())
  {
    query += L"  `" + column->name + L"` ";

    if (column->parent)
      foreign_keys += L"  FOREIGN KEY (`" + column->name + L"`) REFERENCES `" + 
        column->name + L"`(`" + column->parent->name + L"`) ON UPDATE CASCADE,\n";

    //                                        int,    string,          date,    reference
    static const vector<wstring> typeName = { L"INT", L"varchar(255)", L"DATE", L"INT" };
    query += typeName[int(column->type)] + L",\n";
  }

  query += L"  PRIMARY KEY (`" + table.GetColumn(0)->name + L"`)";
  if (!foreign_keys.empty())
    query += L",\n" + foreign_keys.substr(0, foreign_keys.length()-2);
  query += L"\n);\n";

  queries.push_back(query);
}

void SqlQuery::ParseDatabase(Database* db)
{
  for (auto table : db->Tables())
  {
    ParseTable(*table);

    wstring query = L"REPLACE INTO `" + table->Name() + L"` VALUES\n";
    for (auto record : table->Records())
      query += L"(" + parseRecord(*table, record) + L"),\n";
    query[query.length()-2] = L';';
    queries.push_back(query);
  }
}
