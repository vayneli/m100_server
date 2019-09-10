#include "socket.hpp"
using namespace std;
Socket::Socket(){
   
     
    //开启socket（）
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_fd < 0){
        cout << "-------------Creat connection error--------------" << endl;
    }
    /*设置socket参数*/
    //使用ipv4地址
    server_addr.sin_family = AF_INET;   
    //设置端口号
    server_addr.sin_port = htons(6666);
    //ip地址由于客户端和服务器不同，在各自的初始化部分设置
    
}

Socket::~Socket(){
    close(sock_fd);
}

int Socket::server_init(){
    //设置ipv4地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定socket
    int ret = bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(ret < 0){
        cout << "Can not bind server socket" << endl;
        close(sock_fd);
        return -1;
    }
    //进入监听状态，等待用户发起请求
    ret = listen(sock_fd, 5);
  
    if(ret < 0){
        cout << "Can not listen socket" << endl;
        close(sock_fd);
        return -1;
    }

    
    return 0;
}

int Socket::client_init(string address){

    //设置ipv4地址,该地址为服务器的ip地址
    server_addr.sin_addr.s_addr = inet_addr(address.c_str());
    ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    //读取服务器传回的数据
    if(ret < 0)
    {
        cout << "Can not connect to the server" << endl;
        return -1;
    }   
        return 0; 
    }


int Socket::client_send_data(const char* data){
    
    int num = write(sock_fd, data, strlen(data));
    return num;
}

int Socket::server_send_data(const char* data){
      cout << "send successful: " << data << endl;

    int num = write(com_fd, data, strlen(data));
    return num;
}

int Socket::client_receive_data(){

    memset(receive_buff,0,1024);
    int num = read(com_fd,receive_buff,sizeof(receive_buff)); 
    // cout << "gf" << num << endl;
    return num;
}

int Socket::server_receive_data(){

    socklen_t len = sizeof(client_addr);
    com_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &len);
    if(com_fd < 0){
        cout << "Can not accept request" << endl;
        close(sock_fd);
        return -1;
    }
    memset(receive_buff,0,1024);
    int num = read(com_fd,receive_buff,sizeof(receive_buff)); 
    cout << "gf" << num << endl;
    return num;
}

