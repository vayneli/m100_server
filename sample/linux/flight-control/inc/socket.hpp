#pragma once

#include <iostream>
#include <set>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//inet需要包含下面头文件

#include <arpa/inet.h>
#include <netinet/in.h>

/*初始化客户端和服务器的socket 1.打开socket 2.设置地址和端口号 3.绑定 
4.服务器进入监听模式，客户端连接目标地址*/

/*SOCKET中分服务器和客户端，注意客户端read()和write()通过sock_fd，
服务器的read()和write()通过com_fd*/
class Socket{

    public:
        Socket();        
        ~Socket();
        
        int server_init();
        /*server初始化*/
        int client_init(std::string address);
        /*client初始化*/

        int server_receive_data();
        /*服务器接收数据*/       
        int client_receive_data();
        /*客户端接受数据*/


        int client_send_data(const char* data);
        /*客户端发送数据*/
        int server_send_data(const char* data);
        /*服务器发送数据*/
        

    
    public:
        struct sockaddr_in client_addr;
        struct sockaddr_in server_addr;
        char receive_buff[1024];
        int sock_fd;
        int com_fd;//服务器accept到client后返回的id                         
        int ret;//临时变量

};



