#define __MSP432P401R__

#include <xdc/runtime/System.h>
#include <xdc/std.h>

#include <Board.h>
#include <Sources/GreenBoard.h>

#include <Sources/Utils/Logger.h>
#include <Sources/LLHA/Sensors/Radar.h>

using namespace sources::llha::sensors;
using namespace sources::utils;

Radar::Radar(RadarInstanceType radar_type) : m_radar_type(radar_type)
{
    Logger::print((String)"Creating instance...");

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
    };

    init();
}

Radar::~Radar()
{
    // Destructor
}

Radar* Radar::get_instance(RadarInstanceType radar_type)
{
    switch(radar_type)
    {
    case RADAR_NORTH:
        if(radar_north != nullptr)
        {
            return radar_north;
        }
        else
        {
            radar_north = new Radar(radar_type);
            return radar_north;
        }
    case RADAR_EAST:
        if(radar_north != nullptr)
        {
            return radar_east;
        }
        else
        {
            radar_east = new Radar(radar_type);
            return radar_east;
        }
    case RADAR_SOUTH:
        if(radar_north != nullptr)
        {
            return radar_south;
        }
        else
        {
            radar_south = new Radar(radar_type);
            return radar_south;
        }
    case RADAR_WEST:
        if(radar_north != nullptr)
        {
            return radar_west;
        }
        else
        {
            radar_west = new Radar(radar_type);
            return radar_west;
        }
    default:
        return nullptr; // TODO: Throw exception
    };
}

void Radar::init()
{
    SPI_init();  // Initialize the SPI driver

    SPI_Params_init(&spiParams);  // Initialize SPI parameters
    spiParams.dataSize = 8;       // 8-bit data size
}

double Radar::get_distance()
{
    spi = SPI_open(Board_SPI0, &spiParams);
    if (spi == NULL)
    {
        while (1);  // SPI_open() failed
    }

    // Fill in transmitBuffer

    spiTransaction.count = 10; // Need to reconsider message size
    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;

    transferOK = SPI_transfer(spi, &spiTransaction);
    if (!transferOK)
    {
        // Error in SPI or transfer already in progress.
        while (1);
    }

    return 0.00;
}

double Radar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}
