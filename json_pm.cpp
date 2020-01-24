#include<iostream>
#include<string>
#include<iomanip>
#include<string.h>
#include<fstream>
#define GET_VARIABLE_NAME(Variable) (#Variable)
using namespace std;
#define cont 1
#define stop 0

void maker(char Main[],string,string,int flag=cont);
void maker(char Main[],string,double,int flag=cont);
//double gives 6 floating point
int parser();

int main()
{
    char Main[500];
    string a[10];
    double ram,cpu,io;

//    string ram,cpu,io;
//    int b=10;
//    a[1]=to_string(b);

    strcat(Main,"{ ");                  //Begening bracketter

//    getline(cin,a[0]);
//    maker(Main,GET_VARIABLE_NAME(a[0]),a[0],1);
//
//    getline(cin,a[1]);
//    maker(Main,GET_VARIABLE_NAME(a[1]),a[1],0);
    cout<<"Ram: ";
    cin>>ram;
    maker(Main,GET_VARIABLE_NAME(ram),ram);

    cout<<"CPU: ";
    cin>>cpu;
    maker(Main,GET_VARIABLE_NAME(cpu),cpu);

    cout<<"IO: ";
    cin>>io;
    maker(Main,GET_VARIABLE_NAME(io),io,stop);

//    strcat(Main,"\"a");
//    strcat(Main,a[1].c_str());
//    strcat(Main,"\" : \"");         //Variable [0];
//
//    getline(cin,a[0]);
//    strcat(Main,a[0].c_str());          //Value [0];
//    strcat(Main,"\"");

    strcat(Main," }");                  //End bracketter


//    cout<<GET_VARIABLE_NAME(a[1])<<endl;
    ofstream fout("JSON_txt.json");
    fout<<Main<<endl;
    fout.close();

     parser();

    return 0;
}

void maker(char Main[],string varName,string strval,int flag)
{
    strcat(Main,"\"");
    strcat(Main,varName.c_str());
    strcat(Main,"\"");

    strcat(Main," : ");

    strcat(Main,"\"");
    strcat(Main,strval.c_str());
    strcat(Main,"\"");
    if(flag==1)
        strcat(Main,", ");

}

void maker(char Main[],string varName,double strval,int flag)
{
    strcat(Main,"\"");
    strcat(Main,varName.c_str());
    strcat(Main,"\"");

    strcat(Main," : ");

//    strcat(Main,"\"");
    strcat(Main,to_string(strval).c_str());
//    strcat(Main,"\"");
    if(flag==1)
        strcat(Main,", ");

}

