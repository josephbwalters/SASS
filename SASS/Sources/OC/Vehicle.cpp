#include <Sources/OC/Vehicle.h>

using namespace sources::oc;

Vehicle::Vehicle(Directions dir, float s)
{
   direction = dir;
   speed = s;
}

void Vehicle::set_speed(float s)
{
     speed = s;
}

Directions Vehicle::get_direction()
{
    return direction;
}

float Vehicle::get_speed()
{
    return speed;
}


