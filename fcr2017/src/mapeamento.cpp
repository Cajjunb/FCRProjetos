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

void Mapeamento::mostrarMapa(int nroMapa){
     pid_t pID = fork();
     //PROCESSO FILHO  E O ULTIMO ELSE E O PAI DO PROCESSO
    if (pID == 0){  
        //COMECA A EXECUTAR
        //Concatena os argumentos para ser utilizado o chamado no sistema
        //Executa uma operacao no bash
        // Termina o processo filho 
        std::stringstream sstm,sstm2;
        sstm << PATH_PASTA << "mostraMapa.jar " <<PATH_PASTA<<"node" <<  nroMapa << ".txt" << " &";
        std::string arg = sstm.str();
        system(arg.c_str());
        exit(0);
    }
    else if (pID < 0){            // failed to fork
        cerr << "Falhou no fork" << endl;
        exit(0);
        // Throw exception
    }
    else{                                   // parent
        //Continue o processo filho caso nao tenha comecao
        kill(pID,SIGCONT);
        return ;
    }
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
    //Nro utilizado para calcular o modulo da forca
    //Nro utilizado para calcular o modulo da acelaracao,
    //Velocidade utilizado mod_x e Velocidade utilizado mod_y
    //Velocidade utilizado costheta cateto adjacente , Velocidade utilizado costheta e distancia entre pontos
    // ANGULO de odometria do robo yaw
    //Nro utilizado para calcular o modulo da acelaracao
    //boolean
    double new_linear_vel;
    double new_angular_vel ;
    double mod_v_x_robo;
    double mod_v_y_robo;
    double mod_forca ;
    double mod_aceleracao;
    double catetoAdjascente  =  y_desejado - this->pose_msg_.pose.pose.position.y  ;
    double catetoOposto      =  x_desejado - this->pose_msg_.pose.pose.position.x  ;
    double hipotenusa        = sqrt(pow(catetoOposto,2)+pow(catetoAdjascente,2));
    double costheta = catetoAdjascente / hipotenusa  ;
    double sentheta = catetoOposto / hipotenusa  ;
    double yaw = tf::getYaw(this->pose_msg_.pose.pose.orientation);
    double aux;
    bool final_state1 = false;
    bool final_state2 = false;
    //Calcular a distancia entre o ponto desejado e o do robo
    //Calculado mod_forca
    //Calculadno mod_aceleracao f = m*a
    //Calculando a velocidade linear
    //Calculando a velocidade angular
    aux = pow(x_desejado - this->pose_msg_.pose.pose.position.x,2 );
    aux += pow(y_desejado - this->pose_msg_.pose.pose.position.y,2);
    mod_forca = sqrt(aux) * CONSTANTE_FORCA ;
    mod_aceleracao = mod_forca / PESO_ROBO;
    mod_v_x_robo = mod_aceleracao * costheta * TEMPO_POR_SPIN;
    mod_v_y_robo = mod_aceleracao * sentheta * TEMPO_POR_SPIN;

    new_linear_vel = desired_vel_.linear.x + mod_v_x_robo * sin(yaw) + mod_v_y_robo * cos(yaw);
    new_angular_vel = desired_vel_.angular.z + mod_v_x_robo * cos(yaw) - mod_v_y_robo * sin(yaw) * RAIO_ROBO;
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
    if(new_linear_vel <= 0){
        new_linear_vel = 0;
        if(new_angular_vel <= 0)
            new_angular_vel -= 0.5;
        if(new_angular_vel > 0)
            new_angular_vel += 0.5;
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
      if(   this->scan_msg_.ranges[i] >= this->scan_msg_.range_min  &&  this->scan_msg_.ranges[i] <= this->scan_msg_.range_max &&
            min_distancia > this->scan_msg_.ranges[i] 
            && j < ANGULOS_POR_GRUPO){
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
                                                sqrt(sqrt(sqrt(min_distancia))),
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
    if(new_linear_vel == 0 && new_angular_vel == 0){
        if(rand() % 2 == 0)
            new_angular_vel -= 0.5;
        if(new_angular_vel > 0)
            new_angular_vel += 0.5;
    }
    if(emergence_state)
        emergence_coeficiente = 0;
    /*
    if(abs(new_angular_vel) > VELOCIDADE_MAXIMA_ANGULAR){
        if (new_angular_vel < 0)
            new_angular_vel = -VELOCIDADE_MAXIMA_ANGULAR;
        if (new_angular_vel > 0)
            new_angular_vel = -VELOCIDADE_MAXIMA_ANGULAR;
    }*/
    command_vel_.linear.x = new_linear_vel * emergence_coeficiente ;
    command_vel_.angular.z = new_angular_vel ;
    /*
        cout << "\n\t vel_lin =" << command_vel_.linear.x << "\t e1cos" << e1cos <<"\t e1sen"<< e1sen;
        cout << "\n\t vel_ang =" << command_vel_.angular.z << "\t e1cos" << e2cos <<"\t e1sen"<< e2sen ;
        vis_i = 0;
    */
}

void Mapeamento::mapearNode(Node *nodeAtual,double yaw){
    //Nro de angulos feito por uma conta simples
    //Nro utilizado para eleger o minimo range em um subgrupo
    // indices para ser feito a eleicao dos minimos em cada subgrupo
    //boolean
    float nro_angulos = (this->scan_msg_.angle_max - this->scan_msg_.angle_min) / this->scan_msg_.angle_increment ;
    float min_distancia = this->scan_msg_.range_max;
    int j = 0;
    int min_index = -1;
    int emergence_coeficiente = 1;
    bool emergence_state = false;
    //Feito para nao quebrar o codigo
    for (int i = 0; i < nro_angulos ; i+=8 ,++j){
       if(this->scan_msg_.ranges[i] >= this->scan_msg_.range_min  &&  this->scan_msg_.ranges[i] <= this->scan_msg_.range_max )
                nodeAtual->mapa->setInfoCell(   this->scan_msg_.angle_min + ( i * this->scan_msg_.angle_increment ) + yaw,
                                                this->scan_msg_.ranges[i],this->scan_msg_.range_max);   
    }
    nodeAtual->mapa->printMapa();
    return;
   
}

void Mapeamento::pararRobo(){
    command_vel_.linear.x = 0 ;
    command_vel_.angular.z = 0 ;
    vel_pub_.publish(command_vel_);
}

void Mapeamento::spin(){
    //Ponteiro de arquivo
    //Argumentos x e y para o algoritmo captado pelo grafo
    //Funcao que recebe o angulo da orientacao do robo
    //GRAFO Geral
    //NODE ALVO AUXILIAR
    // Lista Caminho para o ir para o node escolhido
    //Booleano de flag para parar o algoritmo de ir de ponto a ponto
    // inteiros ids para insercao dos Nodes E NODE INICIAL
    // ros handler
    FILE *fp;
    double x_arg, y_arg,larguraMapa,alturaMapa;
    double yaw ;
    Grafo g1;
    Node *nodeAlvo, *nodeAtual ;
    list<Node*> *caminho;
    list<int> *explorarCic = new list<int>;
    bool chegou ;
    bool mudou ;
    bool debug =false;
    int id1= 0, id2 = 0,idInicio = NODE_INICIAL ;
    ros::NodeHandle n;
    
    //Tratamento arquivo Nodes!
    //Tratamento arquivo ARESTAS!
    fp = fopen("/home/caju/catkin_ws/src/fcr2017/src/cic2.txt","r");
    if(fp == NULL){
        ROS_ERROR("\t NAO FOI ENCONTRADO O ARQUIVO DE GRAFO E MAP\n");
        return ;
    }
    while( fscanf(fp,"%lf%*c%lf%*c%lf%*c%lf%*c",&x_arg,&y_arg,&larguraMapa,&alturaMapa) != EOF){
        g1.addNode(x_arg,y_arg,larguraMapa,alturaMapa);
    }
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
    //Iniciando a lista de caminho!
    ros::Rate loop_rate(10);
    nodeAtual = g1.getNode(NODE_INICIAL) ;
/*  explorarCic->push_back(4);
    explorarCic->push_back(1);
    explorarCic->push_back(2);
    explorarCic->push_back(3);
    explorarCic->push_back(5);
    explorarCic->push_back(6);
    explorarCic->push_back(8);
    explorarCic->push_back(7);
    explorarCic->push_back(10);
    explorarCic->push_back(14);
    explorarCic->push_back(13);
    explorarCic->push_back(12);
    explorarCic->push_back(9);
    explorarCic->push_back(14);
    explorarCic->push_back(16);
    explorarCic->push_back(17);
    explorarCic->push_back(18);
    explorarCic->push_back(15);
*/  
    explorarCic->push_back(9);
    explorarCic->push_back(11);
    explorarCic->push_back(12);
    explorarCic->push_back(14);
    printf("\tComecando a percorrer o CIC...\n\t");
    while(!explorarCic->empty()){
        // USA DIJKSTRA para pegar uma lista de nodes que sao o melhor caminho
        // TRATAMENTO CASO JA ESTEJA NO PONTO
        //LOOP DE IR PONTO A PONTO
        //PEGA O NODE ALVO PARA IR PARA O ALGORITMO
        caminho = g1.algoritmoDijkstra(nodeAtual->getId(),explorarCic->front());
        while(!caminho->empty()){
            /*DEBUG*///nodeAlvo->getNodesVizinhos();
            //Pega as coordenandas do node corrente alvo
            //FLAG DE FINAL DO ALGORITMO
            //Processa o caminho
            //Tira o Node corrente do caminho
            nodeAlvo = g1.getNode(caminho->front()->getId());
            this->setParametros(nodeAlvo->getx(),nodeAlvo->gety());
            chegou = false; 
            mudou = false;
            cout << "\t Indo para "<< nodeAlvo->getId()<<"...\n";
            while(ros::ok() && !chegou){
                //spin uma vez no contexto da simulacao
                //Verifica-se eainda esta no mapa do node atual ou se eh necessario mudar
                //Calcular o setparametros base mais 
                //Mapeie onde voce esta no node
                //Executa andar ate o proximo passo se nao chegou
                //Desvie os obstaculos
                //publicar
                ros::spinOnce();
                if(!nodeAtual->mapa->estaNoMapa(this->pose_msg_.pose.pose.position.x , this->pose_msg_.pose.pose.position.y) && !mudou){
                    //Eliminar do mapa antigo o robo
                    // Mostra o mapa antigo
                    // Muda o mapa a ser mapeado para o proximo
                    nodeAtual->mapa->eliminaRobo();
                    mostrarMapa(nodeAtual->getId());
                    nodeAtual = g1.getNode(nodeAlvo->getId());
                    cout << "\t ############# MUDEI PRO " << nodeAlvo->getId() << " #############\n";
                    mudou = true;
                }
                nodeAtual->mapa->setRoboPosicao( this->pose_msg_.pose.pose.position.x , this->pose_msg_.pose.pose.position.y );
                yaw = tf::getYaw(this->pose_msg_.pose.pose.orientation);
                mapearNode(nodeAtual,yaw);
                if(!chegou) 
                    chegou = navegarGrafo();
                desvieObstaculo();
                limitarVelocidade();
                vel_pub_.publish(command_vel_);
                loop_rate.sleep();
            }
            // 
            cout << "\t############# Cheguei no centro do node #############"<< nodeAlvo->getId()  <<"\n"; 
            caminho->pop_front();
        }
        explorarCic->pop_front();
    }
    //Mostra o ultimo mapa aonde o robo parou
    mostrarMapa(nodeAtual->getId());  
    this->pararRobo();
    cout << "\t ############# FIM MAPEAMENTO ##########\n";
}

Mapeamento::~Mapeamento()
{
}

void Mapeamento::limitarVelocidade(){

    //VELOCIDADE MAXIMA
    if(abs(command_vel_.linear.x) > VELOCIDADE_MAXIMA_LINEAR ){
        command_vel_.linear.x = VELOCIDADE_MAXIMA_LINEAR;
    }//VELOCIDADE MINIMA
    if(abs(command_vel_.linear.x) < VELOCIDADE_MAXIMA_LINEAR/4 && abs(command_vel_.linear.x) != 0  ){
        command_vel_.linear.x = VELOCIDADE_MAXIMA_LINEAR/4;
    }
}