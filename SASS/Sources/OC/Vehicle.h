/*
 * Vehicle.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef VEHICLE_H_
#define VEHICLE_H_

/* Standard headers */
#include <stdint.h>

/* SASS-specific headers */
#include <Sources/Directions.h>
#include <Sources/OC/DetectedObject.h>

using namespace sources;

namespace sources
{
namespace oc
{


class Vehicle : public DetectedObject
{
public:
    Vehicle(Directions direction);
    ~Vehicle();

    void set_velocity(uint16_t velocity);
    uint16_t get_velocity();
    Directions get_direction();

private:
    Directions m_direction;
    uint16_t m_velocity;
};


}} // sources::oc

#endif // VEHICLE_H_
