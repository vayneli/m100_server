#include "Commands.hpp"
using namespace std;

Commands::Commands()
{
    
}

Commands::~Commands()
{
}

int Commands::getCommand(const char* data){
    // clearCommand();
    string ss = data;
    string tmp;
    int begin, end;
    begin = -1;
    int count = 0;
    
    
    //mark 作为标志位，1代表可以开始读书据，-1代表结束读数据
    for(char&c:ss){
        //cout<<c;
        //读完一条指令，计数器count加1
        if(c == '#'){  
            begin = 0;//指令开始         
        }
        if(c == '$' && begin != -1){
            if(begin == (num_v + 1)){
                count ++;
                // cout << "count: " << count << endl;
                myCommand.push_back(control_command);
            }
            else{               
                // clearCommand();
            }
            begin = -1;
        }
        
        if(begin != -1 && c!= '#'){//读到$，不再执行下面程序;
            
            if(c != ' '){
                if(begin == 0){//判断是否为命令类型位，并记录命令类型;
                    type = c;

                    begin++;
                    // cout << "begin: " << begin << endl;
                    continue;
                }                  
                else{

                    tmp = tmp + c;

                }
            }                
            else 
            {//命令类型的字符均为小写
                switch (type)
                {
                    case 'c':
                                              
                        begin = fuckCommandC(tmp, begin);
                        // cout << "tmp: " << tmp << endl;
                        // cout << begin << "----------------"<< endl;
                        tmp.clear();
                        break;
                    case 'r':
                        begin = fuckCommandS();
                        if(begin == -1)
                            // cout << "yes,sir. Ready to send" << endl;
                         break;
                    case 'x':
                        begin = fuckCommandX();
                        break;

                    case 'p':
                        begin = fuckCommandP(tmp, begin);
                
                    default:
                        break;
                }
                
            }
            
                
        }        
    }
}

int Commands::clearCommand(){

    control_command = {0,0,0,0,0,0,0,0};
    myCommand.clear();

}

int Commands::fuckCommandC(string tmp, int begin){
    
    switch (begin)
    {
        case 1:
            control_command.num = stoi(tmp);
            num_v = control_command.num + 1;
            // cout << "num_c: " << num_v << endl;
            break;
        case 2:
            control_command.speed_x = stof(tmp);
            break;
        case 3:
            control_command.speed_y = stof(tmp);
            break;
        case 4:
            control_command.speed_z = stof(tmp);
            break;
        case 5:
            control_command.yaw_rate = stof(tmp);
            break;
        case 6:
            control_command.key_1 = stoi(tmp);
            break;
        case 7:
            control_command.key_2 = stoi(tmp);
            break;
        case 8:
            control_command.key_3 = stoi(tmp);
            break;
    
        default:
            break;
    }
    begin++;
    if(begin > (num_v + 1)){
        clearCommand();
        return 0;
    }
    
    return begin;
} 

//发送RC数据
int Commands::fuckCommandS(){

    Vel.getvalue(vehicle);   
    // cout << Vel.tosendData() << endl;  
    // cout << "a" << endl;  
    socket->server_send_data(Vel.tosendData().c_str());
    // cout << "b" << endl;
    return 0;
}


//
//注释部分为向客户端发送无人机姿态用于坐标变换
//未注释部分是将数传得到的pid参数字符串发送给客户端，用于调pid
//
int Commands::fuckCommandX(){

    //  Vel.getValueQua(vehicle);   
    // cout << Vel.tosendData() << endl;  
    cout << "send successful: " << pid_param.c_str() << endl;
    socket->server_send_data(pid_param.c_str());
    
    
    return 0;
}




//
//pid调参的数据包格式为 #p4 kp ki kd sampleTime $
//
int Commands::fuckCommandP(string tmp, int begin){
    
    switch (begin)
    {
        case 1:
           pid.num = stoi(tmp);
            num_v = pid.num + 1;
            // cout << "num_c: " << num_v << endl;
            break;
        case 2:
            pid.kp = stof(tmp);
            break;
        case 3:
            pid.ki = stof(tmp);
            break;
        case 4:
            pid.kd = stof(tmp);
            break;
        case 5:
            pid.SampleTime = stof(tmp);
            break;
        default:
            break;
    }
    begin++;
    if(begin > (num_v + 1)){
        clearCommand();
        return 0;
    }
    
    return begin;
} 




