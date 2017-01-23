
#ifndef MOD_VETOR_H
#define MOD_VETOR_H
//Includes C default
#include <stdio.h>
#include <math.h>
//Includes CPP
#include <iostream>
#include <sstream>
using namespace std;

class Vetor;


//Classe objeto vetor fisico para soma vetorial
class Vetor{
	public: 
		double angulo;
		double componente_x;
		double componente_y;
		double componente_z ;
		double modulo;
		Vetor();
		Vetor(double angulo,double modulo,bool inverso );
		Vetor(double x,double y,double z ,bool inverso );
		void setAngulo(double angulo,double modulo);
		void multiplicarEscalar(double escalar);
		void setComponentes(double componente_x ,double componente_y, double modulo);
		void somaVetorial(Vetor *v2);
		void somaVetorial(double angulo,double modulo, bool inverso);
		void setComponenteZ(double z);
		void produtoVetorial(double x2,double y2,double z2);

};



#endif