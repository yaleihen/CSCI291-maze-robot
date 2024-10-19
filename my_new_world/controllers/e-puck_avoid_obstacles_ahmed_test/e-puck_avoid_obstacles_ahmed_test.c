#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28
#define WALL_THRESHOLD 80.0
#define TURN_THRESHOLD 100.0

// Function Protoypes
void move(double left_speed, double right_speed);

// Initialize motors and sensors
WbDeviceTag left_motor, right_motor; // Motors
WbDeviceTag ds_left, ds_right, ds_front; // Distance Sensors

// Main function
int main(int argc, char **argv) {
  wb_robot_init();

  // Get the motor devices
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  
  // Initialize distance sensors
  ds_left = wb_robot_get_device("ps5");  // Left Sensor
  ds_right = wb_robot_get_device("ps2"); // Right Sensor
  ds_front = wb_robot_get_device("ps7"); // Front Sensor
  
  wb_distance_sensor_enable(ds_left, TIME_STEP);
  wb_distance_sensor_enable(ds_right, TIME_STEP);
  wb_distance_sensor_enable(ds_front, TIME_STEP);
   
  while (wb_robot_step(TIME_STEP) != -1) {
    // Read sensor values
    // double left_dist = wb_distance_sensor_get_value(ds_left); // Removed, since it's not used
    double right_dist = wb_distance_sensor_get_value(ds_right);
    double front_dist = wb_distance_sensor_get_value(ds_front);

/* FIX THIS PART
    // Wall following Algorithm
    if (front_dist > TURN_THRESHOLD) {
        // No wall in front, move forward
        if (right_dist < WALL_THRESHOLD) {
            // Wall is close on the right, move forward while following the wall
            move(MAX_SPEED, MAX_SPEED);
        } else {
            // No wall on the right, turn right slightly to align with the wall
            move(MAX_SPEED * 0.8, MAX_SPEED);
        }
    } else {
        // Wall directly in front, turn left to avoid the wall
        move(-MAX_SPEED * 0.5, MAX_SPEED * 0.5);
    }
  }
*/

  wb_robot_cleanup();
  return 0;
}

// Movement function
void move(double left_speed, double right_speed) {
  wb_motor_set_velocity(left_motor, left_speed);
  wb_motor_set_velocity(right_motor, right_speed);
}
