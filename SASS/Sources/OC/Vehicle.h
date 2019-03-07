/*
 * Vehicle.h
 * Created by: Joseph Walters, Trent Sellers (University of Central Florida)
 * Date: March 6, 2019
 * Last modified: March 6, 2019
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

    void set_speed(uint16_t speed);
    uint16_t get_speed();
    Directions get_direction();

private:
    Directions m_direction;
    uint16_t m_speed;
};


}} // sources::oc

#endif // VEHICLE_H_
