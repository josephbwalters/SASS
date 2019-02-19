#define __MSP432P401R__

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

#include <pthread.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#include <Board.h>

#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Sensors/Radar.h>
#include <Sources/Utils/Logger.h>
#include <Sources/Directions.h>

using namespace sources::utils;
using namespace sources::llha::sensors;
using namespace sources::llha::lights;

// Stack size for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

void *demoThread(void *args)
{
    Lights lights;
    Lidar* lidar_north = Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);

    while(1)
    {
        uint16_t dist = lidar_north->get_distance();
        Logger::print_value((String)"Distance is", dist);

        // RED: P7.4
        // YELLOW: P7.6

        if(dist < 200)
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);

            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN7);
            //lights.set_yellow(Directions::NORTH);
        }
        else
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);

            GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);
            //lights.set_red(Directions::NORTH);
        }
    }

    return 0;
}

void *mosfetToggleThread(void *args)
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

void *printThread(void *args)
{
    Logger::print((String)"Print thread running.");
}

int main()
{
    Board_initGeneral();

    // Initialize GPIO pins
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN3);
    Lights::init();

    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, Radar::radarTestThread, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    pthread_t           print_thread;
    pthread_attr_t      print_attrs;
    struct sched_param  print_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&print_attrs);

    /* Set priority, detach state, and stack size attributes */
    print_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&print_attrs, &print_priParam);
    retc |= pthread_attr_setdetachstate(&print_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&print_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&print_thread, &attrs, printThread, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    BIOS_start();    /* does not return */
    return(0);
}
