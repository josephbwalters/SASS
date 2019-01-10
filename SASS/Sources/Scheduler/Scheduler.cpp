#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


// TODO: Check Imports!
#include <Sources/Lights/Lights.cpp>
#include <Sources/Objects/Vehicle.cpp>
#include <../Headers/Commands.h>
#include <../Headers/Directions.h>

class Scheduler
{
    std::deque<int> traffic_queue;

    void scheduler_thread()
    {
        while(true)
        {
            // TODO: Implement Panic feature

            // TODO: Implement with directions input into the queue, not numbers!
            if (!traffic_queue.empty())
            {
                if (traffic_queue.front() == 1)
                {
                    Lights.control(Commands::PROCEED, Directions::NORTH);
                    // Timed delay
                }
                else if (traffic_queue.front() == 2)
                {
                    Lights.control(Commands::PROCEED, Directions::EAST);
                    // Timed delay
                }
            }
            else
            {
                Lights.wait();
            }
        }
    }
};
