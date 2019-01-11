#ifndef VEHICLE_H_
#define VEHICLE_H_

// Custom Headers
#include "../Directions.h"

using namespace sources;

namespace sources
{
namespace objects
{


class Vehicle
{
private:
    Directions direction;
    float speed;

protected:

public:
    Vehicle(Directions dir, float s);
    void set_speed(float s);
    Directions get_direction();
    float get_speed();
};


}} // sources::objects





#endif // VEHICLE_H_
