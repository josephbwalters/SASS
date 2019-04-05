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

void Lights::turn_on_red()
{
    turn_on_red(Directions::NORTH);
    turn_on_red(Directions::EAST);
}

void Lights::turn_on_red(Directions direction)
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
    };
}

void Lights::turn_on_yellow()
{
    turn_on_yellow(Directions::NORTH);
    turn_on_yellow(Directions::EAST);
}

void Lights::turn_on_yellow(Directions direction)
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
    };
}

void Lights::turn_off_red()
{
    turn_off_red(Directions::NORTH);
    turn_off_red(Directions::EAST);
}

void Lights::turn_off_red(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
        break;
    case Directions::EAST:
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

void Lights::turn_off_yellow()
{
    turn_off_yellow(Directions::NORTH);
    turn_off_yellow(Directions::EAST);
}

void Lights::turn_off_yellow(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
        break;
    case Directions::EAST:
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

void Lights::toggle_yellow(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        turn_off_red(Directions::NORTH);

        if (GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN6))
        {
            turn_off_yellow(Directions::NORTH);
        }
        else
        {
            turn_on_yellow(Directions::NORTH);
        }
        break;
    case Directions::EAST:
        turn_off_red(Directions::EAST);

        if (GPIO_getInputPinValue(GPIO_PORT_P7, GPIO_PIN7))
        {
            turn_off_yellow(Directions::EAST);
        }
        else
        {
            turn_on_yellow(Directions::EAST);
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

void Lights::turn_on()
{
    turn_on(Directions::NORTH);
    turn_on(Directions::EAST);
}

void Lights::turn_on(Directions direction)
{
    turn_on_red(direction);
    turn_on_yellow(direction);
}

void Lights::turn_off()
{
    turn_off(Directions::NORTH);
    turn_off(Directions::EAST);
}

void Lights::turn_off(Directions direction)
{
    turn_off_red(direction);
    turn_off_yellow(direction);
}

void Lights::set_all_red()
{
    turn_on_red(Directions::NORTH);
    turn_off_yellow(Directions::NORTH);
    turn_on_red(Directions::EAST);
    turn_off_yellow(Directions::EAST);
}

void Lights::schedule(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        turn_on_yellow(Directions::NORTH);
        turn_off_red(Directions::NORTH);
        turn_on_red(Directions::EAST);
        turn_off_yellow(Directions::EAST);
        turn_on_red(Directions::SOUTH);
        turn_off_yellow(Directions::SOUTH);
        turn_on_red(Directions::WEST);
        turn_off_yellow(Directions::WEST);
        break;
    case Directions::EAST:
        turn_on_yellow(Directions::EAST);
        turn_off_red(Directions::EAST);
        turn_on_red(Directions::SOUTH);
        turn_off_yellow(Directions::SOUTH);
        turn_on_red(Directions::WEST);
        turn_off_yellow(Directions::WEST);
        turn_on_red(Directions::NORTH);
        turn_off_yellow(Directions::NORTH);
        break;
    case Directions::SOUTH:
        turn_on_yellow(Directions::SOUTH);
        turn_off_red(Directions::SOUTH);
        turn_on_red(Directions::WEST);
        turn_off_yellow(Directions::WEST);
        turn_on_red(Directions::NORTH);
        turn_off_yellow(Directions::NORTH);
        turn_on_red(Directions::EAST);
        turn_off_yellow(Directions::EAST);
        break;
    case Directions::WEST:
        turn_on_yellow(Directions::WEST);
        turn_off_red(Directions::WEST);
        turn_on_red(Directions::NORTH);
        turn_off_yellow(Directions::NORTH);
        turn_on_red(Directions::EAST);
        turn_off_yellow(Directions::EAST);
        turn_on_red(Directions::SOUTH);
        turn_off_yellow(Directions::SOUTH);
        break;
    default:
        // TODO: Throw exception
    };
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
