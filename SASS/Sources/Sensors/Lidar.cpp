#include <xdc/runtime/System.h>
#include <xdc/std.h>

#include <ti/drivers/I2C.h>

#include <Sources/Board.h>
#include <Sources/Sensors/Lidar.h>
#include <Sources/Logger/Logger.h>



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
    init();
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

uint16_t Lidar::get_distance()
{
    // Write hex 0x04 to 0x00
    // Read reg  0x01, repeat until lsb goes low
    // Read 2 bytes from 0x8f (high byte 0x0f, low byte 0x10)

//    uint8_t write_buf = 0x04;
//    uint8_t read_buf;
//    write((uint8_t)0x00, &write_buf, 1);
//    uint8_t temp;
//    while(true) {
//        read((uint8_t)0x01, &read_buf, 1);
//        temp = read_buf << 7;
//        temp = temp >> 7;
//        System_printf("%x\n", temp);
//        System_flush();
//        if (temp == 0) break;
//    }

    Logger::print((String)"Getting distance!");


    uint16_t distance;
    uint8_t data_bytes[2];

    // Read two bytes from register 0x0f and 0x10 (autoincrement)
    Logger::print((String)"Reading distance from lidar now...");
    read(0x8f, data_bytes, 2);

    Logger::print_buffer((String)"Distance:", data_bytes);
    // Shift high byte and add to low byte
    distance = (data_bytes[0] << 8) | data_bytes[1];

    return (distance);
}

uint16_t Lidar::get_velocity()
{
    uint16_t velocity;
    uint8_t data_bytes[2];

    // Read two bytes from register 0x0f and 0x10 (autoincrement)
    read(0x0f, data_bytes, 2);

    // Shift high byte and add to low byte
    velocity = (data_bytes[0] << 8) | data_bytes[1];

    return (velocity);
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
    Logger::print_buffer((String)"Writing data", data_bytes);

    I2C_Handle handle;
    I2C_Params params;
    I2C_Transaction i2cTransaction;

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
    Logger::print_buffer((String)"Buffer contents:", data_bytes);

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


    I2C_close(handle);
    Logger::print((String)"Closed I2C transmission");
}


