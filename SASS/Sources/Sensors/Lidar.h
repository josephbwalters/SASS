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
    static Lidar* get_instance(LidarInstanceType lidar_type);

    double get_distance();
    double get_velocity();

private:
    Lidar();
    virtual ~Lidar();

    static Lidar* lidar_north;
    static Lidar* lidar_east;
    static Lidar* lidar_south;
    static Lidar* lidar_west;
};


}} // sources::sensors

#endif // LIDAR_H_
