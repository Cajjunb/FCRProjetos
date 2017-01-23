
#include "vetor.h"


//Classe objeto vetor fisico para soma vetorial
class Vetor{
	public: double angulo;
	public: double componente_x;
	public: double componente_y;
	public: double modulo;

	Vetor(double angulo,double modulo){
		this->angulo = angulo;
		this->componente_x = cos (angulo) * modulo;
		this->componente_y = sin (angulo) * modulo;
		this->modulo;
	}


	void setAngulo(double angulo,double modulo){
		this->angulo = angulo;
		this->componente_x = cos (angulo) * modulo;
		this->componente_y = sin (angulo) * modulo;
		cout <<"\t componentes "<<this->componente_x <<","<<this->componente_y  ;
		this->modulo;
	}


	void setComponentes(double componente_x ,double componente_y, double modulo){
		this->componente_x =componente_x * modulo;
		this->componente_y =componente_y * modulo;
		return;
	}

	void somaVetorial(Vetor *v2){
		this->componente_x += v2->componente_x;
		this->componente_y += v2->componente_y;
		return;
	}
};
 


