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

    u_long interface;
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
    
    // Randomly assigns waiting time between 0 and MAX_WAITING_TIME. Should be managed by external alpha node.
    void AssignWaitingTime();
    
    // Creates new block with prevHash == currHash of last block in chain, if enough transactions gathered in pending_transactions.
    Block CreateBlock();
    
    // Gets assigned a waiting time. After waiting time is over checks if new blocks came in that contain transactions in block. If not adds block to blockchain.
    bool SubmitBlock(Block &block);
    
    // Checks if port and address valid, then adds.
    bool AddHost(std::string address, int port);
    
    // Checks signature and if balance of sender ok (already done in block), then add to pending+new.
    bool AddTransaction(Transaction transaction);
    
private:
    static constexpr int MAX_WAITING_TIME = 5;

    
};
#endif /* Node_hpp */
/*
 Theoretically: 1 Thread Serving, 1 Thread Connecting(Broadcasting block), 1 Thread Transactions..
 Broadcast Transactions decoupled from block exchange
 Store Transaction + Transaction hash in Transactions
 ->Make block with transactions choosen
 ->submit block
 -> get assigned random time
 -> wait time
 -> check if incoming blocks contain transactions on my block
 -> if not: block valid, get reward upload block
 */
