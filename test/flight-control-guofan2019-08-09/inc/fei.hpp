#ifndef FEI_H
#define FEI_H

#include "flight_control_sample.hpp"
#include <string.h>
#include <stdio.h>

class RCVelocity
{
private:
    
public:
    float Vx;
    float Vy;
    float Vz;
    float Yaw;
    int mode;
public:
void getvalue(DJI::OSDK::Vehicle *vehicle);
std::string tosendData();

};


#endif