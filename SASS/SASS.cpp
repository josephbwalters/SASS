/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

#include <pthread.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#include <Board.h>

#include <Sources/Lights/Lights.h>
#include <Sources/Sensors/Lidar.h>
#include <Sources/Logger/Logger.h>
#include <Sources/Directions.h>

using namespace sources::logger;
using namespace sources::sensors;
using namespace sources::lights;

// Stack size for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

void *demoThread(void *Uarg0)
{
    Lights lights;
    Lidar*  lidar_north = Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);

    while(1)
    {
        uint16_t dist = lidar_north->get_distance();
        Logger::print_value((String)"Distance is", dist);

        // RED: P7.4
        // YELLOW: P7.6

        if(dist < 200)
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);
            lights.set_yellow(Directions::NORTH);
        }
        else
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);
            lights.set_red(Directions::NORTH);
        }
    }

    /*bool is_on = 0;

    while(1)
    {
        for(int i = 0; i < 1000000; i++)
        {
            // Delay
        }

        if(is_on == 0)
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);
            is_on = 1;
        }
        else
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);
            is_on = 0;
        }
        // GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    }*/

    return 0;
}

int main()
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    Board_initGeneral();

    // Initialize GPIO pins
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN3);
//    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN4);
    Lights::init();

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, STACK_SIZE_LARGE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, demoThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    BIOS_start();    /* does not return */
    return(0);
}
