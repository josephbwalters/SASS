#include <stdint.h>
#include "Wire.h"

using namespace std;
using namespace sources;
using namespace sources::utils;


void Wire::beginTransmission(int lidarliteAddress)
{
    ;
}

int Wire::endTransmission()
{
    ;
}

// false means perform repeated start
int Wire::endTransmission(bool true_or_false)
{
    ;
}

// - a value to send as a single byte
void Wire::write(uint8_t value)
{
    ;
}

// - a string to send as a series of bytes
void Wire::write(char* string)
{
    ;
}

// - an array of data to send as bytes
void Wire::write(uint8_t *data, int length)
{
    ;
}

// dataBytes, (int) numBytes
// uint8_t * dataBytes

uint8_t Wire::read()
{
    return 0;
}

void Wire::requestFrom(int lidarliteAddress, int numBytes)
{
    ;
}

bool Wire::available()
{
    return false;
}



