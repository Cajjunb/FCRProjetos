#include "fcr2017/mapeamento.h"
#include <stdlib.h>

//Indice de acesso dos vetores;
int v_i = 0; 

int main(int argc, char** argv){
    //INICIALIZACAO DO ROS E RODA O ALGORITMO
    ros::init(argc, argv, "mapeamento");
    ros::NodeHandle n;
    Mapeamento obsavoidance(n);
    obsavoidance.spin();
}
