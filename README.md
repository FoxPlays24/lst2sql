# lst2sql

**lst2sql** - CLI for parsing .LST format, creating query.txt with generated SQL script and automaticaly executing it in remote MySQL database.
LST - special markdown SQL-tables, examples can be seem in "bin/examples".
Raw project for a job application. I haven't heard back for couple months, so I'm uploading it to github as free-to-use, don't want to maintain it, but you're welcome to do so.

How to use:
```lst2sql.exe -r database:server:username:password filetoexecute.LST```

Q: Why'd yo have .dlls in your bin folder?!
A: It's a raw project, so I decided not to handle automatic dependency loading, by the way, a shell script is used to build the project

Knows issues:
1. 3+ table levels. Can be fixed with recursion functions for column parsing and table creation.
2. If first column is a text column, program won't be able to create records in it. Can fix this by checking whether the first column is a text type, and if so, create another column before it that will serve as a key, such as "ID".
3. Check LST files more thoroughly for their structure to ensure that invalid formats aren't overlooked.
