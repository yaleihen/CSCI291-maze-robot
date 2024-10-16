#include <webots/robot.h>
#include <webots/light_sensor.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <stdio.h>

#define TIME_STEP 64  // Time step for simulation in ms
#define MAX_SPEED 10

void move_forward();
void turn_left();
void turn_right();
void stop();

int main(int argc, char **argv) {
  // Initialize the Webots API
  wb_robot_init();

  move_forward();
  turn_left();
  move_forward();
  turn_right();
  stop();

  // Define and enable the light sensor (light sensor)
  WbDeviceTag light_sensor = wb_robot_get_device("light sensor");  // Replace "lts" with the actual name of the light sensor
  wb_light_sensor_enable(light_sensor, TIME_STEP);

  // Main loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // Read the light sensor value
    double light_value = wb_light_sensor_get_value(light_sensor);

    // Display the value
    printf("Light Sensor Value: %f\n", light_value);
  }

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}

void move_forward(){
  // get the motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  // set the target position of the motors
  wb_motor_set_position(left_motor, MAX_SPEED);
  wb_motor_set_position(right_motor, MAX_SPEED);
}

void turn_left(){
  // get the motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  // set the target position of the motors
  wb_motor_set_position(left_motor, MAX_SPEED);
  wb_motor_set_position(right_motor, -MAX_SPEED);
}

void turn_right(){
  // get the motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  // set the target position of the motors
  wb_motor_set_position(left_motor, -MAX_SPEED);
  wb_motor_set_position(right_motor, MAX_SPEED);
}

void stop(){
  // get the motor devices
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  // set the target position of the motors
  wb_motor_set_position(left_motor, 0);
  wb_motor_set_position(right_motor, 0);
}