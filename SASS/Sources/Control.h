/*
 * Control.h
 * Created by: Joseph Walters, Trent Sellers
 */

#ifndef CONTROL_H_
#define CONTROL_H_

/* XDC module headers */
#include <xdc/runtime/Error.h>

/* BIOS module headers */
#include <ti/sysbios/knl/Swi.h>

namespace sources
{


class Control
{
public:
    static Control* get_instance();
    void fail_system();

    // Thread-able method(s)/callback functions
    static void default_failure_callback();

private:
    Control();
    virtual ~Control();
    void init();

    Swi_Handle failure_interrupt_handle;
    Swi_Params failure_interrupt_params;
    Error_Block eb;

    static Control* control;
};


} // sources

#endif // CONTROL_H_
