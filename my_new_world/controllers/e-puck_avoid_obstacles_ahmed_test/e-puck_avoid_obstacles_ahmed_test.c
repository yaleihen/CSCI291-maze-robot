#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28

// Thresholds for earlier wall detection. Requires Tweaking for better navigation
#define WALL_THRESHOLD 100.0    // For right or left walls
#define TURN_THRESHOLD 150.0    // For front walls
#define CRITICAL_DISTANCE 200.0 // For very close walls

// Function Prototypes
void move(double left_speed, double right_speed);

// Initialize motors and sensors
WbDeviceTag left_motor, right_motor;  // Motors
WbDeviceTag ds_left, ds_right, ds_front;  // Distance Sensors

int main(int argc, char **argv) {
  wb_robot_init();
  
  // Get the motor devices
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  
  // Initialize distance sensors
  ds_left = wb_robot_get_device("ps5");   // Left Sensor
  ds_right = wb_robot_get_device("ps2");  // Right Sensor
  ds_front = wb_robot_get_device("ps7");  // Front Sensor
  
  wb_distance_sensor_enable(ds_left, TIME_STEP);
  wb_distance_sensor_enable(ds_right, TIME_STEP);
  wb_distance_sensor_enable(ds_front, TIME_STEP);
   
  while (wb_robot_step(TIME_STEP) != -1) {
    // Read sensor values
    double left_dist = wb_distance_sensor_get_value(ds_left);
    double right_dist = wb_distance_sensor_get_value(ds_right);
    double front_dist = wb_distance_sensor_get_value(ds_front);
    
    // For avoiding being too close to walls, as it tries to breakout of maze
    if (front_dist > CRITICAL_DISTANCE || left_dist > CRITICAL_DISTANCE || right_dist > CRITICAL_DISTANCE) {
        // Sharp turn away it too close to wall
        if (left_dist > right_dist) {
            // Turn right if left wall is closer
            move(MAX_SPEED, -MAX_SPEED);
        } else {
            // Turn left if right wall is closer
            move(-MAX_SPEED, MAX_SPEED);
        }
        continue; 
    }
    
    // Wall following Algorithm
    if (front_dist > TURN_THRESHOLD) {
        // No wall in front
        if (right_dist < WALL_THRESHOLD) {
            // Right wall is close, steer slightly left
            move(MAX_SPEED, MAX_SPEED * 0.5);
        } else if (right_dist > WALL_THRESHOLD * 1.5) {
            // Far from right wall, steer slightly right
            move(MAX_SPEED * 0.5, MAX_SPEED);
        } else {
            // Move straight
            move(MAX_SPEED, MAX_SPEED);
        }
    } else {
        // Wall ahead, wider turns
        if (left_dist < right_dist) {
            // More space right, turn right (To avoid getting stuck
            move(MAX_SPEED, -MAX_SPEED * 0.5);
        } else {
            // More space left, turn left
            move(-MAX_SPEED * 0.5, MAX_SPEED);
        }
    }
  }
  
  wb_robot_cleanup();
  return 0;
}

// Movement function
void move(double left_speed, double right_speed) {
  wb_motor_set_velocity(left_motor, left_speed);
  wb_motor_set_velocity(right_motor, right_speed);
}