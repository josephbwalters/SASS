/**
 * LIDARLite I2C Example
 * Author: Garmin
 * Modified by: Shawn Hymel (SparkFun Electronics)
 * Date: June 29, 2017
 *
 * Read distance from LIDAR-Lite v3 over I2C
 *
 * See the Operation Manual for wiring diagrams and more information:
 * http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
 */

/*
 * Modified by: Trent Sellers
 * Date: January 17, 2019
 * Description: Arduino-based example adapted to TI MSP432.
 */

#include <Sources/Utils/Wire.h>
#include <Sources/Utils/Energia.h>
#include "LIDARLite_v3HP.h"

using namespace sources::sensors;

// Globals
LIDARLite_v3HP lidarLite;
int cal_cnt = 0;

void setup();
void loop();

void run_test()
{
    setup();

    while(1)
    {
        loop();
    }
}

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings

  // Method immediately below does not exist in LIDARLite_v3HP.h (must be difference between normal and HP versions):
  // lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{
  int dist;

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction
  if ( cal_cnt == 0 ) {
    dist = lidarLite.readDistance();      // With bias correction
  } else {
    dist = lidarLite.readDistance(false); // Without bias correction
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  // Display distance
  Serial.print(dist);
  Serial.println(" cm");

  delay(10);
}
