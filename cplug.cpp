class cplug
{
    int sock;
    int port;
    int connectRes;
    string ipAddress;
    int bytesReceived;
    char buf[4608];
    string strRec;
    sockaddr_in hint;
    double ram_avg=0,perusage;
    double idle_avg=0,cpu_idle;
    double io_avg=0,io;
    char Main[500];

    public:

    cplug()                                                          //Sorted
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==-1)
            cerr<<"\033[1;31mCan't create socket\033[0m";
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

    void avg_each()
    {
        ram_avg+=perusage/itr;
        idle_avg+=(100-cpu_idle)/itr;            //change
        io_avg+=io/itr;
    }

    void get_ram()
    {
        ::count++;
        ifstream ram;
        string line;
        ram.open("/proc/meminfo");
        double ava,free,total,totFree,used;

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

//      cout<<"\033[1;32m % Usage: \033[0m "<<setprecision(4)<<perusage<<" %"<<endl;
        ram.close();
        if(::count%itr==0)
        {
//          cout<<"Average: "<<ram_avg<<" %"<<endl;
            strcpy(Main,"{ ");
            maker(Main,GET_VARIABLE_NAME(ram_avg),ram_avg);
            ram_avg=0;
        }
    }

    int data_to_server()
    {
        int sendRes=send(sock,(const char*)&Main,sizeof(Main),0);
        if(sendRes==-1)
        {
            cerr<<"\033[1;31m Can't Send! \033[0m"<<endl;
            return -1;
        }

        return 1;
    }

    void runMultiThread()
    {
        thread t1(&cplug::get_ram,this);
        thread t2(&cplug::get_cpu_idle_io,this);
        thread t3(&cplug::avg_each,this);
        t1.join();
        t2.join();
        t3.join();
    }

    void get_cpu_idle_io()
    {
        ifstream fin;
        system("iostat > log_iostat.txt");
        fin.open("log_iostat.txt");
        string line;
        double dump;
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

//      cout<<(100-cpu_idle)<<" %"<<endl;
//      cout<<io<<" %"<<endl;

        if(::count%itr==0)
        {
    //      cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<idle_avg<<" %"<<endl;
            maker(Main,GET_VARIABLE_NAME(idle_avg),idle_avg);
    //      cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<io_avg<<" %"<<endl;
            maker(Main,GET_VARIABLE_NAME(io_avg),io_avg,stop);
            idle_avg=0;
            io_avg=0;
        }

        fin.close();
    }

    ~cplug()
    {
        close(sock);
    }
};

