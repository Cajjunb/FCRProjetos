
#include <sstream>
#include "std_msgs/Float32.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

  
  
#include "ros/ros.h"
#include <string>
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"

std_msgs::Float32 vright, vleft;

void rightVelocityCallback(const std_msgs::Float32::ConstPtr& msg)
{
    vright.data = msg->data;
}

void leftVelocityCallback(const std_msgs::Float32::ConstPtr& msg)
{
    vleft.data = msg->data;
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "forward_kinematic_pioneer");
    ros::NodeHandle n;
    std::string right = "/v_right";
    std::string left = "/v_left";
    ros::Subscriber right_sub = n.subscribe(right, 10, rightVelocityCallback);
    ros::Subscriber left_sub = n.subscribe(left, 10, leftVelocityCallback);
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

    ros::Rate loop_rate(100);
    loop_rate.sleep();

    float vel_lin, vel_ang;
    vright.data = vleft.data = 0.0;
    vel_lin = vel_ang = 0.0;
    //Programa que vai pegando a sua entrada e vai passando para o pwm
    std::string menu;
    geometry_msgs::Twist vel;

    while(ros::ok()){
        do{
          sleep(1);
          for(int i=0; i < 4; i++){
                        //Anda reto
            cout << "\tMANDEI MENSAGEM: RETA)\n";
            // forward kinematics
            vel_lin = 1.0;
            vel_ang = 0;
            vel.linear.x = vel_lin;
            vel.angular.z = vel_ang;

            vel_pub.publish(vel);

            sleep(2);
            //ros::spinOnce();
            //Girar para esquerda
            cout << "\tMANDEI MENSAGEM: GIRAR)\n";
            // forward kinematics
            vel_lin = 0.0;
            vel_ang = 1.0;
            vel.linear.x = vel_lin;
            vel.angular.z = vel_ang;
            vel_pub.publish(vel);
            sleep(2.5);   
            //ros::spinOnce();
          }
          vel_lin = 0.0;
          vel_ang = 0;
          
          vel.linear.x = vel_lin;
          vel.angular.z = vel_ang;

          vel_pub.publish(vel);

          cout << "\tQuer mandar mais uma mensagem? (quit)\n";
          cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
          getline(cin,menu);
        }while(menu.compare("quit"));
        
        ros::spinOnce();
    }
}

