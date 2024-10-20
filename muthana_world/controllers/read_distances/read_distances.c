#include <webots/robot.h>
#include <webots/light_sensor.h>
#include <stdio.h>

#define TIME_STEP 64  // Time step for simulation in ms

float avg= 0;
float sum = 0;
int main(int argc, char **argv) {
  // Initialize the Webots API
  wb_robot_init();

  // Define the names of the 8 built-in light sensors
  const char *light_sensors_names[8] = {
    "ls0", "ls1", "ls2", "ls3", "ls4", "ls5", "ls6", "ls7"
  };

  WbDeviceTag light_sensors[8];
  
  // Enable the light sensors
  for (int i = 0; i < 8; i++) {
    light_sensors[i] = wb_robot_get_device(light_sensors_names[i]);
    wb_light_sensor_enable(light_sensors[i], TIME_STEP);
  }

  // Main loop
  while (wb_robot_step(TIME_STEP) != -1) {
  sum =0;
    // Read and display the values from the 8 light sensors
    for (int i = 0; i < 8; i++) {
      double light_value = wb_light_sensor_get_value(light_sensors[i]);
      //printf("Light Sensor %d Value: %f\n", i, light_value);
      sum += light_value;
    }
    printf("\n");  // Add a newline for better readability
    avg = sum/8.0;
    printf("Average light is %lf\n", avg);
  }

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}
