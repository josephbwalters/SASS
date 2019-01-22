#include <Sources/Sensors/Lidar.h>

using namespace sources::sensors;

Lidar::Lidar(LidarInstanceType lidar_type) : m_current_addr(default_addr), m_lidar_type(lidar_type)
{
    init();
}

Lidar::~Lidar()
{
    // Destructor
}

Lidar* Lidar::get_instance(LidarInstanceType lidar_type)
{
    switch(lidar_type)
    {
    case LIDAR_NORTH:
        if(lidar_north != nullptr)
        {
            return lidar_north;
        }
        else
        {
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

double Lidar::get_distance()
{
    // TEMPORARY RETURN
    return 0.00;
}

double Lidar::get_velocity()
{
    // TEMPORARY RETURN
    return 0.00;
}

void Lidar::init()
{
    set_i2c_addr(default_addr);
    configure(default_addr);
}

// Currently using template code from Garmin libraries
void Lidar::set_i2c_addr(uint8_t new_addr)
{
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
    uint8_t sigCountMax;
    uint8_t acqConfigReg;
    uint8_t refCountMax;
    uint8_t thresholdBypass;

    switch (config)
    {
    case 0: // Default mode, balanced performance
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

    write(0x02, &sigCountMax, 1);
    write(0x04, &acqConfigReg, 1);
    write(0x12, &refCountMax, 1);
    write(0x1c, &thresholdBypass, 1);
}

void Lidar::write(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{

}

void Lidar::read(uint8_t reg_addr, uint8_t * data_bytes, uint16_t num_bytes)
{

}


