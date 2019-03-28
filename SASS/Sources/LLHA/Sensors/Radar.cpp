/*
 * Radar.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#define __MSP432P401R__
// #define DEBUG

/* Standard headers */
#include <stdio.h>

/* XDC module headers */
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* System headers */
#include <ti/sysbios/knl/Task.h>

/* Board-specific headers */
#include <Board.h>
#include <Sources/GreenBoard.h>

/* SASS-specific headers */
#include <Sources/LLHA/Sensors/Radar.h>

using namespace sources::llha::sensors;

// Initializes radar instances to null for multiton pattern
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_north = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_east = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_south = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_west = nullptr;

/**
    Constructor determines the direction and initializes the radar sensor.

    @param radar_type (representing the direction/ports of the sensor).
*/
Radar::Radar(Directions direction) : m_direction(direction)
{
    switch(direction)
    {
    case Directions::NORTH:
        m_hardware_module = RADAR_N;
        break;
    case Directions::EAST:
        m_hardware_module = RADAR_E;
        break;
    case Directions::SOUTH:
        m_hardware_module = RADAR_S;
        break;
    case Directions::WEST:
        m_hardware_module = RADAR_W;
        break;
    default:
        // TODO: Throw exception
        break;
    };

    init();
}

Radar::~Radar()
{
    // TODO: Clean up (if necessary)
}

/**
    Radar follows the `multiton` design pattern, 
    therefore we check to see if an object in the specified direction exists before instantiating.
    
    @param radar_type (representing the direction/ports of the sensor).
*/
Radar* Radar::get_instance(Directions direction)
{
    switch(direction)
    {
    case Directions::NORTH:
        if (radar_north == nullptr)
        {
            radar_north = new Radar(direction);
        }
        return radar_north;

    case Directions::EAST:
        if (radar_east == nullptr)
        {
            radar_east = new Radar(direction);
        }
        return radar_east;

    case Directions::SOUTH:
        if (radar_south == nullptr)
        {
            radar_south = new Radar(direction);
        }
        return radar_south;

    case Directions::WEST:
        if (radar_west == nullptr)
        {
            radar_west = new Radar(direction);
        }
        return radar_west;
        
    default:
        // TODO: Throw exception
        return nullptr; 
    };
}

/**
    Initializes hardware on MSP432 for SPI communication.
*/
void Radar::init()
{
    SPI_init();
    SPI_Params_init(&spiParams); 
    spiParams.dataSize = 8;
    spiParams.mode = SPI_MASTER;
}

/**
    Reads from mmWave device to get distance.

    @return distance to the nearest moving object that the radar detected.
*/
uint16_t Radar::get_distance()
{
    tuple<uint16_t, uint16_t> radar_data;
    radar_data = get_data();

    uint16_t distance = get<0>(radar_data);

    return distance;
}

/**
    Reads from mmWave device to get velocity.

    @return velocity of the nearest moving object that the radar detected.
*/
uint16_t Radar::get_velocity()
{
    tuple<uint16_t, uint16_t> radar_data;
    radar_data = get_data();

    uint16_t velocity = get<1>(radar_data);

    return velocity;
}

tuple<uint16_t, uint16_t> Radar::get_data()
{
    uint16_t distance = 0;
    uint16_t velocity = 0;
    bool transferOK = false;

    spi = SPI_open(m_hardware_module, &spiParams);
    if (spi == NULL)
    {
        // TODO: Throw exception
    }

    printf("SPI opened.\n");

    // Fill transmit buffer
    txBuffer[0] = 0x0F;

    spiTransaction.count = 1;
    spiTransaction.txBuf = txBuffer;
    spiTransaction.rxBuf = rxBuffer;

    transferOK = SPI_transfer(spi, &spiTransaction);
    if (!transferOK)
    {
        // TODO: Throw exception
    }

    rxBuffer[0] = 0x00;
    rxBuffer[1] = 0x00;
    rxBuffer[2] = 0x00;

    spiTransaction.count = 3; // 2 distance + 1 velocity
    spiTransaction.txBuf = txBuffer;
    spiTransaction.rxBuf = rxBuffer;

    transferOK = SPI_transfer(spi, &spiTransaction);
    if (!transferOK)
    {
        // Throw exception
    }

    SPI_close(spi);

    printf("SPI closed.\n");

    distance = (rxBuffer[0] << 8) | rxBuffer[1];

    velocity = rxBuffer[2];

    tuple<uint16_t, uint16_t> radar_data;
    radar_data = make_tuple(distance, velocity);

    return radar_data;
}

/**
    Test thread for mmWave reading.
*/
void *Radar::radarTestThread(void *args)
{
    // Instantiating RADAR_EAST due to availability of pins on red board.
    Radar* radar_north = Radar::get_instance(Directions::EAST);

    while (true)
    {
        printf("Getting distance...\n");
        uint16_t dist = radar_north->get_distance();
        printf("Distance: %d cm\n", dist);
        Task_yield();
    }
}
