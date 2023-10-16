#include "Compte.h"
#include <iostream>

using namespace std;

namespace pr {
void Compte::crediter (unsigned int val) {
	unique_lock<recursive_mutex> g(m);
	solde+=val ;
	cout<<"CREDITER"<<endl;
}
bool Compte::debiter (unsigned int val) {
	unique_lock<recursive_mutex> g(m);
	bool doit = solde >= val;
	if (doit) {
		solde-=val ;
		cout<<"DEBITER"<<endl;
	}
	return doit;
}
int Compte::getSolde() const  {
	unique_lock<recursive_mutex> g(m);
	return solde;
}
// NB : vector exige Copyable, mais mutex ne l'est pas...
Compte::Compte(const Compte & other) {
	other.m.lock();
	solde = other.solde;
	other.m.unlock();
}

recursive_mutex & Compte::getMutex(){
	return m;
}

}
