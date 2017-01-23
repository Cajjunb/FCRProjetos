#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
//Includes CPP
#include <iostream>
#include <sstream>
using namespace std;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

void odomCallBack(const nav_msgs::Odometry::ConstPtr& msg)
{
    double x,y, yaw;
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;
    yaw = tf::getYaw(msg->pose.pose.orientation);

    ROS_INFO("pose: x = %lf, y = %lf, yaw = %lf", x, y, yaw);
}

int main(int argc, char **argv)
{  
  ros::init(argc, argv, "odometria_teste");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("pose", 10, odomCallBack);


  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}