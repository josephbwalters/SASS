/*
 * SASS.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif

/* Standard headers */
#include <stdio.h>

/* XDC module headers */
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* POSIX thread support */
#include <pthread.h>

/* BIOS module headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Device drivers for GPIO */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/drivers/GPIO.h>

/* Board-specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/Control.h>
#include <Sources/Directions.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Sensors/Radar.h>
#include <Sources/OC/Classifier.h>
#include <Sources/TLC/Scheduler.h>

// Stack sizes for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

using namespace sources::llha::sensors;
using namespace sources::llha::lights;
using namespace sources::tlc;

/**
    Simple thread that prints to  the console
*/
void *test_print_thread(void *args)
{
    while (true)
    {
        printf("Print thread running.\n");
        Task_sleep(500);
    }
}

void *sass_init_thread(void *args)
{
    GPIO_setCallback(Board_GPIO_BUTTON0, Classifier::emergency_hwi_callback);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_setCallback(Board_GPIO_BUTTON1, Classifier::emergency_hwi_callback);
    GPIO_enableInt(Board_GPIO_BUTTON1);
    GPIO_setCallback(Board_GPIO_MMW1, Classifier::emergency_hwi_callback);
    GPIO_enableInt(Board_GPIO_MMW1);
    GPIO_setCallback(Board_GPIO_MMW2, Classifier::emergency_hwi_callback);
    GPIO_enableInt(Board_GPIO_MMW2);

    pthread_t           scheduler_handle;
    pthread_attr_t      scheduler_attrs;
    struct sched_param  scheduler_priParam;
    int                 thread_error;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&scheduler_attrs);

    /* Set priority, detach state, and stack size attributes */
    scheduler_priParam.sched_priority = 1;
    thread_error = pthread_attr_setschedparam(&scheduler_attrs, &scheduler_priParam);
    thread_error |= pthread_attr_setdetachstate(&scheduler_attrs, PTHREAD_CREATE_DETACHED);
    thread_error |= pthread_attr_setstacksize(&scheduler_attrs, STACK_SIZE_LARGE);
    if (thread_error) {
        /* failed to set attributes */
        // TODO: Throw exception
    }

    thread_error = pthread_create(&scheduler_handle, &scheduler_attrs, Scheduler::scheduler_thread, NULL);
    if (thread_error) {
        /* pthread_create() failed */
        // TODO: Throw exception
    }

    // NOTE: May not need watchman thread
    //    pthread_t           watchman_handle;
    //    pthread_attr_t      watchman_attrs;
    //    struct sched_param  watchman_priParam;
    //
    //    /* Initialize the attributes structure with default values */
    //    pthread_attr_init(&watchman_attrs);
    //
    //    /* Set priority, detach state, and stack size attributes */
    //    watchman_priParam.sched_priority = 1;
    //    thread_error = pthread_attr_setschedparam(&watchman_attrs, &watchman_priParam);
    //    thread_error |= pthread_attr_setdetachstate(&watchman_attrs, PTHREAD_CREATE_DETACHED);
    //    thread_error |= pthread_attr_setstacksize(&watchman_attrs, STACK_SIZE_LARGE);
    //    if (thread_error) {
    //        /* failed to set attributes */
    //        // TODO: Throw exception
    //        while (true) {}
    //    }
    //
    //    thread_error = pthread_create(&watchman_handle, &watchman_attrs, Classifier::watchman_thread, NULL);
    //    if (thread_error) {
    //        /* pthread_create() failed */
    //        // TODO: Throw exception
    //        while (true) {}
    //    }

    pthread_t           classifier_n_handle;
    pthread_attr_t      classifier_n_attrs;
    struct sched_param  classifier_n_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&classifier_n_attrs);

    /* Set priority, detach state, and stack size attributes */
    classifier_n_priParam.sched_priority = 1;
    thread_error = pthread_attr_setschedparam(&classifier_n_attrs, &classifier_n_priParam);
    thread_error |= pthread_attr_setdetachstate(&classifier_n_attrs, PTHREAD_CREATE_DETACHED);
    thread_error |= pthread_attr_setstacksize(&classifier_n_attrs, STACK_SIZE_LARGE);
    if (thread_error) {
        /* failed to set attributes */
        // TODO: Throw exception
    }

    thread_error = pthread_create(&classifier_n_handle, &classifier_n_attrs, Classifier::classifier_thread,
                          (void *)Directions::NORTH);
    if (thread_error) {
        /* pthread_create() failed */
        // TODO: Throw exception
    }

    pthread_t           classifier_e_handle;
    pthread_attr_t      classifier_e_attrs;
    struct sched_param  classifier_e_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&classifier_e_attrs);

    /* Set priority, detach state, and stack size attributes */
    classifier_e_priParam.sched_priority = 1;
    thread_error = pthread_attr_setschedparam(&classifier_e_attrs, &classifier_e_priParam);
    thread_error |= pthread_attr_setdetachstate(&classifier_e_attrs, PTHREAD_CREATE_DETACHED);
    thread_error |= pthread_attr_setstacksize(&classifier_e_attrs, STACK_SIZE_LARGE);
    if (thread_error) {
        /* failed to set attributes */
        // TODO: Throw exception
    }

    thread_error = pthread_create(&classifier_e_handle, &classifier_e_attrs, Classifier::classifier_thread,
                          (void *)Directions::EAST);
    if (thread_error) {
        /* pthread_create() failed */
        // TODO: Throw exception
    }

    pthread_exit(NULL);
}

/**
    Initial control loop for device
    Starts threads and gives control of the overall device to those threads.
*/
int main()
{
    Board_initGeneral();

    GPIO_init();
    Lights::init();

    pthread_t           sass_init_handle;
    pthread_attr_t      sass_init_attrs;
    struct sched_param  sass_init_priParam;
    int                 thread_error;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&sass_init_attrs);

    /* Set priority, detach state, and stack size attributes */
    sass_init_priParam.sched_priority = 1;
    thread_error = pthread_attr_setschedparam(&sass_init_attrs, &sass_init_priParam);
    thread_error |= pthread_attr_setdetachstate(&sass_init_attrs, PTHREAD_CREATE_DETACHED);
    thread_error |= pthread_attr_setstacksize(&sass_init_attrs, STACK_SIZE_LARGE);
    if (thread_error) {
        /* failed to set attributes */
        // TODO: Throw exception
    }

    thread_error = pthread_create(&sass_init_handle, &sass_init_attrs, sass_init_thread, NULL);
    if (thread_error) {
        /* pthread_create() failed */
        // TODO: Throw exception
    }

    BIOS_start();    /* does not return */
    return(0);
}
