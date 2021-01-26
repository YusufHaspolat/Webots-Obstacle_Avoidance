#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
  wb_robot_init();
  int i;
  int front_flag = 0;
  int right_flag = 0;
  int left_flag = 0;
  int right_corn_flag = 0;
  int left_corn_flag = 0;
  int trap_flag_r = 0;
  int trap_flag_l = 0;
  WbDeviceTag ds[3];
  double left_speed;
  double right_speed;
  double ds_values[3];
  /* Defining the distance sensors */
  char ds_names[3][10] = {"frnt", "r_corn", "l_corn"};
  for (i = 0; i < 3; i++) {
    ds[i] = wb_robot_get_device(ds_names[i]);
    wb_distance_sensor_enable(ds[i], TIME_STEP);
  }
  WbDeviceTag wheels[4];
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (i = 0; i < 4; i++) {
    wheels[i] = wb_robot_get_device(wheels_names[i]);
    wb_motor_set_position(wheels[i], INFINITY);
  }
  while (wb_robot_step(TIME_STEP) != -1) {
    left_speed = 1.0;
    right_speed = 1.0;
    if(front_flag){
      if (ds_values[2] < ds_values[1]){
        right_flag = 1;
      }
      else{
        left_flag = 1;
      }
    }
    for (i = 0; i < 3; i++){
      ds_values[i] = wb_distance_sensor_get_value(ds[i]);
    }
    if (ds_values[0] < 650.0){
      front_flag = 1;
    }
    else if (ds_values[1] < 450.0 && ds_values[2] < 450.0){
      if(ds_values[2] < ds_values[1]){
        trap_flag_r = 1;
      }
      else{
        trap_flag_l = 1;
      }
    }
    else if (ds_values[1] < 400.0){
      right_corn_flag = 1;
    }
    else if (ds_values[2] < 400.0){
      left_corn_flag = 1;
    }
    else {
      if (front_flag == 1 && ds_values[0] == 1000.0){
        front_flag = 0;
       }
    }
    if (right_flag){
      left_speed = 1.0;
      right_speed = -1.0;
      left_flag = 0;
      if (ds_values[0] == 1000.0){
        right_flag = 0;
      }
    }
    if (left_flag){
      left_speed = -1.0;
      right_speed = 1.0;
      right_flag = 0;
      if (ds_values[0] == 1000.0){
        left_flag = 0;
      }
    }
    if (right_corn_flag){
      left_speed = -1.0;
      right_speed = 1.0;
      left_corn_flag = 0;
      if (ds_values[1] > 500.0){
        right_corn_flag = 0;
      }
    }
    if (left_corn_flag){
      left_speed = 1.0;
      right_speed = -1.0;
      right_corn_flag = 0;
      if (ds_values[2] > 500.0){
        left_corn_flag = 0;
      }
    }
    if (trap_flag_r){
      left_speed = -1.0;
      right_speed = 1.0;
      trap_flag_l = 0;
      if (ds_values[0] > 800.0 && ds_values[1] > 800.0){
        trap_flag_r = 0;
      }
    }
    if (trap_flag_l){
      left_speed = 1.0;
      right_speed = -1.0;
      trap_flag_r = 0;
      if (ds_values[0] > 800.0 && ds_values[2] > 800.0){
        trap_flag_l = 0;
      }
    }
    wb_motor_set_velocity(wheels[0], left_speed);
    wb_motor_set_velocity(wheels[1], right_speed);
    wb_motor_set_velocity(wheels[2], left_speed);
    wb_motor_set_velocity(wheels[3], right_speed);
  }
  wb_robot_cleanup();
  return 0;  // EXIT_SUCCESS
}
