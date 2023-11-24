#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    


using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

void*sp;
void handler(int signal){
	munmap(sp,sizeof(Stack<char>));
	shm_unlink("/monshm");
}

int main () {
	/* Sigaction */
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handler;
	sa.sa_flags = 0;
	sigaction(SIGINT,&sa,NULL);

	int N = 2;
	int M = 2;
	int fd;

	/* Creer un segment monshm, ouverture en R/W */
	if((fd = shm_open("/monshm", O_RDWR|O_CREAT|O_EXCL,0600)) == -1){
		perror("shm_open");
		exit(1);
	}
	/* Allouer au segment une taille pour stocker un char */
	if(ftruncate(fd, sizeof(Stack<char>)) == -1){
		perror("ftruncate");
		exit(1);
	}
	/* Attachement */
	if((sp = mmap(NULL,sizeof(Stack<char>),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
		perror("mmap");
		exit(1);
	}
	
	Stack<char> * s = new (sp)Stack<char>();

	pid_t pp = fork();
	if (pp==0) {
		for(int i = 0; i<N; i++){
			producteur(s);
		}
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		for(int i = 0; i<M; i++){
			consomateur(s);
		}
		return 0;
	}

	wait(0);
	wait(0);

	delete s;
	return 0;
}

