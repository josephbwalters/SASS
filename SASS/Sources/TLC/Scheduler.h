/*
 * Scheduler.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/* Standard headers */
#include <deque>

/* POSIX thread support */
#include <pthread.h>

/* SASS-specific headers */
#include <Sources/Directions.h>
#include <Sources/LLHA/Lights/Lights.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/OC/Vehicle.h>

using namespace std;
using namespace sources::llha::lights;
using namespace sources::llha::sensors;
using namespace sources::oc;

namespace sources
{
namespace tlc
{


class Scheduler
{
public:
    static Scheduler* get_instance();

    bool is_clear(Directions direction);
    deque<Vehicle>* get_vehicle_queue();

    // Thread-able method(s)
    static void *scheduler_thread(void *refs);

private:
    Scheduler();
    ~Scheduler();

    deque<Vehicle> vehicle_queue;

    static Scheduler* scheduler;
};


}} // sources::tlc

#endif // SCHEDULER_H_
