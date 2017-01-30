#include "mapa.h"


MapaLocal::MapaLocal(const int larguraMapa,const  int alturaMapa){
	this->Dados = new Cell *[larguraMapa];
	for(int y = 0; y < larguraMapa; y++){
		this->Dados[y] = new Cell[alturaMapa];
	}
	this->larguraMapa = LARGURA_MAPA;
	this->alturaMapa = ALTURA_MAPA;
}


void MapaLocal::printMapa(){
	FILE *fp = fopen("mapa.txt","w");
	for(int y = 0; y < LARGURA_MAPA; y++){
		for(int x = 0; x < ALTURA_MAPA; x++){
		  //if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
		    //cellArray[y][x] = new Cell(pos);
		    //cellArray[y][x].index =  (y*LARGURA) +x;
		    fprintf(fp,"%d ",Dados[x][y].getInfo());
		    /*debug*///cout << "\tx ="<< x << ",y ="<< y <<"\t valor =" << Dados[x][y].getInfo() <<"\n" ;
	  	}
	  	fprintf(fp,"\n");
	}
	fclose(fp);
}

MapaLocal::~MapaLocal(){
	for(int y = 0; y < larguraMapa; y++){
		delete [] this->Dados[y];
	}
	delete [] this->Dados;

}

void MapaLocal::setRoboPosicao(int x, int y){
	this->Dados[y][x].setIsRobo(true);
	this->Dados[y][x].setInfo(POSICAO_ROBO);
	this->xRobo = x;
	this->yRobo = y;
}


void MapaLocal::setInfoCell(double angulo,double distancia ){
	//x e y componente a partir da distancia
	//x e y no mapa
	//flag bool usado para colocar obstacula ou espaco livre caso seja o range maximo nao eh obstaculo
	double xComponente ;
	double yComponente ;
	int xMapa = -1 ;
	int yMapa = -1 ;
	bool ehObstaculo = distancia == RANGE_MAXIMO_LAZER? false : true;
	do{
		xComponente = distancia * cos(angulo);
		yComponente = distancia * sin(angulo);
		cout << "\txComponente"<< xComponente << "yComponente"<< yComponente;
		//Testa se a mudanca foi o suficiente para diminuir algum
		//x ou y  CASO SIM SETA O NOVO VALOR
		if (xMapa!= this->xRobo + (double)xComponente/LADO_QUADRADO_MAPA ||
			yMapa != this->yRobo + (double)yComponente/LADO_QUADRADO_MAPA){
			xMapa = this->xRobo +(double) xComponente/LADO_QUADRADO_MAPA;
			yMapa = this->yRobo + (double)yComponente/LADO_QUADRADO_MAPA;
			cout << "\t #####DENTRO DO IF x,y =" <<xMapa <<"," <<yMapa ;
			if( xMapa < this->larguraMapa && xMapa > -1  && 
				yMapa < this->alturaMapa && yMapa > -1  &&
				( !this->Dados[yMapa][xMapa].ehRobo() )) {
				if(ehObstaculo ){
					cout << "\t###Setei OBSTACULO\n";
					this->Dados[yMapa][xMapa].setInfo(OBSTACULO);
					ehObstaculo =false;
				}
				else if(this->Dados[yMapa][xMapa].getInfo() != OBSTACULO){
					cout << "\t###Setei LIVRE\n";
					this->Dados[yMapa][xMapa].setInfo(LIVRE);
				}
			}
		}
		cout << "\t Distancia =" << distancia;
		distancia -= LADO_QUADRADO_MAPA;
	}while(distancia > 0 );
}


Cell::Cell(){
	this->info = 0;
	this->isRobo = false;
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

bool Cell::ehRobo(){
	return this->isRobo ;
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