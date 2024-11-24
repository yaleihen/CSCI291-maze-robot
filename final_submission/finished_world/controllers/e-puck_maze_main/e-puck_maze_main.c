#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>
#include <webots/led.h>
#include <stdio.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28
#define WALL_THRESHOLD 80 // Threshold for wall detection
#define FRONT_WALL_THRESHOLD 120 // Threshold for front wall detection

void turn_right(WbDeviceTag left_motor, WbDeviceTag right_motor);

// Variables for light intensity calculation
float avg = 0;
float sum = 0;
float station_light[10]; // Array to store light intensities at each station
int station_count = 1;
int brightest_station_index = 0; // Index of the brightest station
float brightest_intensity = 0.0; // Brightest light intensity
int light_calculated = 0; // Ensures light is calculated only once
bool second_loop = false; // Flag to indicate second loope

int main(int argc, char **argv) {
  wb_robot_init();

  // Motors
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  // Light sensors
  const char *light_sensors_names[8] = {
    "ls0", "ls1", "ls2", "ls3", "ls4", "ls5", "ls6", "ls7"
  };
  WbDeviceTag light_sensors[8];
  for (int i = 0; i < 8; i++) {
    light_sensors[i] = wb_robot_get_device(light_sensors_names[i]);
    wb_light_sensor_enable(light_sensors[i], TIME_STEP);
  }

  // Proximity sensors
  WbDeviceTag ps_sensors[8];
  char ps_sensors_tags[8];
  for (int i = 0; i < 8; i++) {
    sprintf(ps_sensors_tags, "ps%d", i);
    ps_sensors[i] = wb_robot_get_device(ps_sensors_tags);
    wb_distance_sensor_enable(ps_sensors[i], TIME_STEP);
  }

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;

  while (wb_robot_step(TIME_STEP) != -1) {
    // Read proximity sensor values
    double leftWall = wb_distance_sensor_get_value(ps_sensors[5]);
    double leftCorner = wb_distance_sensor_get_value(ps_sensors[6]);
    double frontWall = wb_distance_sensor_get_value(ps_sensors[7]);
    
    // debug
    /* printf("Sensor 5: %f\n", leftWall);
       printf("Sensor 6: %f\n", leftCorner);
       printf("Sensor 7: %f\n", frontWall);
    */

    // If front wall is detected
     if (frontWall > FRONT_WALL_THRESHOLD) {
      // Nested condition to check for a station
      if (leftWall > WALL_THRESHOLD && leftCorner >WALL_THRESHOLD) {
        if (!light_calculated) {
          sum = 0;
          for (int i = 0; i < 8; i++) {
            sum += wb_light_sensor_get_value(light_sensors[i]);
          }
          avg = sum / 8.0;
          printf("Station %d Light Intensity: %f\n", station_count, avg);

          // Store light intensity for this station
          station_light[station_count - 1] = avg;

          // Update brightest station
          if (avg > brightest_intensity) {
            brightest_intensity = avg;
            brightest_station_index = station_count;
          }

          light_calculated = 1; // Ensure calculation is only done once
          station_count += 1;
        }
      }

      // Rotate right
      turn_right(left_motor, right_motor);
    } 
    // Normal wall-following behavior
    else {
      light_calculated = 0; // Reset light calculation flag

      // If left wall is detected, drive straight
      if (leftWall > WALL_THRESHOLD) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
      }
      // If no wall detected, turn left
      else {
        left_speed = MAX_SPEED / 8;
        right_speed = MAX_SPEED;
      }

      // Handle close corners
      if (leftCorner > WALL_THRESHOLD) {
        left_speed = MAX_SPEED / 2;
        right_speed = MAX_SPEED / 8;
      }
    }

    // Second loop: stop at the brightest station
    if (second_loop && station_count == brightest_station_index + 1) {
      printf("Stopping at Station %d with Brightest Intensity: %f\n", brightest_station_index, brightest_intensity);
      left_speed = 0.0;
      right_speed = 0.0;
    }

    // Set motor velocities
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
    
    // Transition to second loop if all stations are visited
    if (!second_loop && station_count > 10) {
      printf("Completed first loop. Brightest Station: %d\n", brightest_station_index);
      second_loop = true;
      station_count = 1; // Reset station counter for second loop
    }
  }

  wb_robot_cleanup();
  return 0;
}

void turn_right(WbDeviceTag left_motor, WbDeviceTag right_motor) {
  double turn_speed = MAX_SPEED / 2; 
  wb_motor_set_velocity(left_motor, turn_speed);
  wb_motor_set_velocity(right_motor, 0.0);
  int turn_duration = (int)(1500 / TIME_STEP); 
  int c = 0;
  while (c < turn_duration) {
    wb_robot_step(TIME_STEP);
    c++;
  }
}
