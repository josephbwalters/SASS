/*
 * Lidar.h
 * Created by: Joseph Walters, Trent Sellers
 */

#ifndef LIDAR_H_
#define LIDAR_H_

/* Standard headers */
#include <stdint.h>

/* System headers */
#include <ti/drivers/I2C.h>

/* SASS-specific headers */
#include <Sources/Directions.h>

#define SLAVE_ADDR 0x62

// Registers on the LiDAR-Lite v3HP device
#define DEVICE_COMMAND_REG 0x00
#define SYSTEM_STATUS_REG 0x01
#define DISTANCE_REG 0x8f
#define POWER_CONTROL_REG 0x65
#define PEAK_DETECTION_REG 0x1c
#define REFERENCE_COUNT_REG 0x12
#define ACQUISITION_CONFIG_REG 0x04
#define MAX_ACQUISITION_COUNT_REG 0x02

namespace sources
{
namespace llha
{
namespace sensors
{


class Lidar
{
public:
    static Lidar* get_instance(Directions direction);

    uint16_t get_distance();
    uint16_t get_velocity();

    // Thread-able method(s)
    static void *lidarDemoThread(void *args);

private:
    Lidar(Directions direction);
    virtual ~Lidar();

    static uint8_t default_addr;
    uint8_t m_current_addr;

    Directions m_direction;
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

    void init();
    void config_1();
    void config_2();
    void config_3();
    void config_4();
    void start_reading();
    void wait_until_ready();
    uint16_t read_dist();

};


}}} // sources::llha::sensors

#endif // LIDAR_H_
