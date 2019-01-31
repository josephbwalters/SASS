#include <xdc/runtime/System.h>
#include <xdc/std.h>

#include <ti/drivers/I2C.h>

#include <Board.h>
#include <Sources/Sensors/Lidar.h>
#include <Sources/Logger/Logger.h>

#include <pthread.h>


#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* Example/Board Header files */
#include "Board.h"

using namespace sources::sensors;
using namespace sources::logger;

sources::sensors::Lidar* sources::sensors::Lidar::lidar_north = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_east = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_south = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_west = nullptr;
uint8_t sources::sensors::Lidar::default_addr = 0x62;

Lidar::Lidar(LidarInstanceType lidar_type) : m_current_addr(default_addr), m_lidar_type(lidar_type)
{
    Logger::print((String)"Creating instance...");
    //init();
}

Lidar::~Lidar()
{
    // Destructor
}

Lidar* Lidar::get_instance(LidarInstanceType lidar_type)
{
    Logger::print((String)"Getting lidar instance...");
    switch(lidar_type)
    {
    case LIDAR_NORTH:
        if(lidar_north != nullptr)
        {
            return lidar_north;
        }
        else
        {
            Logger::print((String)"Lidar instance to create - NORTH");
            lidar_north = new Lidar(lidar_type);
            return lidar_north;
        }
    case LIDAR_EAST:
        if(lidar_north != nullptr)
        {
            return lidar_east;
        }
        else
        {
            Logger::print((String)"Lidar instance to create - EAST");
            lidar_east = new Lidar(lidar_type);
            return lidar_east;
        }
    case LIDAR_SOUTH:
        if(lidar_north != nullptr)
        {
            return lidar_south;
        }
        else
        {
            Logger::print((String)"Lidar instance to create - SOUTH");
            lidar_south = new Lidar(lidar_type);
            return lidar_south;
        }
    case LIDAR_WEST:
        if(lidar_north != nullptr)
        {
            return lidar_west;
        }
        else
        {
            Logger::print((String)"Lidar instance to create - WEST");
            lidar_west = new Lidar(lidar_type);
            return lidar_west;
        }
    default:
        return nullptr;
    };
}

// TODO: Unnecessary ?
//uint16_t Lidar::get_distance()
//{
//    Logger::print((String)"Getting distance!");
//
//    uint16_t distance;
//    uint8_t data_bytes[2];
//
//    // Read two bytes from register 0x0f and 0x10 (autoincrement)
//    Logger::print((String)"Reading distance from lidar now...");
//    read(0x8f, data_bytes, 2);
//
//    Logger::print_buffer((String)"Distance:", data_bytes, 2);
//    // Shift high byte and add to low byte
//    distance = (data_bytes[0] << 8) | data_bytes[1];
//
//    return (distance);
//}

// TODO: Check if we can do this
//uint16_t Lidar::get_velocity()
//{
//    uint16_t velocity;
//    uint8_t data_bytes[2];
//
//    // Read two bytes from register 0x0f and 0x10 (autoincrement)
//    read(0x0f, data_bytes, 2);
//
//    // Shift high byte and add to low byte
//    velocity = (data_bytes[0] << 8) | data_bytes[1];
//
//    return (velocity);
//}

uint16_t Lidar::get_distance()
{
    uint16_t dist = 0;
    bool retVal = false;
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;
    uint8_t i = 0, k = 0;

    /* Buffers used in this code example */
    uint8_t             txBuffer[10];
    uint8_t             rxBuffer[10];


    while(k++ < 100)
    {
        /* Initialize all buffers */
        for (i = 0; i < 10; i++) {
            rxBuffer[i] = 0x00;
            txBuffer[i] = 0x00;
        }

        /* Call driver init functions */
        I2C_init();

        /* Create I2C for usage */
        I2C_Params_init(&i2cParams);
        i2cParams.transferMode = I2C_MODE_BLOCKING;
        i2cParams.bitRate = I2C_100kHz;

        //printf("Opening connection\n");
        i2c = I2C_open(Board_I2C_TMP, &i2cParams);

        if (i2c == NULL) {
            Logger::print((String)"Error Initializing I2C!\n");

        } else {
            Logger::print((String)"I2C Initialized!\n");
        }


        Logger::print((String)"I2C Config1!\n");

        // Config 1
        txBuffer[0] = 0x02;
        txBuffer[1] = 0x80;

        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 0;

        Logger::print((String)"Attempting to write");
        /* Re-try writing to slave till I2C_transfer returns true */
        do {
            Logger::print((String)"Attempting transfer");
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);


        Logger::print((String)"I2C Config2!\n");

        // Config 2
        txBuffer[0] = 0x04;
        txBuffer[1] = 0x08;

        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 0;

        retVal = I2C_transfer(i2c, &i2cTransaction);

        Logger::print((String)"I2C Config3!\n");

        // Config 3
        txBuffer[0] = 0x12;
        txBuffer[1] = 0x05;

        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 0;

        /* Re-try writing to slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);

        Logger::print((String)"I2C Config4!\n");

        // Config 4
        txBuffer[0] = 0x1c;
        txBuffer[1] = 0x00;

        /* Write the actual 0x13 bytes */
        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 0;

        /* Re-try writing to slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);

        Logger::print((String)"I2C Write to device to read!\n");

        // Write 04 to 00
        txBuffer[0] = 0x00;
        txBuffer[1] = 0x04;

        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 0;

        /* Re-try reading from slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);


        // Read 0x01 until bit 0 goes low
        txBuffer[0] = 0x01;
        rxBuffer[0] = 0xFF;

        while((rxBuffer[0] & 0x01) != 0x00)
        {
            i2cTransaction.slaveAddress = SLAVE_ADDR;
            i2cTransaction.writeBuf = txBuffer;
            i2cTransaction.writeCount = 1;
            i2cTransaction.readBuf = rxBuffer;
            i2cTransaction.readCount = 1;

            do {
                retVal = I2C_transfer(i2c, &i2cTransaction);
            } while(!retVal);

        }

        Logger::print((String)"I2C Getting dist!\n");

        // Read two bytes from 0x8f (High byte 0x0f then low byte 0x10) to obtain the 16-bit measured distance in centimeters.
        txBuffer[0] = 0x8f;

        rxBuffer[0] = 0x00;
        rxBuffer[1] = 0x00;

        i2cTransaction.slaveAddress = SLAVE_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = rxBuffer;
        i2cTransaction.readCount = 2;

        /* Re-try reading from slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);

        dist = (rxBuffer[0] << 8) | rxBuffer[1];
        Logger::print_value((String)"Distance (cm)", dist);

        if (dist < (uint16_t)100) {
            GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN3);
            GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN3);
        }

        Logger::print((String)"I2C master/slave transfer complete\n");

        /* Deinitialized I2C */
        I2C_close(i2c);
        Logger::print((String)"I2C closed!\n");

        //sleep(1);
    }
    return dist;
}

void Lidar::init()
{
    Logger::print((String)"Initializing Lidar...");
    set_i2c_addr(default_addr);
    configure((uint8_t) 0);
}

// Currently using template code from Garmin libraries
void Lidar::set_i2c_addr(uint8_t new_addr)
{
    Logger::print((String)"Setting lidar i2c address if necessary");
    if (new_addr == default_addr) return;

    uint8_t dataBytes[2];

    // Read UNIT_ID serial number bytes and write them into I2C_ID byte locations
    read(0x16, dataBytes, 2);
    write(0x18, dataBytes, 2);

    // Write the new I2C device address to registers
    // left shift by one to work around data alignment issue in v3HP
    dataBytes[0] = (new_addr << 1);
    write(0x1a, dataBytes, 1);

    // Enable the new I2C device address using the default I2C device address
    read(0x1e, dataBytes, 1);
    dataBytes[0] = dataBytes[0] | (1 << 4); // set bit to enable the new address
    write(0x1e, dataBytes, 1);

    // Disabling default device
    m_current_addr = new_addr;
    read (0x1e, dataBytes, 1);
    dataBytes[0] = dataBytes[0] | (1 << 3); // set bit to disable default address
    write(0x1e, dataBytes, 1);
}

// Currently using template code from Garmin libraries
void Lidar::configure(uint8_t config)
{
    Logger::print((String)"Configuring lidar...");
    uint8_t sigCountMax;
    uint8_t acqConfigReg;
    uint8_t refCountMax;
    uint8_t thresholdBypass;

    switch (config)
    {
    case 0: // Default mode, balanced performance
        Logger::print((String)"Config - Default Mode");
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x00; // Default
        break;

    case 1: // Maximum range
        Logger::print((String)"Config - Max Range");
        sigCountMax     = 0xff;
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x00; // Default
        break;

    case 2: // High sensitivity detection, high erroneous measurements
        Logger::print((String)"Config - High Sense, High Error");
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x80;
        break;

    case 3: // Low sensitivity detection, low erroneous measurements
        Logger::print((String)"Config - Low Sense, Low Error");
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0xb0;
        break;
    }

    Logger::print((String)"Writing configuration!");
    write(0x02, &sigCountMax, 1);
    write(0x04, &acqConfigReg, 1);
    write(0x12, &refCountMax, 1);
    write(0x1c, &thresholdBypass, 1);
    Logger::print((String)"Writing configuration -- COMPLETE");
}

void Lidar::write(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{
    Logger::print_buffer((String)"Data to write:", data_bytes, num_bytes);

    I2C_Handle handle;
    I2C_Params params;
    I2C_Transaction i2cTransaction;

    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_CALLBACK;
    // params.transferCallbackFxn = NULL; // Originally, NULL was "someI2CCallbackFunction" - actually is default
    params.bitRate = I2C_100kHz;

    Logger::print((String)"Opening I2C transmission...");
    handle = I2C_open(Board_I2C0, &params); // Originally, 0 was "someI2C_configIndexValue"
    if (!handle)
    {
        Logger::print((String)"I2C Failed to Open!");
        // return exception something ??
    }

    i2cTransaction.slaveAddress = m_current_addr; // Originally, "some7BitI2CSlaveAddress"

    i2cTransaction.writeBuf = &reg_addr; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = 1; // Originally, "numOfBytesToWrite"

    bool ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        Logger::print((String)"Unsuccessful I2C transfer on write addr.");
    }

    i2cTransaction.writeBuf = data_bytes; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = num_bytes; // Originally, "numOfBytesToWrite"

    ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        Logger::print((String)"Unsuccessful I2C transfer on write data");
    }

    I2C_close(handle);
    Logger::print((String)"Closed I2C transmission");
}

void Lidar::read(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{
    Logger::print((String)"Reading data into buffer");
    Logger::print_buffer((String)"Current Buffer contents:", data_bytes, num_bytes);

    I2C_Handle handle;
    I2C_Params params;

    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_CALLBACK;
    params.transferCallbackFxn = NULL; // Originally, NULL was "someI2CCallbackFunction" - actually is default
    params.bitRate = I2C_100kHz;

    Logger::print((String)"Opening I2C transmission...");
    handle = I2C_open(Board_I2C0, &params); // Originally, 0 was "someI2C_configIndexValue"
    if (!handle)
    {
        Logger::print((String)"I2C Failed to Open!");
        // return exception something ??
    }

    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = m_current_addr; // Originally, "some7BitI2CSlaveAddress"

    i2cTransaction.writeBuf = &reg_addr; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = 1; // Originally, "numOfBytesToWrite"
    bool ret = I2C_transfer(handle, &i2cTransaction);
    while (!ret)
    {
        Logger::print((String)"Unsuccessful I2C transfer on write addr.");
    }

    i2cTransaction.writeCount = 0;
    i2cTransaction.readBuf = data_bytes;
    i2cTransaction.readCount = num_bytes;
    ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        Logger::print((String)"Unsuccessful I2C transfer on read data");
    }

    Logger::print_buffer((String)"New Buffer contents:", data_bytes, num_bytes);

    I2C_close(handle);
    Logger::print((String)"Closed I2C transmission");
}
