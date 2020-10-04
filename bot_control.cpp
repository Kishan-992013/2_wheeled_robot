#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include <iostream>

float dist1,dist2,dist3;

void front_callback(const sensor_msgs::LaserScan::ConstPtr& scan){
  int n,i;
  n=scan->ranges.size();
  float sum=0;
  for (i=12;i<=16;i++)
  {
    sum+=scan->ranges[i];
  }
  dist1=sum/5;
  ROS_INFO("front_avg:%f",dist1);
}

void right_callback(const sensor_msgs::LaserScan::ConstPtr& scan){
  int n,i;
  n=scan->ranges.size();
  float sum=0;
  for (i=16;i<21;i++)
  {
    sum+=scan->ranges[i];
  }
  dist2=sum/5;
  ROS_INFO("right_avg:%f",dist2);
}

void left_callback(const sensor_msgs::LaserScan::ConstPtr& scan){
  int n,i;
  n=scan->ranges.size();
  float sum=0;
  for (i=10;i<15;i++)
  {
    sum+=scan->ranges[i];
  }
  dist3=sum/5;
  ROS_INFO("left_avg:%f",dist3);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv,"sensor_front");
  ros::NodeHandle nsf;
  ros::Subscriber sub1 = nsf.subscribe<sensor_msgs::LaserScan>("/sensor_front/laser/scan",1,front_callback);

  ros::init(argc, argv,"sensor_right");
  ros::NodeHandle nsr;
  ros::Subscriber sub2 = nsr.subscribe<sensor_msgs::LaserScan>("/sensor_right/laser/scan",1,right_callback);

  ros::init(argc, argv,"sensor_left");
  ros::NodeHandle nsl;
  ros::Subscriber sub3 = nsl.subscribe<sensor_msgs::LaserScan>("/sensor_left/laser/scan",1,left_callback);

  ros::init(argc, argv,"my_2wheel_bot");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

  geometry_msgs::Twist vel;
  vel.linear.x=0.0;
  vel.linear.y=0.0;
  vel.linear.z=0.0;
  vel.angular.x=0.0;
  vel.angular.y=0.0;
  vel.angular.z=0.0;
  ros::Rate loop_rate(1);
  while (ros::ok())
  {
    if (dist1>1.5)
    {
      if (dist2<1 && dist3<1)
      {
        vel.linear.x=0.2;
        vel.angular.z=0;
        pub.publish(vel);
        ROS_INFO("Moving forward");
      }
      else if (dist2>1 && dist3<1)
      {
        vel.linear.x=0.2;
        vel.angular.z=-0.05;
        pub.publish(vel);
        ROS_INFO("turning right");
      }
      else if (dist2<1 && dist3>1)
      {
        vel.linear.x=0.2;
        vel.angular.z=0.05;
        pub.publish(vel);
        ROS_INFO("Turning left");
      }
      else
      {
        vel.linear.x=0;
        vel.angular.z=0;
        pub.publish(vel);
        ROS_INFO("stopped");
      }
    }
    else if (dist1<1.5)
    {
      if (dist2>dist3)
      {
        vel.linear.x=0.1;
        vel.angular.z=-0.1;
        pub.publish(vel);
        ROS_INFO("turning right");
      }
      else if (dist2<dist3)
      {
        vel.linear.x=0.1;
        vel.angular.z=0.1;
        pub.publish(vel);
        ROS_INFO("Turning left");
      }
      else
      {
        vel.linear.x=0;
        vel.angular.z=0;
        pub.publish(vel);
        ROS_INFO("stopped");
      }
    }

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
