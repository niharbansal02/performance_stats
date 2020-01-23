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
//#include<jsoncpp/json/json.h>
#include<json-c/json.h>
#define corr "Correct!"
#define incorrid "Enter correct ID!"
#define incorrpass "Enter correct Password!"
#define def "Please enter correct choice!"

using namespace std;

#define ignore cin.ignore(numeric_limits<streamsize>::max(),'\n')
class splug
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
    char buf[4096];
    char passbuf[8];
    int bytesRecv;
    string getString;                           //Test var.

    public:
    int dec=1;
    splug()
    {
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
    }

    void close_listening()
    {
        close(listening);
        memset(host,0,NI_MAXHOST);                                      //Cleanup of host
        memset(svc,0,NI_MAXSERV);
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

    ~splug()
    {
        close(clientSocket);
    }
};

int main()
{
    system("clear");
    char ch;
    while(1)
    {
//        system("clear");
        splug obj;
        if(obj.bind_socket()==-1)
            exit(0);
        if(obj.listen_in()==-1)
            exit(0);
        if(obj.accept_call()==-1)
            exit(0);
        obj.close_listening();
        obj.connect_message();

    //    json_object *jobj=json_object_new_object();
    //    json_object *jobj=json_object_new_object();
        while(obj.data()==1);
//        this_thread::sleep_for(chrono::seconds(5));
    }

    return 0;
}

int splug::data()
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
}


/*
  int ticket_read()
    {
        int location,tno,age;
        char sex1[2];
        ifstream fin;
        ofstream fout;
        int pnr;
        bytesRecv=recv(clientSocket,(int *)&pnr,sizeof(pnr),0);     //FINE
        string strPNR;
        strPNR=to_string(pnr);
        string spnr;
        int flag=-1;

        fin.open("ticket.txt",ios::binary);
        while(!fin.eof())
        {
            fin>>spnr;
            if(spnr==strPNR)
            {
                flag=0;
                break;
            }
        }

        int aadf=1;
        if(flag==-1)
        {
            send(clientSocket,(int *)&flag,sizeof(flag),0);
            return -1;
        }
        else
        {
            send(clientSocket,(int *)&aadf,sizeof(aadf),0);
            location=fin.tellg();
            location++;             //Location is the start of TNO

            //*Get tno
            fin.seekg(location);
            fin>>tno;
//            cout<<"Train no.: "<<tno<<endl;
            send(clientSocket,(int*)&tno,sizeof(tno),0);

            //* Get from
            fin.seekg(location+6);
            char from[11];
            fin>>from;
//            cout<<"From: ";
            send(clientSocket,from,sizeof(from),0);

//            for(int i=0;i<11;i++)
//            {
//                if(from[i]=='0')
//                    cout<<" ";
//                else
//                    cout<<from[i];
//            }
//            cout<<endl;

            //*Get to
            char to[11];
            fin>>to;
//            cout<<"To: ";
            send(clientSocket,to,sizeof(to),0);
//            for(int i=0;i<11;i++)
//            {
//                if(to[i]=='0')
//                    cout<<" ";
//                else
//                    cout<<to[i];
//            }
//            cout<<endl;

            //*Get no. of psgr
            fin.close();
//            fin.open("ticket.txt",ios::binary);
//            int psgrno;
//            fin.seekg(location+28);
//            fin>>psgrno;
//            cout<<psgrno;
//            fin.close();
//            send(clientSocket,(int*)&psgrno,sizeof(psgrno),0);
//            //* Get coach
//            for(int i=0;i<psgrno;i++)
//            {
//                fin.open("ticket.txt",ios::binary|ios::in);
//                fin.seekg(location+30+(42*i));
//                char coach[4];
//                fin.getline(coach,4);
////                cout<<coach<<"\t\t\t\t\t";
//                send(clientSocket,coach,sizeof(coach),0);
//                fin.close();
//            }
////            cout<<endl;
//
//            //* Get Name
//            for(int i=0;i<psgrno;i++)
//            {
//                fin.open("ticket.txt",ios::binary);
//                fin.seekg(location+36+(42*i));
//                char name1[31];
//                fin.getline(name1,31);
//                send(clientSocket,name1,sizeof(name1),0);
////                for(int j=0;j<31;j++)
////                {
////                    if(name1[j]=='0')
////                        name1[j]=' ';
////                }
////                cout<<"NAME: "<<name1<<"\t";
//                fin.close();
//            }
//
////            cout<<endl;
//
//            //* Get Age
//            for(int i=0;i<psgrno;i++)
//            {
//                fin.open("ticket.txt",ios::binary);
//                fin.seekg(location+67+(42*i));
//                int age1;
//                fin>>age1;
//                send(clientSocket,(int *)&age1,sizeof(age),0);
////                cout<<"Age: "<<age1<<" yrs.\t\t\t\t";
//                fin.close();
//            }
//
////            cout<<endl;
//
//            //* Get sex
//            for(int i=0;i<psgrno;i++)
//            {
//                fin.open("ticket.txt",ios::binary);
//                fin.seekg(location+70+(42*i));
//                fin>>sex1;
//                send(clientSocket,sex1,sizeof(sex1),0);
////                cout<<"Sex: "<<sex1<<"\t\t\t\t\t";
//                fin.close();
//            }
//
////            cout<<endl;
//            //? TODO: Get date of jpurney
//            //* Get Distance
//            int dis;
//            fin.open("ticket.txt",ios::in|ios::binary);
//            fin.seekg(location+114);
//            fin>>dis;
//            send(clientSocket,(int*)&dis,sizeof(dis),0);
////            cout<<"Distance: "<<dis<<" km"<<endl;
//
//            //* Get Fare
//            int cost;
//            fin.seekg(location+118);
//            fin>>cost;
//            send(clientSocket,(int*)&cost,sizeof(cost),0);
////            cout<<"Fare: ₹"<<cost<<endl;

        }



    }
*/
