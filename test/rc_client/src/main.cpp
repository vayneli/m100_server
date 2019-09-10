#include <iostream>
#include <set>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "socket.hpp"
//inet需要包含下面头文件

#include <arpa/inet.h>
#include <netinet/in.h>

#define UNIX_DOMAIN "/home/gf/test/socket/UNIX.domain"
using namespace std;

int main(int argc, char const *argv[])
{
    string askstr;
    askstr+="#r $";
      //cout << askstr.c_str()<< endl;
    char receive_buff[1024];
    while (1)
    {
        Socket client1;
        client1.client_init("127.0.0.1");
        client1.send_data(askstr.c_str());
        read(client1.sock_fd,receive_buff,sizeof(receive_buff));
        cout<< receive_buff<<endl;
        close(client1.sock_fd);

        Socket client2;
        client2.client_init("192.168.50.88");
        client2.send_data(receive_buff);
        close(client2.sock_fd);
        memset(receive_buff,0,1024);

        usleep(20000);//20ms
    }


}
