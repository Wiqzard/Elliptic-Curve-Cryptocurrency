//
//  Server.cpp
//  Neww
//
//  Created by Sebastian Stapf on 18.09.22.
//


#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
//#include <string.h>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include <sys/un.h>
#include <unistd.h>

#include <iostream>

#define PORT 5000
using namespace std;

using Job = struct Job{
    struct Job * next;
    int client;
};
using ThreadPool = struct Threadpool{
    bool isActive;
    Job * nextJob;
    Job * lastJob;
    pthread_t * pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;
    
    
};





int main(int args, const char* argv[]){
    
    // Server
    struct sockaddr_in saddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    int option = 1;
    int saddrSize = sizeof(saddr);
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, &option, sizeof(option));
    if (socketServer == -1){
        std::cerr << "Not able to create socket!" << std::endl;
        return -1;
    }
    
    // Client
    struct sockaddr_in caddr;
    socklen_t caddrSize = sizeof(caddr);
    int socketClient;
    
    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    // bind() passes file descriptor, the address structure,
    // and the length of the address structure
    // Binding our socket to the IP-address and port
    bind(socketServer, (struct sockaddr*)&saddr, sizeof(saddr));
    

        // This listen() call tells the socket to listen to the incoming connections.
        // The listen() function places all incoming connection into a backlog queue
        // until accept() call accepts the connection.
    listen(socketServer, SOMAXCONN);
    
    // Print
    std::stringstream ss;
    ss << PORT;
    std::cout << "[Server] Listeing on port " << ss.str() << std::endl;
    
    
    char buff[4096];
    unsigned int sizeInBytesOfReceivedData;
    
    // While waiting for client
    while (true) {
        Job * job = new Job[sizeof(Job)];  // ALOCATE MEMORY FOR NEW JOB WITH SIZE IF JOB NOTICE STRUCT
        //Job * job = (Job *) malloc(sizeof(Job));
        job->client = accept(socketServer, (struct sockaddr*)&caddr, (socklen_t*)&caddrSize); //SAVE CLIENT IN job
        std::cout << "[Server] Client connected with success." << std::endl;
            // The accept() call actually accepts an incoming connection.
            // The accept() returns a new socket file descriptor for the accepted connection.
            // So, the original socket file descriptor can continue to be used
            // for accepting new connections while the new socker file descriptor is used for
            // communicating with the connected client.
//        socketClient = accept(socketServer, (struct sockaddr*)&caddr, (socklen_t*)&caddrSize);
//        std::cout << "[Server] Client connected with success." << std::endl;
        
        // Find out who the client is.
        char hostClient[NI_MAXHOST];
        char portClient[NI_MAXSERV];
        memset(hostClient, 0, NI_MAXHOST); // Clear memory
        memset(portClient, 0, NI_MAXSERV);
        if (getnameinfo((sockaddr*)&caddr, sizeof(caddr), hostClient, NI_MAXHOST, portClient, NI_MAXSERV, 0)) {
            std::cout << " --> " << hostClient << " connected to the port " << portClient << std::endl;
        }else{
            inet_ntop(AF_INET, &caddr.sin_addr, hostClient, NI_MAXHOST);
            std::cout << "--> " << hostClient << " connected to the port " << ntohs(caddr.sin_port) << std::endl;
        }
        
        // Receive data
        //BUFF WILL BE REQUESTS:
        
        
        
        
        
        
        sizeInBytesOfReceivedData = recv(socketClient, buff, 4096, 0);
        if (sizeInBytesOfReceivedData ==  -1) {
            std::cerr << "Error receiving message. Quit." << std::endl;
            break;
        } else if (sizeInBytesOfReceivedData == 0){
            std::cout << "Client Disconnected" << std::endl;
            break;
        }
        send(socketClient, buff, sizeInBytesOfReceivedData + 1, 0); // RECEIVE DATA AND SEND IT BACK
        
        std::cout << std::string(buff, 0, sizeInBytesOfReceivedData) << std::endl;
        
        close(socketClient);
    }
    
    
    
    return 0;
}
