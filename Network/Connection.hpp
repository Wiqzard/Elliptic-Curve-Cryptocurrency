//
//  Connection.hpp
//  Neww
//
//  Created by Sebastian Stapf on 23.09.22.
//

#ifndef Connection_hpp
#define Connection_hpp

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <stdio.h>
//#include <iostream>
//#include <stdlib.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <unistd.h>
//#include <string>
//#include <sstream>
//#include <vector>
#include "Node.hpp"


class Connection{
public:
    int version = 1;
    int port = 9999;
    bool packet;
    std::string direction;
    struct sockaddr_in my_Address;
    struct sockaddr_in other_Address;
    char* myAddress;
    char* otherAddress;
    Node &in_node;
    int nodeSocket;
    
    
    // Change port stuff
    Connection(char* nmyAddress, char* notherAddress,Node &node, int nsocket, bool npacket);
    ~Connection();
    
    void nconnect();
    
    static std::vector<std::string> unpack_data(std::vector<char> &data, std::string);
    static std::vector<Block> unpack_blocks(std::vector<char> &data);

    
    void send_packet(char* packet, int size);
    void send_packet(std::vector<char> &packet);
    
    unsigned long get_our_block_height();
    
    void send_version();
    
    std::vector<char> get_packet();
    void do_command(std::vector<char> &data);
    
    // Packs data then sends "version,hosts,blocks|metadata"
    void cmd_version(std::vector<char> &data);
    void cmd_gethosts();
    void cmd_getblocks(std::vector<char> &data);
    void cmd_gettransactions();
    
    // cmd_getblocks but returns what otherwise would get send
    
    // Test functions
    static std::vector<char> cmd_getblocks_test(Blockchain blockchain, std::vector<char> &data);
    static std::vector<char> cmd_gethosts_test(Node node, std::vector<char> &data);
    static std::vector<std::pair<std::string, int>> cmd_hosts_test(std::vector<char> &data);
    static std::vector<char> cmd_gettransaction_test(Node node);
    static std::vector<Transaction> cmd_transactions_test(std::vector<char> &data);
    
    // Reacts on reception of data
    void cmd_verack();
    void cmd_hosts(std::vector<char> &data);
    void cmd_blocks(std::vector<char> &data);
    void cmd_transactions(std::vector<char> &data);
    
    //Make if clause if want to participate in transaction gathering
    
    
    //static std::vector<Block> statunpack_blocks(std::vector<char> &data);


    void go();
    
};


#endif /* Connection_hpp */
