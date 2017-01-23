#include "fcr2017/graph.h"




// Declarracao Aresta
void Aresta::printAresta(){
    cout    << "\t Aresta" 
            << "\t node 1  id =" << node1->getId()
            << "\t node 2  id =" << node2->getId()
            << "\t distancia  =" << this->distancia
            << "\n";
}

Aresta::Aresta(Node *nodeAlvo1, Node *nodeAlvo2, double distancia){
    this->node1 = nodeAlvo1;
    this->node2 = nodeAlvo2;
    this->distancia = distancia;
}

Aresta::~Aresta(){
    this->node1 = NULL;
    this->node2 = NULL;
}


Node* Aresta::getVizinho(int idCorrente){
    if(this->node1->getId() == idCorrente && this->node2->getId() != idCorrente)
        return this->node2;
    if(this->node2->getId() == idCorrente && this->node1->getId() != idCorrente)
        return this->node1;
    return NULL;
}

Node* Aresta::getNode1(){
    return this->node1;
}

Node* Aresta::getNode2(){
    return this->node2;
}

double Aresta::getDistancia(){
    return this->distancia;
}


// DECLARACAO GRAFO    
Grafo::Grafo(){
    this->nroArestas=0;
    this->nroNodes=0;
}

//Limpa o grafo inteiro e desaloca tudo
Grafo::~Grafo(){
    for (list<Node*>::iterator it=this->listaNodes.begin();it!=this->listaNodes.end();++it){
        delete (*it);
    }
    for (list<Aresta*>::iterator it=listaArestas.begin();it!=listaArestas.end();++it){
       delete (*it); 
    }
    this->listaNodes.clear();
    this->listaArestas.clear();
}

void Grafo::addNode(double x, double y){
        this->listaNodes.push_back(new Node(++(this->nroNodes),x,y));
}

bool Grafo::addAresta(int id1, int id2,double distancia){
    Node *aux1= NULL;
    Node *aux2= NULL;
    bool erro = false;
    // Pesquisa sequencial dos Node para pegar a referencia
    // caso nao exista retorne falso
    for (list<Node*>::iterator it=this->listaNodes.begin();it!=this->listaNodes.end();++it){
        if((*it)->idIgual(id1))
            aux1 = (*it);
        if((*it)->idIgual(id2))
            aux2 = (*it);
    }
    if(aux1 == NULL || aux2 == NULL)
        return false;
    Aresta *arestaNovo = new Aresta(aux1,aux2,distancia);
    aux1->vizinhos->push_back(arestaNovo);
    aux2->vizinhos->push_back(arestaNovo);
    this->listaArestas.push_back(arestaNovo);
    this->nroArestas++;
    return true;
}

void Grafo::printaGrafo(){
    for (list<Node*>::iterator it=listaNodes.begin();it!=listaNodes.end();++it){
       (*it)->printNode(); 
    }
    for (list<Aresta*>::iterator it=listaArestas.begin();it!=listaArestas.end();++it){
       (*it)->printAresta(); 
    }
}

list<Node*> *Grafo::algoritmoDijkstra(int idCorrente,int idAlvo){
    //Lista de inteiros inteiro com o caminho a ser retornado
    //Set de grafos nao visitados auxiliar
    // NODE CORRENTE AUXILIAR
    // Auxiliar calculo da distancia
    list<Node*> *caminho = new list<Node*>;
    list<Node*> naoVisitados ;
    list<Node*> visitados ;
    list<Node*> *vizinhosVisitado ;
    Node *nodeCorrente;
    double distanciaAux = 0;
    //seta todos os nodes nao visitados e distancia tentada ifinita 
    for (list<Node*>::iterator it=listaNodes.begin();it!=listaNodes.end();++it){
       (*it)->setVisitado(false);
       (*it)->prevDijkstra = NULL;
       naoVisitados.push_back(*it);
       (*it)->setDistanciaTentada(INFINITE);
    }
    //NODE primeiro VISITADO e distancia 0
    // tira o node da lista de nao visitados e coloca na de visitados
    nodeCorrente = this->getNode(idCorrente);
    nodeCorrente->setVisitado(true);
    nodeCorrente->setDistanciaTentada(0);
    naoVisitados.remove(nodeCorrente);
    visitados.push_back(nodeCorrente);
    //Enquanto nao testamos todos ou nao encontramos o node alvo
    while(!naoVisitados.empty()&& !nodeCorrente->idIgual(idAlvo)){
        vizinhosVisitado = nodeCorrente->getNodesVizinhos();
        //Verificando as possiveis tentativas
        for (list<Node*>::iterator it=vizinhosVisitado->begin();it!=vizinhosVisitado->end();++it){
            //Calcula a distancia tentativa, distancia atual + distancia da aresta
            //Caso seja menor que a ja calculada naquele node nao faca nada
            distanciaAux =  nodeCorrente->getDistanciaTentada() + (*this->getAresta(nodeCorrente->getId(), (*it)->getId())).getDistancia();
            if(distanciaAux < (*it)->getDistanciaTentada()){
                (*it)->setDistanciaTentada(distanciaAux);
                (*it)->prevDijkstra = nodeCorrente;
            }
        }
        //Da sort colocando os nodes com menor distancia primeiro
        naoVisitados.sort(Node::comparaNodes);
        nodeCorrente = (*naoVisitados.begin());
        nodeCorrente->setVisitado(true);
        naoVisitados.remove(nodeCorrente);
        visitados.push_back(nodeCorrente);
    }
    //SE O ULTIMO NODE CORRENTE NAO EH O ALVO PQ NAO FOI ACHADO
    if(!nodeCorrente->idIgual(idAlvo)){
        delete caminho;
        caminho = NULL;
    }
    else{
        //Coloca se no topo da pilha e mantem colocando ate que o proximo seja null
        //Final coloca se o primeiro 
        while(nodeCorrente->prevDijkstra != NULL){
            caminho->push_front(nodeCorrente);
            nodeCorrente = nodeCorrente->prevDijkstra;
        }
    }
    //RETORNA RESULTADO
    return caminho;
}

// Funcao que retorna um node do grafo a partir do id dele
Node* Grafo::getNode(int idNode){
    Node *aux1= NULL;
    for (list<Node*>::iterator it=this->listaNodes.begin();it!=this->listaNodes.end();++it){
        if((*it)->idIgual(idNode))
            aux1 = (*it);
    }
    return aux1;
}


Aresta* Grafo::getAresta(int idAlvo1,int idAlvo2){
    for (list<Aresta*>::iterator it=this->listaArestas.begin();it!=this->listaArestas.end();++it){
        if( (*it)->getNode1()->idIgual(idAlvo1) && (*it)->getNode2()->idIgual(idAlvo2) ||
            (*it)->getNode1()->idIgual(idAlvo2) && (*it)->getNode2()->idIgual(idAlvo1))
            return (*it);   
    }
    return NULL;
}




// Codigo Node
Node::~Node(){
    this->vizinhos->clear();
    delete this->vizinhos;
}

void Node::setVisitado(bool visitado){
    this->visitado = visitado;
}


void Node::setDistanciaTentada(double distancia){
    this->distanciaTentada = distancia;
}

// DECLARACAO Node
Node::Node(int id, double x, double y){
    this->id = id;
    this->x = x;
    this->y = y;  
    this->vizinhos = new list<Aresta*>;
    //vizinhos.push_back(arestaVizinha);
}

bool Node::idIgual(int idAlvo){
    if(this->id == idAlvo )
        return true;
    else
        return false;
}
void Node::printNode(){
    cout    << "\t Node id =" << this->id 
            << "\t x = " << this->x
            << "\t y = " << this->y
            << "\t Vizinhos: \n";
    for (list<Aresta*>::iterator it=vizinhos->begin();it!=vizinhos->end();++it){
       (*it)->printAresta(); 
    }
}

list <Node*> * Node::getNodesVizinhos(){
    list<Node*> *listaVizinhos = new list<Node*>;
    for (list<Aresta*>::iterator it=vizinhos->begin();it!=vizinhos->end();++it){
        listaVizinhos->push_back((*it)->getVizinho(this->id)); 
    }
    return listaVizinhos;
}

bool Node::comparaNodes( Node* primeiro, Node* segundo){
    if(primeiro->getDistanciaTentada() < segundo->getDistanciaTentada())
        return true;
    else
        return false;
}


int Node::getId(){
    return this->id;    
}
double Node::getx(){
    return this->x;    
}
double Node::gety(){
    return this->y;    
}
double Node::getDistanciaTentada(){
    return this->distanciaTentada;    
}

