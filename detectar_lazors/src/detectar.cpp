#include "vetor.h"
//Includes ros
#include "ros/ros.h"
#include "std_msgs/String.h"
//Include tipo sonar array 
#include "sensor_msgs/LaserScan.h"
//Include vetor
#include "vetor.cpp"
//Include vector de vetores
#include <vector>
//Vetor global dos vetores de campo
std::vector<Vetor> vetoresCampo;
//Indice de acesso dos vetores;
int v_i = 0; 


void lazorCallback(const sensor_msgs::LaserScan msg){
  float nro_angulos = (msg.angle_max - msg.angle_min) / msg.angle_increment ;
  //cout << "\t Max = "<< msg.angle_max << "\t min = "<< msg.angle_min <<"\t increment = " << msg.angle_increment ;
  //cout << "\n\t nro de angulos =   " << nro_angulos;
  int j = 0;
  int min_index = -1;
  float min_distancia = 60;
  for (int i = 0; i < nro_angulos; ++i ,++j){
    /*debug*///cout <<"\t range ="<<  msg.ranges[i]<< "\t angulo = " <<  (msg.angle_min + ( i * msg.angle_increment )*180 )
    //                                                          / M_PI <<" \n";
    // Testa se eh a minima distancia e esta no intervalor ainda do subgrupo
    if(min_distancia > msg.ranges[i] && j < 36){
      min_distancia = msg.ranges[i];
      min_index = i;
    }
    else if( j >= 80){
      cout << "\n\tmin distancia do subgrupo = "<< min_distancia << "\tResultado = "<< msg.angle_min + ( min_index * msg.angle_increment ) << " \t modulo = " << msg.range_max - min_distancia; 
      j = 0;
      vetoresCampo[v_i].setAngulo(msg.angle_min + ( min_index * msg.angle_increment ), msg.range_max - min_distancia );
      min_distancia = 60;
      v_i++;
    }
  }
  for (int i = 0; i < 9; ++i){
    cout << "\n\tVetor do subgrupo nro  "<< i << "\t x ="<< vetoresCampo[i].componente_x << " y = " <<vetoresCampo[i].componente_y ; 
  }
  getchar();
}

int main(int argc, char **argv){
  ros::init(argc, argv, "detectar_lazors");
  ros::NodeHandle n;
  for (int i = 0; i < 20; ++i){
    vetoresCampo.push_back(Vetor(0,0));  
  } 
  ros::Subscriber sub = n.subscribe("hokuyo_scan", 1000, lazorCallback);
  ros::spin();
  return 0;
}