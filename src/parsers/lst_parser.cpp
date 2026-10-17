// ПРОБЛЕМА:
//   Что если мы столкнёмся со столбцом с повторяющимися значениями, 
//   но который не является ребенком какой-либо таблицы?
//
// РЕШЕНИЕ:
//   Моим решением было записывать их также как и обычные столбцы. 
//   Ведь по такому файлу не будет предельно ясно, случайно или последовательно повторяется
//   то или иное значение.
//
//   P.S. Из вариантов решения было: если такой столбец хранит буквенные значения, то он 
//        станет самостоятельной таблицей с ключом и одним столбцом хранящим это значение.

#include "lst_parser.h"

bool EOL(const wstring::size_type& it) { return it == wstring::npos; }
bool IsHLine(const wstring& s)
{ 
  return s.length() > 1 && (s == wstring(s.length(), '-'));
}
bool IsDate(const wstring& s) { return (FormatString(s) == L"Г.М.Ч."); }

void LstParser::ReadColumn(Table& table)
{
  auto it = line.find(delimiter, read) + 1;

  if (EOL(it - 1))
  {
    if (line.back() == endofheader)
    {
      endOfHeader = true;
      db->FormatTables();
      return;
    }

    // Уже есть прочитанные символы
    if (read != 0)
    {
      wstring name = line.substr(read, line.length());
      if (isFirstLine)
      {
        marks[line.length()] = table.AddColumn(name, ColumnType::integer);
        isFirstLine = false;
      }
      else if (!IsHLine(name))
      {
        table.CombineColumnNames(marks.rbegin()->second, name);
      }
    }

    getline(fs, line);
    read = 0;
    if (!fs.eof())
      ReadColumn(table);

    return;
  }

  auto name = line.substr(read, it - read - 1);
  auto prev = read;
  read = it;

  // Проверки
  if (IsHLine(name))
    return;

  if (IsDate(name))
  {
    table.SetColumnType(marks[it], ColumnType::date);
    return;
  }

  if (isFirstLine)
  {
    marks[it] = table.AddColumn(name, ColumnType::integer);
    return;
  }

  if ((markExists(it) && markExists(prev) && marks[it]->type != ColumnType::reference) 
      || (!prev && markExists(it)))
  {
    table.CombineColumnNames(marks[it], name);
  }
  // Добавление столбца
  else
  {
    auto parent = marks.lower_bound(it)->second;
    auto formatted = FormatString(name);

    if (tableBuffer != nullptr && tableBuffer->Name() == FormatString(parent->name))
    {
      marks[it] = tableBuffer->AddColumn(formatted, ColumnType::integer);
      refTables[it] = tableBuffer;
    }
    else
    {
      auto newTable = new Table(FormatString(parent->name));
      auto refColumn = newTable->AddColumn(L"ID", ColumnType::integer);
      table.ToReference(parent, refColumn);
      auto newColumn = newTable->AddColumn(formatted, ColumnType::integer);

      db->AddTable(newTable);
      table.SetColumnType(parent, ColumnType::reference);
      marks[it] = newColumn;
      refTables[it] = newTable;

      tableBuffer = newTable;
    }
  }
}

// Некрасивая функция, выделенная отдельно, чтобы не сильно нагружать функцию ReadRecord()
void LstParser::addChildRecords(Mark& lastMark, Record& parentRecord, map<Mark, Column*>::iterator& it)
{
  auto refTable = refTables[it->first];
  Record record;
  
  // i = 1, Исключаем проверку столбца-ключа
  for (int i = 1; i < refTable->Columns().size(); i++)
  {
    auto value = line.substr(lastMark, it->first - lastMark - 1);
    auto formatted = FormatString(value);
    record.push_back(formatted);
    lastMark = it->first;
    it++;
  }
  it--;

  // Проверяем, существует ли уже такая запись: 
  //   если да  - записываем ключ в род. запись,
  //   если нет - создаем новую запись и берем созданный ключ, записываем его в род. запись
  Record* existing = refTable->FindRecord(record);
  if (existing != nullptr)
  {
    parentRecord.push_back(existing->front());
  }
  else
  {
    auto idx = to_wstring(refTable->RecordsCount());
    parentRecord.push_back(idx);
    record.push_front(idx);
    refTable->AddRecord(record);
  }

  lastMark = it->first;
}

void LstParser::ReadRecord(Table* table)
{
  getline(fs, line);

  Mark last_m = 0;
  Record record;

  for (auto it = marks.begin(); it != marks.end(); it++)
  {
    auto value = it == prev(marks.end()) ? line.substr(last_m) : line.substr(last_m, it->first - last_m - 1);
    auto formatted = FormatString(value);

    if (!IsInt(formatted))
      it->second->type = ColumnType::string;

    if (refTables.find(it->first) != refTables.end())
    {
      addChildRecords(last_m, record, it);
      continue;
    }

    record.push_back(formatted);
    last_m = it->first;
  }

  if (!table->Indexed() && marks.begin()->second->type == ColumnType::string)
    table->AddIndexing();

  if (table->Indexed())
    record.push_front(to_wstring(table->idx++));
  table->AddRecord(record);
}