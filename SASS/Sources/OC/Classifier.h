/*
 * Classifier.h
 *
 *  Created on: Feb 4, 2019
 *      Author: jwalters
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

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
