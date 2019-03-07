/*
 * Classifier.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

/* SASS-specific headers */
#include <Sources/Directions.h>

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

    static Classifier* classifier_north;
    static Classifier* classifier_east;
    static Classifier* classifier_south;
    static Classifier* classifier_west;
};


}} // sources::oc

#endif // CLASSIFIER_H_
