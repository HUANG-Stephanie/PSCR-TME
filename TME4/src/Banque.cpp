#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	//unique_lock<mutex> gb(mb);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];

	recursive_mutex & m1 = debiteur.getMutex();
	recursive_mutex & m2 = crediteur.getMutex();

	//mutex & m1 = debiteur.getMutex();
	//mutex & m2 = crediteur.getMutex();
	
	if(deb<cred){
		m1.lock();
		cout << "lock 1" << endl;
	}else{
		m2.lock();
		cout << "lock 2" << endl;
	}

	cout << "valeur " << val << endl;
	cout << "debiteur AVANT " << deb << " " << debiteur.getSolde() << endl;
	cout << "crediteur AVANT " << cred << " " << crediteur.getSolde() << endl;
	if (debiteur.debiter(val)) {
		cout << "debiteur APRES " << deb << " " << debiteur.getSolde() << endl;
		crediteur.crediter(val);
		cout << "crediteur APRES " << cred << " " << crediteur.getSolde() << endl;
	}
	cout<<"----------------"<<endl;
	cout << "debiteur " << deb << " " << debiteur.getSolde() << endl;
	cout << "crediteur " << cred << " " << crediteur.getSolde() << endl;
	cout<<"---------------------------------------"<<endl;

	m1.unlock();
	cout << "unlock 1" << endl;
	m2.unlock();
	cout << "unlock 2" << endl;
}
size_t Banque::size() const {
	//unique_lock<mutex> gb(mb);
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	//unique_lock<mutex> gb(mb);
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		//LOCK le compte lu pour ne pas que les threads de transfert n'y accède
		Compte c = compte;
		recursive_mutex & m = c.getMutex();
		m.lock();
		cout << "compte lu LOCK" << endl;

		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	
	//UNLOCK les comptes lu
	for(const auto & compte : comptes){
		Compte c = compte;
		recursive_mutex & m = c.getMutex();
		m.unlock();
		cout << "compte lu UNLOCK" << endl;
	}
	
	return bilan == attendu;
}
}
