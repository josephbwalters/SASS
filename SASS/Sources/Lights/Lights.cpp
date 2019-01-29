#define __MSP432P401R__

// For easy access to drivers for pins
// #include <gpio.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#include "Lights.h"

using namespace sources;
using namespace sources::lights;

Lights::Lights()
{
    bool panic_flag = false;
    bool waiting = true;
}

void Lights::control(Commands c, Directions d)
{
    m_command = c;
    m_direction = d;
}

void Lights::stop(Directions direction)
{
    if (m_direction == Directions::NORTH)
    {
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
    }
    else if (m_direction == Directions::EAST)
    {
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
    }
}

void Lights::all_stop()
{
    stop(Directions::NORTH);
    stop(Directions::EAST);
}

void Lights::panic()
{
    m_panic_flag = true;
    all_stop();
}

void Lights::proceed(Directions direction)
{
    if (m_direction == Directions::NORTH)
    {
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
    }
    else if (m_direction == Directions::EAST)
    {
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
    }
}

void Lights::wait()
{
    // Set P7.4 and P7.5 as outputs
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);

    // Toggle red lights
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);

    // TODO: Timer based delay. (Use 1 second)
    for (int i = 500000; i > 0; i--)
        ;
}

void Lights::safe()
{
    m_panic_flag = false;
}

void Lights::light_thread(void)
{
    if (m_panic_flag)
    {
        all_stop();
    }

    if (m_command == Commands::PANIC)
    {
        panic();
    }
    else if (m_command == Commands::SAFE)
    {
        safe();
    }
    else if (m_command == Commands::PROCEED)
    {
        proceed(m_direction);
    }
    else if (m_command == Commands::STOP)
    {
        stop(m_direction);
    }
    else if (m_command == Commands::WAIT)
    {
        wait();
    }
    else
    {
        ; // TODO: Handle invalid command failure
    }
}

