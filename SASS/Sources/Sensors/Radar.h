#ifndef RADAR_H_
#define RADAR_H_

enum RadarInstanceType
{
    RADAR_NORTH,
    RADAR_EAST,
    RADAR_SOUTH,
    RADAR_WEST
};

namespace sources
{
namespace sensors
{


class Radar
{
public:
    Radar* get_instance(RadarInstanceType radar_type);

    double get_distance();
    double get_velocity();

private:
    Radar();
    virtual ~Radar();

    Radar* radar_north;
    Radar* radar_east;
    Radar* radar_south;
    Radar* radar_west;
};


}} // sources::sensors

#endif // RADAR_H_
