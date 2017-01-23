/*
 * C++ Program to Check whether Undirected Graph is Connected using DFS
 */

#ifndef MOD_GRAFO_H
#define MOD_GRAFO_H

#include <iostream>
#include <list>
#include <stack>
#include <stdio.h>
using namespace std;

#define INFINITE 90000
/*
 * Class Declaration
 */
class Nodes;
class Aresta;
class Grafo;


class  Node{
    private:
        int id;
        double x;
        double y;
        bool visitado;
        double distanciaTentada;
    public:
        list<Aresta*> *vizinhos;
        // Construtor Grafo sozinho
        Node(int id, double x, double y);
        Node(int id, double x, double y, Aresta arestaVizinha );
        ~Node();
        bool    idIgual(int idAlvo);
        void    printNode();
        void    setVisitado(bool visitado);
        int     getId();
        list <Node*>* getNodesVizinhos();
        double  getx();
        double  gety();
        double  getDistanciaTentada();
        void    setDistanciaTentada(double distancia);
        Node    *prevDijkstra;
        static bool    comparaNodes( Node* primeiro, Node* segundo);

};

class Aresta{
    private:
        Node *node1;
        Node *node2;
        double distancia;
    public:
        Aresta(Node *nodeAlvo1, Node *nodeAlvo2,double distancia);
        ~Aresta();
        Node* getNode1();
        Node* getNode2();
        Node* getVizinho(int idCorrente);
        void printAresta();
        double  getDistancia();

};


class Grafo{
private:
    list<Node*> listaNodes;    
    list<Aresta*> listaArestas;
    int nroNodes;
    int nroArestas;
public:
    Grafo();
    ~Grafo();
    void addNode( double x, double y);
    void printaGrafo();
    bool addAresta(int id1, int id2,double distancia);
    Aresta* getAresta(int idAlvo1,int idAlvo2);
    Node* getNode(int idNode);
    list<Node*> *algoritmoDijkstra(int idCorrente,int idAlvo);
   
    /* data */
};

#endif