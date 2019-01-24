/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
//#include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include <Sources/Board.h>

#include <Sources/Sensors/Lidar.h>

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

Task_Struct task1Struct;
Char task1Stack[2048];

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((UInt)arg0);
        GPIO_toggle(Board_LED0);
    }
}

void test_lidar()
{
    sources::sensors::Lidar* lidar_north = sources::sensors::Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);

    while(1)
    {
        // uint16_t dist = lidar_north->get_distance();
        System_printf("Distance!\n");
        // System_printf("Distance: %d\n", dist);
        System_flush();
        Task_sleep(100);
    }
}

int main(void)
{
    Task_Params taskParams0;
    Task_Params taskParams1;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initWatchdog();
    // Board_initWiFi();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams0);
    taskParams0.arg0 = 1000;
    taskParams0.stackSize = TASKSTACKSIZE;
    taskParams0.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams0, NULL);

    Task_Params_init(&taskParams1);
    taskParams1.arg0 = 1000;
    taskParams1.stackSize = 2048;
    taskParams1.stack = &task1Stack;
    Task_construct(&task1Struct, (Task_FuncPtr)test_lidar, &taskParams1, NULL);

    /* Construct RADAR Monitoring Thread */
    // NOTE: This thread should be capable of either spawning another thread
    //       or there should be another thread that leaves LiDAR in sleep mode
    //       until it is activated by this thread.

    /* Construct Scheduler Thread */

    /* Construct Light Control Thread */

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
