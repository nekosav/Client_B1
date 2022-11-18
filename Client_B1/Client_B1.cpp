// Client_B1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>

using namespace std;

WSADATA wsaData;
ADDRINFO hints;
ADDRINFO* addrResult = NULL;
SOCKET ConnectSocket = INVALID_SOCKET;

bool send_to_server(SOCKET server) {

    string sdBf;
    char sendBuffer[4096];
    int result;

    cin.clear();
 
    

    getline(cin, sdBf);

 
    strcpy_s(sendBuffer, sdBf.c_str());

    result = send(server, sendBuffer, (int)strlen(sendBuffer), 0);
    if (result == SOCKET_ERROR) {
        cout << "send failed, error" << result << endl;
        closesocket(server);
        freeaddrinfo(addrResult);
        WSACleanup();
        return false;
    }
}

int main()
{

    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");



    char sendBuffer[4096];
    char recvBuffer[4096];
    
    string sdBf;

    

    int result;




    result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        cout << "WSAstartup failed" << result << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    result = getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        cout << "Getaddrinfo failed with error" << result << endl;
        WSACleanup();
        return 1;
    }

    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Unable connect to server" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    while (true) {

 

        ZeroMemory(recvBuffer, sizeof(recvBuffer));
        result = recv(ConnectSocket, recvBuffer, sizeof(recvBuffer), 0);
        send(ConnectSocket, "0", 1, 0);

        if ((result > 0) and !(recvBuffer[0] == '#') and !(recvBuffer[1] == '#')) {
           // cout << "Received " << result << " bytes" << endl;
            cout << recvBuffer;
            
          }
        else if (result == 0) {
            cout << "Connection closed" << endl;
          }
        else if (!(result >=0)) {
            cout << "recv failed with error" << endl;
            break;
         } 


        

        if ((recvBuffer[0] == '#') and (recvBuffer[1] == '#')) {
            switch (recvBuffer[2]) {
            case '1':
                send_to_server(ConnectSocket);
                break;
            case 'c':
                system("cls");
                break;
            case 'q':
                exit(0);
            }

         }

    }


    closesocket(ConnectSocket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;
}