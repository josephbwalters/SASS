#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <deque>

#include "../Lights/Lights.h"

using namespace std;
using namespace sources::lights;

namespace sources
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


}} // sources::scheduler



#endif // SCHEDULER_H_
