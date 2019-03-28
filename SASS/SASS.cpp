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

/* Board-specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Sensors/Radar.h>
#include <Sources/OC/Classifier.h>
#include <Sources/TLC/Scheduler.h>
#include <Sources/Directions.h>

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
    while(1)
    {
        printf("Print thread running.\n");
        Task_sleep(500);
    }
}

void *sass_init(void *args)
{
//    printf("Setting reference distance north.\n");
//    Classifier::set_reference_distance(Directions::NORTH);
//    printf("Set reference distance north.\n");
//    printf("Setting reference distance east.\n");
//    Classifier::set_reference_distance(Directions::EAST);
//    printf("Set reference distance east.\n");

    GPIO_setCallback(Board_GPIO_BUTTON0, Classifier::callback_hwi);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_setCallback(Board_GPIO_BUTTON1, Classifier::callback_hwi);
    GPIO_enableInt(Board_GPIO_BUTTON1);
    GPIO_setCallback(Board_GPIO_MMW1, Classifier::callback_hwi);
    GPIO_enableInt(Board_GPIO_MMW1);
    GPIO_setCallback(Board_GPIO_MMW2, Classifier::callback_hwi);
    GPIO_enableInt(Board_GPIO_MMW2);

    pthread_t           scheduler_handle;
    pthread_attr_t      scheduler_attrs;
    struct sched_param  scheduler_priParam;
    int                 retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&scheduler_attrs);

    /* Set priority, detach state, and stack size attributes */
    scheduler_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&scheduler_attrs, &scheduler_priParam);
    retc |= pthread_attr_setdetachstate(&scheduler_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&scheduler_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&scheduler_handle, &scheduler_attrs, Scheduler::scheduler_thread, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    pthread_t           watchman_handle;
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

    retc = pthread_create(&watchman_handle, &watchman_attrs, Classifier::watchman, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    Directions north = Directions::NORTH;
    Directions east = Directions::EAST;

    pthread_t           classf_n_handle;
    pthread_attr_t      classf_n_attrs;
    struct sched_param  classf_n_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&classf_n_attrs);

    /* Set priority, detach state, and stack size attributes */
    classf_n_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&classf_n_attrs, &classf_n_priParam);
    retc |= pthread_attr_setdetachstate(&classf_n_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&classf_n_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&classf_n_handle, &classf_n_attrs, Classifier::classifier_thread, (void *)north);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    pthread_t           classf_e_handle;
    pthread_attr_t      classf_e_attrs;
    struct sched_param  classf_e_priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&classf_e_attrs);

    /* Set priority, detach state, and stack size attributes */
    classf_e_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&classf_e_attrs, &classf_e_priParam);
    retc |= pthread_attr_setdetachstate(&classf_e_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&classf_e_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&classf_e_handle, &classf_e_attrs, Classifier::classifier_thread, (void *)east);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
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

    pthread_t           init_thr_handle;
    pthread_attr_t      init_thr_attrs;
    struct sched_param  init_thr_priParam;
    int                 retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&init_thr_attrs);

    /* Set priority, detach state, and stack size attributes */
    init_thr_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&init_thr_attrs, &init_thr_priParam);
    retc |= pthread_attr_setdetachstate(&init_thr_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&init_thr_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    retc = pthread_create(&init_thr_handle, &init_thr_attrs, sass_init, NULL);
    if (retc) {
        /* pthread_create() failed */
        // TODO: Throw exception
        while (1) {}
    }

    BIOS_start();    /* does not return */
    return(0);
}
