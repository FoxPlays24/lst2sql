# lst2sql

**lst2sql** - CLI for parsing .LST format, creating query.txt with generated SQL script and automaticaly executing it in remote MySQL database. ```LST``` - special markdown SQL-tables, examples can be seem in "bin/examples".<br>
Raw project for a job application. I haven't heard back for couple months, so I'm uploading it to github as free-to-use, don't want to maintain it, but you're welcome to do so.

***Q:*** Why'd yo have .dlls in your bin folder?!<br>
***A:*** It's a raw project, so I decided not to handle automatic dependency loading, by the way, a shell script is used to build the project

**Knows issues:**<br>
1. 3+ table levels. Can be fixed with recursion functions for column parsing and table creation.
2. If first column is a text column, program won't be able to create records in it. Can fix this by checking whether the first column is a text type, and if so, create another column before it that will serve as a key, such as "ID".
3. Check LST files more thoroughly for their structure to ensure that invalid formats aren't overlooked.

**How to use:**<br>
```lst2sql.exe -r database:server:username:password filetoexecute.LST```

Simple table type:<br>
<img width="367" height="126" alt="Screenshot 2026-10-17 171856" src="https://github.com/user-attachments/assets/06c981b2-fc24-4200-8fa5-8f11616cb857" />

Advanced:<br>
<img width="596" height="163" alt="image" src="https://github.com/user-attachments/assets/53841af4-0b16-4c37-81cc-58a0ad06a24b" />

Generated ```query.txt``` from advanced example:<br>
<img width="613" height="910" alt="image" src="https://github.com/user-attachments/assets/0b1d2333-a5b7-47bc-9ae7-06e0d7ea26d2" />
