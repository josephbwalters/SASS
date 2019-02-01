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

    static void init();

    void control(Commands c, Directions d);

    void set_red(Directions direction);

    void set_all_red();

    void panic();

    void set_yellow(Directions direction);

    void wait();

    void safe();

    void light_thread(void);
};


}} // sources::lights

#endif // LIGHTS_H_
