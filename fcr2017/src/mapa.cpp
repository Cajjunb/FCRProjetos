#include "fcr2017/mapa.h"


MapaLocal::MapaLocal(const int larguraMapa,const  int alturaMapa,int id , double xCentro, double yCentro){
	this->Dados = new Cell *[alturaMapa];
	for(int y = 0; y < alturaMapa; y++){
		this->Dados[y] = new Cell[larguraMapa];
	}
	this->larguraMapa = larguraMapa;
	this->alturaMapa = alturaMapa;
	this->xRobo = -1;
	this->yRobo = -1;
	this->xCentro = xCentro;
	this->yCentro = yCentro;
	this->id = id;
}


void MapaLocal::printMapa(){
	/*debug*///cout << "\t #### PRINTANDO MAPA ERRO ?" << fp << "\n";
	std::string arg ;	
	// 5. with IOStreams
	std::stringstream sstm;
	sstm << PATH_PASTA <<"node"<<  this->id << ".txt";
	arg = sstm.str();
	this->fp = fopen(arg.c_str(),"w");
	if(fp != NULL){
		for(int y =  this->alturaMapa-1; y >= 0; y--){
			for(int x = 0; x < this->larguraMapa; x++){
			  //if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
			    //cellArray[y][x] = new Cell(pos);
			    //cellArray[y][x].index =  (y*LARGURA) +x;
			    fprintf(fp,"%d\t",Dados[y][x].getInfo());
			    /*debug*///cout << "\tx ="<< x << ",y ="<< y <<"\t valor =" << Dados[x][y].getInfo() <<"\n" ;
		  	}
		  	fprintf(fp,"\n");
		}
	fclose(fp);
	}else{
		cout << "\t nao consegui abrir! ="<< arg <<"\n";
	}
	/*debug*///cout << "\t #### PRINTEI SEM ERRO! \n";
	return;
}

MapaLocal::~MapaLocal(){
	for(int y = 0; y < larguraMapa; y++){
		delete [] this->Dados[y];
	}
	delete [] this->Dados;
	fclose(this->fp);

}

int MapaLocal::getLargura(){
	return this->larguraMapa;
}

int MapaLocal::getAltura(){
	return this->alturaMapa;
}


void MapaLocal::eliminaRobo(){
	this->Dados[(int)this->yRobo][(int)this->xRobo].setIsRobo(false);
	this->Dados[(int)this->yRobo][(int)this->xRobo].setInfo(LIVRE);
	this->xRobo = -1;
	this->yRobo = -1;
	this->printMapa();
	return ;
}


bool MapaLocal::estaNoMapa(double x,double y){
	// x auxiliar onde sera calculado onde esta o robo
	// y auxiliar onde sera calculado onde esta o robo
	// Teste se a largura e a altura devem ser consideradas negativas ou positivas
	int xAux;
	int yAux;
	//cout <<"\t CENTRO [ "<< this->xCentro/0.5 <<"," << this->yCentro/0.5 << "]\t " <<(this->larguraMapa / 2)  <<(this->alturaMapa / 2) <<"\n" ;
	xAux = (int)round( (x - this->xCentro)/ LADO_QUADRADO_MAPA + (this->larguraMapa / 2)-1 ) ;
	yAux = (int)round(  (y - this->yCentro)/ LADO_QUADRADO_MAPA + (this->alturaMapa/2 ) -1 ) ;
	if(xAux >= this->larguraMapa){
		/*debug*/// cout << " \t DIREITA [x,y] = [" << xAux << "," << yAux << "]\n";
		return false;
	}
	if(xAux < 0 ){
		/*debug*/// cout << "\t ESQUERDA[x,y] = [" << xAux << "," << yAux << "]\n";
		return false;
	}
	if(yAux >= this->alturaMapa){
		/*debug*/// cout << "\t CIMA[x,y] = [" << xAux << "," << yAux << "]\n";
		return false;
	}
	if(yAux < 0 ){
		/*debug*/// cout << "\t BAIXO [x,y] = [" << xAux << "," << yAux << "]\n";
		return false;
	}
	return true;
}


void MapaLocal::setRoboPosicao(double x,double y){
	//CONVERSAO PARA POSICOES DA MATRIZ ATRAVES DA ODOMETRIA DO ROBO
	// CASO O ROBO ESTEJA VINDO PELA PRIMEIRA VEZ
	// TESTES DO LIMITE DO MAPA
	// x auxiliar onde sera calculado onde esta o robo
	// y auxiliar onde sera calculado onde esta o robo
	// Teste se a largura e a altura devem ser consideradas negativas ou positivas
	int xAux;
	int yAux;
	/*debug*///cout <<"\t CENTRO [ "<< this->xCentro <<"," << this->yCentro << "\n'" ;
	xAux = (int)round( (x -this->xCentro)/ LADO_QUADRADO_MAPA + (this->larguraMapa / 2)-1 ) ;
	yAux = (int)round(  (y - this->yCentro)/ LADO_QUADRADO_MAPA + (this->alturaMapa/2 ) -1 ) ;
	/*debug*///cout<<	"\t odom.robo ["<< x<<","<<y << "]"<< " CONtAS = " << (this->larguraMapa/2)<<","<<(this->alturaMapa/2 ) << "Centro [" << this->xCentro / LADO_QUADRADO_MAPA << "," << this->yCentro/ LADO_QUADRADO_MAPA <<"]\n";
	if(this->yRobo != -1 && this->xRobo != -1){
		this->Dados[(int)this->yRobo][(int)this->xRobo].setIsRobo(false);
		this->Dados[(int)this->yRobo][(int)this->xRobo].setIsObstaculo(false);
		this->Dados[(int)this->yRobo][(int)this->xRobo].setInfo(LIVRE);
	}
	if(xAux >= larguraMapa)
		xAux =larguraMapa -1;
	if(yAux >= alturaMapa)
		yAux = alturaMapa -1;
	if(xAux < 0)
		xAux = 0;
	if(yAux < 0)
		yAux = 0;
	/*debug*///cout<< "\t matrix [x,y] = [" << xAux << "," << yAux <<"]";
	this->Dados[yAux][xAux].setIsRobo(true);
	this->Dados[yAux][xAux].setIsObstaculo(false);
	this->Dados[yAux][xAux].setInfo(POSICAO_ROBO);
	this->xRobo = xAux;
	this->yRobo = yAux;
	this->printMapa();
}


void MapaLocal::setInfoCell(double angulo,double distancia,double rangeMax){
	//x e y componente a partir da distancia
	//x e y no mapa
	//flag bool usado para colocar obstacula ou espaco livre caso seja o range maximo nao eh obstaculo
	double xComponente ;
	double yComponente ;
	int xMapa = -1 ;
	int yMapa = -1 ;
	bool ehObstaculo ;
	bool obstaculoDetectado = false ;
	do{
		xComponente = distancia * cos(angulo);
		yComponente = distancia * sin(angulo);
		//Testa se a mudanca foi o suficiente para diminuir algum
		//x ou y  CASO SIM SETA O NOVO VALOR
		if (xMapa!= this->xRobo + (double)xComponente/LADO_QUADRADO_MAPA ||
			yMapa != this->yRobo + (double)yComponente/LADO_QUADRADO_MAPA){
			//Conversao dos pontos x e y
			//Caso seja maior que os limites sete como o minimo ou o maximo
			//PRIMEIRO X E DEPOIS Y
			xMapa = round(this->xRobo +(double) xComponente/LADO_QUADRADO_MAPA);
			yMapa = round(this->yRobo + (double)yComponente/LADO_QUADRADO_MAPA);
			ehObstaculo = 	( 
								(xMapa < this->larguraMapa || xMapa < 0 || yMapa < this->larguraMapa || yMapa < 0) && 
								distancia < rangeMax  && 
								!obstaculoDetectado 
							)? true : false;
			/*debug*///cout << "\nxComponente,yComponente =" << xComponente<< "," << yComponente  << "\n";
			/*debug*///cout << "\nxRobo,yRobo =" << xRobo<< "," << yRobo  << "\n";
			if(xMapa >= this->larguraMapa)
				xMapa = this->larguraMapa-1  ;
			if(xMapa < 0 )
				xMapa = 0;
			if(yMapa >= this->alturaMapa)
				yMapa = this->alturaMapa-1;
			if(yMapa < 0 )
				yMapa = 0;	
			if( !this->Dados[yMapa][xMapa].ehRobo() ) {
				if(ehObstaculo){
					/*debug*///cout << "\t botei OBSTACULO [x,y]\t" << xMapa <<"," << yMapa <<"\t info = "<< this->Dados[yMapa][xMapa].getInfo()<<"\n" ;
					this->Dados[yMapa][xMapa].setInfo(OBSTACULO);
					this->Dados[yMapa][xMapa].setIsObstaculo(true);
					ehObstaculo =false;
					obstaculoDetectado =true;
				}
				else {
					if (abs(angulo - 1.57) < 0.1)
					/*debug*///cout << "\t botei LIVRE [x,y]\t" << xMapa <<"," << yMapa <<"\n" ;
					this->Dados[yMapa][xMapa].setInfo(LIVRE);
					this->Dados[yMapa][xMapa].setIsObstaculo(false);
				}
			}
		}
		distancia -= LADO_QUADRADO_MAPA;
		/*debug*///cout <<"\t###Interacao ang,dis" << angulo <<"," << distancia << "\n" ;
	}while(distancia >= 0 );
}


Cell::Cell(){
	this->info = 0;
	this->isRobo = false;
	this->isObstaculo = false;
}


Cell::~Cell(){

}

void Cell::addVizinho(Cell* vizinho){
	this->vizinhos.push_front(vizinho);
}

void Cell::setInfo(int info){
	this->info = info;
}

void Cell::setIsRobo(bool valor){
	this->isRobo = valor;
}

void Cell::setIsObstaculo(bool valor){
	this->isObstaculo = valor;
}


bool Cell::ehRobo(){
	return this->isRobo ;
}

bool Cell::ehObstaculo(){
	return this->isObstaculo ;
}


int Cell::getInfo(){
	return this->info ;
}



/*debug*//*
int main(int argc, char const *argv[])
{
	MapaLocal mapa(LARGURA_MAPA,ALTURA_MAPA);
	mapa.setRoboPosicao(LARGURA_MAPA/2, ALTURA_MAPA/2);
	mapa.setInfoCell(M_PI/3,6.50);
	mapa.printMapa();
	return 0;
}
*/