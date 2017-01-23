FCR 2017
========

Pacote do ROS para o curso de Fundamentos Computacionais de Robótica - CIC - UnB

* Pacote criado e testado no Ubuntu 14.04 e ROS Indigo

* Antes de usar o pacote instale o p2os-msgs:

$ sudo apt-get install ros-indigo-p2os-msgs


Entrada
-------
Nada, Programa Auxiliar que deverá ser utilizado com teleop.launch

Saida
-----
Nada, Programa Auxiliar que deverá ser utilizado com teleop.launch


Dependencias do ROS
-------------------
* roscpp

Dependencias fora do ROS
------------------------
* bibliotecas padrao do C++98

Algoritmo
---------
O algoritmo implementado foi lecionado na aula de Fundamentos Computacionais de Robótica, utiliza-se conhecimentos de calculo e geometria analítica e calculo vetorial.
for (int i = primeiroAnguloMeio ; i < UltimoAnguloMeio;){
      if(min_distancia > this->scan_msg_.ranges[i] && j < ANGULOS_POR_GRUPO){
        min_distancia = this->scan_msg_.ranges[i];
        min_index = i;
      }
      else if( j >= ANGULOS_POR_GRUPO){
        if(min_distancia > RANGE_SEGURANCA_MIN){
        	//CASO EMERGENCIA
            VetorResultante.SOMAVETORIAL(ANGULO,0);
            
        }else{
            VetorResultante.SOMAVETORIAL(ANGULO, MODULO);           
        }
        if(min_distancia < RANGE_SEGURANCA_MAX)
            emergence_state = true;

        }
    }
//ROTACAO DO VETOR VELOCIDADE LINEAR E ANGULAR
new_linear_vel = desired_vel_.linear.x  - (e1sen + e2cos);
new_angular_vel = desired_vel_.angular.z  - (e1cos - e2sen) * RAIO_ROBO ;
//getchar();
//CASO EMERGENCIA
if(emergence_state)
    emergence_coeficiente = 0;
command_vel_.linear.x = new_linear_vel * emergence_coeficiente ;
command_vel_.angular.z = new_angular_vel;

Descrição dos arquivos
----------------------

include/
    |--> camera_command
            |--> obstacle_avoidance.h: header da classe que implementa o controle de evitar a colisão
            |--> vetor.h: header da classe auxiliar que é utilizado para calculos vetoriais
            |--> vetor.cpp: Definição da classe vetor e seus metodos, deixei incluidos sem ser linkados por falta de intimidade com o ROS
    |--> teleop.launch: launch file que é a entrada do programa que utiliza o obstacle_avoidance como programa auxiliar
src/
    |--> forward_kinematics_pioneer.cpp: definicao de movimento utilizado pelo teleop 
    |--> gazebo_sonar.cpp: nao eh usado nesse trabalho
    |--> obstacle_avoidance.cpp: Definicao dos metodos e classe Obstacle avoidance que serao chamados pelo obstacle_avoidance_node.cpp quando o node sera criado
    |--> obstacle_avoidance_node.cpp: Programa principal que da start em todo processamento. 
CMakeLists.txt: Arquivo de configuração da build deste pacote
package.xml: Arquivo de configuração de dependecias deste pacote e informações de versão, autor e descrição
README.md: Este arquivo

Adaptado por Leandro Ramalho Motta Ferreira 100033571
0.2017