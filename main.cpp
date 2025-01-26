#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
    // crear socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "No se pudo crear un socket! Saliendo." << endl;
        return 1;
    }

    // estructura hint para el servidor en el que nos conectamos
    int port = 54000;
    string ipAddress = "192.168.1.70";

    sockaddr_in hint;
    hint.sin_family = AF_INET; // Especificamos el protocolo IPv4
    hint.sin_port= htons(port); // Transformamos el puerto que se encuentra en una estructura big endian a little endian
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


    // conecta al servidor en el socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1) {
        cerr << "No se pudo conectar el servidor al socket! Saliendo." << endl;
        return 1;
    }

    // while loop:
    char buf[4096];
    string userInput;

    do {

        //      Entrar lineas de texto
        cout  << "[Cliente]> ";
        getline(cin, userInput);

        //      Enviar al servidor
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1) {
            cout << "No se pudo enviar string al SERVER. Saltando.";
            continue;
        }

        //      Wait for Response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        //      Display response
        cout << "[Servidor]> " << string(buf, bytesReceived) << "\n";

    } while (true);

    // cerrar socket

    close(sock);

    return 0;
}
