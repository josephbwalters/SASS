/*
 * Control.cpp
 * Created by: Joseph Walters, Trent Sellers
 */

/* BIOS module headers */
#include <ti/sysbios/knl/Task.h>

/* SASS-specific headers */
#include <Sources/Control.h>
#include <Sources/LLHA/Lights/Lights.h>

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

    failure_interrupt_handle = Swi_create((ti_sysbios_knl_Swi_FuncPtr)default_failure_callback,
                                          &failure_interrupt_params, &eb);

    if (failure_interrupt_handle == nullptr)
    {
        // TODO: Throw exception
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
        Task_sleep(1000);
        Lights::turn_off();
        Task_sleep(1000);
    }
}
