/*! @file flight-control/main.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  main for Flight Control API usage in a Linux environment.
 *  Provides a number of helpful additions to core API calls,
 *  especially for position control, attitude control, takeoff,
 *  landing.
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "flight_control_sample.hpp"
#include "socket.hpp"
#include "Commands.hpp"
#include <mutex>
#include <stdlib.h>
#include "serial_port.hpp"
#include <sys/time.h>
#include <time.h>
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace std;
using namespace mrobot;

mutex mut;
mutex mut_pid;

void *threadFunction(void *args)
 //数传
{    

  Commands *commands = (Commands*)args;
  // Vehicle* vehicle = commands->vehicle;

  //设置串口信息，端口，波特率，位数，停止位，校验位；
  string str;
  string name = "/dev/ttyUSB0";
  baudrate_option baud = baudrate_option::b57600;
  data_bits_option data = data_bits_option::eight;
  parity_option parity = parity_option::none;
  stop_bits_option stop_bits = stop_bits_option::one;
  //set buffers
  vector<char> bufferSend={' '};
  vector<char> bufferReceive = {'\0'};
  
  serial_port serial_device(name, baud, data, parity, stop_bits);
  
  int num=1;
  int mark = 1;
  string tmp;

  cout<<"Waiting for request"<<endl;
  while(serial_device.is_ready())
  {
      if(serial_device.is_data_ready()>0)
      {
        //一位一位读，遇到#开始记录，遇到$停止记录并将该字符串存入commands
        serial_device.receive_data(bufferReceive);
        if(bufferReceive[0] == '#')
          mark = 1;
        if(mark == 1)
          tmp+=bufferReceive[0];
        if(bufferReceive[0]=='$')
        {
          mark = 0;         
          commands->pid_param = tmp;
          tmp.clear();          
        }
    
        usleep(100);
      }
      // 显示收到的消息
      // cout<<"Received data: ";
      // cout << bufferReceive << endl;
      //  cout << bufferReceive << endl; 
      // cout<<endl;
     

    if (bufferReceive[0]=='p')
    {
      bufferSend.clear();
      cout << "receive R" << endl;
      

      // cout<<str<<endl;
      mut_pid.lock();
      for(int i=0;i<commands->ground_data.size();i++)
      {
        bufferSend.push_back(commands->ground_data[i]);
      }
      bufferSend.push_back('\n');
      
      cout << "ground : " << commands->ground_data << endl;
      mut_pid.unlock();
      serial_device.send_data(bufferSend);
      
     
      usleep(100000);

    //重复发送程序，此处暂不考虑
    }
    // if (bufferRecieve.at(end)=='P')
    // {
    //   if(serial_device.is_ready())
    //   {
    //     serial_device.send_data(bufferSend);
    //     cout<<"send again successful";
    //   }
    //   usleep(10000);
    //   memset(bufferReceive)

    // }
  
}
}


void *threadSocket(void *args)
{

  Commands *commands = (Commands*)args;
  Socket server;  
  server.server_init();
  commands->socket = &server;
  while(1)
  {
    int num = server.server_receive_data();
    
    commands->getCommand(server.receive_buff);
    close(server.com_fd);
    // 打印收到的命令
    cout << "-------------Receive successfully !----------------" << endl;
    cout << server.receive_buff << endl;
    cout << commands->control_command.num << endl;
    cout << "speed_x: " << commands->control_command.speed_x << endl;
    cout << "speed_y: " << commands->control_command.speed_y << endl;
    cout << "speed_z: " << commands->control_command.speed_z << endl;
    cout << "yaw_rate: " << commands->control_command.yaw_rate << endl;
   
  }
 

}


/*! main
 *
 */
int main(int argc, char** argv)
{

  // Initialize variables
  int functionTimeout = 1;//获取控制权执行时间（单位是秒s）

  // 初始化服务器对象 
  // Socket server;  
  // server.server_init();
  // 初始化命令对象
  Commands order;

  //创建线程，将数据发送到地面站，并接受地面站的pid参数
  pthread_t thread1;//
  pthread_create(&thread1,NULL,threadFunction,&order);


  pthread_t thread2;
  pthread_create(&thread2, NULL, threadSocket, &order);
  RCVelocity vel;  
  // Setup OSDK.
  LinuxSetup linuxEnvironment(argc, argv);
  time_t totalseconds = time(NULL);//获取总妙数
  struct tm *st = localtime(&totalseconds);
  string log_name = to_string(st->tm_year + 1900) + "-" + to_string(st->tm_mon + 1) + "-" + 
  to_string(st->tm_mday) + "-" + to_string(st->tm_hour) + "-" + to_string(st->tm_min) 
  + "-" + to_string(st->tm_sec) + ".txt";
  ofstream log;
  log.open("./log/" + log_name);

  struct timeval dwStart;
            struct timeval dwEnd;
            unsigned long dwTime=0;
            unsigned long dwTime1=0;
  while(true)
  {
    int lastMode = 1;//记录上一时刻mode
    //初始化vehicle对象
    Vehicle* vehicle = linuxEnvironment.getVehicle();
    
    if (vehicle == NULL)
    {
      std::cout << "Vehicle not initialized, exiting.\n";
      return -1;
    }

    

    // Obtain Control Authority
    vehicle->obtainCtrlAuthority(functionTimeout);

    //获取初始位置，将坐标系从gps坐标系转换为起飞位置为原点的坐标系
    order.originBroadcastGP = vehicle->broadcast->getGlobalPosition();  
    // order.socket = &server;
    order.Vel = vel;
    order.clearCommand();
    // 执行控制命令

    while(true){
      gettimeofday(&dwEnd,NULL);
      dwTime = 1000000*(dwEnd.tv_sec-dwStart.tv_sec)+(dwEnd.tv_usec-dwStart.tv_usec);
      mut_pid.lock();
      getPositionAndQuaternion(vehicle, order.ground_data , order.originBroadcastGP);
      cout << "wozhencai: " << endl;
      cout << order.ground_data << endl;
      log << order.ground_data;
      // order.ground_data.clear();
      mut_pid.unlock();


      // int num = server.server_receive_data();
      // mut.lock();
      // order.getCommand(server.receive_buff);
      // mut.unlock();
      // close(server.com_fd);
      // // 打印收到的命令
      // cout << "-------------Receive successfully !----------------" << endl;
      // cout << server.receive_buff << endl;
      // cout << order.control_command.num << endl;
      // cout << "speed_x: " << order.control_command.speed_x << endl;
      // cout << "speed_y: " << order.control_command.speed_y << endl;
      // cout << "speed_z: " << order.control_command.speed_z << endl;
      // cout << "yaw_rate: " << order.control_command.yaw_rate << endl;
      // cout << "mode: " << vehicle->broadcast->getRC().mode << endl;  
      
      //遥控器mode检测         
      if(vehicle->broadcast->getRC().mode == 8000)
      {
        if(lastMode == 0){        
          cout << "————————————Changing mode is detected !——————————————" << endl;
          break;          
        }
        // 记录上一时刻mode  
          mut.lock();
          bodyVelocityControl(vehicle, order.control_command.speed_x, 
          order.control_command.speed_y,order.control_command.speed_z, 
          order.control_command.yaw_rate);
          mut.unlock();
          // Control::attitudeAndVertPosCtrl(vehicle, order.control_command.speed_x, 
          // order.control_command.speed_y,order.control_command.speed_z, 
          // order.control_command.yaw_rate);
          //单位是弧度

        gettimeofday(&dwEnd,NULL);
        dwTime1 = 1000000*(dwEnd.tv_sec-dwStart.tv_sec)+(dwEnd.tv_usec-dwStart.tv_usec);
        cout << "timedelay2: " << (dwTime1 - dwTime/1000.0) << endl; 
        
      }

      usleep(100000);
      
      lastMode = vehicle->broadcast->getRC().mode;
      //清除终端中多余的信息
      // system("clear");
    }

    log.close();
    
    
    
   
    // 客户端事例程序  
    // char data[] = "lol's full name is league of lengend!";
    // Socket client;
    // client.client_init("192.168.50.162");
    // client.client_send_data(data);
    // cout << "send sussceful" << endl;
    // while(1){   
    //   int num = server.server_receive_data();
    //   if(num == 0)
    //     break;
    // order.getCommand(server.receive_buff);
    // cout << order.control_command.num << endl;
    // cout << order.control_command.speed_x << endl;
    // cout << order.control_command.speed_y << endl;
    // cout << order.control_command.speed_z << endl;
    
    // }

    
    
  }
  return 0;
}
