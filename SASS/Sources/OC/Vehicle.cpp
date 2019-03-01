#include <Sources/OC/Vehicle.h>

using namespace sources::oc;

Vehicle::Vehicle(Directions direction) : m_direction(direction)
{

}

void Vehicle::set_speed(float speed)
{
     m_speed = speed;
}

Directions Vehicle::get_direction()
{
    return m_direction;
}

float Vehicle::get_speed()
{
    return m_speed;
}


