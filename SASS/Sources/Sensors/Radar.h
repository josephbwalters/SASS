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
    static Radar* get_instance(RadarInstanceType radar_type);

    double get_distance();
    double get_velocity();
    void init();

private:
    Radar();
    virtual ~Radar();

    static Radar* radar_north;
    static Radar* radar_east;
    static Radar* radar_south;
    static Radar* radar_west;
};


}} // sources::sensors

#endif // RADAR_H_
