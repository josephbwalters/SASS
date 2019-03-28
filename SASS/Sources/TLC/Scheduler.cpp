/*
 * Scheduler.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

#define __MSP432P401R__

/* Standard headers */
#include <stdio.h>

/* System headers */
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/Timer.h>

/* Board-specific headers */
#include <Board.h>

/* SASS-specific headers */
#include <Sources/TLC/Scheduler.h>
#include <Sources/Directions.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/OC/Classifier.h>

// Pass requirement for verification of vehicle absence
#define PASS 2

using namespace std;
using namespace sources;
using namespace sources::oc;
using namespace sources::tlc;
using namespace sources::llha::lights;

sources::tlc::Scheduler* sources::tlc::Scheduler::scheduler = nullptr;

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{
    // TODO: Clean up (if necessary)
}

Scheduler* Scheduler::get_instance()
{
    if (scheduler == nullptr)
    {
        scheduler = new Scheduler();
    }
    return scheduler;
}

bool Scheduler::is_clear(Directions direction) // Dont need dir?
{
    uint16_t ref_dist = Classifier::get_reference_distance(direction);
    Lidar* lidar = Lidar::get_instance(direction);

    uint16_t dist = lidar->get_distance();

    return !(dist < ref_dist - 20);
}

Lights* Scheduler::get_lights()
{
    return &lights;
}

deque<Vehicle>* Scheduler::get_vehicle_queue()
{
    return &vehicle_queue;
}

pthread_mutex_t* Scheduler::get_queue_mutex()
{
    return &queue_mutex;
}

void *Scheduler::scheduler_thread(void *args)
{
    Scheduler* scheduler = get_instance();

    Timer_init();

    Timer_Handle    timer_handle;
    Timer_Params    timer_params;

    Timer_Params_init(&timer_params);
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.period = 3000000;
    timer_params.timerMode  = Timer_ONESHOT_BLOCKING;
    timer_params.timerCallback = NULL;

//    timer_handle = Timer_open(Board_TIMER0, &timer_params);
//    if (timer_handle == NULL) {
//        // Timer_open() failed
//        // TODO: Throw exception
//    }

    while (true)
    {
        // TODO: Implement Panic feature
        // TODO: Implement with directions input into the queue, not numbers!
        while (!scheduler->get_vehicle_queue()->empty())
        {
            Vehicle vehicle = scheduler->get_vehicle_queue()->front();
            Directions direction = vehicle.get_direction();

            printf("[Scheduler] Processing vehicle %d...\n", direction);

            timer_params.period = 2000000;
            timer_handle = Timer_open(Board_TIMER0, &timer_params);
            printf("[Scheduler] Starting 2-second timer...\n");
            int status = Timer_start(timer_handle);
            Timer_close(timer_handle);

            if (status == Timer_STATUS_ERROR) {
                //Timer_start() failed
                // TODO: Throw exception
            }

            printf("[Scheduler] 2-second timer expired.\n");

            scheduler->get_lights()->schedule(direction);

            uint8_t score = 0;

            // Confirm vehicle is absent
            while (score < PASS)
            {
                for (int i = 0; i < PASS; i++)
                {
                    if(scheduler->is_clear(direction))
                    {
                        score++;
                    }
                    else
                    {
                        score = 0;
                    }

                    Task_sleep(200);
                }

                GPIO_toggleOutputOnPin(GPIO_PORT_P7, GPIO_PIN3);
            }

            GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN3);

//            timer_params.period = 1000000;
//            timer_handle = Timer_open(Board_TIMER0, &timer_params);
//            printf("[Scheduler] Starting 1-second timer...\n");
//            status = Timer_start(timer_handle);
//            Timer_close(timer_handle);
//
//            if (status == Timer_STATUS_ERROR) {
//                //Timer_start() failed
//                // TODO: Throw exception
//            }
//
//            printf("[Scheduler] 1-second timer expired.\n");

            scheduler->get_lights()->set_all_red();

            timer_params.period = 2000000;
            timer_handle = Timer_open(Board_TIMER0, &timer_params);
            printf("[Scheduler] Starting 2-second timer...\n");
            status = Timer_start(timer_handle);
            Timer_close(timer_handle);

            if (status == Timer_STATUS_ERROR) {
                //Timer_start() failed
                // TODO: Throw exception
            }

            // Timer_stop(timer_handle);
            printf("[Scheduler] 2-second timer expired.\n");

            scheduler->get_vehicle_queue()->pop_front();
            printf("[Scheduler] Vehicle exited and removed from queue.\n");

            Task_sleep(200);
        }

        scheduler->get_lights()->set_all_red();
        printf("[Scheduler] All lights set red.\n");

        Task_sleep(200);
    }
}
