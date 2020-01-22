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
//#include<jsoncpp/json/json.h>
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
    double avg=0;

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

    int get_ram();
    int data_to_server();

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
    obj.init_hint_struct();
    if(obj.connect_to_server()==-1)
        exit(0);
    obj.data_to_server();
    for(int i=0;i<10;i++)
        obj.get_ram();
    obj.data_to_server();
    return 0;
}


int cplug::get_ram()
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

    avg+=(perusage)/10;

    this_thread::sleep_for(chrono::milliseconds(50));
//    cout<<"Total: "<<total<<endl;
//    cout<<"Free: "<<free<<endl;
//    cout<<"Available: "<<ava<<endl;
    cout<<"\033[1;32m % Usage: \033[0m "<<setprecision(4)<<perusage<<" %"<<endl;
    ram.close();
    if(::count%10==0)
        cout<<"Average: "<<avg<<" %"<<endl;

    cout<<"\n";
    return 1;

}

int cplug::data_to_server()
{
    string stravg;
    stravg=to_string(avg);
    cout<<"STRAVG: "<<stravg.c_str()<<endl;

    if(send(sock,(char*)&stravg,sizeof(stravg),0)==-1)
    {
        cerr<<"\033[1;31m Can't Send! \033[0m"<<endl;
        return -1;
    }

    return 1;
}

