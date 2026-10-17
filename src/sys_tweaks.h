#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <codecvt>

#define WIN32_LEAN_AND_MEAN 0
#include <windows.h>

using namespace std;

void InitTerminal()
{
  // Работаем с UTF-8 символами, поэтому устанавливаем программе соответстующую locale
  locale utf8(locale(), new codecvt_utf8<wchar_t>);
  locale::global(utf8);
  wcout.imbue(utf8); 

  #ifdef _WIN32

  SetConsoleOutputCP(CP_UTF8);

  #endif // _WIN32
}

static string ToUTF8(const wstring& s)
{
  wstring_convert<codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(s);
}