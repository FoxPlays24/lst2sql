#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>

#include "database.h"
#include "utils.h"

using namespace std;
using Mark = int;

class LstParser
{
private:
  Database* db;

  wifstream fs;
  wchar_t delimiter, endofheader;

  wstring line;
  wstring::size_type read = 0;
  bool isFirstLine = true;
  bool endOfHeader = false;

  map<Mark, Column*> marks;
  map<Mark, Table*> refTables;

  Table* tableBuffer = nullptr;

  bool markExists(const Mark m) { return marks.find(m) != marks.end(); }
  void addChildRecords(Mark& lastMark, Record& parentRecord, map<Mark, Column*>::iterator& it);

public:
  string filename;

  LstParser() = delete;
  LstParser(const string& filepath, Database* db, wchar_t delimiter = ':', wchar_t endofheader = 'Q')
  : db(db), delimiter(delimiter), endofheader(endofheader)
  {
    try
    {
      fs.open(filepath);
      if (!fs.is_open())
        throw (wstring(L"Не найден файл \"" + wstring(filepath.begin(), filepath.end()) + L"\""));
      filename = GetFilename(filepath);
      wcout << L"Открыт файл: \"" << wstring(filename.begin(), filename.end()) << L"\"." << endl;
    }
    catch (const wstring& ws)
    {
      wcout << L"Ошибка:  " << ws << endl;
    }
  }
  ~LstParser() { fs.close(); };

  void ReadColumn(Table& table);
  void ReadRecord(Table* table);
  bool EOHeader() { return endOfHeader; }
  bool EOFile() { return fs.eof(); }
};