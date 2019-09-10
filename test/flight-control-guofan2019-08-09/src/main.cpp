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
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;
using namespace std;

/*! main
 *
 */
int main(int argc, char** argv)
{

  // Initialize variables
  int functionTimeout = 1;
  mutex mut;
  int mark = 0;
  // Setup OSDK.
  LinuxSetup linuxEnvironment(argc, argv); 
  //循环使飞机切换档位后能够继续执行程序
  while(true){
  
  Vehicle*   vehicle = linuxEnvironment.getVehicle();

  if (vehicle == NULL)
  {
    std::cout << "Vehicle not initialized, exiting.\n";
    return -1;
  }
  
  // Obtain Control Authority
  vehicle->obtainCtrlAuthority(functionTimeout);
 
  
  // Socket server; 
  // server.server_init();

  // Commands order;
  // RCVelocity vel;
  // order.vehicle = vehicle;
  // order.socket = &server;
  // order.Vel = vel;
  int lastMode = 1;
  ACK::ErrorCode tmp;

  while(true){ 

    // int num = server.receive_data();
    // order.getCommand(server.receive_buff);
    // cout << server.receive_buff << endl;
    // close(server.com_fd);
    // system("clear");
   // cout << "-------------test successfully !----------------" << endl;

    // cout << order.control_command.num << endl;
    // cout << "speed_x: " << order.control_command.speed_x << endl;
    // cout << "speed_y: " << order.control_command.speed_y << endl;
    // cout << "speed_z: " << order.control_command.speed_z << endl;
    // cout << "yaw_rate: " << order.control_command.yaw_rate << endl;
    // cout << order.control_command.key_1 << endl;  
    // mut.lock();
   
    if(vehicle->broadcast->getRC().mode == 8000)
    {
      if(lastMode == 0){
          // delete vehicle;
          // linuxEnvironment.killMe();
          
          cout << "-------------------changing mode is detected!------------- " << endl;
          sleep(1);
          break;
      }
        
      bodyVelocityControl(vehicle, 2, 0, 0, 0);
      usleep(10000);
    }
    lastMode = vehicle->broadcast->getRC().mode;
    cout << "lastMode: " << lastMode << endl;
  
  }
  cout << "jump successfully" << endl;
  //跳出循环后判断是否有档位变动

                                                            

  }
  

  
  

  return 0;
}
