FCR 2017
========

Pacote do ROS para o curso de Fundamentos Computacionais de Robótica - CIC - UnB

* Pacote criado e testado no Ubuntu 14.04 e ROS Indigo

* Antes de usar o pacote instale o p2os-msgs:

$ sudo apt-get install ros-indigo-p2os-msgs


Entrada
-------
Entrada depende dois arquivos txt, o cic.txt e cicArestas.txt
e entrada padrãp de teclado para informar qual eh node de destinação
AS ENTRADAS ACEITAS SÃO 1 - 23 Seguindo grafo que está no relatório

Saida
-----
Movimentação do pioneer de acordo com um grafo criado no cic.txt

Dependencias do ROS
-------------------
* roscpp

Dependencias fora do ROS
------------------------
* bibliotecas padrao do C++98

Algoritmo
---------
Algoritmo de desviar Obstaculos
|O algoritmo implementado foi lecionado na aula de Fundamentos Computacionais de Robótica, utiliza-se conhecimentos de calculo e geometria analítica e calculo vetorial.
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


Algoritmo de Dijkstra

   function Dijkstra(Graph, source, target):
 
       create vertex set Q
 
      for each vertex v in Graph:             // Initialization
          dist[v] ← INFINITY                  // Unknown distance from source to v
          prev[v] ← UNDEFINED                 // Previous node in optimal path from source
          add v to Q                          // All nodes initially in Q (unvisited nodes)

      dist[source] ← 0                        // Distance from source to source
      
      while Q is not empty && first(Q) != target:
          u ← vertex in Q with min dist[u]    // Node with the least distance will be selected first
          remove u from Q 
         
          for each neighbor v of u:           // where v is still in Q.
              alt ← dist[u] + length(u, v)
              if alt < dist[v]:               // A shorter path to v has been found
                  dist[v] ← alt 
                  prev[v] ← u 

      return dist[], prev[]

O caminho menor eh prev[first(Q)] e segue-se a lista encadeada pela função prev[]



Descrição dos arquivos
----------------------

include/
    |--> camera_command
            |--> obstacle_avoidance.h: header da classe que implementa o controle de evitar a colisão
            |--> vetor.h: header da classe auxiliar que é utilizado para calculos vetoriais
    |--> teleop.launch: launch file que é a entrada do programa que utiliza o obstacle_avoidance como programa auxiliar
src/
    |--> forward_kinematics_pioneer.cpp: definicao de movimento utilizado pelo teleop 
    |--> gazebo_sonar.cpp: nao eh usado nesse trabalho
    |--> vetor.cpp: Definição da classe vetor e seus metodos;
    |--> graph.cpp: Definição da classe Grafo, Aresta e Node e seus metodos junto com algoritmo de Dijkstra;
    |--> odometria_campo.cpp: Definicao dos metodos e classe Odometria campo que serao chamados pelo odometria_campo_node.cpp quando o node sera criado, Faz tratamento da entrada do usuário e processa outros algoritmo importantes.
    |--> odometria_campo_node.cpp: Programa principal que da start em todo processamento. 
    |--> obstacle_avoidance.cpp: Não utilizado no trabalho
    |--> obstacle_avoidance_node.cpp: Não utilizado no trabalho
package.xml: Arquivo de configuração de dependecias deste pacote e informações de versão, autor e descrição
README.md: Este arquivo

Adaptado por Leandro Ramalho Motta Ferreira 100033571
0.2017