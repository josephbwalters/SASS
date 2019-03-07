/*
 * Classifier.h
 * Created by: Joseph Walters, Trent Sellers (University of Central Florida)
 * Date: March 6, 2019
 * Last modified: March 6, 2019
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
