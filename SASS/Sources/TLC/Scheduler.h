#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <deque>

#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/OC/Vehicle.h>

using namespace std;
using namespace sources::llha::lights;
using namespace sources::oc;

namespace sources
{
namespace tlc
{
namespace scheduler
{


class Scheduler
{
public:
    Scheduler();

    // Thread-able method(s)
    static void *scheduler_thread(void *args);

private:
    Lights m_lights;
    deque<Vehicle> m_traffic_queue;
};


}}} // sources::tlc::scheduler



#endif // SCHEDULER_H_
