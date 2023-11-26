#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[])
{   
    /* Le nom de la machine du serveur est passé en argument */
    if(argc < 3)
        exit(0);
    
    int PORT = atoi(argv[2]);
    int reponse;
    struct sockaddr_in dest;
    int sock;
    socklen_t fromlen = sizeof(dest);
    char message[80];
    struct addrinfo *result;// IP

    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket"); 
        exit(1);
    }

    if(getaddrinfo(argv[1], NULL, NULL, &result) != 0)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    dest.sin_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr;// IP
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);

    strcpy(message, "MESSAGE DU CLIENT");

    printf("CLIENT SEND\n");
    if(sendto(sock, message, strlen(message)+1, 0, (struct sockaddr*)&dest, sizeof(dest)) == -1)
    {
        perror("sendto");
        exit(1);
    }
    
    if(recvfrom(sock, &reponse, sizeof(reponse), 0, 0, &fromlen) == -1)
    {
        perror("recfrom");
        exit(1);
    }

    printf("Reponse : %d\n", reponse);
    close(sock);
    return 0;
}