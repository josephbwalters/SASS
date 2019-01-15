#ifndef WIRE_H_
#define WIRE_H_

#include <stdint.h>

namespace sources
{
namespace utils
{


class Wire
{
private:

protected:

public:
    static void beginTransmission(int lidarliteAddress);
    static void endTransmission();
    static void endTransmission(bool true_or_false); // false means perform repeated start

    static void write(uint8_t value);         // - a value to send as a single byte
    static void write(char *str);        // - a string to send as a series of bytes
    static void write(uint8_t *data, int length);  // - an array of data to send as bytes

    // dataBytes, (int) numBytes
    // uint8_t * dataBytes

    static uint8_t read();

    static void requestFrom(int lidarliteAddress, int numBytes);

    static bool available();
};


}} // sources::utils


#endif // WIRE_H_
