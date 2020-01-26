#include"sock_macro.cpp"
#include"dbms.cpp"
#include"splug.cpp"

using namespace std;

int main()
{
    system("clear");
    char ch;

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
            ::counts++;
            obj.data_to_dbms();
        }
    }

    return 0;
}
