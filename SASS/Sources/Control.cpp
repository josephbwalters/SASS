/*
 * Control.cpp
 * Created by: Joseph Walters, Trent Sellers
 */

#ifndef __MSP432P401R__
#define __MSP432P401R__
#endif

/* System headers */
#include <ti/devices/msp432p4xx/driverlib/timer32.h>
#include <ti/sysbios/knl/Task.h>

/* SASS-specific headers */
#include <Sources/Control.h>
#include <Sources/LLHA/Lights/Lights.h>

#define HALF_SECOND 550000

using namespace sources;
using namespace sources::llha::lights;

// Initialize Control instance to null for singleton pattern
sources::Control* sources::Control::control = nullptr;

Control::Control()
{
    init();
}

Control::~Control()
{
    // TODO: Clean up (if necessary)
}

void Control::init()
{
    Error_init(&eb);
    Swi_Params_init(&failure_interrupt_params);

    failure_interrupt_handle = Swi_create((ti_sysbios_knl_Swi_FuncPtr)default_failure_callback, &failure_interrupt_params, &eb);

    if (failure_interrupt_handle == nullptr)
    {
        fail_system();
    }
}

Control* Control::get_instance()
{
    if (control == nullptr)
    {
        control = new Control();
    }
    return control;
}

void Control::fail_system()
{
    Swi_post(failure_interrupt_handle);
}

void Control::default_failure_callback()
{
    while (true)
    {
        Lights::set_all_red();

        int us_delay = HALF_SECOND;
        Timer32_haltTimer(TIMER32_0_BASE);
        Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
        Timer32_setCount(TIMER32_0_BASE, 48 * us_delay);
        Timer32_startTimer(TIMER32_0_BASE, true);

        while (Timer32_getValue(TIMER32_0_BASE) > 0) {}

        Lights::turn_off();

        us_delay = HALF_SECOND;
        Timer32_haltTimer(TIMER32_0_BASE);
        Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
        Timer32_setCount(TIMER32_0_BASE, 48 * us_delay);
        Timer32_startTimer(TIMER32_0_BASE, true);

        while (Timer32_getValue(TIMER32_0_BASE) > 0) {}
    }
}
