/*
 * SASS.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#define __MSP432P401R__
// #define DEBUG

/* Standard headers */
#include <stdio.h>

/* XDC module headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

/* POSIX thread support */
#include <pthread.h>

/* BIOS module headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Device drivers for GPIO */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/drivers/GPIO.h>

/* Board specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Sensors/Radar.h>
#include <Sources/OC/Classifier.h>
#include <Sources/TLC/Scheduler.h>
#include <Sources/Directions.h>

using namespace sources::llha::sensors;
using namespace sources::llha::lights;
using namespace sources::tlc;

// Stack sizes for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

/**
    Simple thread that prints to  the console
*/
void *printThread(void *args)
{
    while(1)
    {
        printf("Print thread running.\n");
        Task_yield();
    }
}

/**
    Initial control loop for device
    Starts threads and gives control of the overall device to those threads.
*/
int main()
{
    Board_initGeneral();

    Lights::init();

    // Call GPIO driver init function
    GPIO_init();

    // Turn on user LED
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    // Install Button callback
    GPIO_setCallback(Board_GPIO_BUTTON0, Classifier::callback_hwi);

    // Enable interrupts
    GPIO_enableInt(Board_GPIO_BUTTON0);

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

    retc = pthread_create(&thread, &attrs, Scheduler::scheduler_thread, NULL);
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

    retc = pthread_create(&print_thread, &print_attrs, printThread, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    pthread_t           watchman_thread;
    pthread_attr_t      watchman_attrs;
    struct sched_param  watchman_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&watchman_attrs);

    /* Set priority, detach state, and stack size attributes */
    watchman_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&watchman_attrs, &watchman_priParam);
    retc |= pthread_attr_setdetachstate(&watchman_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&watchman_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&watchman_thread, &watchman_attrs, Classifier::watchman, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    BIOS_start();    /* does not return */
    return(0);
}
