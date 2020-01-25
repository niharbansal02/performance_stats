using namespace std;
#define GET_VARIABLE_NAME(Variable) (#Variable)
#define cont 1
#define stop 0

void maker(char Main[],string,string,int flag=cont);
void maker(char Main[],string,double,int flag=cont);

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

    strcat(Main,to_string(strval).c_str());

    if(flag==1)
        strcat(Main,", ");

}

