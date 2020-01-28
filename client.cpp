#include"sock_macro.cpp"
#include"json_maker.cpp"
#include"cplug.cpp"

int main()
{
    cplug obj;
    unsigned long extraTime=0;
    double extraTime2=0;

    obj.init_hint_struct();

    if(obj.connect_to_server()==-1)
        exit(0);

    for(int k=0;k<12;k++)
    {
        auto start=chrono::system_clock::now();
        for(int i=0;i<itr;i++)
        {
            ::count++;
            obj.runAsync();
        }

        auto endw=chrono::system_clock::now();
        chrono::duration<double> elp=endw-start;
        elp*=1000000;
        extraTime2=elp.count()-10000000;
        correction2=extraTime2/200;
//        cout<<"\033[1;31m"<<extraTime2<<endl;
//        cout<<"\033[1;32m "<<(elp.count())/1000000<<"\033[0m"<<endl;
    }



    return 0;
}
