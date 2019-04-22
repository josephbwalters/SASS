/*
 * Vehicle.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

/* SASS-specific headers */
#include <Sources/OC/Vehicle.h>

using namespace sources::oc;

Vehicle::Vehicle(Directions direction) : m_direction(direction)
{

}

Vehicle::~Vehicle()
{
    // TODO: Clean up (if necessary)
}

void Vehicle::set_velocity(uint16_t speed)
{
     m_velocity = speed;
}

Directions Vehicle::get_direction()
{
    return m_direction;
}

uint16_t Vehicle::get_velocity()
{
    return m_velocity;
}


