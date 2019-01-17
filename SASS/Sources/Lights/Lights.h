#ifndef LIGHTS_H_
#define LIGHTS_H_

// Our custom headers
#include "../Directions.h"
#include "../Commands.h"

using namespace sources;

namespace sources
{
namespace lights
{


class Lights
{
private:
    Commands m_command;
    Directions m_direction;
    bool m_panic_flag;
    bool m_waiting;

protected:

public:
    Lights();

    void control(Commands c, Directions d);

    void stop(Directions direction);

    void all_stop();

    void panic();

    void proceed(Directions direction);

    void wait();

    void safe();

    void light_thread(void);
};


}} // sources::lights

#endif // LIGHTS_H_
