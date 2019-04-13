/*
 * Lidar.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif

#include <stdio.h>

/* System headers */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* XDC module headers */
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* Board-specific headers */
#include <Board.h>
#include <Sources/GreenBoard.h>

/* SASS-specific headers */
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/Control.h>
#include <Sources/LLHA/Lights/Lights.h>

using namespace sources::llha::sensors;
using namespace sources::llha::lights;

/* Initialize LiDAR instances to null for multiton pattern */
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_north = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_east = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_south = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_west = nullptr;

/* Set default address for LiDAR-Lite */
uint8_t sources::llha::sensors::Lidar::default_addr = SLAVE_ADDR;

/**
    Constructor determines the direction and initializes the radar sensor.

    @param lidar_type (representing the direction/ports of the sensor).
*/
Lidar::Lidar(Directions direction) : m_current_addr(default_addr), m_direction(direction)
{
    switch(direction)
    {
    case Directions::NORTH:
        m_hardware_module = LIDAR_N;
        break;
    case Directions::EAST:
        m_hardware_module = LIDAR_E;
        break;
    case Directions::SOUTH:
        m_hardware_module = LIDAR_S;
        break;
    case Directions::WEST:
        m_hardware_module = LIDAR_W;
        break;
    default:
        Control::get_instance()->fail_system();
        break;
    };

    init();
}

Lidar::~Lidar()
{
    // TODO: Clean up (if necessary)
}

/**
    LiDAR follows the `multiton` design pattern, 
    therefore we check to see if an object in the specified direction exists before instantiating.
    
    @param lidar_type (representing the direction/ports of the sensor).
*/
Lidar* Lidar::get_instance(Directions direction)
{
    switch(direction)
    {

    case Directions::NORTH:
        if (lidar_north == nullptr)
        {
            lidar_north = new Lidar(direction);
        }
        return lidar_north;
    
    case Directions::EAST:
        if (lidar_east == nullptr)
        {
            lidar_east = new Lidar(direction);
        }
        return lidar_east;
    
    case Directions::SOUTH:
        if (lidar_south == nullptr)
        {
            lidar_south = new Lidar(direction);
        }
        return lidar_south;
    
    case Directions::WEST:
        if (lidar_west == nullptr)
        {
            lidar_west = new Lidar(direction);
        }
        return lidar_west;
    
    default:
        Control::get_instance()->fail_system();
        return nullptr; 
    };
}




/**
    Initializes hardware on MSP432 for I2C communication.
*/
void Lidar::init()
{    
    /* Initialize all buffers */
    for (uint8_t i = 0; i < 10; i++) {
        rxBuffer[i] = 0x00;
        txBuffer[i] = 0x00;
    }

    /* Call driver init functions */
    I2C_init();

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.bitRate = I2C_100kHz;
}

/**
    Sets LiDAR-Lite v3HP config  (part 1)
*/
void Lidar::config_1()
{    
    bool transferOK = false;

    txBuffer[0] = MAX_ACQUISITION_COUNT_REG;
    txBuffer[1] = 0x80;

    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);
}

/**
    Sets LiDAR-Lite v3HP config  (part 2)
*/
void Lidar::config_2()
{
    bool transferOK = false;

    txBuffer[0] = ACQUISITION_CONFIG_REG;
    txBuffer[1] = 0x08;

    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);
}

/**
    Sets LiDAR-Lite v3HP config  (part 3)
*/
void Lidar::config_3()
{
    bool transferOK = false;

    txBuffer[0] = REFERENCE_COUNT_REG;
    txBuffer[1] = 0x05;

    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);
}

/**
    Sets LiDAR-Lite v3HP config  (part 4)
*/
void Lidar::config_4()
{
    bool transferOK = false;

    txBuffer[0] = PEAK_DETECTION_REG;
    txBuffer[1] = 0x00;

    /* Write the actual 0x13 bytes */
    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);
}

/**
    Sets LiDAR-Lite v3HP config  (part 5)
    This sends a start command to the device
*/
void Lidar::start_reading()
{
    bool transferOK = false;

    txBuffer[0] = DEVICE_COMMAND_REG;
    txBuffer[1] = 0x04;

    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Re-try reading from slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);
}

/**
    Sets LiDAR-Lite v3HP config  (part 6)
    This reads the status register until it is empty, which indicates that we can get a distance.
*/
void Lidar::wait_until_ready()
{
    bool transferOK = false;

    txBuffer[0] = SYSTEM_STATUS_REG;
    rxBuffer[0] = 0xFF;

    while ((rxBuffer[0] & 0x01) != 0x00)
    {
        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 1;

        do {
            transferOK = I2C_transfer(i2c, &i2cTransaction);
            if (!transferOK)
            {
                Control::get_instance()->fail_system();
            }
        } while (!transferOK);
    }
}

/**
    Reads from LiDAR device to get distance.

    @return distance to the nearest moving object that the LiDAR detected.
*/
uint16_t Lidar::read_dist()
{
    bool transferOK = false;

    txBuffer[0] = DISTANCE_REG;

    rxBuffer[0] = 0x00;
    rxBuffer[1] = 0x00;

    i2cTransaction.slaveAddress = SLAVE_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    /* Re-try reading from slave till I2C_transfer returns true */
    do {
        transferOK = I2C_transfer(i2c, &i2cTransaction);
        if (!transferOK)
        {
            Control::get_instance()->fail_system();
        }
    } while (!transferOK);

    uint16_t dist = (rxBuffer[0] << 8) | rxBuffer[1];

    return dist;
}

/**
    Gets distance from LiDAR device.

    @return distance to the nearest moving object that the LiDAR detected.
*/
uint16_t Lidar::get_distance()
{
    uint16_t dist = -1;

    i2c = I2C_open(m_hardware_module, &i2cParams);

    if (i2c == NULL) {
        Control::get_instance()->fail_system();
    }

    // TODO: Check if we need to do the config every time we want a distance or not
    config_1();
    config_2();
    config_3();
    config_4();

    start_reading();
    wait_until_ready();

    dist = read_dist();

    I2C_close(i2c);

    return dist;
}

/**
    Calculates velocity from previously seen distances.

    @return distance to the nearest moving object that the LiDAR detected.
*/
uint16_t Lidar::get_velocity()
{
    return 0;
}
