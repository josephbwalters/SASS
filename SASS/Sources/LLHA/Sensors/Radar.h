#ifndef RADAR_H_
#define RADAR_H_

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

    double get_distance();
    double get_velocity();
    void init();

private:
    Radar(RadarInstanceType radar_type);
    virtual ~Radar();

    RadarInstanceType m_radar_type;
    uint_least8_t m_hardware_module;

    // SPI-related variables
    SPI_Handle      spi;
    SPI_Params      spiParams;
    SPI_Transaction spiTransaction;
    uint8_t         transmitBuffer[10]; // Need to reconsider message size
    uint8_t         receiveBuffer[10];
    bool            transferOK;

    static Radar* radar_north;
    static Radar* radar_east;
    static Radar* radar_south;
    static Radar* radar_west;
};


}}} // sources::llha::sensors

#endif // RADAR_H_
