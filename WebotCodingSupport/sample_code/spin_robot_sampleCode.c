/* include headers */
#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define MAX_SPEED 6.28
#define TIME_STEP 32

void spin_stop_robot(const double theta, const int scale,WbDeviceTag  left_motor, WbDeviceTag  right_motor);

// this a delay function for sec seconds not used in this program but might be useful to implement some steps of the LWA
void passive_wait(double sec);
 

int main(int argc, char **argv) {

wb_robot_init();

// get the motor devices

WbDeviceTag  left_motor = wb_robot_get_device("left wheel motor");
WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
wb_motor_set_position(left_motor, INFINITY);
wb_motor_set_position(right_motor, INFINITY);


 // 360 degree, call the function with 360 and 9
 //180 degree call the function with 180 and 9
 // 90 degree call the function with 90 and 5.8
spin_stop_robot(360,9, left_motor, right_motor);

while ( (wb_robot_step(TIME_STEP) != -1 )  ) {

}

wb_robot_cleanup();

return 0;
}




/*
This is mainly used to create a delay of TIME_STEP
https://cyberbotics.com/doc/reference/robot#wb_robot_step
*/

void step()
{
    if (wb_robot_step(TIME_STEP) == -1)
    {
        printf(" The application is closing....\n");
        wb_robot_cleanup();
        return;
     }
}



void spin_stop_robot(const double Phi, const  int scale,WbDeviceTag  left_motor, WbDeviceTag  right_motor)
{
  if (Phi==0) return ;

  double duration = scale* abs(Phi)* 0.003009259;

// get the curent time
  double start_time = wb_robot_get_time();

// if phi> 0, rotate left
   if (Phi > 0)
      {
       wb_motor_set_velocity(left_motor, -MAX_SPEED);
       wb_motor_set_velocity(right_motor, MAX_SPEED);
      }
    // if Phi < 0,  rotate to right
   else if (Phi < 0)
      {
        wb_motor_set_velocity(left_motor, MAX_SPEED);
        wb_motor_set_velocity(right_motor, -MAX_SPEED);
      }
     
     // busy waiting loop until  the above duration elapses;  
    do
        step();
    while (wb_robot_get_time() < start_time + duration);
  
 
  // stop the motors. 
  wb_motor_set_velocity(left_motor, 0);
  wb_motor_set_velocity(right_motor, 0);
}




void passive_wait(double sec) {

double start_time = wb_robot_get_time();

do {

step();

} while (start_time + sec > wb_robot_get_time());
}