// XDCtools Header files
#include <xdc/std.h>
#include <xdc/runtime/System.h>

// BIOS Header files
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

// TI-RTOS Header files
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

// Board Header file
#include <Sources/Board.h>

// Custom Header files
#include <Sources/Sensors/Lidar.h>
#include <Sources/Logger/Logger.h>

using namespace sources::logger;

// Stack size for new tasks
#define TASK_STACK_SIZE_SMALL 512
#define TASK_STACK_SIZE_LARGE 2048

Task_Struct task0Struct;
Char task0Stack[TASK_STACK_SIZE_SMALL];

Task_Struct task1Struct;
Char task1Stack[TASK_STACK_SIZE_LARGE];

Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((UInt)arg0);
        GPIO_toggle(Board_LED0);
        System_printf("Heartbeat\n");
        System_flush();
    }
}

void test_lidar()
{
    sources::sensors::Lidar* lidar_north = sources::sensors::Lidar::get_instance(LidarInstanceType::LIDAR_NORTH);

    while(1)
    {
        uint16_t dist = lidar_north->get_distance();
        Logger::print_value((String)"Distance is", dist);
        Task_sleep(1000);
    }
}

int main(void)
{
    Task_Params taskParams0;
    Task_Params taskParams1;

    // Call board init functions
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initWatchdog();
    // Board_initWiFi();

    // Construct heartBeat Task thread
    Logger::print((String)"Creating heartbeat task");
    Task_Params_init(&taskParams0);
    taskParams0.arg0 = 1000;
    taskParams0.stackSize = TASK_STACK_SIZE_SMALL;
    taskParams0.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams0, NULL);


    Logger::print((String)"Creating Lidar task");
    Task_Params_init(&taskParams1);
    taskParams1.arg0 = 1000;
    taskParams1.stackSize = TASK_STACK_SIZE_LARGE;
    taskParams1.stack = &task1Stack;
    Task_construct(&task1Struct, (Task_FuncPtr)test_lidar, &taskParams1, NULL);

    GPIO_write(Board_LED0, Board_LED_ON);

    Logger::print((String)"Starting tasks!");

    BIOS_start();

    return (0);
}
