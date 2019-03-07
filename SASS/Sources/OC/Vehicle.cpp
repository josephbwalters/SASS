/*
 * Vehicle.cpp
 * Created by: Joseph Walters, Trent Sellers (University of Central Florida)
 * Date: March 6, 2019
 * Last modified: March 6, 2019
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

void Vehicle::set_speed(uint16_t speed)
{
     m_speed = speed;
}

Directions Vehicle::get_direction()
{
    return m_direction;
}

uint16_t Vehicle::get_speed()
{
    return m_speed;
}


