#ifndef VEHICLE_H_
#define VEHICLE_H_

// Custom Headers
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

    void set_speed(float speed);
    float get_speed();
    Directions get_direction();

private:
    Directions m_direction;
    float m_speed;
};


}} // sources::oc





#endif // VEHICLE_H_
