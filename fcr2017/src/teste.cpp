

#include <iostream>
#include <list>
#include <stack>
using namespace std;
class A{
    public: 
    	int x ;
    A(int x){
    	this->x =x;
    }
    void printHello(){std::cout << "hello";}
};

list<A*>* funcao(){
	A *a = new A(2);
	return new list<A*>(1,a);
}



int main (){
	list<A*>* f = funcao();
	for (list<A*>::iterator it=f->begin();it!=f->end();++it){
		cout << (*it)->x;
    }

}