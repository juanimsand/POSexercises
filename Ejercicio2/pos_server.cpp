#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <cstdlib>

#define DEBUG_OUTPUT        0   // 1 for console debug, else 0

#define MENSAJE_BIENVENIDA  "***  Bienvenido al ejercicio 2 - Server side  ***"

#define PORT 8080

using namespace std;

int socketServer()
{
    int server_fd, serverSocket, valread, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "socket failed" << endl;
        return -1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cout << "setsockopt" << endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cout << "bind failed" << endl;
        return -1;
    }

#if DEBUG_OUTPUT
    cout << "Before calling listen function!" << endl;
#endif

    if (listen(server_fd, 3) < 0) {
        cout << "listen failed" << endl;
        return -1;
    }

#if DEBUG_OUTPUT
    cout << "Before calling accept function!" << endl;
#endif

    if ((serverSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        cout << "accept" << endl;
        return -1;
    }

#if DEBUG_OUTPUT
    cout << "Before calling read function!" << endl;
#endif

    valread = read(serverSocket, buffer, 1024);

#if DEBUG_OUTPUT
    cout << "Data recibida: " << buffer << endl;
#endif
    int randomNumber = rand() % 10;
    if(randomNumber < 5){
#if DEBUG_OUTPUT
        cout << "random number < 5: " << randomNumber << endl;
#endif
        send(serverSocket, "00", strlen("00"), 0);
    }
    else{
#if DEBUG_OUTPUT
        cout << "random number >= 5: " << randomNumber << endl;
#endif
        send(serverSocket, "01", strlen("01"), 0);
    }


    return 0;
}

int main()
{
    cout << MENSAJE_BIENVENIDA << endl;
    
    return socketServer();
}