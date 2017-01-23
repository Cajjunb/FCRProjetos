#include "fcr2017/obstacle_avoidance.h"

//Indice de acesso dos vetores;
int v_i = 0; 

int main(int argc, char** argv)
{
    ros::init(argc, argv, "obsavoidance");
    ros::NodeHandle n;
    ObstacleAvoidance obsavoidance(n);
    obsavoidance.spin();
}
