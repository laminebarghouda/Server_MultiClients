#define HAVE_STRUCT_TIMESPEC
#define _WIN32_WINNT 0x0600
#include <iostream>
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "pthread.h"
using namespace std;
#pragma comment (lib, "ws2_32.lib")

static void * Read(SOCKET sock){
    string userInput;
    // Do-while loop to send data
    while (true){
    // Prompt the user for some text
        getline(cin, userInput);
    if (userInput.size() > 0){		// Make sure the user has typed in something
        // Send the text
        if(userInput == "QUIT") {
            cout << "You're quitting the chat room " << endl;
            system("pause");
            exit(0);
        }
        send(sock, userInput.c_str(), userInput.size() + 1, 0);
    }
}
    }

static void * Receive(SOCKET sock){
    while(true) {
        char buf[4096];
        string msg;
        // Wait for response
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        while(bytesReceived==0){
            cout << "";
        }
        msg = buf;
        if(msg == "Server is shutting down. Goodbye"){
            system("pause");
            exit(0);
        }
        cout << string(buf, 0, bytesReceived) << endl;
        }
    }



int main()
{
    string ipAddress = "127.0.0.1";			// IP Address of the server
    int port = 54000;						// Listening port # on the server

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return 0;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }
    // Using Pre-processor directives so we can write and receive messages from other clients at the same time
        pthread_t read,receive;
        pthread_create(&read, 0, reinterpret_cast<void *(*)(void *)>(&Read), reinterpret_cast<void *>(sock));
        pthread_create(&receive, 0, reinterpret_cast<void *(*)(void *)>(&Receive), reinterpret_cast<void *>(sock));
        pthread_join(read, 0);
        pthread_join(receive, 0);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
    return 0;
}
