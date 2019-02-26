#ifndef LIGHTS_H_
#define LIGHTS_H_

// Our custom headers
#include <Sources/Directions.h>

using namespace sources;

namespace sources
{
namespace llha
{
namespace lights
{


class Lights
{
public:
    Lights();
    static void init();

    static void set_red(Directions direction);
    static void set_yellow(Directions direction);
    static void schedule(Directions direction);
    static void set_all_red();
    static void wait();
    static void panic();
    static void safe();

    // Thread-able method(s)
    void *light_thread(void *args);
    static void *mosfetToggleThread(void *args);

private:
    Directions m_direction;
    bool m_panic_flag;
    bool m_waiting;
};


}}} // sources::llha::lights

#endif // LIGHTS_H_
