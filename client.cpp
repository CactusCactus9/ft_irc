#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include"server.hpp"
#include <pthread.h>

void *handleClient(void *arg) {
    int connectionServerId = *((int*)arg);
    char receivedMsg[1024];
    memset(receivedMsg, 0, sizeof(receivedMsg));
    recv(connectionServerId, receivedMsg, 1024, 0);
    std::cout << "Server received: " << receivedMsg << std::endl;
    close(connectionServerId);
    return NULL;
}

int main() {
    try {
        int socketId = socket(AF_INET, SOCK_STREAM, 0);
        if (socketId == -1) {
            std::cerr << "Server not created!" << std::endl;
            return 1;
        }

        struct sockaddr_in sockaddress;
        memset(&sockaddress, 0, sizeof(sockaddress));
        sockaddress.sin_family = AF_INET;
        sockaddress.sin_port = htons(6667);
        sockaddress.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(socketId, (struct sockaddr *)&sockaddress, sizeof(sockaddress)) == -1) {
            std::cerr << "Error at binding" << std::endl;
            close(socketId);
            return 1;
        }
        std::cout << "Server is binded to port: 6667" << std::endl;

        if (listen(socketId, 200) == -1) {
            std::cerr << "Not listening!" << std::endl;
            return 1;
        }
        std::cout << "Server is listening ..." << std::endl;

        while (1) {
            struct sockaddr_in connectedClientAddr;
            memset(&connectedClientAddr, 0, sizeof(connectedClientAddr));
            socklen_t ClientAdrLength = sizeof(connectedClientAddr);
            int connectionServerId = accept(socketId, (struct sockaddr *) &connectedClientAddr, &ClientAdrLength);
            if (connectionServerId == -1) {
                close(socketId);
                throw "Failed to accept connection with client!";
            }
            std::cout << "Accepted a request at socket ID: " << connectionServerId << std::endl;

            // Create a new thread to handle the client connection
            pthread_t threadId;
            if (pthread_create(&threadId, NULL, handleClient, &connectionServerId) != 0) {
                std::cerr << "Error creating thread" << std::endl;
                close(connectionServerId);
                continue;
            }
            pthread_detach(threadId); // Detach the thread to avoid memory leaks
        }
        close(socketId);
    } catch (...) {
        // Handle exceptions
    }

    return 0;
}
// Certainly! After calling listen(), which sets up the server to accept incoming connections, the server enters a loop where it continuously waits for and handles incoming client connections. Here are the steps after listen():

// Initialize poll() Structures: Before entering the main loop, initialize a vector of pollfd structures to store file descriptors and events to monitor. Initially, add the server socket to this vector.

// Main Loop: Enter a loop that runs indefinitely, continuously checking for events on the monitored file descriptors using poll().

// Call poll(): Inside the loop, call poll() to wait for events on the file descriptors. The poll() function blocks until an event occurs or until the specified timeout (in this case, -1 indicates no timeout, meaning it will block indefinitely).

// Handle Events: After poll() returns, iterate through the pollfd structures to check which file descriptors have events pending.

// New Client Connection: If an event occurs on the server socket (indicating a new client connection), accept the connection using accept() function. Upon successful acceptance, add the new client socket to the vector of pollfd structures.

// Existing Client Socket: If an event occurs on an existing client socket, indicating data is available for reading, read from the socket using recv(). Process the received data as needed.

// Error Handling: Check for errors while reading from the client socket. If an error occurs or if the client closes the connection, handle the error gracefully by closing the client socket and removing it from the vector of pollfd structures.

// Loop Continuation: After handling the events, continue to the next iteration of the loop, where poll() will again be called to wait for new events.

// Clean-up: Upon exiting the loop (which in this case is infinite), close the server socket to release the associated resources.

// This loop effectively allows the server to handle multiple client connections concurrently without blocking on any single client's I/O operation. The server can respond to new client connections and incoming data from existing clients without being limited by the number of threads or processes.
