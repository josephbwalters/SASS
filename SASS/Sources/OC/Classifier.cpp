/*
 * Classifier.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif

/* Standard headers */
#include <stdio.h>

/* POSIX thread support */
#include <pthread.h>

/* System headers */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/timer32.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Idle.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

/* Board-specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/Control.h>
#include <Sources/OC/Classifier.h>
#include <Sources/OC/Vehicle.h>

// Pass requirement for verification of vehicle presence
#define PASS 2

// Check requirement for verification of intersection safety
#define CHECKS_NEEDED 2

// Stack sizes for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

#define OFFSET 20

ti_sysbios_knl_Semaphore_Handle sw1_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle sw2_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle mmw1_sem = Semaphore_create(0, NULL, NULL);
ti_sysbios_knl_Semaphore_Handle mmw2_sem = Semaphore_create(0, NULL, NULL);

// Initialize reference distances
uint16_t sources::oc::Classifier::ref_dist_north = 0;
uint16_t sources::oc::Classifier::ref_dist_east = 0;
uint16_t sources::oc::Classifier::ref_dist_south = 0;
uint16_t sources::oc::Classifier::ref_dist_west = 0;

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
        m_lidar = Lidar::get_instance(Directions::NORTH);
        m_radar = Radar::get_instance(Directions::NORTH);
        printf("Initialized classifier north with sensors.\n");
        break;
    case Directions::EAST:
        m_lidar = Lidar::get_instance(Directions::EAST);
        m_radar = Radar::get_instance(Directions::EAST);
        printf("Initialized classifier east with sensors.\n");
        break;
    case Directions::SOUTH:
        m_lidar = Lidar::get_instance(Directions::SOUTH);
        m_radar = Radar::get_instance(Directions::SOUTH);
        break;
    case Directions::WEST:
        m_lidar = Lidar::get_instance(Directions::WEST);
        m_radar = Radar::get_instance(Directions::WEST);
        break;
    default:
        Control::get_instance()->fail_system();
        printf("Did not initialize sensors in classifier.\n");
    };

    init();
}

Classifier::~Classifier()
{

}

void Classifier::init()
{
    set_reference_distance();
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
        Control::get_instance()->fail_system();
        return nullptr;
    };
}

void Classifier::set_reference_distance()
{
    uint16_t ref_dist;
    uint16_t reading_1 = 0;
    uint16_t reading_2 = 0;
    uint16_t reading_3 = 0;

    Task_sleep(200);
    printf("Reading 1...\n");
    reading_1 = m_lidar->get_distance();
    Task_sleep(200);
    printf("Reading 2...\n");
    reading_2 = m_lidar->get_distance();
    Task_sleep(200);
    printf("Reading 3...\n");
    reading_3 = m_lidar->get_distance();
    Task_sleep(200);
    ref_dist = (reading_1 + reading_2 + reading_3) / 3;

    switch (m_direction)
    {
    case Directions::NORTH:
        sources::oc::Classifier::ref_dist_north = ref_dist;
        break;
    case Directions::EAST:
        sources::oc::Classifier::ref_dist_east = ref_dist;
        break;
    case Directions::SOUTH:
        sources::oc::Classifier::ref_dist_south = ref_dist;
        break;
    case Directions::WEST:
        sources::oc::Classifier::ref_dist_west = ref_dist;
        break;
    default:
        Control::get_instance()->fail_system();
        break;
    };
}

uint16_t Classifier::get_reference_distance()
{
    switch (m_direction)
    {
    case Directions::NORTH:
        return Classifier::ref_dist_north;
    case Directions::EAST:
        return Classifier::ref_dist_east;
    case Directions::SOUTH:
        return Classifier::ref_dist_south;
    case Directions::WEST:
        return Classifier::ref_dist_west;
    default:
        Control::get_instance()->fail_system();
        return 0;
    };
}

uint8_t Classifier::track()
{
    if (m_lidar->get_distance() < get_reference_distance() - OFFSET)
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

    while (true)
    {
        uint8_t score = 0;

        // Confirm vehicle is present
        while (score < PASS)
        {
            printf("[Classifier %d] Tracking vehicle...\n", direction);

            for (int i = 0; i < PASS; i++)
            {
                if (classifier->track() == 1)
                {
                    score++;
                }
                else
                {
                    score = 0;
                }

                Task_sleep(200);
            }
        }

        Vehicle current_vehicle(direction);
        printf("[Classifier %d] Vehicle detected.\n", direction);
        deque<Vehicle>* queue = scheduler->get_vehicle_queue();

        // Send vehicle to queue
        queue->push_back(current_vehicle);
        printf("[Classifier %d] Vehicle stopped and added to queue.\n", direction);

        bool vehicle_present = true;
        deque<Vehicle>::iterator it;

        while (vehicle_present == true)
        {
            vehicle_present = false;

            for (it = queue->begin(); it != queue->end(); it++)
            {
                if (it->get_direction() == direction)
                {
                    vehicle_present = true;
                }
            }

            Task_sleep(200);
        }

        Task_sleep(200);
    }

    printf("[Classifier %d] Exiting classifier thread...\n", direction);

    pthread_exit(NULL);
}

void *Classifier::watchman_thread(void *args)
{
    pthread_t           classifier_n_thread;
    pthread_t           classifier_e_thread;
    pthread_attr_t      classifier_attrs;
    struct sched_param  classifier_priParam;
    int                 thread_error;

    pthread_attr_init(&classifier_attrs);

    classifier_priParam.sched_priority = 1;
    thread_error = pthread_attr_setschedparam(&classifier_attrs, &classifier_priParam);
    thread_error |= pthread_attr_setdetachstate(&classifier_attrs, PTHREAD_CREATE_DETACHED);
    thread_error |= pthread_attr_setstacksize(&classifier_attrs, STACK_SIZE_LARGE);
    if (thread_error)
    {
        Control::get_instance()->fail_system();
    }

    while (true)
    {
        bool sw1_status = Semaphore_pend(sw1_sem, BIOS_NO_WAIT);
        bool sw2_status = Semaphore_pend(sw2_sem, BIOS_NO_WAIT);
        bool mmw1_status = Semaphore_pend(mmw1_sem, BIOS_NO_WAIT);
        bool mmw2_status = Semaphore_pend(mmw2_sem, BIOS_NO_WAIT);

        if (sw1_status == true)
        {
            // printf("Creating Classifier north thread...\n");

            thread_error = pthread_create(&classifier_n_thread, &classifier_attrs, Classifier::classifier_thread,
                                  (void *)Directions::NORTH);
            if (thread_error)
            {
                /* pthread_create() failed */
                Control::get_instance()->fail_system();
            }
        }

        if (sw2_status == true)
        {
            // printf("Creating Classifier east thread...\n");

            thread_error = pthread_create(&classifier_e_thread, &classifier_attrs, Classifier::classifier_thread,
                                  (void *)Directions::EAST);
            if (thread_error)
            {
                Control::get_instance()->fail_system();
            }
        }

        if (mmw1_status == true)
        {
            printf("Creating Classifier north thread...\n");

            thread_error = pthread_create(&classifier_n_thread, &classifier_attrs, Classifier::classifier_thread, (void *)Directions::NORTH);
            if (thread_error)
            {
                Control::get_instance()->fail_system();
            }
        }

        if (mmw2_status == true)
        {
            printf("Creating Classifier east thread...\n");

            thread_error = pthread_create(&classifier_e_thread, &classifier_attrs, Classifier::classifier_thread, (void *)Directions::EAST);
            if (thread_error)
            {
                Control::get_instance()->fail_system();
            }
        }

        Idle_run();
        Task_sleep(200);
    }
}

void Classifier::classifier_hwi_callback(uint_least8_t index)
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
        Control::get_instance()->fail_system();
    };
}

void Classifier::emergency_hwi_callback(uint_least8_t index)
{
    bool unsafe = true;
    uint8_t i;
    uint8_t score = 0;
    
    uint8_t gpio_map[CHECKS_NEEDED] = {Board_GPIO_MMW1, Board_GPIO_MMW2};
    for (i = 0; i < CHECKS_NEEDED; i++)
    {
        GPIO_disableInt(gpio_map[i]);
    }

    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN7);

    int us_delay = 3000000;
    Timer32_haltTimer(TIMER32_0_BASE);
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, 48 * us_delay);
    Timer32_startTimer(TIMER32_0_BASE, true);

    while (Timer32_getValue(TIMER32_0_BASE) > 0) {}

    while (score < CHECKS_NEEDED)
    {
        for (i = 0; i < CHECKS_NEEDED; i++)
        {
            unsafe = GPIO_read(gpio_map[i]) == 1;

            if (unsafe)
            {
                score = 0;
            }
            else
            {
                score++;
            }
        }
    }

    for (i = 0; i < CHECKS_NEEDED; i++)
    {
        GPIO_enableInt(gpio_map[i]);
    }
}
