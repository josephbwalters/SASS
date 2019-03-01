#define __MSP432P401R__
// #define DEBUG

/* System headers */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* XDC module Headers */
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* Board specific configurations */
#include <Board.h>
#include <Sources/GreenBoard.h>

/* Custom headers */
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Lights/Lights.h>

using namespace sources::llha::sensors;
using namespace sources::llha::lights;

// Initialize LiDAR instances to null for multiton pattern
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_north = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_east = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_south = nullptr;
sources::llha::sensors::Lidar* sources::llha::sensors::Lidar::lidar_west = nullptr;

// Set default address for LiDAR-Lite
uint8_t sources::llha::sensors::Lidar::default_addr = SLAVE_ADDR;

/**
    Constructor determines the direction and initializes the radar sensor.

    @param lidar_type (representing the direction/ports of the sensor).
*/
Lidar::Lidar(LidarInstanceType lidar_type) : m_current_addr(default_addr), m_lidar_type(lidar_type)
{
    switch(lidar_type)
    {
    case LIDAR_NORTH:
        m_hardware_module = LIDAR_N;
        break;
    case LIDAR_EAST:
        m_hardware_module = LIDAR_E;
        break;
    case LIDAR_SOUTH:
        m_hardware_module = LIDAR_S;
        break;
    case LIDAR_WEST:
        m_hardware_module = LIDAR_W;
        break;
    default:
        // TODO: Throw exception
        break;
    };

    init();
}

Lidar::~Lidar()
{
    // Destructor
}

/**
    LiDAR follows the `multiton` design pattern, 
    therefore we check to see if an object in the specified direction exists before instantiating.
    
    @param lidar_type (representing the direction/ports of the sensor).
*/
Lidar* Lidar::get_instance(LidarInstanceType lidar_type)
{
    switch(lidar_type)
    {

    case LIDAR_NORTH:
        if (lidar_north == nullptr)
        {
            lidar_north = new Lidar(lidar_type);
        }
        return lidar_north;
    
    case LIDAR_EAST:
        if (lidar_east == nullptr)
        {
            lidar_east = new Lidar(lidar_type);
        }
        return lidar_east;
    
    case LIDAR_SOUTH:
        if (lidar_south == nullptr)
        {
            lidar_south = new Lidar(lidar_type);
        }
        return lidar_south;
    
    case LIDAR_WEST:
        if (lidar_west == nullptr)
        {
            lidar_west = new Lidar(lidar_type);
        }
        return lidar_west;
    
    default:
        // TODO: Throw exception
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
    Sets LiDAR-Lite v3 config  (part1)
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
    } while(!transferOK);
}

/**
    Sets LiDAR-Lite v3 config  (part2)
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
    } while(!transferOK);
}

/**
    Sets LiDAR-Lite v3 config  (part3)
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
    } while(!transferOK);
}

/**
    Sets LiDAR-Lite v3 config  (part4)
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
    } while(!transferOK);
}

/**
    Sets LiDAR-Lite v3 config  (part5)
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
    } while(!transferOK);
}

/**
    Sets LiDAR-Lite v3 config  (part6)
    This reads the status register until it is empty, which indicates that we can get a distance.
*/
void Lidar::wait_until_ready()
{
    bool transferOK = false;

    txBuffer[0] = SYSTEM_STATUS_REG;
    rxBuffer[0] = 0xFF;

    while((rxBuffer[0] & 0x01) != 0x00)
    {
        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 1;

        do {
            transferOK = I2C_transfer(i2c, &i2cTransaction);
        } while(!transferOK);
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
    } while(!transferOK);

    uint16_t dist = (rxBuffer[0] << 8) | rxBuffer[1];

    return dist;
}

/**
    Gets distance from LiDAR device.

    @return distance to the nearest moving object that the LiDAR detected.
*/
uint16_t Lidar::get_distance()
{
    uint16_t dist = 0;

    i2c = I2C_open(m_hardware_module, &i2cParams);

    if (i2c == NULL) {
        // TODO: Throw exception
    }

    // TODO: Check if we need to do the config everytime we want a distance or not
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

/**
    Thread to toggle MOSFETs based on LiDAR input.
*/
void *Lidar::lidarDemoThread(void *args)
{
    Lights lights;
    Lidar* lidar_north = Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);

    while(1)
    {
        uint16_t dist = lidar_north->get_distance();

        // RED: P7.4
        // YELLOW: P7.6

        if(dist < 200)
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);

            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
            //lights.set_yellow(Directions::NORTH);
        }
        else
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);

            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
            //lights.set_red(Directions::NORTH);
        }
    }
}
