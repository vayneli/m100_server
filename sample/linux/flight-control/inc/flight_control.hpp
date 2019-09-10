#include "flight_control_sample.hpp"

class FlightControl{
public:
    DJI::OSDK::Vehicle *vehicle;
    DJI::OSDK::Telemetry::Vector3f localOffset;


public:
    int coWtoB();
    int coBtoW();
    int coWorldtoLocal();




};
