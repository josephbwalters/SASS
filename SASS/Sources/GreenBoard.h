#ifndef GREENBOARD_H_
#define GREENBOARD_H_

#include <ti/devices/msp432p4xx/driverlib/gpio.h>

#include <Board.h>

// RADAR assignment
#define RADAR_N Board_SPI4
#define RADAR_E Board_SPI5
#define RADAR_S 100 // NOT IMPLEMENTED
#define RADAR_W 100 // NOT IMPLEMENTED

// LiDAR assignment
#define LIDAR_N Board_I2C1
#define LIDAR_E Board_I2C2
#define LIDAR_S 100 // NOT IMPLEMENTED
#define LIDAR_W 100 // NOT IMPLEMENTED

#define RED_N_PORT GPIO_PORT_P7
#define RED_E_PORT GPIO_PORT_P7
#define RED_S_PORT
#define RED_W_PORT

#define RED_N_PIN GPIO_PIN4
#define RED_E_PIN GPIO_PIN5
#define RED_S_PIN
#define RED_W_PIN


#define YELLOW_N_PORT GPIO_PORT_P7
#define YELLOW_E_PORT GPIO_PORT_P7
#define YELLOW_S_PORT
#define YELLOW_W_PORT

#define YELLOW_N_PIN GPIO_PIN6
#define YELLOW_E_PIN GPIO_PIN7
#define YELLOW_S_PIN
#define YELLOW_W_PIN






#endif // GREENBOARD_H_
