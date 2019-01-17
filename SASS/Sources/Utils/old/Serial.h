#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdarg.h>
#include <stdint.h>

namespace sources
{
namespace utils
{


class Serial
{
private:

protected:

public:
    static void println(char* str);
    static void println(uint16_t val);
};


}} // sources::utils


#endif //SERIAL_H_
