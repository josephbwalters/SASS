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

void Lights::init()
{
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN7);
}

void Lights::set_red(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
        break;
    case Directions::EAST:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
        break;
    }
}

void Lights::set_all_red()
{
    set_red(Directions::NORTH);
    set_red(Directions::EAST);
}

void Lights::panic()
{
    m_panic_flag = true;
    set_all_red();
}

void Lights::set_yellow(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
        break;
    case Directions::EAST:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
        break;
    }
}

void Lights::wait()
{
    // Toggle red lights
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);

    // TODO: Timer based delay. (Use 1 second)
    for (int i = 500000; i > 0; i--);
}

void Lights::safe()
{
    m_panic_flag = false;
}

void Lights::light_thread(void)
{
    if (m_panic_flag)
    {
        set_all_red();
    }

    switch (m_command)
    {
    case Commands::PANIC:
        panic();
        break;
    case Commands::SAFE:
        safe();
        break;
    case Commands::PROCEED:
        set_yellow(m_direction);
        break;
    case Commands::STOP:
        set_red(m_direction);
        break;
    case Commands::WAIT:
        wait();
        break;
    default:
        ; // Catch/Throw an error
    }
}

