#include <Sources/Sensors/Lidar.h>

using namespace sources::sensors;

Lidar::Lidar()
{
    init();
}

Lidar::~Lidar()
{
    // Destructor
}

Lidar* Lidar::get_instance(LidarInstanceType lidar_type)
{
    switch(lidar_type)
    {
    case LIDAR_NORTH:
        if(lidar_north != nullptr)
        {
            return lidar_north;
        }
        else
        {
            lidar_north = new Lidar();
            return lidar_north;
        }
    case LIDAR_EAST:
        if(lidar_north != nullptr)
        {
            return lidar_east;
        }
        else
        {
            lidar_east = new Lidar();
            return lidar_east;
        }
    case LIDAR_SOUTH:
        if(lidar_north != nullptr)
        {
            return lidar_south;
        }
        else
        {
            lidar_south = new Lidar();
            return lidar_south;
        }
    case LIDAR_WEST:
        if(lidar_north != nullptr)
        {
            return lidar_west;
        }
        else
        {
            lidar_west = new Lidar();
            return lidar_west;
        }
    default:
        return nullptr;
    };
}

double Lidar::get_distance()
{
    // TEMPORARY RETURN
    return 0.00;
}

double Lidar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}

void Lidar::init()
{

}
