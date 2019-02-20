#define __MSP432P401R__

#include <xdc/runtime/System.h>
#include <xdc/std.h>

#include <ti/sysbios/knl/Task.h>

#include <Board.h>
#include <Sources/GreenBoard.h>

#include <Sources/Utils/Logger.h>
#include <Sources/LLHA/Sensors/Radar.h>

using namespace sources::llha::sensors;
using namespace sources::utils;

sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_north = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_east = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_south = nullptr;
sources::llha::sensors::Radar* sources::llha::sensors::Radar::radar_west = nullptr;

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
    Logger::print((String)"Beginning initialization...");
    SPI_init();  // Initialize the SPI driver

    SPI_Params_init(&spiParams);  // Initialize SPI parameters
    spiParams.dataSize = 8;       // 8-bit data size

    Logger::print((String)"Finished initialization.");
}

uint16_t Radar::get_distance()
{
    uint16_t dist = 0;
    bool transferOK = false;

    spi = SPI_open(m_hardware_module, &spiParams);
    if (spi == NULL)
    {
        Logger::print((String)"Error initializing SPI.\n");
        // TODO: Throw exception
    }
    else
    {
        Logger::print((String)"I2C initialized.\n");
    }

    // Fill in transmitBuffer

    txBuffer[0] = 0x0F;

    spiTransaction.count = 1;
    spiTransaction.txBuf = txBuffer;
    spiTransaction.rxBuf = rxBuffer;

    transferOK = SPI_transfer(spi, &spiTransaction);
    if (!transferOK)
    {
        Logger::print((String)"Error starting SPI transaction.\n");
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
        Logger::print((String)"Error starting SPI transaction.\n");
    }

    SPI_close(spi);
    Logger::print((String)"SPI closed!\n");

    return dist;
}

double Radar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}

void *Radar::radarTestThread(void *args)
{
    Radar* radar_north = Radar::get_instance(RadarInstanceType::RADAR_NORTH);

    while(1)
    {
        uint16_t dist = radar_north->get_distance();
        Logger::print_value((String)"Distance is", dist);
        Task_yield();
    }
}
