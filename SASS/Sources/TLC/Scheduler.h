#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <deque>

#include <Sources/LLHA/Lights/Lights.h>

using namespace std;
using namespace sources::llha::lights;

namespace sources
{
namespace tlc
{
namespace scheduler
{


class Scheduler
{
private:
    Lights m_lights;
    deque<int> m_traffic_queue;

protected:

public:
    Scheduler();
    void scheduler_thread();
};


}}} // sources::tlc::scheduler



#endif // SCHEDULER_H_
