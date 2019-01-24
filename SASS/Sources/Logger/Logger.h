#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdint.h>
#include <xdc/std.h>

namespace sources
{
namespace logger
{


class Logger
{
public:
    static void print(String statement);
    static void print_value(String statement, int value);
    static void print_buffer(String statement, uint8_t * buffer);
};


}} // sources::logger



#endif // LOGGER_H_
