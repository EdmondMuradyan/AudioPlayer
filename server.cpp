#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); 
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port 12345" << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            std::cerr << "Accept failed" << std::endl;
            close(serverSocket);
            return 1;
        }
        std::cout << "Client connected" << std::endl;

        std::ifstream musicFile("erg.mp3", std::ios::binary);
        if (!musicFile.is_open()) {
            std::cerr << "Failed to open music file" << std::endl;
            close(clientSocket);
            continue; 
        }

        char buffer[1024];
        while (!musicFile.eof()) {
            musicFile.read(buffer, sizeof(buffer));
            int bytesRead = musicFile.gcount();
            if (bytesRead <= 0) {
                break;
            }
            if (send(clientSocket, buffer, bytesRead, 0) == -1) {
                std::cerr << "Failed to send music file" << std::endl;
                close(clientSocket);
                musicFile.close();
                continue;
            }
        }

        close(clientSocket);
        musicFile.close();
        std::cout << "Music file sent" << std::endl;
    }

    close(serverSocket);
    return 0;
}


