#include <Sources/TLC/Scheduler.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/Commands.h>
#include <Sources/Directions.h>

using namespace sources;

using namespace sources::tlc::scheduler;
using namespace sources::llha::lights;

void Scheduler::scheduler_thread()
{
    while(true)
    {
        // TODO: Implement Panic feature
        // TODO: Implement with directions input into the queue, not numbers!
        if (!m_traffic_queue.empty())
        {
            if (m_traffic_queue.front() == 1)
            {
                m_lights.control(Commands::PROCEED, Directions::NORTH);
                // Timed delay
            }
            else if (m_traffic_queue.front() == 2)
            {
                m_lights.control(Commands::PROCEED, Directions::EAST);
                // Timed delay
            }
        }
        else
        {
            m_lights.wait();
        }
    }
}
