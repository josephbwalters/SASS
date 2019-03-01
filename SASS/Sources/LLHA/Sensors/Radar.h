#ifndef RADAR_H_
#define RADAR_H_

#include <stdint.h>
#include <ti/drivers/SPI.h>

enum RadarInstanceType
{
    RADAR_NORTH,
    RADAR_EAST,
    RADAR_SOUTH,
    RADAR_WEST
};

namespace sources
{
namespace llha
{
namespace sensors
{


class Radar
{
public:
    static Radar* get_instance(RadarInstanceType radar_type);
    void init();

    uint16_t get_distance();
    double get_velocity();

    // Thread-able method(s)
    static void *radarTestThread(void *args);

private:
    Radar(RadarInstanceType radar_type);
    virtual ~Radar();

    RadarInstanceType m_radar_type;
    uint_least8_t m_hardware_module;

    // SPI-related variables
    SPI_Handle      spi;
    SPI_Params      spiParams;
    SPI_Transaction spiTransaction;
    uint8_t         txBuffer[10]; // Need to reconsider message size
    uint8_t         rxBuffer[10];

    static Radar* radar_north;
    static Radar* radar_east;
    static Radar* radar_south;
    static Radar* radar_west;
};


}}} // sources::llha::sensors

#endif // RADAR_H_
