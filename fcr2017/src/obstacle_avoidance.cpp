#include "fcr2017/obstacle_avoidance.h"

// VETOR GLOBAL DE VETORES DO CAMPO OBSTACLE AVOIDANCE DECLARACAO
Vetor ObstacleAvoidance::vetorResultante;
int vis_i = 0;

ObstacleAvoidance::ObstacleAvoidance(ros::NodeHandle nh) : nh_(nh)
{
    vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    laser_sub_ = nh_.subscribe("hokuyo_scan", 10, &ObstacleAvoidance::laserCallback, this);
    dsr_sub_ = nh_.subscribe("desired_vel", 10, &ObstacleAvoidance::dsrCallback, this);
    sonar_sub_ = nh_.subscribe("sonar", 10, &ObstacleAvoidance::sonarCallback, this);
    pose_sub_ = nh_.subscribe("pose", 10, &ObstacleAvoidance::poseCallback, this);
}

void ObstacleAvoidance::laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg)
{
    this->scan_msg_ = *laser_msg;
}

void ObstacleAvoidance::poseCallback(const nav_msgs::Odometry::ConstPtr& pose_msg)
{
    this->pose_msg_ = *pose_msg;
}
void ObstacleAvoidance::dsrCallback(const geometry_msgs::Twist::ConstPtr& desired_vel){
    this->desired_vel_ = *desired_vel;
}

void ObstacleAvoidance::sonarCallback(const p2os_msgs::SonarArray::ConstPtr& sonar_msg){
    this->sonar_msg_ = *sonar_msg;
}

void ObstacleAvoidance::algorithm(){
    //Velocidades lineares e angulares novas
    double new_linear_vel;
    double new_angular_vel ;
    //ESCREVA SEU CODIGO AQUI
    command_vel_.linear.x = new_linear_vel  ;
    command_vel_.angular.z = new_angular_vel;
    
    //Nro de angulos feito por uma conta simples
    float nro_angulos = (this->scan_msg_.angle_max - this->scan_msg_.angle_min) / this->scan_msg_.angle_increment ;
    //Nro utilizado para eleger o minimo range em um subgrupo
    float min_distancia = this->scan_msg_.range_max;
    // indices para ser feito a eleicao dos minimos em cada subgrupo
    int j = 0;
    int min_index = -1;
    int emergence_coeficiente = 1;
    //boolean
    bool emergence_state = false;
    //Feito para nao quebrar o codigo
    for (int i = (ANGULOS_POR_GRUPO*1)-1; i < nro_angulos - ANGULOS_POR_GRUPO*1; ++i ,++j){
      /*debug*///cout <<"\t range ="<<  this->scan_msg_.ranges[i]<< "\t angulo = " <<  (this->scan_msg_.angle_min + ( i * this->scan_msg_.angle_increment )*180 )
      //                                                          / M_PI <<" \n";
      // Testa se eh a minima distancia e esta no intervalor ainda do subgrupo
      if(min_distancia > this->scan_msg_.ranges[i] && j < ANGULOS_POR_GRUPO){
        min_distancia = this->scan_msg_.ranges[i];
        min_index = i;
      }
      else if( j >= ANGULOS_POR_GRUPO){
        if(min_distancia > RANGE_SEGURANCA_MIN){
            ObstacleAvoidance::vetorResultante.somaVetorial(this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ),
                                                0,
                                                true);
            
        }else{
            ObstacleAvoidance::vetorResultante.somaVetorial(this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ),
                                                min_distancia,
                                                true);            
        }
        if(min_distancia < RANGE_SEGURANCA_MAX){
            emergence_state = true;
        }

        /*debug*///cout << "\n\tmin distancia do subgrupo = "<< min_distancia << "\tResultado = "<< this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ) << " \t modulo = " << this->scan_msg_.range_max - min_distancia; 
        j  = 0;
        min_distancia = this->scan_msg_.range_max;
      }
    }
    double angulo_resultante = ObstacleAvoidance::vetorResultante.angulo;
    //Convertendo para vetores
    //Contribuicao da aceleracao  na velocidade em um instante V = V0 + a t  t sendo infinitesimal
    ObstacleAvoidance::vetorResultante.multiplicarEscalar(TEMPO_POR_SPIN * PESO_ROBO);
    // Vetor rotacionado para um plano perpendicular na direcao do vetor 
    double e1cos = ObstacleAvoidance::vetorResultante.componente_x * cos( ((M_PI/2)- angulo_resultante)) ;
    double e1sen = ObstacleAvoidance::vetorResultante.componente_x * sin( ((M_PI/2)- angulo_resultante)) ;
    double e2cos = ObstacleAvoidance::vetorResultante.componente_y * cos( ((M_PI/2)- angulo_resultante)) ;
    double e2sen = ObstacleAvoidance::vetorResultante.componente_y * sin( ((M_PI/2)- angulo_resultante)) ;
    new_linear_vel = desired_vel_.linear.x  - (e1sen + e2cos);
    new_angular_vel = desired_vel_.angular.z  - (e1cos - e2sen) * RAIO_ROBO ;
    //getchar();

    if(emergence_state)
        emergence_coeficiente = 0;
    command_vel_.linear.x = new_linear_vel * emergence_coeficiente ;
    command_vel_.angular.z = new_angular_vel ;
    /*
        cout << "\n\t vel_lin =" << command_vel_.linear.x << "\t e1cos" << e1cos <<"\t e1sen"<< e1sen;
        cout << "\n\t vel_ang =" << command_vel_.angular.z << "\t e1cos" << e2cos <<"\t e1sen"<< e2sen ;
        vis_i = 0;
    */
}

void ObstacleAvoidance::spin()
{
    ros::Rate loop_rate(50);
    while(ros::ok())
    {
        ros::spinOnce();
        algorithm();
        vel_pub_.publish(command_vel_);
        loop_rate.sleep();
    }
}

ObstacleAvoidance::~ObstacleAvoidance()
{
}
