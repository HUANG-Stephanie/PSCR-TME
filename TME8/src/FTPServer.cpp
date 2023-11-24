#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 3000

using namespace std;

int createSocket(struct sockaddr_in sin)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    bind(s, (struct sockaddr *)&sin, sizeof(sin));
    listen(s, 5);
    return s;
}

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
    socklen_t fromlen = sizeof(sin);
    const char* hello = "Hello World";
    int msg;
  
    int socketServer = createSocket(sin);
    
    int w = accept(0, (struct sockaddr *)&sin, &fromlen); // envoie une descripteur
    read(w,&msg,sizeof(msg));
    cout << msg << endl;
    send(w, hello, strlen(hello), 0);
    cout << "Server send" << endl;
    shutdown(w, 2);
    close(w);

	return 0;
}