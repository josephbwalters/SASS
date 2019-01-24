#include <xdc/runtime/System.h>
#include <Sources/Logger/Logger.h>

using namespace sources::logger;

void Logger::print(String statement)
{
    System_printf("%s\n", statement);
    System_flush();
}

void Logger::print_value(String statement, int value)
{
    System_printf("%s: %d\n", statement, value);
    System_flush();
}

void Logger::print_buffer(String statement, uint8_t * buffer)
{
    System_printf("%s\n", statement);
    int buffer_length = sizeof(buffer)/sizeof(buffer[0]);
    for(int buffer_index = buffer_length-1; buffer_index >= 0; buffer_index--)
    {
        System_printf("%02x ", buffer[buffer_index]);
    }
    System_printf("\n");
    System_flush();
}



