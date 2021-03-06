
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include "visualization_msgs/MarkerArray.h"
#include "geometry_msgs/Pose2D.h"
#include "object_detection.h"
#include <cmath> // 추가 
#include "object_detection_gl_ros/obj_array.h" // 추가 



int idx = 0;
float min_x= 0.0;
float min_y = 0.0;
float min_theta = 0.0;



void msgCallback_1(const sensor_msgs::LaserScan::ConstPtr& laser_msg)
{

	//angle_max = laser_msg -> angle_max;
	//angle = 180 / angle_max; // 1라디안과 같은 degree
	//ROS_INFO("angle=%f", angle);

}


void msgCallback_2(const visualization_msgs::MarkerArray::ConstPtr& marker_msg)
{	
	/*int size = marker_msg -> markers.size();
	ROS_INFO("obj_size=%d", size);
	/*for (int i=0; i<size; i++)
	{	
		ROS_INFO("x of %d = %f" ,i , marker_msg -> markers[i].pose.position.x);
		ROS_INFO("y of %d = %f" ,i , marker_msg -> markers[i].pose.position.y);
	}*/

}



void msgCallback_3(const object_detection_gl_ros::obj_array::ConstPtr& obj_msg)
{
	//ros::NodeHandle nh_pub;
	
	//ros::Rate loop_rate(10); 

	int size = 0;
	//float degree=0.0;
	float min_r = 10000.0;
    	float x = 0.0;
	float y = 0.0;
	float t = 0.0;
	float r =0.0;
	float degree = 0.0;
	//float angle = 57.295792;

	
	size = obj_msg -> xy.size();
	ROS_INFO("number of obj=%d", size);


	for(int i=0; i< size; i++){

		x = obj_msg -> xy[i].x; // 퍼블리시 노드에서 가져온 물체의 x좌표
		y = obj_msg -> xy[i].y; // 퍼블리시 노드에서 가져온 물체의 y좌표
		t = atan2(y,x); // 라이다와 물체 사이의 각도(theta) 
		r = sqrt((x*x) + (y*y)); // 객체와 라이다 사이의 거리
		degree = t * 57.295792; // 라이다와 물체 사이의 각도(degree) 
		//degree = floor(t * angle);
		
		ROS_INFO("(x,y) of %d = (%.2f, %.2f)", i,  x , y);
		ROS_INFO("radian of %d = %.2f" ,i, t);
		//cout << fixed;
		//cout.precision(2);
		//cout<< degree << endl;
		//round(degree *pow(10,3)) / pow(10,3)
		ROS_INFO("degree of %d = %.2f", i , degree);

		ROS_INFO("distance of %d = %.2fm", i , r);		

		if(r < min_r)
		{
			idx = i;
			min_r = r;
			min_x = x;
			min_y = y;
			min_theta = t;
		}


		//ROS_INFO("min_distance(obj_number = %d) = %f", idx, min_r);
		ROS_INFO(" ");
	}
	
}


void msgCallback_4(const visualization_msgs::MarkerArray::ConstPtr& ard_msg){

}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "object_detection_sub_node"); // 노드명 초기화
	ros::NodeHandle nh; // ROS 시스템과 통신을 위한 노드핸들 선언
	//ros::AsyncSpinner spinner(0);
	//spinner.start();

	//ros::Subscriber detection_sub1=nh.subscribe("raw_lidar", 1, msgCallback_1);
	//ros::Subscriber detection_sub2=nh.subscribe("marker_array", 1, msgCallback_2); 
	ros::Subscriber obj_sub=nh.subscribe("position", 1, msgCallback_3);
	//ros::Subscriber arduino_sub=nh.subscribe("arduino", 1, msgCallback_4);

	ros::Publisher arduino_pub=nh.advertise<geometry_msgs::Pose2D>("min_position",1);
	ros::Rate loop_rate(20); // 0.5 초 -> 0.1초로 수정 
	geometry_msgs::Pose2D msg;

	while(ros::ok()){
		msg.x = min_x;
		msg.y = min_y;
		msg.theta= min_theta;
		//ROS_INFO("min_psition(obj_number = %d) =(%f, %f)", idx, msg.x , msg.y);
		//ROS_INFO(" ");
		arduino_pub.publish(msg);
		loop_rate.sleep(); 
		ros::spinOnce();

	}
	
	ros::spin();
	//ros::waitForShutdown();
	
	return 0;

}

