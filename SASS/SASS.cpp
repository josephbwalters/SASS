/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== bigtime.cpp ========
 */

/*
 * Modified by: Joseph Walters and Trent Sellers, University of Central Florida
 * Date: January 29, 2019
 * Description: Used bigtime.cpp (TI) as a basis for the main code controlling the
 * Situation Aware Stop Signal.
 *
 * Last modified: January 29, 2019
 */

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#include <Board.h>

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

void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((UInt)arg0);
        // GPIO_toggle(Board_LED0);
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

int main()
{
    Task_Params taskParams0;
    Task_Params taskParams1;

    // Call board init functions
    Board_initGeneral();
    // Board_initGPIO();
    // Board_initI2C();

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

    Logger::print((String)"Starting tasks!");

    BIOS_start();    /* does not return */
    return(0);
}
