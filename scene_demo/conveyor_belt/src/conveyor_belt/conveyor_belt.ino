#include "main.h"
#define SOFTWARE_VERSION "V1.2.5\n"
extern enum control_systerm system_s;
extern enum pick_mode_e mode;
bool work_state = false;
bool detect_switch_flag = false;
extern Adafruit_TCS34725 tcs;
Ultrasonic ultrasonic(23);
extern int i;

// ros
#include <ros.h>
#include <std_msgs/Bool.h>
ros::NodeHandle nh;
std_msgs::Bool is_conveyor_on;
ros::Publisher pub("is_conveyor_on", &is_conveyor_on);

void start_cb(const std_msgs::Bool &msg){
  if(msg.data){
    set_speed(1, 200, NULL, false);  // modify direction from belt_move()
    is_conveyor_on.data = true;
  }
}

void stop_cb(const std_msgs::Bool &msg){
  if(msg.data){
    belt_stop();
    is_conveyor_on.data = false;
  }
}

ros::Subscriber<std_msgs::Bool> sub_start("start_conveyor", &start_cb);
ros::Subscriber<std_msgs::Bool> sub_stop("stop_conveyor", &stop_cb);

void setup() {
  Serial.begin(115200);
  Serial.println(SOFTWARE_VERSION);
  Serial2.begin(115200);
  Serial3.begin(115200);
  pinMode(25, INPUT_PULLUP);
  belt_init();
  oled_init();
  convert_rgb();
  //initiate stepper driver
  step_init_ll();
  uarm_reset();
  tcs.begin();
  system_s = conver_belt;
  i = 0;
  mode = NOTHING_MODE;
  delay(1000);

   // ros
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub_start);
  nh.subscribe(sub_stop);
  nh.advertise(pub);
}
void loop() {
  first_page();
  //get_color();
  //conver_work();
  //belt_move();

  //ros
  nh.spinOnce();
  pub.publish(&is_conveyor_on);
}
