#include"sock_macro.cpp"
#include"json_maker.cpp"
#include"cplug.cpp"

int main()
{
    cplug obj;

    obj.init_hint_struct();

    if(obj.connect_to_server()==-1)
        exit(0);

    for(int k=0;k<ENTRIES;k++)
    {
        for(int i=0;i<itr;i++)
        {
            this_thread::sleep_for(chrono::milliseconds(DELAY));
            obj.runMultiThread();
            obj.complete_json();
        }
        obj.print_json();
        obj.data_to_server();
    }

    return 0;
}
