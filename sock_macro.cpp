#include<ctype.h>
#include<chrono>
#include<iomanip>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<limits>
#include<netdb.h>
#include<string>
#include<thread>
#include<math.h>
#include<time.h>
#include<fstream>
#include<string.h>
#include<arpa/inet.h>
#include<algorithm>
#include<stdlib.h>

#define ignore cin.ignore(numeric_limits<streamsize>::max(),'\n')
#define ENTRIES 60
#define DELAY 50
#define SENDTIME 1000

using namespace std;

int itr=SENDTIME/DELAY;

static long long count=0;
