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

    void data();

    ~splug()
    {
        close(clientSocket);
    }
};

/*

string encrypt(string &pass)
    {
        for(int i=0;pass[i]!='\0';i++)
        {
            pass[i]=pass[i]+(i+1);
        }
        return pass;
    }

    string decrypt(string epass)
    {
    string dpass;
        for(int i=0;epass[i]!='\0';i++)
        {
            epass[i]=(epass[i])-(i+1);
        }
        return epass;
    }


*/




/*

    int id_to_client()
    {
        //Clear the buffer
        idbuf=0;

        // Wait for message
        bytesRecv=recv(clientSocket,(int *)&idbuf,sizeof(idbuf),0);            //recv() function recieves data from client
        //recv(socket,variable to store,size of variable,flags)

        if(bytesRecv==-1)
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            return -1;
            exit(0);
        }
        else if(bytesRecv==0)
        {
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            return -1;
            exit(0);
        }

        ifstream fin;
        fin.open("id.txt",ios::binary);
        if(!fin.is_open())
        {
            cerr<<"\033[1;31m Error opening id file! \033[0m;";
            return -1;
            exit(0);
        }

        int compid;
        fin>>compid;
        fin.close();

        //Displady message
        if(idbuf==compid)
        {
            cout<<"\033[1;32mClient: "<<idbuf<<"\033[0m"<<endl;
            send(clientSocket,corr,sizeof(corr),0);
        }
        else
        {
            cout<<"\033[1;31mClient: "<<idbuf<<"\033[0m"<<endl;
            send(clientSocket,incorrid,sizeof(incorrid),0);
        }
        return 1;
    }

    int pass_to_client()
    {

        //Clear the buffer
        //memset(buf,0,4096);
        strcpy(buf,"0");
        string str;
        // Wait for message
        bytesRecv=recv(clientSocket,buf,4096,0);            //recv() function recieves data from client
        //recv(socket,variable to store,size of variable,flags)

        if(bytesRecv==-1)
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            return -1;
        }
        else if(bytesRecv==0)
        {
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            return -1;
        }

        str=string(buf,bytesRecv);

        //* Get pass from file
        ifstream fin;
        fin.open("pass.txt",ios::binary);
        if(!fin.is_open())
        {
            cerr<<"\033[1;31m Error opening pass file! \033[0m;";
            return -1;
            exit(0);
        }
        // char compPass[8];
        string copass;
        fin>>copass;
        // fin>>compPass;                  //This is working fine!
        fin.close();
        //Displady message
        if(strcmp(copass.c_str(),str.c_str())==0)
        {
            cout<<"\033[1;32mClient: "<<str<<"\033[0m"<<endl;
            send(clientSocket,corr,sizeof(corr),0);
        }
        else
        {
            cout<<"\033[1;31mClient: "<<str<<"\033[0m"<<endl;
            send(clientSocket,incorrpass,sizeof(incorrpass),0);
        }
        return 1;
    }

    int edit_pass()
    {
        //MEMSET
        strcpy(passbuf,"0");

        //Wait for message
        bytesRecv=recv(clientSocket,passbuf,sizeof(passbuf),0);

        cout<<"\033[1;33m"<<passbuf<<"\033[0m"<<endl;           //remove it later

        if(bytesRecv==-1)
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            return -1;
            exit(0);
        }
        else if(bytesRecv==0)
        {
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            return -1;
            exit(0);

        }

        fstream fout;
        fout.open("pass.txt",ios::binary|ios::out);
        if(!fout.is_open())
        {
            cerr<<"\033[1;31m Error opening pass file! \033[0m;";
            return -1;
            exit(0);
        }
        fout<<passbuf;
        fout.close();

        fstream fin;
        fin.open("pass.txt",ios::in|ios::binary);
        if(!fin.is_open())
        {
            cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
            exit(0);
        }

        char conf[8];
        fin>>conf;
        if(strcmp(conf,passbuf)==0)
        {
            if(send(clientSocket,corr,sizeof(corr),0)==-1)
            {
                cerr<<"\n \033[1;31mSending error!! \033[0m";
                return -1;
            }
        }
        else
        {
            send(clientSocket,incorrid,sizeof(incorrid),0);
            cerr<<"\n \033[1;31mError changing password!\n Contact dev. \033[0m"<<endl;
            return -1;
            exit(0);
        }

        fout.close();
        return 0;
    }

    int decider()
    {
        int buffer;
        buffer=0;
        bytesRecv=recv(clientSocket,(int*)&buffer,sizeof(buffer),0);
        if(bytesRecv==-1)
        {
            cerr<<"\033[1;31mThere was a connection issue\033[0m"<<endl;
            dec=-1;
            return -1;
        }
        else if(bytesRecv==0)
        {
            cout<<"\033[1;31mThe client disconnected\033[0m"<<endl;
            dec=-1;
            return -1;
        }

        switch(buffer)
        {
            case 1: dec=id_to_client();
                    break;
            case 2: dec=pass_to_client();
                    break;
            case 3: dec=edit_pass();
                    break;
            case 4: ticket_read();                    //Edit this
                    break;
            case 100:   dec=-1;
                        return -1;
            default: cerr<<"\033[1;33mWaithing for message...\033[0m"<<endl;
        }
    }*/

