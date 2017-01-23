#include "fcr2017/odometria_campo.h"
#include <stdlib.h>

//Indice de acesso dos vetores;
int v_i = 0; 

int main(int argc, char** argv){
    //INICIALIZACAO DO ROS E RODA O ALGORITMO
    ros::init(argc, argv, "odometria_campo");
    ros::NodeHandle n;
    OdometriaCampo obsavoidance(n);
    obsavoidance.spin();
}
