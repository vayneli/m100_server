#include "fei.hpp"
#define PI 3.14
using namespace std;
void RCVelocity::getvalue(DJI::OSDK::Vehicle *vehicle)
{
    DJI::OSDK::Telemetry::RC currentRC;
    currentRC=vehicle->broadcast->getRC();
    Vx=currentRC.pitch/2000.0;
    Vy=currentRC.roll/2000.0;
    Vz=currentRC.throttle*3.0/10000.0;
    Yaw=90*currentRC.yaw/10000;
    mode = currentRC.mode;

}

void RCVelocity::getValueQua(DJI::OSDK::Vehicle *vehicle){
  DJI::OSDK::Telemetry::Quaternion broadcastQ;
  broadcastQ = vehicle->broadcast->getQuaternion();
  pitch = toEulerAngle((static_cast<void*>(&broadcastQ))).x;
  roll = toEulerAngle((static_cast<void*>(&broadcastQ))).y;
  yaw = toEulerAngle((static_cast<void*>(&broadcastQ))).z;

}



string RCVelocity::tosendData(){
    string str ;
    str+="#c4 ";
    str=str+to_string(Vx)+" "+to_string(Vy)+" "+to_string(Vz)+" "+to_string(Yaw)+" $";
    return str;
}

string RCVelocity::tosendX(){
    string str ;
    str+="#c4 ";
    str=str+to_string(pitch)+" "+to_string(roll)+" "+to_string(yaw)+" "+to_string(Yaw)+" $";
    return str;
}