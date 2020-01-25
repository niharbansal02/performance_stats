#include<ctype.h>
#include<chrono>
#include<iomanip>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<limits>
#include<netdb.h>
#include<string>
#include<pthread.h>
#include<thread>
#include<math.h>
#include<time.h>
#include<fstream>
#include<string.h>
#include<arpa/inet.h>
#include<algorithm>
#include<stdlib.h>
#include"json_maker.cpp"

#define corr "Correct!"
#define incorrid "Enter correct ID!"
#define incorrpass "Enter correct Password!"
#define def "Please enter correct choice!"
#define ENTRIES 60
#define DELAY 50
#define SENDTIME 2000

int itr=SENDTIME/DELAY;
using namespace std;
static long long count=0;

typedef unsigned int ui;

class cplug
{
    int ch;                            //*! int !! Test var.
    char passkey[50];
    // int sock;
    int port;
    int sendInt;                //!Replace the ID,PASS,CHANGE to sendInt later
    int connectRes;
    string ipAddress;
    int bytesReceived;
    char buf[4608];
    string strRec;
    sockaddr_in hint;
    double ram_avg=0;
    double idle_avg=0;
    double io_avg=0;
    char Main[500];

    public:
    int sock;
    cplug()                                                          //Sorted
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==-1)
            cerr<<"\033[1;31mCan't create socket\033[0m";

//        strcpy(Main,"{ ");
    }

    void print_json()
    {
        cout<<Main<<endl;
    }
    void init_hint_struct()                                         //Sorted
    {
        port=54000;
        ipAddress="127.0.0.1";
        hint.sin_family=AF_INET;
        hint.sin_port=htons(port);
        inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);
    }

    int connect_to_server()
    {
        connectRes=connect(sock,(sockaddr*)&hint,sizeof(hint));
        if(connectRes==-1)
        {
            cerr<<"\033[1;31mConnection failed\033[0m";
            return -1;
        }
    }

    void complete_json()
    {
        strcat(Main," }");
    }

    void get_ram()
    {
        ::count++;
        ifstream ram;
        string line;
        ram.open("/proc/meminfo");
        double ava,free,total,perusage,totFree,used;

        while(ram>>line)
        {
            if(line=="MemTotal:")
            {
                ram>>total;
            }
            else if(line=="MemFree:")
            {
                ram>>free;
            }
            else if(line=="MemAvailable:")
                ram>>ava;
        }

        totFree=ava;
        used=total-totFree;

        perusage=(used/total)*100;

        ram_avg+=(perusage)/itr;         //change

        this_thread::sleep_for(chrono::milliseconds(DELAY));
        cout<<"\033[1;32m % Usage: \033[0m "<<setprecision(4)<<perusage<<" %"<<endl;
        ram.close();
        if(::count%itr==0)           //change
        {
    //        cout<<"Average: "<<ram_avg<<" %"<<endl;
            strcpy(Main,"{ ");
            maker(Main,GET_VARIABLE_NAME(ram_avg),ram_avg);
    //        data_to_server(ram_avg);                              //!
            ram_avg=0;
    //        ::count=0;
        }
    }

    int data_to_server();

    void runMultiThread();

    void get_cpu_idle_io();

    ~cplug()
    {
//        int exitsignal=100;
//        send(sock,(int *)&exitsignal,sizeof(exitsignal),0);
        close(sock);
    }
};


void *PrintNihar(void *threadid)        //Test func for threads
{
//    this_thread::sleep_for(chrono::seconds(1));
    long tid;
    tid=(long)threadid;
    cout<<"Nihar, "<<tid<<endl;

    pthread_exit(NULL);
}

int main()
{
    cplug obj;
    obj.init_hint_struct();

    if(obj.connect_to_server()==-1)
        exit(0);

    for(int k=0;k<ENTRIES;k++)
    {
//        this_thread::sleep_for(chrono::milliseconds(50));
        for(int i=0;i<itr;i++)
        {
//            this_thread::sleep_for(chrono::milliseconds(50));
            obj.runMultiThread();
        }




//        for(int i=0;i<itr;i++)
//            obj.get_cpu_idle_io();

//
        obj.complete_json();
        obj.print_json();
        obj.data_to_server();

        cout<<endl<<" MAIN(): "<<endl;

    }
    return 0;
}

void cplug::runMultiThread()
{
    thread t1(&cplug::get_ram,this);
    thread t2(&cplug::get_cpu_idle_io,this);
    t1.join();
    t2.join();
}

int cplug::data_to_server()
{
    int sendRes=send(sock,(const char*)&Main,sizeof(Main),0);
    if(sendRes==-1)
    {
//        cerr<<"\033[1;31m Can't Send! \033[0m"<<endl;
        return -1;
    }

    return 1;
}

void cplug::get_cpu_idle_io()
{
    ::count++;
    ifstream fin;
//    fin.open("log_iostat.txt");
    this_thread::sleep_for(chrono::milliseconds(DELAY));
    system("iostat > log_iostat.txt");
    fin.open("log_iostat.txt");
    string line;
    double cpu_idle,io,dump;
    while(fin>>line)
    {
        if(line=="%idle")
        {
            fin>>dump;
            fin>>dump;
            fin>>dump;
            fin>>io;
            fin>>dump;
            fin>>cpu_idle;
            break;
        }
    }

    idle_avg+=(100-cpu_idle)/itr;            //change
    io_avg+=io/itr;                          //change

    cout<<(100-cpu_idle)<<" %"<<endl;
    cout<<io<<" %"<<endl;
    if(::count%itr==0)                   //change
    {
//        cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<idle_avg<<" %"<<endl;
        maker(Main,GET_VARIABLE_NAME(idle_avg),idle_avg);
//        cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<io_avg<<" %"<<endl;
        maker(Main,GET_VARIABLE_NAME(io_avg),io_avg,stop);
        idle_avg=0;
        io_avg=0;
    }

    fin.close();

//    return 1;
}
