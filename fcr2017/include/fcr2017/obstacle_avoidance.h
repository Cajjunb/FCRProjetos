//INCLUDE GUARD
#ifndef MOD_AVOID
#define MOD_AVOID

#include <vector>
#include <string>
#include <ros/ros.h>
#include <ros/console.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <p2os_msgs/SonarArray.h>
#include <nav_msgs/Odometry.h>
//Include vetor
#include "vetor.h"
//Include vector de vetores
#include <vector>


#define ANGULOS_POR_GRUPO 80
#define NRO_GRUPOS 9
#define TEMPO_POR_SPIN 0.005
#define PESO_ROBO 20
#define RAIO_ROBO 1.0
#define RANGE_SEGURANCA_MAX 0.5
#define RANGE_SEGURANCA_MIN 1.5

class ObstacleAvoidance
{
public:
    // VETOR GLOBAL DE VETORES DO CAMPO OBSTACLE AVOIDANCE
    static Vetor vetorResultante;
    ObstacleAvoidance(ros::NodeHandle nh);
    void spin();
    ~ObstacleAvoidance();

private:
    ros::NodeHandle nh_;
    ros::Publisher vel_pub_;
    ros::Subscriber laser_sub_, sonar_sub_, pose_sub_, dsr_sub_;

    geometry_msgs::Twist command_vel_, desired_vel_;
    sensor_msgs::LaserScan scan_msg_;
    p2os_msgs::SonarArray sonar_msg_;
    nav_msgs::Odometry pose_msg_;


    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg);
    void sonarCallback(const p2os_msgs::SonarArray::ConstPtr& sonar_msg);
    void dsrCallback(const geometry_msgs::Twist::ConstPtr& desired_vel);
    void poseCallback(const nav_msgs::Odometry::ConstPtr& pose_msg);

    void algorithm();
};


#endif