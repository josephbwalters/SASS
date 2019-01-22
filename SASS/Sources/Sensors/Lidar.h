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
    void set_i2c_addr(uint8_t new_addr, uint8_t addr);
    void configure(uint8_t config, uint8_t lidar_lite_addr);
    void write(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes, uint8_t addr);
    void read(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes, uint8_t addr);

private:
    Lidar();
    virtual ~Lidar();

    uint8_t addr;

    static Lidar* lidar_north;
    static Lidar* lidar_east;
    static Lidar* lidar_south;
    static Lidar* lidar_west;
};


}} // sources::sensors

#endif // LIDAR_H_
