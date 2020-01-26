# Performance Stats

This is a c++ console application which extracts CPU Performance information such as CPU Utilization, RAM Usage & DISK I/O Usage. It echoes the same to a localhost server in a .json object.
The server parses the .json object and stores the information in a database using SQLite.

This is a platform specific console application.
This was coded on Kali Linux 2019.3
It shall work fine for every Linux distribution.
DBMS used:- SQLite

## Note
Use the following command to compile the code in your terminal:-

#### client.cpp
######  $ g++ client.cpp -lpthread -o client
######  $ ./client

### server.cpp
###### $ g++ server.cpp -lsqlite3 -o server
###### $ ./server

There might occur a binding error in case of server. In that case, execute the following command:-
###### $ pidof server
(pid)
###### $ kill (pid)        
