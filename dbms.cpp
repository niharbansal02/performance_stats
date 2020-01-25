#include<sqlite3.h>
//#include<iostream>
//#include<string.h>

using namespace std;
class dbms
{
    sqlite3 *DB;
    int flag=0;
    char sql[500];
    string createTable;
    char* msgErr;
    public:

    dbms()
    {
        flag=sqlite3_open("database.db",&DB);

        if(flag)
        {
            cerr<<"\033[1;31m Error opening database \033[0m"<<endl;
            cerr<<sqlite3_errmsg(DB);
            exit(-1);
        }
        else
        {
            cout<<"\033[1;32m Successfull opening of database \033[0m"<<endl;
        }

        createTable="CREATE TABLE IF NOT EXISTS PERF (SNO int UNIQUE PRIMARY KEY, RAM float, CPU float, DISK_IO float);";
        flag=sqlite3_exec(DB,createTable.c_str(),NULL,0,&msgErr);

        //* Initialization of database/table PERF
        char sql[500];
        int i=1;
        for(i=1;i<=50;i++)
        {
            strcpy(sql,"INSERT INTO PERF VALUES(");
            strcat(sql,to_string(i).c_str());
            strcat(sql,",0,0,0);");
            flag=sqlite3_exec(DB,sql,NULL,0,&msgErr);
//            cout<<msgErr;
        }
    }

    int update(double ram,double cpu,double io, int sno)
    {

        strcpy(sql,"UPDATE PERF SET RAM=");         // UPDATE PERF SET RAM=
        strcat(sql,to_string(ram).c_str());         // UPDATE PERF SET RAM=1.00
        strcat(sql,", CPU=");                       // UPDATE PERF SET RAM=1.00, CPU=
        strcat(sql,to_string(cpu).c_str());         // UPDATE PERF SET RAM=1.00, CPU=2.00
        strcat(sql,", DISK_IO=");                   // UPDATE PERF SET RAM=1.00, CPU=2.00, DISK_IO=
        strcat(sql,to_string(io).c_str());          // UPDATE PERF SET RAM=1.00, CPU=2.00, DISK_IO=3.00
        strcat(sql," WHERE SNO=");                  // UPDATE PERF SET RAM=1.00, CPU=2.00, DISK_IO=3.00 WHERE SNO=
        strcat(sql,to_string(sno).c_str());         // UPDATE PERF SET RAM=1.00, CPU=2.00, DISK_IO=3.00 WHERE SNO=4
        strcat(sql,";");                            // UPDATE PERF SET RAM=1.00, CPU=2.00, DISK_IO=3.00 WHERE SNO=4;

        flag=sqlite3_exec(DB,sql,NULL,0,&msgErr);

        return 0;
    }

};


    /*FORMAT FOR PASSING VALUE*/
    /*
        sno=count%51;
        obj.update(ram,cpu,io,sno);
    */

//    int main()
//    {
//        int i=0;
//        dbms obj;
//        for(;i<=50;i++)
//        obj.update(0,0,0,i);
//
//        return 0;
//    }
//
