#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include "std_msgs/Float32.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv){
  //Programa que vai pegando a sua entrada e vai passando para o pwm
  std::string menu;
  float left;
  float right;
  //Inicializando node para publicar para o gazebo
  ros::init(argc, argv, "gazeboMensagem");
  //HANDLE
  ros::NodeHandle objHandle;
  //Publishers para os dois topicos onde vamos publicar as mensagens
  ros::Publisher objPublisherLeft = objHandle.advertise<std_msgs::Float32>("v_left",400);
  ros::Publisher objPublisherRight = objHandle.advertise<std_msgs::Float32>("v_right",400);
  //Frame Rate
    ros::Rate loop_rate(10);
  //TRANSFORMANDO PARA O FORMATO DA MENSAGEM QUE O ROS PODE PASSAR VIA PUBLISH
  std_msgs::Float32 v_left;
  std_msgs::Float32 v_right;  
  do{
    //LEITURA DOS ARGUMENTOS A SEREM PASSADOS
    cout << "Velocidade v_left = ";
    cin >> left;
    cout << "Velocidade v_right = ";
    cin >> right;
    while(!ros::ok)
      cout << "\tEsperando ros...\n";
    v_left.data = left ;
    v_right.data = right;

    objPublisherLeft.publish(v_left);
    objPublisherRight.publish(v_right);

    cout << "\tQuer mandar mais uma mensagem? (quit)\n";
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    getline(cin,menu);
  }while(menu.compare("quit"));

  return 0 ;
}