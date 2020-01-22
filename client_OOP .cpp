#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

using namespace std;
class cplug
{
    int sock;
    int port;
    int connectRes;
    string ipAddress;
    char buf[4096];
    string userInput;
    sockaddr_in hint;
    
    public:
    cplug()                                                          //Sorted
    {
        sock=socket(AF_INET,SOCK_STREAM,0);
        if(sock==-1)
            cerr<<" Can't create socket";
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
            cerr<<" Connection failed";
            return 1;
        }
    }
    
    void data_to_server();
    
    ~cplug()
    {
        close(sock);
    }
};

int main()
{
    cplug obj;
    obj.init_hint_struct();
    if(obj.connect_to_server()==1)
        exit(0);
    obj.data_to_server();

    return 0;
}

void cplug::data_to_server()
{
    while(true)
    {

        // Enter lines of text
        cout<<"> ";
        getline(cin,userInput);

        // sent to server
        int sendRes=send(sock,userInput.c_str(),sizeof(userInput)+1,0);             //+1 because we send the /0 also
        if(sendRes==-1)
        {
            cerr<<" Coudnot connect to server ";
            continue;                                                               //It will repeat the loop from start
        }

        // wait for response
        memset(buf,0,4096);
        int bytesRecieved=recv(sock,buf,4096,0);

        // display response
        cout<<"Server> "<<string(buf,bytesRecieved)<<" \r\n";    
    }
}