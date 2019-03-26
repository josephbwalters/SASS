/*
 * Classifier.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#define __MSP432P401R__

/* Standard headers */
#include <stdio.h>

/* POSIX thread support */
#include <pthread.h>

/* System headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Idle.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/Timer.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>

/* Board-specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/OC/Classifier.h>
#include <Sources/OC/Vehicle.h>

// Stack sizes for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

ti_sysbios_knl_Semaphore_Handle sw1_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle sw2_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle mmw1_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle mmw2_sem = Semaphore_create(0, NULL, NULL);

// Initialize LiDAR instances to null for multiton pattern
sources::oc::Classifier* sources::oc::Classifier::classifier_north = nullptr;
sources::oc::Classifier* sources::oc::Classifier::classifier_east = nullptr;
sources::oc::Classifier* sources::oc::Classifier::classifier_south = nullptr;
sources::oc::Classifier* sources::oc::Classifier::classifier_west = nullptr;

using namespace sources;
using namespace sources::oc;

Classifier::Classifier(Directions direction) : m_direction(direction)
{
    switch (m_direction)
    {
    case Directions::NORTH:
        m_lidar = Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);
        m_radar = Radar::get_instance(RadarInstanceType::RADAR_NORTH);
        printf("Initialized classifier north with sensors.\n");
        break;
    case Directions::EAST:
        m_lidar = Lidar::get_instance(LidarInstanceType::LIDAR_EAST);
        m_radar = Radar::get_instance(RadarInstanceType::RADAR_EAST);
        printf("Initialized classifier east with sensors.\n");
        break;
    case Directions::SOUTH:
        m_lidar = Lidar::get_instance(LidarInstanceType::LIDAR_SOUTH);
        m_radar = Radar::get_instance(RadarInstanceType::RADAR_SOUTH);
        break;
    case Directions::WEST:
        m_lidar = Lidar::get_instance(LidarInstanceType::LIDAR_WEST);
        m_radar = Radar::get_instance(RadarInstanceType::RADAR_WEST);
        break;
    default:
        // TODO: Throw exception
        printf("Did not initialize sensors in classifier.\n");
    };
}

Classifier::~Classifier()
{

}

Classifier* Classifier::get_instance(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        if (classifier_north == nullptr)
        {
            classifier_north = new Classifier(Directions::NORTH);
            printf("Created Classifier north object.\n");
        }
        return classifier_north;

    case Directions::EAST:
        if (classifier_east == nullptr)
        {
            classifier_east = new Classifier(Directions::EAST);
            printf("Created Classifier east object.\n");
        }
        return classifier_east;

    case Directions::SOUTH:
        if (classifier_south == nullptr)
        {
            classifier_south = new Classifier(Directions::SOUTH);
            printf("Created Classifier south object.\n");
        }
        return classifier_south;

    case Directions::WEST:
        if (classifier_west == nullptr)
        {
            classifier_west = new Classifier(Directions::WEST);
            printf("Created Classifier west object.\n");
        }
        return classifier_west;

    default:
        // TODO: Throw exception
        return nullptr;
    };
}

uint8_t Classifier::track()
{
    uint16_t dist = m_lidar->get_distance();
    // uint16_t dist = 60; // 100

    if (dist < 50)
    {
        return 1;
    }

    return 0;
}

void *Classifier::classifier_thread(void *args)
{
    Directions direction;
    direction = static_cast<Directions>((int)args);

    printf("[Classifier %d] Created classifier thread.\n", direction);

    Classifier* classifier = Classifier::get_instance(direction);
    Scheduler* scheduler = Scheduler::get_instance();

    while (1)
    {
        Vehicle current_vehicle(direction);
        printf("[Classifier %d] Vehicle detected.\n", direction);
        uint8_t status = 0;

        while (status == 0)
        {
            printf("[Classifier %d] Tracking vehicle...\n", direction);

            status = classifier->track();

            // DEBUGGING ONLY
//            if(scheduler->get_vehicle_queue()->empty())
//            {
//                status = 1;
//            }

            Task_sleep(500);
            // Task_yield();
        }

        // Send vehicle to queue
        scheduler->get_vehicle_queue()->push_back(current_vehicle);
        printf("[Classifier %d] Vehicle stopped and added to queue.\n", direction);

        status = 1;

        // PRODUCTION ONLY
        while (status == 1)
        {
            status = classifier->track();
            Task_sleep(500);
            // Task_yield();
        }

        Task_sleep(500);
        // Task_yield();
    }

    printf("[Classifier %d] Exiting classifier thread...\n", direction);

    pthread_exit(NULL);
}

void *Classifier::watchman(void *args)
{
    pthread_t           classf_north_thread;
    pthread_t           classf_east_thread;
    pthread_attr_t      classf_attrs;
    struct sched_param  classf_priParam;
    int                 retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&classf_attrs);

    /* Set priority, detach state, and stack size attributes */
    classf_priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&classf_attrs, &classf_priParam);
    retc |= pthread_attr_setdetachstate(&classf_attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&classf_attrs, STACK_SIZE_LARGE);
    if (retc) {
        /* failed to set attributes */
        // TODO: Throw exception
        while (1) {}
    }

    Directions north = Directions::NORTH;
    Directions east = Directions::EAST;

    while(1)
    {
        bool sw1_status = Semaphore_pend(sw1_sem, BIOS_NO_WAIT);
        bool sw2_status = Semaphore_pend(sw2_sem, BIOS_NO_WAIT);
        bool mmw1_status = Semaphore_pend(mmw1_sem, BIOS_NO_WAIT);
        bool mmw2_status = Semaphore_pend(mmw2_sem, BIOS_NO_WAIT);

        if (sw1_status == true)
        {
            // Create classifier thread
            // printf("Creating Classifier north thread...\n");

            retc = pthread_create(&classf_north_thread, &classf_attrs, Classifier::classifier_thread, (void *)north);
            if (retc) {
                /* pthread_create() failed */
                // TODO: Throw exception
                while (1) {}
            }
        }

        if (sw2_status == true)
        {
            // Create classifier thread
            // printf("Creating Classifier east thread...\n");

            retc = pthread_create(&classf_east_thread, &classf_attrs, Classifier::classifier_thread, (void *)east);
            if (retc) {
                /* pthread_create() failed */
                // TODO: Throw exception
                while (1) {}
            }
        }

        if (mmw1_status == true)
        {
            // Create classifier thread
            printf("Creating Classifier north thread...\n");

            retc = pthread_create(&classf_north_thread, &classf_attrs, Classifier::classifier_thread, (void *)north);
            if (retc) {
                /* pthread_create() failed */
                // TODO: Throw exception
                while (1) {}
            }
        }

        if (mmw2_status == true)
        {
            // Create classifier thread
            printf("Creating Classifier north thread...\n");

            retc = pthread_create(&classf_east_thread, &classf_attrs, Classifier::classifier_thread, (void *)east);
            if (retc) {
                /* pthread_create() failed */
                // TODO: Throw exception
                while (1) {}
            }
        }

        Idle_run();
        Task_yield();
    }
}

void Classifier::callback_hwi(uint_least8_t index)
{
    switch (index)
    {
    case 0:
        Semaphore_post(sw1_sem);
        break;
    case 1:
        Semaphore_post(sw2_sem);
        break;
    case 13:
        Semaphore_post(mmw1_sem);
        break;
    case 14:
        Semaphore_post(mmw2_sem);
        break;
    default:
        // TODO: Throw exception
    };
}
