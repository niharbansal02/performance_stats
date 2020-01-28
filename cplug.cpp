unsigned long correction2=0;

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

    long correctionR=0,correctionIOC=0,correctionS=0;

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
        idle_avg+=(100-cpu_idle)/itr;
        io_avg+=io/itr;
    }

    void get_ram()
    {
        auto start=chrono::system_clock::now();
        this_thread::sleep_for(chrono::microseconds(50000-correctionR-correction2));
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
        ram.close();
        totFree=ava;
        used=total-totFree;

        perusage=(used/total)*100;

//        cout<<"\033[1;32m % Usage: \033[0m "<<setprecision(4)<<perusage<<" %"<<endl;
        if(::count%(itr-1)==0)
        {
//            cout<<"Average: "<<ram_avg<<" %"<<endl;
            strcpy(Main,"{ ");
            maker(Main,GET_VARIABLE_NAME(ram_avg),ram_avg);
            ram_avg=0;
        }
        auto endr=chrono::system_clock::now();
        chrono::duration<double> elapsed_time=endr-start;
        elapsed_time*=1000000;
        correctionR=abs(elapsed_time.count()-50000);
//        cout<<"\033[1;32m"<<correctionR/1000<<"\033[0m"<<endl;
    }

    int data_to_server()
    {
        complete_json();
        print_json();
        auto start=chrono::system_clock::now();
//        this_thread::sleep_for(chrono::microseconds(10000000-correctionS));

        int sendRes=send(sock,(const char*)&Main,sizeof(Main),0);
        if(sendRes==-1)
        {
            cerr<<"\033[1;31m Can't Send! \033[0m"<<endl;
            return -1;
        }
        auto ends=chrono::system_clock::now();
        chrono::duration<double> elapsed_time=ends-start;
        elapsed_time*=1000000;
        correctionS=(elapsed_time.count()-10000000);

        return 1;
    }

    void runAsync()
    {
        future<int> send4;

        future<void> ram1=async(launch::async,&cplug::get_ram,this);
        future<void> ioc2=async(launch::async,&cplug::get_cpu_idle_io,this);
        future<void> avg3=async(launch::async,&cplug::avg_each,this);

        if(::count%200==0)
            send4=async(launch::async,&cplug::data_to_server,this);

        ram1.get();
        ioc2.get();
        avg3.get();

        if(::count%200==0)
            send4.get();
    }

    void get_cpu_idle_io()
    {
        auto beg=chrono::system_clock::now();
        this_thread::sleep_for(chrono::microseconds(50000-correctionIOC-correction2));
        system("iostat > log_iostat.txt");
        ifstream fin;
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

//      cout<<"\033[1;33m"<<(100-cpu_idle)<<" %"<<"\033[0m"<<endl;
//      cout<<"\033[1;33m"<<io<<" %"<<"\033[0m"<<endl;

        if(::count%(itr-1)==0)
        {
    //      cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<idle_avg<<" %"<<endl;
            maker(Main,GET_VARIABLE_NAME(idle_avg),idle_avg);
    //      cout<<"\033[1;32m AVG: \033[0m"<<setprecision(4)<<io_avg<<" %"<<endl;
            maker(Main,GET_VARIABLE_NAME(io_avg),io_avg,stop);
            idle_avg=0;
            io_avg=0;
        }
        fin.close();
        auto endr=chrono::system_clock::now();
        chrono::duration<double> elapsed_time=endr-beg;
        elapsed_time*=1000000;
        correctionIOC=abs((elapsed_time.count()-50000));
    }

    ~cplug()
    {
        close(sock);
    }
};

