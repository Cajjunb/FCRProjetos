//INCLUDE GUARD
#ifndef MOD_MAP
#define MOD_MAP
#include <stdio.h>
#include <vector>
#include <string>
#include <ros/ros.h>
#include <ros/console.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <p2os_msgs/SonarArray.h>
#include <nav_msgs/Odometry.h>
//Include vetor
//Include vector de vetores
#include <vector>
//INCLUDE TFs para utilizar o angulo de odometria do robo
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
//vetor
//grafo
#include "vetor.h"
#include "graphMap.h"


#define ANGULOS_POR_GRUPO 40
#define NRO_GRUPOS 18
#define TEMPO_POR_SPIN 0.005
#define CONSTANTE_FORCA 20
#define CONSTANTE_DESVIO 2.7
#define VELOCIDADE_MAXIMA_LINEAR 10
#define VELOCIDADE_MAXIMA_ANGULAR 4
#define PESO_ROBO 1
#define RAIO_ROBO 1.4
#define RANGE_ERRO 0.1
#define NODE_INICIAL 6
#define RANGE_SEGURANCA_MAX 0.5
#define RANGE_SEGURANCA_MIN 2.0

using namespace std;

class Mapeamento{
public:
    // VETOR GLOBAL DE VETORES DO CAMPO OBSTACLE AVOIDANCE
    static Vetor vetorResultante;
    void spin();
    void setParametros(float x,float y);
    Mapeamento(ros::NodeHandle nh);
    ~Mapeamento();

private:
    ros::NodeHandle nh_;
    ros::Publisher vel_pub_;
    ros::Subscriber laser_sub_, sonar_sub_, pose_sub_, dsr_sub_;

    geometry_msgs::Twist command_vel_, desired_vel_;
    sensor_msgs::LaserScan scan_msg_;
    p2os_msgs::SonarArray sonar_msg_;
    nav_msgs::Odometry pose_msg_;

    float x_desejado ;
    float y_desejado ;
    float z_desejado ;


    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg);
    void sonarCallback(const p2os_msgs::SonarArray::ConstPtr& sonar_msg);
    void dsrCallback(const geometry_msgs::Twist::ConstPtr& desired_vel);
    void poseCallback(const nav_msgs::Odometry::ConstPtr& pose_msg);

    bool navegarGrafo();
    void desvieObstaculo();
    void mapearNode(Node *nodeAtual);
};


#endif