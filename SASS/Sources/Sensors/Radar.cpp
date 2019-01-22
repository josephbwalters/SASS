#include <Sources/Sensors/Radar.h>

using namespace sources::sensors;

Radar::Radar()
{
    init();
}

Radar::~Radar()
{
    // Destructor
}

Radar* Radar::get_instance(RadarInstanceType radar_type)
{
    switch(radar_type)
    {
    case RADAR_NORTH:
        if(radar_north != nullptr)
        {
            return radar_north;
        }
        else
        {
            radar_north = new Radar();
            return radar_north;
        }
    case RADAR_EAST:
        if(radar_north != nullptr)
        {
            return radar_east;
        }
        else
        {
            radar_east = new Radar();
            return radar_east;
        }
    case RADAR_SOUTH:
        if(radar_north != nullptr)
        {
            return radar_south;
        }
        else
        {
            radar_south = new Radar();
            return radar_south;
        }
    case RADAR_WEST:
        if(radar_north != nullptr)
        {
            return radar_west;
        }
        else
        {
            radar_west = new Radar();
            return radar_west;
        }
    default:
        return nullptr;
    };
}

double Radar::get_distance()
{
    // TEMPORARY RETURN
    return 0.00;
}

double Radar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}

void Radar::init()
{

}
