/*
 * Classifier.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

/* SASS-specific headers */
#include <Sources/Directions.h>
#include <Sources/LLHA/Sensors/Lidar.h>
#include <Sources/LLHA/Sensors/Radar.h>
#include <Sources/TLC/Scheduler.h>

using namespace sources::llha::sensors;
using namespace sources::tlc;

namespace sources
{
namespace oc
{


class Classifier
{
public:
    static Classifier* get_instance(Directions direction);

    static uint16_t get_reference_distance(Directions direction);
    uint8_t track();

    // Thread-able method(s)/callback functions
    static void *classifier_thread(void* args);
    static void *watchman_thread(void *args);
    static void classifier_hwi_callback(uint_least8_t index);

private:
    Classifier(Directions direction);
    virtual ~Classifier();

    void init();
    void set_reference_distance();

    Directions m_direction;
    Lidar* m_lidar;
    Radar* m_radar;

    static uint16_t ref_dist_north;
    static uint16_t ref_dist_east;
    static uint16_t ref_dist_south;
    static uint16_t ref_dist_west;

    static Classifier* classifier_north;
    static Classifier* classifier_east;
    static Classifier* classifier_south;
    static Classifier* classifier_west;
};


}} // sources::oc

#endif // CLASSIFIER_H_
