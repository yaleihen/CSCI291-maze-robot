/* include headers */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define MAX_SPEED 6.28
#define TIME_STEP 64
int main(int argc, char **argv) {
  wb_robot_init();
  double left_speed=1, right_speed=1;
  double gs_value=4;
  
   WbDeviceTag gs0= wb_robot_get_device("gs0");
   wb_distance_sensor_enable(gs0,TIME_STEP );
  // get a handler to the motors and set target position to infinity (speed control)
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  // set up the motor speeds at 10% of the MAX_SPEED.
  wb_motor_set_velocity(left_motor, left_speed*MAX_SPEED);
  wb_motor_set_velocity(right_motor, right_speed*MAX_SPEED);

  while (wb_robot_step(TIME_STEP) != -1) {
      // scale the data in order to have a value between 0.0 and 1.0
    // 1.0 representing something to avoid, 0.0 representing nothing to avoid

   gs_value=wb_distance_sensor_get_value(gs0);
   printf("%f\n",gs_value);  
  }

  wb_robot_cleanup(); 
  return 0;
}
