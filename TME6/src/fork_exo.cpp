#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main () {
	const int N = 3;
	int nb_fils = 0;
	std::cout << "main pid=" << getpid() << std::endl;

	for (int i=1, j=N; i<=N && j==N; i++ ) {
		if(fork() == 0){
			nb_fils = 0;
			std::cout << " i:j " << i << ":" << j << std::endl;
			for (int k=1; k<=i && j==N ; k++) {
				if ( fork() == 0) {
					nb_fils = 0;
					j=0;
					std::cout << " k:j " << k << ":" << j << std::endl;
				}else{
					nb_fils++;
				}
			}
		}else{
			nb_fils ++;
			break;
			
		}
	}
	cout << "nb_fils = "<< nb_fils << " de pid " << getpid() << " PERE = "<< getppid() << endl;

	for(int i = 0; i<nb_fils; i++){
		if(wait(NULL) == -1){
			cout << "trop de wait " << getpid() << endl;
		}
	}
	if(wait(NULL) != -1){
		cout << "il en reste" << endl;
	}

	return 0;
}
