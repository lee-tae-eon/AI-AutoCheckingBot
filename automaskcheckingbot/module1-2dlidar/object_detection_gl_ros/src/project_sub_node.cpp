
#include <ros/ros.h>
#include "geometry_msgs/Pose2D.h"
#include "object_detection_gl_ros/obj_array.h" 
#include "object_detection.h"
#include <cmath> 


void msgCallback_3(const object_detection_gl_ros::obj_array::ConstPtr& obj_msg)
{	

	int size = 0;

	float r =0.0;
	int degree=0;
	float angle = 57.295792;
	float x = 0.0;
	float y = 0.0;
	float t = 0.0;

	ros::NodeHandle nh2;
	ros::Publisher arduino_pub=nh2.advertise<geometry_msgs::Pose2D>("arduino_position",1);

	ros::Rate loop_rate(10); 
	geometry_msgs::Pose2D position_msg;
	
	size = obj_msg -> xy.size();
	ROS_INFO("number of obj=%d", size);


	for(int i=0; i< size; i++){

		x = obj_msg -> xy[i].x; // 퍼블리시 노드에서 가져온 물체의 x좌표
		y = obj_msg -> xy[i].y; // 퍼블리시 노드에서 가져온 물체의 y좌표
		t = atan2(y,x); // 라이다와 물체 사이의 각도(theta) 
		r = sqrt((x*x) + (y*y)); // 객체와 라이다 사이의 거리
		degree = floor(t * angle); // 라이다와 물체 사이의 각도(degree) 

		ROS_INFO("(x,y) of %d = (%lf, %lf)", i,  x , y);
		ROS_INFO("radian of %d = %lf" ,i, t);
		ROS_INFO("degree of %d = %d", i , degree);
		ROS_INFO("distance of %d = %lf", i , r);		
		ROS_INFO(" ");
	
		position_msg.x = x;
		position_msg.y = y;
		position_msg.theta= t;

		arduino_pub.publish(position_msg);

	}

	loop_rate.sleep(); 
	ros::spinOnce();
	
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "project_sub_node");
	ros::NodeHandle nh; 


	ros::Subscriber obj_sub=nh.subscribe("position", 1, msgCallback_3);

	
	ros::spin();

	
	return 0;

}

