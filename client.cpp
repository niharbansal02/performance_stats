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
#include<thread>
#include<math.h>
#include<time.h>
#include<fstream>
#include<string.h>
#include<arpa/inet.h>
#include<algorithm>
#include<stdlib.h>
#include<jsoncpp/json/json.h>
#include<json-c/json.h>
#define corr "Correct!"
#define incorrid "Enter correct ID!"
#define incorrpass "Enter correct Password!"
#define def "Please enter correct choice!"

using namespace std;
static int count=0;

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

    public:
    int sock;
    cplug()                                                          //Sorted
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==-1)
            cerr<<"\033[1;31mCan't create socket\033[0m";
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

    void get_ram();
    int data_to_server(double);
    void get_cpu_idle();

    ~cplug()
    {
        int exitsignal=100;
        send(sock,(int *)&exitsignal,sizeof(exitsignal),0);
        close(sock);
    }
};




int main()
{
    cplug obj;
    thread th1,th2;
    obj.init_hint_struct();
    if(obj.connect_to_server()==-1)
        exit(0);
    for(int i=0;i<200;i++)
        obj.get_ram();
//        th1=thread(obj.get_ram);        //Not working
    for(int i=0;i<200;i++)
        obj.get_cpu_idle();
//        th2=thread(obj.get_cpu_idle);
//    threa
//    obj.data_to_server();
    cout<<"HI FROM main() afete dtatda(); "<<endl;
    return 0;
}


void cplug::get_ram()
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

    totFree=ava+free;
    used=total-totFree;

    perusage=(used/total)*100;

    ram_avg+=(perusage)/200;

    this_thread::sleep_for(chrono::milliseconds(50));
//    cout<<"Total: "<<total<<endl;
//    cout<<"Free: "<<free<<endl;
//    cout<<"Available: "<<ava<<endl;
    cout<<"\033[1;32m % Usage: \033[0m "<<setprecision(4)<<perusage<<" %"<<endl;
    ram.close();
    if(::count%200==0)
    {
        cout<<"Average: "<<ram_avg<<" %"<<endl;
        data_to_server(ram_avg);
        ram_avg=0;
    }

//    cout<<"\n";
//    return 1;

}

int cplug::data_to_server(double val)
{
    if(send(sock,(double*)&val,sizeof(val),0)==-1)
    {
        cerr<<"\033[1;31m Can't Send! \033[0m"<<endl;
        return -1;
    }

    return 1;
}

void cplug::get_cpu_idle()
{
    ::count++;
    ifstream fin;
//    fin.open("log_iostat.txt");
    this_thread::sleep_for(chrono::milliseconds(50));
    system("iostat > log_iostat.txt");
    fin.open("log_iostat.txt");
    string line;
    double cpu_idle;
    while(fin>>line)
    {
        if(line=="0.00")
        {
            fin>>cpu_idle;
            break;
        }
    }

    idle_avg+=(100-cpu_idle)/200;

    cout<<(100-cpu_idle)<<" %"<<endl;
    if(::count%200==0)
    {
        cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<idle_avg<<" %";
        data_to_server(idle_avg);
        idle_avg=0;
    }


    fin.close();

//    return 1;
}
