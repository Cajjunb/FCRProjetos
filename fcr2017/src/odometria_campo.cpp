#include "fcr2017/odometria_campo.h"
#include "fcr2017/graph.h"

OdometriaCampo::OdometriaCampo(ros::NodeHandle nh) : nh_(nh){
    vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    laser_sub_ = nh_.subscribe("hokuyo_scan", 10, &OdometriaCampo::laserCallback, this);
    dsr_sub_ = nh_.subscribe("desired_vel", 10, &OdometriaCampo::dsrCallback, this);
    sonar_sub_ = nh_.subscribe("sonar", 10, &OdometriaCampo::sonarCallback, this);
    pose_sub_ = nh_.subscribe("pose", 10, &OdometriaCampo::poseCallback, this);
}


void OdometriaCampo::setParametros(float x,float y){
    this->x_desejado = x;
    this->y_desejado = y;
}


void OdometriaCampo::laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg)
{
    this->scan_msg_ = *laser_msg;
}

void OdometriaCampo::poseCallback(const nav_msgs::Odometry::ConstPtr& pose_msg)
{
    this->pose_msg_ = *pose_msg;
}

void OdometriaCampo::dsrCallback(const geometry_msgs::Twist::ConstPtr& desired_vel){
    this->desired_vel_ = *desired_vel;
}

void OdometriaCampo::sonarCallback(const p2os_msgs::SonarArray::ConstPtr& sonar_msg){
    this->sonar_msg_ = *sonar_msg;
}

bool OdometriaCampo::algorithm(){
    //Velocidades lineares e angulares novas
    double new_linear_vel;
    double new_angular_vel ;
    command_vel_.linear.x = new_linear_vel  ;
    command_vel_.angular.z = new_angular_vel;
    //Nro utilizado para calcular o modulo da forca
    //Nro utilizado para calcular o modulo da acelaracao,
    //Velocidade utilizado mod_x e Velocidade utilizado mod_y
    double mod_v_x_robo;
    double mod_v_y_robo;
    double mod_forca ;
    double mod_aceleracao;
    double catetoAdjascente  =  y_desejado - this->pose_msg_.pose.pose.position.y  ;
    double catetoOposto      =  x_desejado - this->pose_msg_.pose.pose.position.x  ;
    double hipotenusa        = sqrt(pow(catetoOposto,2)+pow(catetoAdjascente,2));
    //Velocidade utilizado costheta cateto adjacente , Velocidade utilizado costheta e distancia entre pontos
    // ANGULO de odometria do robo yaw
    double costheta = catetoAdjascente / hipotenusa  ;
    double sentheta = catetoOposto / hipotenusa  ;
    double yaw = tf::getYaw(this->pose_msg_.pose.pose.orientation);
    //Nro utilizado para calcular o modulo da acelaracao
    //boolean
    double aux;
    bool final_state1 = false;
    bool final_state2 = false;
    //Calcular a distancia entre o ponto desejado e o do robo
    aux = pow(x_desejado - this->pose_msg_.pose.pose.position.x,2 );
    aux += pow(y_desejado - this->pose_msg_.pose.pose.position.y,2);
    //Calculado mod_forca
    mod_forca = sqrt(aux) * CONSTANTE_FORCA ;
    //Calculadno mod_aceleracao f = m*a
    mod_aceleracao = mod_forca / PESO_ROBO;
    //Calculando a velocidade linear
    mod_v_x_robo = mod_aceleracao * costheta * TEMPO_POR_SPIN;
    cout <<    "\tSEN THETA = " << this->pose_msg_.pose.pose.orientation.w <<"\t COS THETA"<<
                         this->pose_msg_.pose.pose.orientation.z <<"\n";
    //Calculando a velocidade angular
    mod_v_y_robo = mod_aceleracao * sentheta * TEMPO_POR_SPIN;

    new_linear_vel = desired_vel_.linear.x + mod_v_x_robo * sin(yaw) + mod_v_y_robo * cos(yaw);
    new_angular_vel = desired_vel_.angular.z + mod_v_x_robo * cos(yaw) - mod_v_y_robo * sin(yaw) * RAIO_ROBO;
    //getchar();


    /*debug*/cout <<"\tvlin = "<< new_linear_vel <<"\n" ;
    /*debug*///cout <<"\tvang = "<< new_angular_vel <<"\n" ;

    //CHEGOU NO PONTO
    if(abs(new_linear_vel) < RANGE_ERRO ){
        new_linear_vel = 0;
        final_state1 = true;
    }
    if(abs(new_angular_vel) < RANGE_ERRO){
        new_angular_vel = 0;
        final_state2 = true;
    }
    //EVITANDO A RE
    if(abs(new_linear_vel) < 0)
        new_angular_vel += 0.2;
    //VELOCIDADE MAXIMA
    if(abs(new_linear_vel) > VELOCIDADE_MAXIMA_LINEAR ){
        new_linear_vel = VELOCIDADE_MAXIMA_LINEAR;
    }
    if(abs(new_angular_vel) > VELOCIDADE_MAXIMA_ANGULAR){
        new_angular_vel = VELOCIDADE_MAXIMA_ANGULAR;
    }
    command_vel_.linear.x = new_linear_vel ;
    command_vel_.angular.z = new_angular_vel ;    
    if(final_state1 && final_state2)
        return true;
    else
        return false;
}

void OdometriaCampo::spin(){
    //Ponteiro de arquivo
    //Argumentos x e y para o algoritmo captado pelo grafo
    //GRAFO Geral
    //NODE ALVO AUXILIAR
    //Booleano de flag para parar o algoritmo de ir de ponto a ponto
    // inteiros ids para insercao dos Nodes
    FILE *fp;
    double x_arg, y_arg;
    Grafo g1;
    Node *nodeAlvo;
    bool chegou ;
    ros::NodeHandle n;
    int id1= 0, id2 = 0 ;
    //Tratamento arquivo Nodes!
    fp = fopen("/home/caju/catkin_ws/src/fcr2017/src/cic.txt","r");
    if(fp == NULL){
        ROS_ERROR("\t NAO FOI ENCONTRADO O ARQUIVO DE GRAFO E MAP\n");
        return ;
    }
    while( fscanf(fp,"%lf%*c%lf%*c",&x_arg,&y_arg) != EOF){
        g1.addNode(x_arg,y_arg);
    }
    //Tratamento arquivo ARESTAS!
    fp = fopen("/home/caju/catkin_ws/src/fcr2017/src/cicArestas.txt","r");
    if(fp == NULL){
        ROS_ERROR("\t NAO FOI ENCONTRADO O ARQUIVO DE ARESTAS E MAP\n");
        return ;
    }
    while( fscanf(fp,"%d%*c%d%*c%lf",&id1,&id2,&x_arg) != EOF){
        g1.addAresta(id1,id2,x_arg);
    }
    //Configuracao FRAME RATE
    ros::Rate loop_rate(10);
    /*debug*///g1.printaGrafo();
    //LOOP DE IR PONTO A PONTO
    do{
        //PEGA O NODE ALVO PARA IR PARA O ALGORITMO
        printf("\tDigite o id do ponto \n");
        scanf("%lf",&x_arg);
        nodeAlvo = g1.getNode(x_arg);
        /*DEBUG*///nodeAlvo->getNodesVizinhos();
        this->setParametros(nodeAlvo->getx(),nodeAlvo->gety());
        //FLAG DE FINAL DO ALGORITMO
        chegou = false; 
       /* while(ros::ok() && !chegou)
        {
            ros::spinOnce();
            if(!chegou) 
                chegou = algorithm();
            vel_pub_.publish(command_vel_);
            loop_rate.sleep();
        }*/
    }while(x_arg > 0);

}

OdometriaCampo::~OdometriaCampo()
{
}
