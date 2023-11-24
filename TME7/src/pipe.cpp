#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char** argv){
    if(argc<4){
        std::cout<<"Using "<< argv[0] << " " << "(path | cmd)+"<<std::endl;
        return 0;
    }
    char*  argv1[10];
    char*  argv2[10];
    bool piped = false;
    int tubeDesc[2];
    pid_t pidf;

    memset(argv1,0,10*sizeof(char*));
    memset(argv2,0,10*sizeof(char*));

    if(pipe(tubeDesc)){
        perror("pipe");
        exit(1);
    }

    for(int arg = 1, i = 0, j = 0; arg<argc; arg++) {
        if (strcmp(argv[i], "|")){
            piped = true;
            continue;
        }
        if(piped){
            argv1[i] = argv[arg];
            i++;
        }else{
            argv2[j] = argv[arg];
            j++;
        }
    }

    if((pidf = fork()) == 0){
        dup2(tubeDesc[1], STDOUT_FILENO);
        close(tubeDesc[0]);
        close(tubeDesc[1]);
        if(execv(argv1[0],argv1)){
            perror("error");
            exit(1);
        }
    }
    else{
        dup2(tubeDesc[0],STDIN_FILENO);
        close(tubeDesc[0]);
        close(tubeDesc[1]);
        if(execv(argv[0],argv2)){
            perror("error");
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}