#ifndef LIDAR_H_
#define LIDAR_H_

enum LidarInstanceType
{
    LIDAR_NORTH,
    LIDAR_EAST,
    LIDAR_SOUTH,
    LIDAR_WEST
};

namespace sources
{
namespace sensors
{


class Lidar
{
public:
    Lidar* get_instance(LidarInstanceType lidar_type);

    double get_distance();
    double get_velocity();

private:
    Lidar();
    virtual ~Lidar();

    Lidar* lidar_north;
    Lidar* lidar_east;
    Lidar* lidar_south;
    Lidar* lidar_west;
};


}} // sources::sensors

#endif // LIDAR_H_
