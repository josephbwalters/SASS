#ifndef LIDAR_H_
#define LIDAR_H_

#include <stdint.h>

#include <ti/drivers/I2C.h>

#define SLAVE_ADDR 0x62

#define DEVICE_COMMAND_REG 0x00
#define SYSTEM_STATUS_REG 0x01
#define DISTANCE_REG 0x8f
#define POWER_CONTROL_REG 0x65

enum LidarInstanceType
{
    LIDAR_NORTH,
    LIDAR_EAST,
    LIDAR_SOUTH,
    LIDAR_WEST
};

namespace sources
{
namespace llha
{
namespace sensors
{


class Lidar
{
public:
    static Lidar* get_instance(LidarInstanceType lidar_type);

    uint16_t get_distance();
    uint16_t get_velocity();
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
    uint_least8_t m_hardware_module;

    // I2C-related variables
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;
    uint8_t         txBuffer[10];
    uint8_t         rxBuffer[10];

    static Lidar* lidar_north;
    static Lidar* lidar_east;
    static Lidar* lidar_south;
    static Lidar* lidar_west;
};


}}} // sources::llha::sensors

#endif // LIDAR_H_
