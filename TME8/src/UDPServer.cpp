#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char** argv)
{
    if(argc < 2)
        exit(0);

    int PORT = atoi(argv[1]);

    struct sockaddr_in sin; // socket serveur
    struct sockaddr_in exp; // socket expediteur
    char host[64];
    int sc;
    socklen_t fromlen = sizeof(exp);
    char message[80];
    int cpt = 1;

    if((sc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if(bind(sc, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        exit(2);
    }
    
    if(recvfrom(sc, message, sizeof(message), 0, (struct sockaddr*)&exp, &fromlen) == -1)
    {
        perror("recvfrom");
        exit(2);
    }

    printf("Exp : <IP = %s, PORT = %d>\n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port));

    if(getnameinfo((struct sockaddr*)&exp, sizeof(exp), host, sizeof(host), NULL, NULL, 0) != 0)
    {
        perror("getnameinfo");
        exit(3);
    }

    printf("Machine : %s\n", host);
    printf("Message : %s\n", message);
    
    printf("SERVER SEND\n");
    if(sendto(sc, &cpt, sizeof(cpt), 0, (struct sockaddr*)&exp, fromlen) == -1)
    {
        perror("sendto");
        exit(4);
    }

    close(sc);
    return 0;
}