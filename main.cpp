//
//  main.cpp
//  Neww
//
//  Created by Sebastian Stapf on 24.09.22.
//


#include "Network/Connection.hpp"
#include "Network/Node.hpp"

#include "Tests.hpp"

void serve(Node &node){
    node.my_Address.sin_family = AF_INET;
    node.my_Address.sin_port = htons(node.port);
    node.my_Address.sin_addr.s_addr = INADDR_ANY;
    node.nodeSocket = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(node.nodeSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if ((bind(node.nodeSocket, (struct sockaddr *)&node.my_Address, sizeof(node.my_Address))) < 0){
        perror("Failed to bind socket (serve).");
        exit(1);
    }
    if (listen(node.nodeSocket, SOMAXCONN) < 0){
        perror("Failed to listen.");
    } else {
        printf("Node listening.");
    }
    socklen_t other_add_length = (socklen_t) sizeof(node.other_Address);
    while(1){
        char hostClient[NI_MAXHOST];
        char portClient[NI_MAXSERV];
        memset(hostClient, 0, NI_MAXHOST);
        memset(portClient, 0, NI_MAXSERV);
        int newSocket = accept(node.nodeSocket, (struct sockaddr*) &node.other_Address, &other_add_length);
        inet_ntop(AF_INET, &node.other_Address.sin_addr, hostClient, NI_MAXHOST);
        std::cout << "--> " << hostClient << " connected to port " << ntohs(node.other_Address.sin_port) << std::endl;
        //Connection(node.myAddress, hostClient, node, newSocket, false);
        Connection* connection = new Connection(node.myAddress, hostClient, node, newSocket, false);
        delete connection;

        // Thread: While(pending_transactions not empty): receive->transactions->createBlock->submitBlock
    }
}

void connect(Node &node, char* address){
    Connection* connection = new Connection(node.myAddress, address, node, 0, false);
    delete connection;

}

void connect_to_all(Node &node){
    for (const auto &host : node.knownHosts){
        std::string add = host.first;
        int len = add.length();
        char address[len+1];
        strcpy(address, add.c_str());
        Connection* connection = new Connection(node.myAddress, address, node, 0, false);
        delete connection;
    }
}

// idea is that one thread always tries to build blocks, should be in Connect?
void block_builder(Node &node){
    // if node.builder == true {}
    while(1){
        Block new_block = node.CreateBlock();
        bool success = node.SubmitBlock(new_block);
        if (success){
            //send to hosts
        }
    }
}


// Netorking test over console echo ... gethosts|, getblocks|1,

int main(){
    // Test ECDSA
    std::cout << std::endl << "TEST ECDSA!" << std::endl;
    test_ecdsa();

    // Test Uint256
    std::cout << std::endl << "TEST UINT256!" << std::endl;
    test_uint();

    // Test Node
    std::cout << "TEST NODE!" << std::endl;
    test_unpack_blocks(); // tests block packing and unpacking
    test_node_block_creation(false);
    test_node_hosts_command();
    test_node_transactions_command();

    return 0;
}
