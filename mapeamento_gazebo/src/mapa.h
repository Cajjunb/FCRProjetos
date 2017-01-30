#ifndef MOD_MAPA
#define MOD_MAPA

#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LARGURA_MAPA 120
#define ALTURA_MAPA	120
#define LADO_QUADRADO_MAPA	0.5
#define POSICAO_ROBO   -2
#define UNDEFINED	0
#define OBSTACULO	-1
#define LIVRE	1
#define RANGE_MAXIMO_LAZER   30.0

using namespace std;


class  MapaLocal;
class  Cell;



class  Cell{
	private:
		int 		info;
		int			distanciaAux;
        list<Cell*> vizinhos;
        bool 		isRobo;
    public:
        Cell();
        ~Cell();
        void setIsRobo(bool valor);
        void addVizinho(Cell* vizinho);
        void setInfo(int info);
		int getInfo();
        bool ehRobo();

};

class  MapaLocal{
	private:
		double 		xRobo;
        double      yRobo;
		int			distanciaAux;
        int         larguraMapa;
        int         alturaMapa;
    	Cell 	    **Dados ;
        bool 		isRobo;
    public:
        MapaLocal(const int larguraMapa,const  int alturaMapa);
        ~MapaLocal();
		void printMapa();
        void setRoboPosicao(int x, int y);
        void setInfoCell(double angulo,double distancia );

};

#endif