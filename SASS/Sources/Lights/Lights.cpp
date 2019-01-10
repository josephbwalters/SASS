// For easy access to drivers for pins
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Our custom headers
#include <../Headers/Directions.h>
#include <../Headers/Commands.h>

class Lights
{
    Commands command;
    Directions direction;
    bool panic_flag = false;
    bool waiting = true;

    static void control(Commands c, Directions d)
    {
        command = c;
        direction = d;
    }

    void stop(Directions direction)
    {
        if (direction == Directions::NORTH)
        {
            GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN4);
        }
        else if (direction == Directions::EAST)
        {
            GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
        }
    }

    void all_stop()
    {
        stop(Directions::NORTH);
        stop(Directions::EAST);
    }

    void panic()
    {
        panic_flag = true;
        all_stop();
    }

    void proceed(Directions direction)
    {
        if (direction == Directions::NORTH)
        {
            GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN6);
        }
        else if (direction == Directions::EAST)
        {
            GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN7);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
        }
    }

    void wait()
    {
        // Set P7.4 and P7.5 as outputs
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
        GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN5);

        // Toggle red lights
        GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN4);
        GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN5);

        // TODO: Timer based delay. (Use 1 second)
        for(i=500000; i>0; i--);
    }

    void safe()
    {
        panic_flag = false;
    }

    void light_thread(void)
    {
        if (panic_flag)
        {
            all_stop();
        }
        
        if (command == Commands::PANIC)
        {
            panic();
        }
        else if (command == Commands::SAFE)
        {
            safe();
        }
        else if (command == Commands::PROCEED)
        {
            proceed(direction);
        }
        else if (command == Commands::STOP)
        {
            stop(direction);
        }
        else if (command == Commands::WAIT)
        {
            wait();
        }
        else
        {
            ; // TODO: Handle invalid command failure
        }
    }
};


