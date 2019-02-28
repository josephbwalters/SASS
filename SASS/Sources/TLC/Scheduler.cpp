#include <stdio.h>

#include <ti/sysbios/knl/Task.h>

#include <Sources/TLC/Scheduler.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/Directions.h>

using namespace sources;

using namespace sources::tlc;
using namespace sources::llha::lights;

Scheduler::Scheduler()
{
    m_vehicle_queue = new deque();
}

void *Scheduler::scheduler_thread(void* args)
{
    Scheduler* scheduler = new Scheduler();

    while (1)
    {
        // TODO: Implement Panic feature
        // TODO: Implement with directions input into the queue, not numbers!
        while (!scheduler->m_vehicle_queue.empty())
        {
            printf("Processing vehicle...\n");
            Vehicle vehicle = scheduler->m_vehicle_queue.front();
            Directions direction = vehicle.get_direction();

            scheduler->m_lights.schedule(direction);
            scheduler->m_lights.wait();
        }

        scheduler->m_lights.set_all_red();
        printf("All lights set red.\n");

        Task_yield();
    }
}
