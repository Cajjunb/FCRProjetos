#include "fcr2017/mapeamento.h"


// VETOR GLOBAL DE VETORES DO CAMPO OBSTACLE AVOIDANCE DECLARACAO
Vetor Mapeamento::vetorResultante;

Mapeamento::Mapeamento(ros::NodeHandle nh) : nh_(nh){
    vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    laser_sub_ = nh_.subscribe("hokuyo_scan", 10, &Mapeamento::laserCallback, this);
    dsr_sub_ = nh_.subscribe("desired_vel", 10, &Mapeamento::dsrCallback, this);
    sonar_sub_ = nh_.subscribe("sonar", 10, &Mapeamento::sonarCallback, this);
    pose_sub_ = nh_.subscribe("pose", 10, &Mapeamento::poseCallback, this);
}


void Mapeamento::setParametros(float x,float y){
    this->x_desejado = x;
    this->y_desejado = y;
}


void Mapeamento::laserCallback(const sensor_msgs::LaserScan::ConstPtr& laser_msg)
{
    this->scan_msg_ = *laser_msg;
}

void Mapeamento::poseCallback(const nav_msgs::Odometry::ConstPtr& pose_msg)
{
    this->pose_msg_ = *pose_msg;
}

void Mapeamento::dsrCallback(const geometry_msgs::Twist::ConstPtr& desired_vel){
    this->desired_vel_ = *desired_vel;
}

void Mapeamento::sonarCallback(const p2os_msgs::SonarArray::ConstPtr& sonar_msg){
    this->sonar_msg_ = *sonar_msg;
}

bool Mapeamento::navegarGrafo(){
    //Velocidades lineares e angulares novas
    double new_linear_vel;
    double new_angular_vel ;
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
    //Calculando a velocidade angular
    mod_v_y_robo = mod_aceleracao * sentheta * TEMPO_POR_SPIN;

    new_linear_vel = desired_vel_.linear.x + mod_v_x_robo * sin(yaw) + mod_v_y_robo * cos(yaw);
    new_angular_vel = desired_vel_.angular.z + mod_v_x_robo * cos(yaw) - mod_v_y_robo * sin(yaw) * RAIO_ROBO;
    //getchar();


    /*debug*///cout <<"\tvlin = "<< new_linear_vel <<"\n" ;
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
    if(new_linear_vel < 0){
        new_linear_vel = 0;
        if(new_angular_vel < 0)
            new_angular_vel -= 0.5;
        if(new_angular_vel > 0)
            new_angular_vel += 0.5;
    }
    //VELOCIDADE MAXIMA
    if(abs(new_linear_vel) > VELOCIDADE_MAXIMA_LINEAR ){
        new_linear_vel = VELOCIDADE_MAXIMA_LINEAR;
    }
    if(abs(new_angular_vel) > VELOCIDADE_MAXIMA_ANGULAR){
        new_angular_vel = VELOCIDADE_MAXIMA_ANGULAR;
    }
    this->command_vel_.linear.x = new_linear_vel ;
    this->command_vel_.angular.z = new_angular_vel ;    
    if(final_state1 && final_state2)
        return true;
    else
        return false;
}


void Mapeamento::desvieObstaculo(){
    //Velocidades lineares e angulares novas
    double new_linear_vel;
    double new_angular_vel ;
    
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
    for (int i = (ANGULOS_POR_GRUPO*2)-1; i < nro_angulos - ANGULOS_POR_GRUPO*2; ++i ,++j){
      /*debug*///cout <<"\t range ="<<  this->scan_msg_.ranges[i]<< "\t angulo = " <<  (this->scan_msg_.angle_min + ( i * this->scan_msg_.angle_increment )*180 )
      //                                                          / M_PI <<" \n";
      // Testa se eh a minima distancia e esta no intervalor ainda do subgrupo
      if(min_distancia > this->scan_msg_.ranges[i] && j < ANGULOS_POR_GRUPO){
        min_distancia = this->scan_msg_.ranges[i];
        min_index = i;
      }
      else if( j >= ANGULOS_POR_GRUPO){
        if(min_distancia > RANGE_SEGURANCA_MIN){
            Mapeamento::vetorResultante.somaVetorial(this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ),
                                                0,
                                                true);
            
        }else{
            Mapeamento::vetorResultante.somaVetorial(this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ),
                                                pow(min_distancia,2),
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
    double angulo_resultante = Mapeamento::vetorResultante.angulo;
    //Convertendo para vetores
    //Contribuicao da aceleracao  na velocidade em um instante V = V0 + a t  t sendo infinitesimal
    Mapeamento::vetorResultante.multiplicarEscalar(TEMPO_POR_SPIN * CONSTANTE_FORCA *CONSTANTE_DESVIO / PESO_ROBO);
    // Vetor rotacionado para um plano perpendicular na direcao do vetor 
    double e1cos = Mapeamento::vetorResultante.componente_x * cos( ((M_PI/2)- angulo_resultante)) ;
    double e1sen = Mapeamento::vetorResultante.componente_x * sin( ((M_PI/2)- angulo_resultante)) ;
    double e2cos = Mapeamento::vetorResultante.componente_y * cos( ((M_PI/2)- angulo_resultante)) ;
    double e2sen = Mapeamento::vetorResultante.componente_y * sin( ((M_PI/2)- angulo_resultante)) ;
    //getchar();
    new_linear_vel = command_vel_.linear.x  - (e1sen + e2cos);
    new_angular_vel = command_vel_.angular.z  - (e1cos - e2sen) * RAIO_ROBO ;
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

void Mapeamento::mapearNode(Node *nodeAtual){
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
    for (int i = 0; i < nro_angulos ; ++i ,++j){
        /*debug*///cout <<"\t range ="<<  this->scan_msg_.ranges[i]<< "\t angulo = " <<  (this->scan_msg_.angle_min + ( i * this->scan_msg_.angle_increment )*180 )
          //                                                          / M_PI <<" \n";
          // Testa se eh a minima distancia e esta no intervalor ainda do subgrupo
        if(min_distancia > this->scan_msg_.ranges[i] && j < ANGULOS_POR_GRUPO){
            min_distancia = this->scan_msg_.ranges[i];
            min_index = i;
        }
        else if( j >= ANGULOS_POR_GRUPO){
            /*debug*/cout << "\t Setei angulo , distancia ="<< this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment )<<","<<this->scan_msg_.ranges[min_index]<<"\n" ;
            nodeAtual->mapa->setInfoCell(this->scan_msg_.ranges[min_index],this->scan_msg_.angle_min + ( min_index * this->scan_msg_.angle_increment ));   
            j  = 0;
            min_distancia = this->scan_msg_.range_max;
        }
    }
    nodeAtual->mapa->printMapa();
    getchar();
    return;
   
}

void Mapeamento::spin(){
    //Ponteiro de arquivo
    //Argumentos x e y para o algoritmo captado pelo grafo
    //GRAFO Geral
    //NODE ALVO AUXILIAR
    // Lista Caminho para o ir para o node escolhido
    //Booleano de flag para parar o algoritmo de ir de ponto a ponto
    // inteiros ids para insercao dos Nodes E NODE INICIAL
    // ros handler
    FILE *fp;
    double x_arg, y_arg,larguraMapa,alturaMapa;
    Grafo g1;
    Node *nodeAlvo, *nodeAtual ;
    list<Node*> *caminho;
    bool chegou ;
    int id1= 0, id2 = 0,idInicio = NODE_INICIAL ;
    ros::NodeHandle n;
    //Tratamento arquivo Nodes!
    fp = fopen("/home/caju/catkin_ws/src/fcr2017/src/cic2.txt","r");
    if(fp == NULL){
        ROS_ERROR("\t NAO FOI ENCONTRADO O ARQUIVO DE GRAFO E MAP\n");
        return ;
    }
    while( fscanf(fp,"%lf%*c%lf%*c%lf%*c%lf%*c",&x_arg,&y_arg,&larguraMapa,&alturaMapa) != EOF){
        g1.addNode(x_arg,y_arg,larguraMapa,alturaMapa);
    }
    //Tratamento arquivo ARESTAS!
    fp = fopen("/home/caju/catkin_ws/src/fcr2017/src/cic2Arestas.txt","r");
    if(fp == NULL){
        ROS_ERROR("\t NAO FOI ENCONTRADO O ARQUIVO DE ARESTAS E MAP\n");
        return ;
    }
    while( fscanf(fp,"%d%*c%d%*c%lf",&id1,&id2,&x_arg) != EOF){
        g1.addAresta(id1,id2,x_arg);
    }
    //Configuracao FRAME RATE
    //INICIALIZACAO NODE ATUAL
    //Seta posicao inicial do robo
    //ENTRADA DE USUARIO
    ros::Rate loop_rate(10);
    nodeAtual = g1.getNode(NODE_INICIAL) ;
    nodeAtual->mapa->setRoboPosicao( nodeAtual->mapa->getLargura()/2, nodeAtual->mapa->getAltura()/2);
    printf("\tDigite o id do ponto \n");
    scanf("%lf",&x_arg);
    do{
        //LOOP DE IR PONTO A PONTO
        //PEGA O NODE ALVO PARA IR PARA O ALGORITMO
        // USA DIJKSTRA para pegar uma lista de nodes que sao o melhor caminho
        caminho = g1.algoritmoDijkstra(nodeAtual->getId(),x_arg);
        while(!caminho->empty()){
            /*DEBUG*///nodeAlvo->getNodesVizinhos();
            //Pega as coordenandas do node corrente alvo
            //FLAG DE FINAL DO ALGORITMO
            //Processa o caminho
            //Tira o Node corrente do caminho
            nodeAlvo = g1.getNode(caminho->front()->getId());
            this->setParametros(nodeAlvo->getx(),nodeAlvo->gety());
            chegou = false; 
            cout << "\t Indo para "<< caminho->front()->getId()<<"...\n";
            while(ros::ok() && !chegou){
                //spin uma vez no contexto da simulacao
                //Mapeie onde voce esta no node
                //Executa andar ate o proximo passo se nao chegou
                //Desvie os obstaculos
                //publicar
                ros::spinOnce();
                mapearNode(nodeAtual);
                /*if(!chegou) 
                    chegou = navegarGrafo();
                desvieObstaculo();
                vel_pub_.publish(command_vel_);
                */loop_rate.sleep();
            }
            nodeAtual = caminho->front();
            caminho->pop_front();
        }
        //P
        printf("\tDigite o id do ponto \n");
        scanf("%lf",&x_arg);
    }while(x_arg >0 && x_arg < 24);

}

Mapeamento::~Mapeamento()
{
}
