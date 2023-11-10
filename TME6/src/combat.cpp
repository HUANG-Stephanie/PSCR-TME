#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "rsleep.h"

using namespace std;

int vie = 3;

void attaque (pid_t adversaire){
    cout << getpid() << " ATTAQUE " << adversaire << endl;

    signal(SIGINT, [](int){
        vie--;
        cout << "points de vie restants = " << vie << " de pid = " << getpid() << endl;

        if(vie == 0){
            cout << "mort du processus " << getpid() << endl;
            exit(1);
        }
    });

    int res =  kill(adversaire,SIGINT);
    if(res < 0){
        cout << "mort de l'adversaire " << adversaire << endl;
        exit(0);
    }
    randsleep();
   
}

void defense(){
    cout << getpid() << " DEFEND" << endl;
    signal(SIGINT, SIG_IGN);
    randsleep();
}

static void handler1(int){
    cout << "coup paré" << endl;
}

void defenseLuke(){
    cout << getpid() << " LUKE DEFEND" << endl;
    // positionner un handler qui affiche "coup paré" avec sigaction
    struct sigaction sa;
    sa.sa_handler = &handler1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // masquer les signaux avec sigprocmask
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // s'endormir pendant une durée aléatoire avec randsleep
    randsleep();

    // invoquer sigsuspend pour tester si une attaque a eu lieu, et donc afficher le message "coup paré" si le signal a été reçu
    sigsuspend(&mask);
}

void combat(pid_t adversaire){
    while(1){
        defense();
        attaque(adversaire);
    }
}

void combat2(pid_t adversaire){
    while(1){
        defenseLuke();
        attaque(adversaire);
    }
}

int wait_till_pid(pid_t pid){
    while(1){
        pid_t p = wait(nullptr);
        if(p == pid){
            return p;
        }
        if(p == -1){
            return -1;
        }
    }
}

bool sigalrm = false;
int wait_till_pid(pid_t pid, int sec){

    signal(SIGALRM, [](int){
        sigalrm = true;
    });
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    sigdelset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    
    alarm(sec);

    while(1){
        sigsuspend(&mask);
        if(sigalrm){
            return 0;
        }
        pid_t p = wait(nullptr);
        if(p == pid){
            alarm(0);
            return p;
        }
        if(p == -1){
            return -1;
        }
    }

} 

int main(){
    srand(time(nullptr));
    pid_t pidf = fork();

    /*
    // QUESTION 3
    if(pidf == 0){
        cout << "Luke pid=" << getpid() << endl;
        combat(getppid());
    }else{
        cout << "Vador pid=" << getpid() << endl;
        combat(pidf);
    }
    */

   /*
   // QUESTION 6
    if(pidf == 0){
        cout << "Luke pid=" << getpid() << endl;
        combat2(getppid());
    }else{
        cout << "Vador pid=" << getpid() << endl;
        combat(pidf);
    }
    */

   // QUESTION 8
    if(pidf == 0){
        cout << "Luke pid=" << getpid() << endl;
        exit(1);
    }else{
        cout << "Vador pid=" << getpid() << endl;
        int p = wait_till_pid(pidf,10);
        if(p == 0){
            cout << "temporisateur expiré" << endl;
        }
        if(p == -1){
            cout << "pid inexistant" << endl;
        }
        if(p == pidf){
            cout << "terminaison du fils" << endl;
        }
    }

}