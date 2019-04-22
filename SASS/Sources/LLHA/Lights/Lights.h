/*
 * Lights.h
 * Created by: Joseph Walters, Trent Sellers
 */

#ifndef LIGHTS_H_
#define LIGHTS_H_

/* SASS-specific headers */
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
    static void init();

    static void set_red(Directions direction);
    static void set_yellow(Directions direction);
    static void schedule(Directions direction);
    static void toggle_yellow(Directions direction);
    static void set_all_red();
    static void turn_off();
    static void turn_off(Directions direction);
    static void set_all();

    // Thread-able method(s)
    static void *mosfet_toggle_thread(void *args);
};


}}} // sources::llha::lights

#endif // LIGHTS_H_
