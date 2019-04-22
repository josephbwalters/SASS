/*
 * Lights.cpp
 * Created by: Joseph Walters, Trent Sellers
 */

#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif

/* System headers */ 
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/sysbios/knl/Task.h>

/* SASS-specific headers */
#include <Sources/LLHA/Lights/Lights.h>

using namespace sources;
using namespace sources::llha::lights;

void Lights::init()
{
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN7);

    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
}

void Lights::set_red(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
        break;
    case Directions::EAST:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
        break;
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    };
}

void Lights::set_yellow(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
        break;
    case Directions::EAST:
        GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
        break;
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    };
}

void Lights::schedule(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        set_yellow(Directions::NORTH);
        set_red(Directions::EAST);
        set_red(Directions::SOUTH);
        set_red(Directions::WEST);
        break;
    case Directions::EAST:
        set_yellow(Directions::EAST);
        set_red(Directions::SOUTH);
        set_red(Directions::WEST);
        set_red(Directions::NORTH);
        break;
    case Directions::SOUTH:
        set_yellow(Directions::SOUTH);
        set_red(Directions::WEST);
        set_red(Directions::NORTH);
        set_red(Directions::EAST);
        break;
    case Directions::WEST:
        set_yellow(Directions::WEST);
        set_red(Directions::NORTH);
        set_red(Directions::EAST);
        set_red(Directions::SOUTH);
        break;
    default:
        // TODO: Throw exception
    };
}

void Lights::toggle_yellow(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN6))
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
        }
        else
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
        }
        break;
    case Directions::EAST:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);

        if (GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN7))
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
        }
        else
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
        }
        break;
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    };
}

void Lights::set_all_red()
{
    set_red(Directions::NORTH);
    set_red(Directions::EAST);
}

void Lights::turn_off()
{
    turn_off(Directions::NORTH);
    turn_off(Directions::EAST);
}

void Lights::turn_off(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
        break;
    case Directions::EAST:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
        break;
    case Directions::SOUTH:
        // Future: Add functionality for south direction
        break;
    case Directions::WEST:
        // Future: Add functionality for west direction
        break;
    default:
        // TODO: Throw exception
    };
}

void Lights::set_all()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
}

/**
    Thread to toggle all of our mosfets
*/
void *Lights::mosfet_toggle_thread(void *args)
{
    // GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);

    bool is_on = 0;

    while (true)
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

        Task_yield();
    }
}
