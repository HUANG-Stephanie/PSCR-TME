#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 3000

using namespace std;

int main(int argc, char ** argv) {
    /*
    if(argc < 2)
        exit(0);

    int port = atoi(argv[1]);
    */

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = INADDR_ANY;
    char buff[256];

    int socketClient = socket(AF_INET, SOCK_STREAM, 0);

    connect(socketClient, (struct sockaddr *) &sin, sizeof(sin));
    int msg = 10;
    write(socketClient, &msg, sizeof(msg));
    cout << "Client send" << endl;
    read(socketClient, buff, 256);
    cout << buff << endl;
    shutdown(socketClient, 2);
    close(socketClient);

	return 0;
}