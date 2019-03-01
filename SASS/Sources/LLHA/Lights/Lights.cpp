#define __MSP432P401R__

/* System headers */ 
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* Custom headers */
#include <Sources/LLHA/Lights/Lights.h>

using namespace sources;
using namespace sources::llha::lights;

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
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    }
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
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    }
}

void Lights::schedule(Directions direction)
{

}

void Lights::set_all_red()
{
    set_red(Directions::NORTH);
    set_red(Directions::EAST);
}

void Lights::wait()
{
    // Toggle red lights
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);

    // TODO: Timer based delay. (Use 1 second)
    for (int i = 500000; i > 0; i--);
}

void Lights::panic()
{
    // TODO: Determine how to implement panic flag in static method
    // m_panic_flag = true;
    set_all_red();
}

void Lights::safe()
{
    // TODO: Determine how to implement panic flag in static method
    // m_panic_flag = false;
}

void *Lights::light_thread(void *args)
{
    if (m_panic_flag)
    {
        set_all_red();
    }
}

/**
    Thread to toggle all of our mosfets
*/
void *Lights::mosfetToggleThread(void *args)
{
    // GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);

    bool is_on = 0;

    while(1)
    {
        for(int i = 0; i < 1000000; i++)
        {
            // Delay
        }

        if(is_on == 0)
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
            is_on = 1;
        }
        else
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
            is_on = 0;
        }
    }
}
