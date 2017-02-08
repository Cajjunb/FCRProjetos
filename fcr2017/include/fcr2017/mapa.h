#ifndef MOD_MAPA
#define MOD_MAPA

#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#define LARGURA_MAPA 120
#define ALTURA_MAPA 120
#define LADO_QUADRADO_MAPA  0.5
#define POSICAO_ROBO   -2
#define UNDEFINED   0
#define OBSTACULO   -1
#define LIVRE   1
#define RANGE_MAXIMO_LAZER   30.0
#define PATH_PASTA "/home/caju/catkin_ws/src/fcr2017/maps/mapaTopologico/"

using namespace std;


class  MapaLocal;
class  Cell;



class  Cell{
    private:
        int         info;
        int         distanciaAux;
        list<Cell*> vizinhos;
        bool        isRobo;
        bool        isObstaculo;
    public:
        Cell();
        ~Cell();
        void setIsRobo(bool valor);
        void setIsObstaculo(bool valor);
        void addVizinho(Cell* vizinho);
        void setInfo(int info);
        int getInfo();
        bool ehRobo();
        bool ehObstaculo();

};

class  MapaLocal{
    private:
        double      xCentro;
        double      yCentro;
        int         distanciaAux;
        int         xRobo;
        int         yRobo;
        int         id;
        int         larguraMapa;
        int         alturaMapa;
        Cell        **Dados ;
        bool        isRobo;
        FILE        *fp ;
    public:
        MapaLocal(const int larguraMapa,const  int alturaMapa,int id , double xCentro, double yCentro);
        ~MapaLocal();
        void printMapa();
        void setRoboPosicao(double x,double y);
        void setInfoCell(double angulo,double distancia,double rangeMax );
        void eliminaRobo();
        int getLargura();
        int getAltura();     
        bool estaNoMapa(double x,double y);

};

#endif