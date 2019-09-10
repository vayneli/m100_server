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

    float roll;
    float pitch;
    float yaw;
public:
    void getvalue(DJI::OSDK::Vehicle *vehicle);
    void getValueQua(DJI::OSDK::Vehicle *vehicle);
    std::string tosendData();
    std::string tosendX();

};


#endif