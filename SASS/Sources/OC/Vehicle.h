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


}} // sources::oc





#endif // VEHICLE_H_
