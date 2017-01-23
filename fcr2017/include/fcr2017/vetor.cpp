
#include "vetor.h"


//Classe objeto vetor fisico para soma vetorial
class Vetor{
	public: double angulo;
	public: double componente_x;
	public: double componente_y;
	public: double componente_z ;
	public: double modulo;

	Vetor(){
		this->angulo = 0;
		this->componente_x = 0;
		this->componente_y = 0;
		this->componente_z = 0;
		this->modulo = 0;
	}


	Vetor(double angulo,double modulo,bool inverso ){
		int inv ;
		// Criar vetor inverso ou nao
		if(inverso)
			inv = -1;
		else
			inv = 1;
		this->angulo = angulo;
		this->componente_x = cos (angulo) * modulo * inv;
		this->componente_y = sin (angulo) * modulo * inv;
		this->componente_z = 0;
		this->modulo;
	}

	Vetor(double x,double y,double z ,bool inverso ){
		int inv ;
		// Criar vetor inverso ou nao
		if(inverso)
			inv = -1;
		else
			inv = 1;
		this->componente_x = x * inv;
		this->componente_y = y * inv;
		this->componente_z = z * inv;
	}



	void setAngulo(double angulo,double modulo){
		this->angulo = angulo;
		this->componente_x = cos (angulo) * modulo;
		this->componente_y = sin (angulo) * modulo;
		this->modulo;
	}


	void multiplicarEscalar(double escalar){
		this->componente_x *= escalar ; 
		this->componente_y *= escalar;
		this->componente_z *= escalar;
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

	void somaVetorial(double angulo,double modulo, bool inverso){
		int inv ;
		// Criar vetor inverso ou nao
		if(inverso)
			inv = -1;
		else
			inv = 1;
		double angulo_v2 = angulo;
		double componente_x_v2 = cos (angulo) * modulo * inv ;
		double componente_y_v2 = sin (angulo) * modulo * inv;
		this->componente_x += componente_x_v2;
		this->componente_y += componente_y_v2;
		/*debug*///cout <<"\t componentes "<<this->componente_x <<","<<this->componente_y  ;
		this->modulo;
	}

	void setComponenteZ(double z){
		this->componente_z = z;
	}


	void produtoVetorial(double x2,double y2,double z2){
		double x1 = this->componente_x;
		double y1 = this->componente_y;
		double z1 = this->componente_z;
		this->componente_x = y1 * z2 - z1 * y2;
		this->componente_y = x1 * z2 - z1 * x2;
		this->componente_z = x1 * y2 - y1 * x2;
		return;
	}

};
 


