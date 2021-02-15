
#include<SoftwareSerial.h>
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <geometry_msgs/Pose2D.h>


SoftwareSerial my_serial(10,11);
ros::NodeHandle  nh;

float data[3];

void msg_cb(const geometry_msgs::Pose2D& position_msg){
    data[0] = position_msg.x;
    data[1] = position_msg.y;
    data[2] = position_msg.theta;
    
    my_serial.print(data[0], 6);
    my_serial.print(',');
    my_serial.print(data[1], 6);
    my_serial.print(',');
    my_serial.print(data[2], 6);
    my_serial.print(',');

  }



ros::Subscriber<geometry_msgs::Pose2D> sub("min_position",&msg_cb);


void setup() {
  my_serial.begin(38400); 
  nh.initNode();
  nh.subscribe(sub);
  //nh.subscribe(sub2);
}

void loop() {

  //delay(10);
  nh.spinOnce();
  delay(10);
  //nh.spinOnce();


}
