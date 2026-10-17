#pragma once

#include <string>
#include <codecvt>

using namespace std;

static string GetFilename(const string& name)
{
  const string path = name.substr(name.find_last_of("/\\") + 1);
  return path.substr(0, path.find('.'));
}

static wstring FormatString(wstring s)
{
  size_t pos = s.find(L"  ");

  // Стираем двойные пробелы
  while (pos != wstring::npos)
  {
    s.replace(pos, 2, L" ");
    pos = s.find(L"  ", pos);
  }

  // Тримим
  if (s[0] == ' ') s = s.substr(1);
  if (s.back() == ' ') s = s.substr(0, s.length()-1);

  return s;
}

static bool IsInt(const wstring& s)
{
  return (s.find_first_not_of(L"0123456789") == wstring::npos);
}