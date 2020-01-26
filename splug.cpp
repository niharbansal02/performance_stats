int counts=0;

class splug : public dbms
{
    int listening;
    sockaddr_in hint,client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket;
    int result;
    int idbuf;
    struct info
    {
        double ram,io,cpu;
    }mem;

    char buf[4096];
    char MAINstr[500];
    int bytesRecv;

    public:
    int dec=1;
    splug()
    {
//        ::counts++;
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

    int data()
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

    int data_to_dbms()
    {
        int sno;
        if(::counts%50==0)
            sno=50;
        else
            sno=::counts%50;
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
        {
            cerr<<"\033[1;31m Invalid \033[0m JSON obj "<<endl;
            return -1;
        }
        fin.seekg(0);
        getline(fin,line);
        len=strlen(line.c_str());
        fin.seekg(len-1);
        fin>>line;
        if(line!="}")
        {
            cerr<<"\033[1;31m Invalid \033[0m JSON obj "<<endl;
            return -1;
        }
        fin.seekg(1);
        while(fin>>line)
        {
//            len=strlen(line.c_str());

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

