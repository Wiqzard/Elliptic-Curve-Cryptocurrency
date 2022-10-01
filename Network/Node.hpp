//
//  Node.hpp
//  Neww
//
//  Created by Sebastian Stapf on 19.09.22.
//

#ifndef Node_hpp
#define Node_hpp

//#include <string>
//#include <unistd.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <regex.h>
//#include <thread>
//#include <iostream>
//#include <utility>
//#include <chrono>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <set>
#include <thread>
#include <random>

#include "Blockchain/Blockchain.hpp"

class Node{
public:
    struct sockaddr_in my_Address;
    struct sockaddr_in other_Address;
    char* myAddress;
    char* otherAddress;
    int port = 9999;
    int nodeSocket;
    int domain;
    int service;
    int protocol;
    int maxTransactions;
    int waiting_time;
    
    User node_user;
    Blockchain blockchain;
    //Connection connection;
    u_long interface;
    //std::vector<std::pair<int, std::string>> knownHosts;
    //port, ip
    std::set<std::pair<std::string, int>> knownHosts;
    
    // Transaction pool from which blocks get built.
    std::set<Transaction> pending_transactions;
    // Transactions that node currently wants to add to block.
    std::set<Transaction> buffer_transactions;
    // Transactions that arrived while node waits to submit block.
    std::set<Transaction> new_transactions;
    
    //Transaction list
    Node();
    Node(char*);
    
    void connect();
    
    Block CreateBlock();
    
    bool SubmitBlock(Block &block);
    
    // Checks if port and address valid, then adds
    bool AddHost(std::string address, int port);
    
    // Check if signed, Check if balance ok (already done in block), then add to pending+new
    bool AddTransaction(Transaction transaction);
    
    void AssignWaitingTime();
    

    
    
    Node(int domain, int service, int protocol, int port, u_long interface, void * (*server_function)(void *arg), void * (*client_function)(void *arg));
    
    
    /*
     
     
     
     
     */
    
};
#endif /* Node_hpp */
