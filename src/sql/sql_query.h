#pragma once

#include <iostream>

#include "database.h"

using QueryList = list<wstring>;

class SqlQuery
{
private:
  QueryList queries;

  wstring parseRecord(const Table& table, Record& record);

public:
  SqlQuery(Database* db) { ParseDatabase(db); };

  friend wostream& operator<<(wostream& wos, SqlQuery& sq) {
    for (auto q : sq.queries) wos << q << endl;
    return wos;
  }

  const auto Data() { return queries; }

  void ParseDatabase(Database* db);
  void ParseTable(Table& table);
};