/*
 * Radar.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef RADAR_H_
#define RADAR_H_

/* Standard headers */
#include <stdint.h>
#include <tuple>

/* System headers */
#include <ti/drivers/SPI.h>

/* SASS-specific headers */
#include <Sources/Directions.h>

using namespace std;

namespace sources
{
namespace llha
{
namespace sensors
{


class Radar
{
public:
    static Radar* get_instance(Directions direction);
    void init();

    uint16_t get_distance();
    uint16_t get_velocity();
    tuple<uint16_t, uint16_t> get_data();

    // Thread-able method(s)
    static void *radarTestThread(void *args);

private:
    Radar(Directions direction);
    virtual ~Radar();

    Directions m_direction;
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
