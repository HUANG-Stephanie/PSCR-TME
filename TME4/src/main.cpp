#include "Banque.h"
#include<iostream>
#include<cstdlib>

using namespace std;

const int NB_THREAD = 10;
int main () {
	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
	threads.reserve(NB_THREAD);
	pr::Banque b(4,100);
	
	srand(time(nullptr));
	for(int n = 0; n<NB_THREAD; ++n){
		size_t i,j;
		do{
			i = rand()%b.size();
			j = rand()%b.size();
		}while(i == j);
		unsigned int m = rand()%100+1;
		threads.emplace_back([&b,i,j,m](){
			for(int k = 0; k<10; ++k){
				b.transfert(i,j,m);
			}
		});
		//auto r = rand() % 2000;
		//this_thread::sleep_for (chrono::milliseconds(r));
		cout << "THREAD Transfert " << n << endl;
		threads.emplace_back([&b](){
			b.comptabiliser(400);
		});
	}

	for (auto & t : threads) {
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
