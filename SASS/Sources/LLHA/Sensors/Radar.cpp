#define __MSP432P401R__
// #define DEBUG
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* Standard Headers */
#include <stdio.h>

#include <ti/sysbios/knl/Task.h>

#include <Board.h>
#include <Sources/GreenBoard.h>

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
Radar::Radar(RadarInstanceType radar_type) : m_radar_type(radar_type)
{
    switch(radar_type)
    {
    case RADAR_NORTH:
        m_hardware_module = RADAR_N;
        break;
    case RADAR_EAST:
        m_hardware_module = RADAR_E;
        break;
    case RADAR_SOUTH:
        m_hardware_module = RADAR_S;
        break;
    case RADAR_WEST:
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
    // Destructor
}

/**
    Radar follows the `multiton` design pattern, 
    therefore we check to see if an object in the specified direction exists before instantiating.
    
    @param radar_type (representing the direction/ports of the sensor).
*/
Radar* Radar::get_instance(RadarInstanceType radar_type)
{
    switch(radar_type)
    {
    case RADAR_NORTH:
        if (radar_north == nullptr)
        {
            radar_north = new Radar(radar_type);
        }
        return radar_north;

    case RADAR_EAST:
        if (radar_east == nullptr)
        {
            radar_east = new Radar(radar_type);
        }
        return radar_east;

    case RADAR_SOUTH:
        if (radar_south == nullptr)
        {
            radar_south = new Radar(radar_type);
        }
        return radar_south;

    case RADAR_WEST:
        if (radar_west == nullptr)
        {
            radar_west = new Radar(radar_type);
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
}

/**
    Reads from mmwave device to get distance.

    @return distance to the nearest moving object that the radar detected.
*/
uint16_t Radar::get_distance()
{
    uint16_t dist = 0;
    bool transferOK = false;

    spi = SPI_open(m_hardware_module, &spiParams);
    if (spi == NULL)
    {
        // TODO: Throw exception
    }

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

    return dist;
}

/**
    Reads from mmwave device to get velocity.

    @return velocity of the nearest moving object that the radar detected.
*/
double Radar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}

/**
    Test thread for mmwave reading.
*/
void *Radar::radarTestThread(void *args)
{
    Radar* radar_north = Radar::get_instance(RadarInstanceType::RADAR_NORTH);

    while(1)
    {
        uint16_t dist = radar_north->get_distance();
        printf("Distance: %d\n", dist);
        Task_yield();
    }
}
