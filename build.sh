#!/bin/bash

MYSQL_CONNECTOR="C:/Program Files/MySQL/MySQL Connector C++ 26.7"

sources="src/main.cpp src/parsers/lst_parser.cpp src/sql/sql_query.cpp"
includes="-Isrc -Isrc/parsers -Isrc/sql"
flags="-std=c++17 -O2" # for debug: -g -O0
warns="-Wall"

clang++ $includes $sources -o bin/lst2sql.exe $flags $warns $mysql \
"-I$MYSQL_CONNECTOR/include" "-L$MYSQL_CONNECTOR/lib64/vs14" -lmysqlcppconn
