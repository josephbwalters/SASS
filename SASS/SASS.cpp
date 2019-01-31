/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

#include <pthread.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#include <Board.h>

#include <Sources/Sensors/Lidar.h>
#include <Sources/Logger/Logger.h>

using namespace sources::logger;
using namespace sources::sensors;

// Stack size for new tasks
#define STACK_SIZE_SMALL 512
#define STACK_SIZE_MEDIUM 1024
#define STACK_SIZE_LARGE 2048

void *mainThread(void *Uarg0)
{
    uint16_t dist = lidar_north->get_distance();
    Lidar*  lidar_north = Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);
    Logger::print_value((String)"Distance is", dist);
    return 0;
}


int main()
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    Board_initGeneral();

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, STACK_SIZE_LARGE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, mainThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }


//    Logger::print((String)"Creating Lidar task");
//    Task_Params_init(&taskParams1);
//    taskParams1.arg0 = 1000;
//    taskParams1.stackSize = TASK_STACK_SIZE_LARGE;
//    taskParams1.stack = &task1Stack;
//    Task_construct(&task1Struct, (Task_FuncPtr)test_lidar, &taskParams1, NULL);


    BIOS_start();    /* does not return */
    return(0);
}
