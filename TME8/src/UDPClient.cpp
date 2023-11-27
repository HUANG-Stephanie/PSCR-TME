#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{   
    /* Le nom de la machine du serveur est passé en argument */
    if(argc < 3)
        exit(0);
    
    int PORT = atoi(argv[2]);
    int reponse;
    struct sockaddr_in dest; // socket du destinataire
    struct addrinfo *result;// IP
    socklen_t fromlen = sizeof(dest);
    int sock;
    char message[80];
    
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket"); 
        exit(1);
    }

    if(getaddrinfo(argv[1], NULL, NULL, &result) != 0) // retrouver l'IP avec le nom
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    dest.sin_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr;// IP
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);

    // TEST
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
    
    // LECTURE REQUETES ET ENVOIE
    char requete[80];
    vector<char*> requetes;
    while(true)
    {
        if(strcmp(requete, "Q") == 0)
        { break;}
        cin.getline(requete, sizeof(requete));
        requetes.push_back(requete);
    }
    
    cout << requetes[0] << endl;
    for(int i = 0; i<requetes.size();)
    {
        cout << "la" << endl;
        cout << requetes[i] << endl;
        if(strcmp(requetes[i], "S") == 0)
        {
            cout << "ici" << endl;
            if(sendto(sock, requetes[i+1], strlen(requetes[i+1])+1, 0, (struct sockaddr*)&dest, sizeof(dest)) == -1)
            {
                perror("sendto");
                exit(1);
            }
            if(sendto(sock, requetes[i+2], strlen(requetes[i+2])+1, 0, (struct sockaddr*)&dest, sizeof(dest)) == -1)
            {
                perror("sendto");
                exit(1);
            }
            i+=2;
        }else{
            i++;
        }
    }

    close(sock);
    return 0;
}