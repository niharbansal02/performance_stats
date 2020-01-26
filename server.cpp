#include"sock_macro.cpp"
#include"dbms.cpp"
#include"splug.cpp"

using namespace std;

int main()
{
    system("clear");
    char ch;
    int flag=1;

    while(1)
    {
        splug obj;
        if(obj.bind_socket()==-1)
            return -1;
//        obj.bind_socket();
        if(obj.listen_in()==-1)
            return -1;
        if(obj.accept_call()==-1)
            return -1;
        obj.close_listening();
        obj.connect_message();

        while(flag==1)
        {
//            auto start=chrono::system_clock::now();
            flag=obj.data();
//            auto endf=chrono::system_clock::now();
//            chrono::duration<double> epl=endf-start;
//            cout<<"\033[1;33m"<<epl.count()<<"\033[0m"<<endl;
            ::counts++;
            obj.data_to_dbms();
        }
    }

    return 0;
}
