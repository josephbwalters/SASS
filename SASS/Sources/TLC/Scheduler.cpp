#include <stdio.h>

#include <ti/sysbios/knl/Task.h>

#include <Sources/TLC/Scheduler.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/Directions.h>

using namespace sources;

using namespace std;
using namespace sources;
using namespace sources::tlc;
using namespace sources::llha::lights;

sources::tlc::Scheduler* sources::tlc::Scheduler::scheduler = nullptr;

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{

}

Scheduler* Scheduler::get_instance()
{
    if (scheduler == nullptr)
    {
        scheduler = new Scheduler();
    }
    return scheduler;
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

    while (1)
    {
        // TODO: Implement Panic feature
        // TODO: Implement with directions input into the queue, not numbers!
        while (!scheduler->get_vehicle_queue()->empty())
        {
            printf("Processing vehicle...\n");
            Vehicle vehicle = scheduler->get_vehicle_queue()->front();
            Directions direction = vehicle.get_direction();

            scheduler->get_lights()->schedule(direction);
            scheduler->get_lights()->wait();
            scheduler->get_vehicle_queue()->pop_front();
        }

        scheduler->get_lights()->set_all_red();
        printf("All lights set red.\n");

        Task_yield();
    }
}
