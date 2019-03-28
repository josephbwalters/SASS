/*
 * DetectedObject.h
 * Created by: Joseph Walters, Trent Sellers 
 */

#ifndef DETECTEDOBJECT_H_
#define DETECTEDOBJECT_H_

/* Standard headers */
#include <stdint.h>

/* SASS-specific headers */
#include <Sources/Directions.h>

namespace sources
{
namespace oc
{


class DetectedObject
{
public:
    DetectedObject(Directions direction);
    ~DetectedObject();

    virtual void set_velocity(uint16_t velocity) = 0;
    virtual uint16_t get_velocity() = 0;
    virtual Directions get_direction() = 0;

protected:
    DetectedObject();

private:
    Directions m_direction;
    uint16_t m_velocity;
};

}} // sources::oc

#endif // DETECTED_OBJECT_H_
