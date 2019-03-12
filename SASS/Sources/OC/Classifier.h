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

    static void *classifier_thread(void* args);
    static void *watchman(void *args);
    static void callback_hwi(uint_least8_t index);

private:
    Classifier(Directions direction);
    virtual ~Classifier();

    Directions m_direction;
    Lidar* m_lidar;
    Radar* m_radar;

    uint8_t track();

    static Classifier* classifier_north;
    static Classifier* classifier_east;
    static Classifier* classifier_south;
    static Classifier* classifier_west;
};


}} // sources::oc

#endif // CLASSIFIER_H_
