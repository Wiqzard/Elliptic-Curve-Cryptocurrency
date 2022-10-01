//
//  Connection.cpp
//
//  Created by Sebastian Stapf on 23.09.22.
//

#include "Connection.hpp"


// nmyAddress and notherAddress should be char* ip addresses, '127.0.0.1'
Connection::Connection(char* nmyAddress, char* notherAddress, Node &node, int nsocket=0, bool npacket=false) : inNode(node){
    _myAddress = nmyAddress;
    _otherAddress = notherAddress;
    this->packet = npacket;
    myAddress.sin_family = AF_INET;
    myAddress.sin_port = htons(port);
    myAddress.sin_addr.s_addr = INADDR_ANY;
    nodeSocket = nsocket;
    if (nodeSocket == 0){
        nodeSocket = socket(AF_INET, SOCK_STREAM, 0);
        direction = "outgoing";
        if ((bind(nodeSocket, (struct sockaddr *)&myAddress, sizeof(myAddress))) < 0){
            perror("Failed to bind socket (connect).");
            close(nodeSocket);
            exit(1);
        }
    }else{
        this->nodeSocket = nsocket;
        direction = "incoming";
    }
    if (nodeSocket == 0){
        perror("Failed to connect socket...");
        exit(1);
    }
    std::cout << "Node running." << std::endl;
    Connection::go();
    close(nodeSocket);
}

Connection::~Connection(){
    std::cout << "Connection deleted." << std::endl;
}

void Connection::nconnect(){
    otherAddress.sin_family = AF_INET;
    otherAddress.sin_port = htons(port);
    otherAddress.sin_addr.s_addr = INADDR_ANY;
    int k = inet_pton(AF_INET, _otherAddress, &otherAddress.sin_addr);
    int j;
    inet_pton(AF_INET, _otherAddress, &j);
    int i = connect(nodeSocket, (struct sockaddr*)&otherAddress, sizeof(otherAddress));
    printf("Connection to server successful! \n");
    std::cout << "i = connection:" << i << " inet_pton " << j << "value: " << k << std::endl;
}

void Connection::send_packet(char* packet, int size){
    send(nodeSocket, packet, size, 0);
    std::string str(packet);
    std::cout << "send packet body 2: " << packet << std::endl;
}

void Connection::send_packet(std::vector<char> &packet){
    send(nodeSocket, packet.data(), packet.size(), 0);
}

unsigned long Connection::get_our_block_height(){
    return inNode.blockchain.GetChainLength();
}

void Connection::send_version(){
    std::string str(_myAddress);
    std::stringstream ss;
    ss << "version" << '|' << version << '|' << std::time(0) << '|' << _myAddress << '|' << _otherAddress << '|' << get_our_block_height();
    std::string temp = ss.std::stringstream::str();
    std::vector<char> version_meta_data(temp.begin(), temp.end());
    send_packet(version_meta_data);
}

std::vector<char> Connection::get_packet(){
    std::vector<char> payload(1024);
    long bytes = recv(nodeSocket, payload.data(), payload.size(), 0);
    std::cout << "get_packet body: payload bytes received: " << bytes << std::endl;
    return payload;
}

std::vector<std::string> Connection::unpack_data(std::vector<char> &data, std::string delimiter){
    std::string s(data.begin(), data.end());
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos){
        token = s.substr(pos_start, pos_end -pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

void Connection::do_command(std::vector<char> &payload){
    std::vector<std::string> load = unpack_data(payload, "|");
    std::string cmd = load[0];

    if (cmd == "version"){
        std::cout << "case version" << std::endl;
        cmd_version(payload);
        
    } else if (cmd == "verack"){
        std::cout << "case verack" << std::endl;
        
    } else if (cmd == "gethosts"){
        std::cout << "case gethosts" << std::endl;
        cmd_gethosts();
        
    } else if (cmd == "getblocks") {
        std::cout << "case getblocks" << std::endl;
        cmd_getblocks(payload);
        
    } else if (cmd == "gethosts") {
        std::cout << "case gethosts" << std::endl;
        cmd_gethosts();
        
    } else if (cmd == "getrans"){
        std::cout << "case getrans" << std::endl;
        cmd_gettransactions();
        
    } else if (cmd == "blocks"){
        std::cout << "case blocks" << std::endl;
        cmd_blocks(payload);
        
    } else {
        std::cout << "Command not found!" << std::endl;
    }
}

void Connection::cmd_version(std::vector<char> &data){
    char cmd[8] = "verack";
    send_packet(cmd, 8);
    std::vector<std::string> data_del = unpack_data(data, "|");
    if ((int)data_del.size() < 13){
        std::cout << "Error in received data." << std::endl;
    }
    unsigned long behind = std::stoul(data_del[5]) - inNode.blockchain.GetChainLength();
    if (behind < 0){
        std::stringstream ss;
        ss << "getblocks|" << inNode.blockchain.GetChainLength();
        char cmd[64];
        strcpy(cmd, ss.str().c_str());
        send_packet(cmd, 64);
    }
    return;
}

void Connection::cmd_verack(){
    char cmd[9] = "gethosts";
    send_packet(cmd, 9);
}

void Connection::cmd_gethosts(){
    // hosts|host1_ip|host1_port|-|host2_ip|host2_port|-...
    std::vector<char> meta_data;
    std::string cmd = "hosts";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }
    for (const auto &host :inNode.knownHosts){
        std::string host_address = host.first;
        int host_port = host.second;
        // port1|address1-port2|address2-port3...
        std::string temp = std::to_string(host_port);
        for (char const &c: temp){
            meta_data.push_back(c);
        }
        meta_data.push_back('|');
        for (char const &c: host_address){
            meta_data.push_back(c);
        }
        meta_data.push_back('-');
    }
    send_packet(meta_data);
}

std::vector<char>  Connection::cmd_gethosts_test(Node node, std::vector<char> &data){
    std::vector<char> meta_data;
    std::string cmd = "hosts";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }
    int i = 0;
    for (const auto &host :node.knownHosts){
        std::string host_address = host.first;
        int host_port = host.second;
        std::string temp = std::to_string(host_port);
        meta_data.push_back('|');
        for (char const &c: temp){
            meta_data.push_back(c);
        }
        meta_data.push_back('|');
        for (char const &c: host_address){
            meta_data.push_back(c);
        }
        meta_data.push_back('|');
        if (i != node.knownHosts.size()-1){
            meta_data.push_back('-');
        }
        i++;
    }
//    std::string s(meta_data.begin(), meta_data.end());
//    std::cout << s << std::endl;
    return meta_data;
}

void Connection::cmd_hosts(std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "-");
    std::vector<std::pair<std::string, int>> host_pairs;
    std::string host_address;
    int host_port;
    for (auto const &host : load){
        std::vector<char> load_vec(host.begin(), host.end());
        std::vector<std::string> host_str = unpack_data(load_vec, "|");
        host_port = std::stoi(host_str [1]);
        host_address = host_str[2];
        host_pairs.push_back(std::pair(host_address, host_port));
        inNode.AddHost(host_address, host_port); // Does the checking for node
    }
}

std::vector<std::pair<std::string, int>> Connection::cmd_hosts_test(std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "-");
    std::vector<std::pair<std::string, int>> host_pairs;
    std::string host_address;
    int host_port;
    for (auto const &host : load){
        std::vector<char> load_vec(host.begin(), host.end());
        std::vector<std::string> host_str = unpack_data(load_vec, "|");
        host_port = std::stoi(host_str [1]);
        host_address = host_str[2];
        host_pairs.push_back(std::pair(host_address, host_port));
    }
    return host_pairs;
}

void Connection::cmd_getblocks(std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "|");
    unsigned long start_height = std::stoul(load[1]);
    unsigned long behind = inNode.blockchain.GetChainLength()-start_height;
    std::vector<Block> requested_blocks = inNode.blockchain.GetSubChain(start_height);
    std::vector<char> meta_data;
    std::string cmd = "blocks";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }
    for (int i = 0; i<behind; i++){
        std::vector<char> block_meta_data = requested_blocks[i].GetBlockMetaData();
        meta_data.insert(meta_data.end(), block_meta_data.begin(), block_meta_data.end());
        if (i != behind-1){
            meta_data.push_back('-');
        }
    }
    send_packet(meta_data);
}

std::vector<char> Connection::cmd_getblocks_test(Blockchain blockchain, std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "|");
    unsigned long start_height = std::stoul(load[1]);
    unsigned long behind = blockchain.GetChainLength()-start_height;
    std::vector<Block> requested_blocks = blockchain.GetSubChain(start_height);
    std::vector<char> meta_data;
    std::string cmd = "blocks";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }
    for (int i = 0; i<behind; i++){
        std::vector<char> block_meta_data = requested_blocks[i].GetBlockMetaData();
        meta_data.insert(meta_data.end(), block_meta_data.begin(), block_meta_data.end());
        if (i != behind-1){
            meta_data.push_back('-');
        }
    }
    return meta_data;
}


void Connection::cmd_blocks(std::vector<char> &data){
    std::vector<Block> new_blocks = unpack_blocks(data);
    for (auto &block: new_blocks){
        if (block.prevHash == inNode.blockchain.GetLastBlock().currHash){
            if (block.GetBlockIndex() == inNode.blockchain.GetLastBlock().GetBlockIndex()+1){
                inNode.blockchain.AddBlock(block);
            } else{
                std::cout << "Block indices missmatch." << std::endl;
            }
            
        } else{
            std::cout << "Block hash missmatch." << std::endl;
        }
    }
}

void Connection::cmd_gettransactions(){
    std::vector<char> meta_data;
    std::string cmd = "transactions";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }
    int i = 0;
    for (auto transaction : inNode.pending_transactions){
        std::vector<char> transaction_meta_data = transaction.GetTransactionMetadata();
        meta_data.insert(meta_data.end(), transaction_meta_data.begin(), transaction_meta_data.end());
        if (i != inNode.pending_transactions.size()-1){
            meta_data.push_back('-');
        }
        i++;
    }
    send_packet(meta_data);
}

 std::vector<char> Connection::cmd_gettransaction_test(Node node){
    std::vector<char> meta_data;
    std::string cmd = "transactions";
    for (const char &c: cmd){
        meta_data.push_back(c);
    }

     int i = 0;
    for (auto transaction : node.pending_transactions){
        std::vector<char> transaction_meta_data = transaction.GetTransactionMetadata();
        meta_data.push_back('|');
        meta_data.insert(meta_data.end(), transaction_meta_data.begin(), transaction_meta_data.end());
        if (i != node.pending_transactions.size()-1){
            meta_data.push_back('-');
        }
        i++;
    }
    return meta_data;
}

void Connection::cmd_transactions(std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "-");
    std::vector<Transaction> transactions;
    std::string host_address;
    for (auto const &trans : load){
        std::vector<char> load_vec(trans.begin(), trans.end());
        std::vector<std::string> transaction_str = unpack_data(load_vec, "|");
        Transaction transaction = Transaction(transaction_str[1], transaction_str[2], stof(transaction_str[3]));
        Uint256 r = Uint256(transaction_str[4]);
        Uint256 s = Uint256(transaction_str[5]);
        transaction.SignTransaction(r, s);
        inNode.AddTransaction(transaction); // Does the checking for node
    }
}
std::vector<Transaction> Connection::cmd_transactions_test(std::vector<char> &data){
    std::vector<std::string> load = unpack_data(data, "-");
    std::vector<Transaction> transactions;
    std::string host_address;
    int host_port;
    for (auto const &trans : load){
        std::vector<char> load_vec(trans.begin(), trans.end());
        std::vector<std::string> transaction_str = unpack_data(load_vec, "|");
        Transaction transaction = Transaction(transaction_str[1], transaction_str[2], stof(transaction_str[3]));
        Uint256 r = Uint256(transaction_str[4]);
        Uint256 s = Uint256(transaction_str[5]);
        transaction.SignTransaction(r, s);
        transactions.insert(transactions.begin(), transaction);
        //transactions.push_back(transaction);
    }
    return transactions;
}

std::vector<Block> Connection::unpack_blocks(std::vector<char> &data){
    std::vector<Block> new_blocks;
    std::vector<std::string> blocks = unpack_data(data, "-");
    int idx = 1;
    for (auto const& block: blocks){
        std::vector<char> block_c(block.begin(), block.end());
        std::vector<std::string> block_data = unpack_data(block_c, "|");
        //_nIndex|_tTime|prevHash|currHash|senderAddr1|
        // receiveraddr1|amount1|r1|s1|senderAddr2|receiveraddr2|amount2|r2|s2|)
        //Construct data from Metadata
        uint32_t nIndex = static_cast<uint32_t>(stoul(block_data[idx]));
        time_t tTime = (time_t) stoul(block_data[idx+1]);
        Sha256Hash prevHash = Sha256Hash(block_data[idx+2]);
        Sha256Hash currHash = Sha256Hash(block_data[idx+3]);
        int nMaxTransactions = 2;
        std::vector<Transaction> transactions;
        for (int i = 0; i < nMaxTransactions; i++){
            Transaction transaction = Transaction(block_data[idx+4+i*5], block_data[idx+5+i*5], stof(block_data[idx+6+i*5]));
            Uint256 r = Uint256(block_data[idx+7+i*5]);
            Uint256 s = Uint256(block_data[idx+8+i*5]);
            transaction.SignTransaction(r, s);
            transactions.push_back(transaction);
            }
        Block constructed_block = Block(nIndex, tTime, prevHash, currHash, transactions);
        //std::cout << nIndex << "constr block prevHash" << std::endl;
        //std::cout << constructed_block.prevHash.Sha256HashToString() << std::endl;
        assert(constructed_block.CheckSignature(transactions[0]));
        assert(constructed_block.CheckSignature(transactions[1]));
        
        if (constructed_block.CheckSignature(transactions[0]) && constructed_block.CheckSignature(transactions[1]))
        {
            new_blocks.push_back(constructed_block);
        }
    }
    return new_blocks;
}

void Connection::go(){
    if (direction == "outgoing"){
        nconnect();
    }
    send_version();
    while(1){
        std::vector<char> cmdpckg = get_packet();
        std::string cmdload(cmdpckg.data());
        std::string delimeter = "|";
        std::string cmd = cmdload.substr(0, cmdload.find(delimeter));
        cmd.pop_back();
        std::cout << "Received command: " << cmdload << std::endl;
        if ( cmd == "_"){
            std::cout << "No more commands.?" << std::endl;
            break;
        }else if(cmd.empty()){
            break;
        }
        do_command(cmdpckg);
        
    }
}

