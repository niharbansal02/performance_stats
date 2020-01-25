//Working fine
//#include"sock_macro.cpp"
//#include"splug.cpp"
#include<ctype.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<limits>
#include<netdb.h>
#include<string>
#include<math.h>
#include<time.h>
#include<fstream>
#include<string.h>
#include<arpa/inet.h>
#include<algorithm>
#include<chrono>
#include<thread>
#include<stdlib.h>
//#include"josn_parser.cpp"
#include"dbms.cpp"
#define corr "Correct!"
#define incorrid "Enter correct ID!"
#define incorrpass "Enter correct Password!"
#define def "Please enter correct choice!"

using namespace std;
int count=0;
#define ignore cin.ignore(numeric_limits<streamsize>::max(),'\n')
class splug : public dbms
{
    int listening;
    sockaddr_in hint,client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket;
    int result;
    int ram;
    int idbuf;
    struct info
    {
        double ram,io,cpu;
    }mem;

    char buf[4096];
    char passbuf[8];
    char MAINstr[500];
    int bytesRecv;
    string getString;                           //Test var.

    public:
    int dec=1;
    splug()
    {
//        ::count++;
        memset(MAINstr,0,500);
        listening=socket(AF_INET,SOCK_STREAM,0);
        if(listening==-1)
            cerr<<"\033[1;31m Can't create socket.\033[0m ";
    }

    int bind_socket()
    {
        hint.sin_family=AF_INET;
        hint.sin_port=htons(54000);
        inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);
        if(bind(listening,(sockaddr*)&hint,sizeof(hint))==-1)
        {
            cerr<<"\033[1;31m Can't bind to IP/PORT\033[0m";
            return -1;
        }
    }

    int listen_in()
    {
        if(listen(listening,SOMAXCONN)==-1)
        {
            cerr<<"\033[1;31m Can't listen!\033[0m";
            return -1;
        }
//        cout<<"listening"<<endl;
    }

    int accept_call()
    {
        clientSize=sizeof(client);
        clientSocket=accept(listening,(sockaddr*)&client,&clientSize);
        if(clientSocket==-1)
        {
            cerr<<"\033[1;31m Problem with client connecting!\033[0m";
            return -1;
        }
//        cout<<"accept call"<<endl;
    }

    void close_listening()
    {
        close(listening);
        memset(host,0,NI_MAXHOST);                                      //Cleanup of host
        memset(svc,0,NI_MAXSERV);
//        cout<<"close listening"<<endl;
    }

    void connect_message()
    {
        result=getnameinfo((sockaddr*)&client,clientSize,host,NI_MAXHOST,svc,NI_MAXSERV,0);
        if(result)                                                      //result==0
        {
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<svc<<"\033[0m"<<endl;
        }
        else
        {
            inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<ntohs(client.sin_port)<<"\033[0m"<<endl;
        }
    }

    int data();

    int data_to_dbms()
    {
        int sno;
        if(::count%50==0)
            sno=50;
        else
            sno=::count%50;
        cout<<"E: "<<sno<<endl;
        update(mem.ram,mem.cpu,mem.io,sno);
    }

    int parser()
    {
        ofstream fout;

        fout.open("JSON_txt.json");
//        fout.seekp(0);
        fout<<MAINstr;
        fout.close();

        ifstream fin("JSON_txt.json");
    //    cout<<fin.is_open();
        string line;
        int len;

        int location=0;

        fin>>line;
        if(line!="{")
            return -1;

        while(fin>>line)
        {
    //        len=strlen(line.c_str());

            //Get RAM
            if(line=="\"ram_avg\"")
            {
                location=fin.tellg();
                fin.seekg(location+3);
                fin>>mem.ram;
            }

            //Get CPU
            else if(line=="\"idle_avg\"")
            {
                location=fin.tellg();
                fin.seekg(location+3);
                fin>>mem.cpu;
            }

            //Get IO
            else if(line=="\"io_avg\"")
            {
                location=fin.tellg();
                fin.seekg(location+3);
                fin>>mem.io;
            }
        }

        cout<<endl<<"RAM: "<<mem.ram<<endl;
        cout<<"CPU: "<<mem.cpu<<endl;
        cout<<"IO: "<<mem.io<<endl;

        fin.close();
        return 1;
    }


    ~splug()
    {
        close(clientSocket);
    }
};

int main()
{
    system("clear");
    char ch;

//        system("clear");
    while(1)
    {
        splug obj;
        if(obj.bind_socket()==-1)
            exit(0);
        if(obj.listen_in()==-1)
            exit(0);
        if(obj.accept_call()==-1)
            exit(0);
        obj.close_listening();
        obj.connect_message();

        while(obj.data()==1)
        {
            ::count++;
            obj.data_to_dbms();
        }
    }

    return 0;
}

/*int splug::data()
{
//    memset(buf,0,4096);
    double dram=0;

    bytesRecv=recv(clientSocket,(double*)&dram,sizeof(dram),0);
//    cout<<"BYTES: "<<bytesRecv<<endl;
    if(bytesRecv==-1)
    {
        cerr<<"\033[1;31m Can't Recieve \033[0m"<<endl;
        return -1;
    }
    else if(bytesRecv==0)
    {
        cerr<<"\033[1;31m Client disconnected. \033[0m"<<endl;
        return -1;
    }


//    cout<<"Ram usage: "<<string(buf,0,bytesRecv)<<" %"<<endl;
//    cout<<"Ram usage: "<<string(buf,0,bytesRecv)<<"\b %"<<endl;
    cout<<"Ram usage: "<<dram<<" %"<<endl;

    return 1;
}*/

int splug::data()
{
    memset(MAINstr,0,500);
//    strcpy(MAINstr,"0");
    bytesRecv=recv(clientSocket,MAINstr,sizeof(MAINstr),0);


    if(bytesRecv==-1)
    {
        cerr<<"\033[1;31m Can't Recieve \033[0m"<<endl;
        return -1;
    }
    else if(bytesRecv==0)
    {
        cerr<<"\033[1;31m Client disconnected. \033[0m"<<endl;
        return -1;
    }

    cout<<MAINstr<<endl;
    parser();

    return 1;
}
