#include <sstream>
#include <stdio.h>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
//Include tipo sonar array 
#include "sensor_msgs/LaserScan.h"
using namespace std;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const p2os_msgs::SonarArray msg)
{
  int i ;
  printf("\t ranges_count %d\n" , msg.ranges_count);
  for ( i = 0; i < msg.ranges_count; ++i){
    cout<< "\t i =" << i <<" \t valor = " << msg.ranges[i];
  }

  getchar();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "detectar_lazors");

  ros::NodeHandle n;
 

  ros::Subscriber sub = n.subscribe("sonar", 1000, chatterCallback);
  while(true) 
    ros::spin();

  return 0;
}