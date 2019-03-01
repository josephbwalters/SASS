#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <deque>

#include <pthread.h>

#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/OC/Vehicle.h>

using namespace std;
using namespace sources::llha::lights;
using namespace sources::oc;

namespace sources
{
namespace tlc
{


class Scheduler
{
public:
    static Scheduler* get_instance();

    Lights* get_lights();
    deque<Vehicle>* get_vehicle_queue();
    pthread_mutex_t* get_queue_mutex();

    // Thread-able method(s)
    static void *scheduler_thread(void *refs);

private:
    Scheduler();
    virtual ~Scheduler();

    Lights lights;
    deque<Vehicle> vehicle_queue;
    pthread_mutex_t queue_mutex;

    static Scheduler* scheduler;
};


}} // sources::tlc

#endif // SCHEDULER_H_
