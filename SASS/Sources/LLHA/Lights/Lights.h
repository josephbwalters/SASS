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

    static void turn_on_red();
    static void turn_on_red(Directions direction);
    static void turn_on_yellow();
    static void turn_on_yellow(Directions direction);
    static void turn_off_red();
    static void turn_off_red(Directions direction);
    static void turn_off_yellow();
    static void turn_off_yellow(Directions direction);
    static void toggle_yellow(Directions direction);
    static void turn_on();
    static void turn_on(Directions direction);
    static void turn_off();
    static void turn_off(Directions direction);
    static void set_all_red();
    static void schedule(Directions direction);
};


}}} // sources::llha::lights

#endif // LIGHTS_H_
