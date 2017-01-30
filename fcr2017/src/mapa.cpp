#include "fcr2017/mapa.h"


MapaLocal::MapaLocal(const int larguraMapa,const  int alturaMapa){
	this->Dados = new Cell *[larguraMapa];
	for(int y = 0; y < larguraMapa; y++){
		this->Dados[y] = new Cell[alturaMapa];
	}
	this->larguraMapa = larguraMapa;
	this->alturaMapa = alturaMapa;
}


void MapaLocal::printMapa(){
	FILE *fp = fopen("/home/caju/catkin_ws/src/mapeamento_gazebo/src/mapa.txt","w");
	cout << "\t #### PRINTANDO MAPA ERRO ?" << fp << "\n";
	if(fp != NULL){
		for(int y = 0; y < this->larguraMapa; y++){
			for(int x = 0; x < this->alturaMapa; x++){
			  //if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
			    //cellArray[y][x] = new Cell(pos);
			    //cellArray[y][x].index =  (y*LARGURA) +x;
			    fprintf(fp,"%d ",Dados[x][y].getInfo());
			    cout << "\tx ="<< x << ",y ="<< y <<"\t valor =" << Dados[x][y].getInfo() <<"\n" ;
		  	}
		  	fprintf(fp,"\n");
		}
	}
	fclose(fp);
}

MapaLocal::~MapaLocal(){
	for(int y = 0; y < larguraMapa; y++){
		delete [] this->Dados[y];
	}
	delete [] this->Dados;

}

int MapaLocal::getLargura(){
	return this->larguraMapa;
}

int MapaLocal::getAltura(){
	return this->alturaMapa;
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
	cout <<"\t###SetInfo ang,dis" << angulo << distancia << "\n" ;
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
			xMapa = this->xRobo +(double) xComponente/LADO_QUADRADO_MAPA;
			yMapa = this->yRobo + (double)yComponente/LADO_QUADRADO_MAPA;
			if(xMapa > this->larguraMapa)
				xMapa = this->larguraMapa;
			if(xMapa < 0 )
				xMapa = 0;
			if(yMapa > this->alturaMapa)
				xMapa = this->alturaMapa;
			if(yMapa < 0 )
				yMapa = 0;	
			if(( !this->Dados[yMapa][xMapa].ehRobo() )) {
				if(ehObstaculo ){
					this->Dados[yMapa][xMapa].setInfo(OBSTACULO);
					ehObstaculo =false;
				}
				else if(this->Dados[yMapa][xMapa].getInfo() != OBSTACULO){
					this->Dados[yMapa][xMapa].setInfo(LIVRE);
				}
			}
		}
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