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
string RCVelocity::tosendData(){
    string str ;
    str+="#c4 ";
    str=str+to_string(Vx)+" "+to_string(Vy)+" "+to_string(Vz)+" "+to_string(Yaw)+" $";
    return str;
}