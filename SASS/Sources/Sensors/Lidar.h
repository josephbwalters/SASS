#ifndef LIDAR_H_
#define LIDAR_H_

#include <stdint.h>

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
    void init();
    void set_i2c_addr(uint8_t new_addr);
    void configure(uint8_t config);
    void write(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes);
    void read(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes);

private:
    Lidar(LidarInstanceType lidar_type);
    virtual ~Lidar();

    static uint8_t default_addr;
    uint8_t m_current_addr;

    LidarInstanceType m_lidar_type;

    static Lidar* lidar_north;
    static Lidar* lidar_east;
    static Lidar* lidar_south;
    static Lidar* lidar_west;
};


}} // sources::sensors

#endif // LIDAR_H_
