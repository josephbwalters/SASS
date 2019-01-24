#include <xdc/runtime/System.h>
#include <ti/drivers/I2C.h>

#include <Sources/Sensors/Lidar.h>

using namespace sources::sensors;

sources::sensors::Lidar* sources::sensors::Lidar::lidar_north = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_east = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_south = nullptr;
sources::sensors::Lidar* sources::sensors::Lidar::lidar_west = nullptr;
uint8_t sources::sensors::Lidar::default_addr = 0;

Lidar::Lidar(LidarInstanceType lidar_type) : m_current_addr(default_addr), m_lidar_type(lidar_type)
{
    System_printf("Constructor\n");
    System_flush();
    init();
}

Lidar::~Lidar()
{
    // Destructor
}

Lidar* Lidar::get_instance(LidarInstanceType lidar_type)
{
    System_printf("Get Instance\n");
    System_flush();
    switch(lidar_type)
    {
    case LIDAR_NORTH:
        if(lidar_north != nullptr)
        {
            return lidar_north;
        }
        else
        {
            System_printf("Making NORTH Lidar\n");
            System_flush();
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
            lidar_west = new Lidar(lidar_type);
            return lidar_west;
        }
    default:
        return nullptr;
    };
}

uint16_t Lidar::get_distance()
{
    uint16_t distance;
    uint8_t data_bytes[2];

    // Read two bytes from register 0x0f and 0x10 (autoincrement)
    read(0x0f, data_bytes, 2);

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
    System_printf("INIT\n");
    System_flush();
    set_i2c_addr(default_addr);
    configure((uint8_t) 0);
}

// Currently using template code from Garmin libraries
void Lidar::set_i2c_addr(uint8_t new_addr)
{
    System_printf("Setting Address\n");
    System_flush();
    if (new_addr == default_addr) {
        System_printf("Default addr. leaving set addr func.\n");
        System_flush();
        return;
    }

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
    System_printf("Configure\n");
    System_flush();
    uint8_t sigCountMax;
    uint8_t acqConfigReg;
    uint8_t refCountMax;
    uint8_t thresholdBypass;

    switch (config)
    {
    case 0: // Default mode, balanced performance
        System_printf("Default Mode\n");
        System_flush();
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x00; // Default
        break;

    case 1: // Maximum range
        sigCountMax     = 0xff;
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x00; // Default
        break;

    case 2: // High sensitivity detection, high erroneous measurements
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0x80;
        break;

    case 3: // Low sensitivity detection, low erroneous measurements
        sigCountMax     = 0x80; // Default
        acqConfigReg    = 0x08; // Default
        refCountMax     = 0x05; // Default
        thresholdBypass = 0xb0;
        break;
    }

    System_printf("Writing for Configs1\n");
    System_flush();
    write(0x02, &sigCountMax, 1);

    System_printf("Writing for Configs2\n");
    System_flush();
    write(0x04, &acqConfigReg, 1);

    System_printf("Writing for Configs3\n");
    System_flush();
    write(0x12, &refCountMax, 1);

    System_printf("Writing for Configs4\n");
    System_flush();
    write(0x1c, &thresholdBypass, 1);
}

void Lidar::write(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{
    System_printf("Writing something\n");
    System_flush();

    I2C_Handle handle;
    I2C_Params params;
    I2C_Transaction i2cTransaction;

    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_CALLBACK;
    params.transferCallbackFxn = NULL; // Originally, NULL was "someI2CCallbackFunction" - actually is default
    handle = I2C_open(0, &params); // Originally, 0 was "someI2C_configIndexValue"
    if (!handle)
    {
        System_printf("I2C did not open(write)\n");
        System_flush();
        // return exception something ??
    }

    i2cTransaction.slaveAddress = m_current_addr; // Originally, "some7BitI2CSlaveAddress"

    i2cTransaction.writeBuf = &reg_addr; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = 1; // Originally, "numOfBytesToWrite"

    bool ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        System_printf("Unsuccessful I2C transfer\n (write)");
        System_flush();
    }

    i2cTransaction.writeBuf = data_bytes; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = num_bytes; // Originally, "numOfBytesToWrite"

    ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        System_printf("Unsuccessful I2C transfer (write)\n");
        System_flush();
    }

    I2C_close(handle);
}

void Lidar::read(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{
    System_printf("Reading something\n");
    System_flush();
    I2C_Handle handle;
    I2C_Params params;
    I2C_Transaction i2cTransaction;

    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_CALLBACK;
    params.transferCallbackFxn = NULL; // Originally, NULL was "someI2CCallbackFunction" - actually is default
    handle = I2C_open(0, &params); // Originally, 0 was "someI2C_configIndexValue"
    if (!handle)
    {
        System_printf("I2C did not open (read)\n");
        System_flush();
        // return exception something ??
    }

    i2cTransaction.slaveAddress = m_current_addr; // Originally, "some7BitI2CSlaveAddress"

    i2cTransaction.writeBuf = &reg_addr; // Originally, "someWriteBuffer"
    i2cTransaction.writeCount = 1; // Originally, "numOfBytesToWrite"

    bool ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        System_printf("Unsuccessful I2C transfer (read)\n");
        System_flush();
    }

    i2cTransaction.readBuf = data_bytes;
    i2cTransaction.readCount = num_bytes;

    ret = I2C_transfer(handle, &i2cTransaction);
    if (!ret)
    {
        System_printf("Unsuccessful I2C transfer (read)\n");
        System_flush();
    }

    I2C_close(handle);
}


