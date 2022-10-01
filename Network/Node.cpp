//
//  Node.cpp
//  Neww
//
//  Created by Sebastian Stapf on 19.09.22.
//

#include "Node.hpp"

Node::Node(char* nmyAddress): knownHosts{std::pair(std::string(nmyAddress), port)}, node_user{"B"}{
    struct sockaddr_in my_Address;
    struct sockaddr_in other_Address;
    myAddress = nmyAddress;
    std::cout << "Node created." << std::endl;
    //knownHosts.insert(std::pair<port, myAddress>);
}


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
Block Node::CreateBlock(){
    Block block = Block(blockchain.GetChainLength() + 1);
    block.prevHash = blockchain.GetLastBlock().currHash;
    if (pending_transactions.size() >= maxTransactions){
        int idx = rand()%pending_transactions.size();
        for (int i=0; i<maxTransactions; i++){
            auto it = pending_transactions.begin();
            for (int i = 0; i<idx; i++){
                it++;
            }
            block.AddTransaction(*it);
            buffer_transactions.insert(*it);
        }
    }
    return block;
}


//Waiting time should be assigned by admin node
void Node::AssignWaitingTime(){
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<int> dist(1, 10);
    waiting_time = dist(engine);
}

/*  Gets waiting time assigned
 *  Checks if blocks that came in while waiting time contain transactions in block
 *
 */
bool Node::SubmitBlock(Block &block){
    AssignWaitingTime();
    std::cout << "Time to wait: " << waiting_time << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
    long long difference = blockchain.GetChainLength() - block.GetBlockIndex();
    if (difference > 0){
        std::vector<Block> new_blocks = blockchain.GetSubChain(block.GetBlockIndex());
        for (int i=0; i<difference; i++){
            std::vector<Transaction> transactions = new_blocks[i].GetTransactions();
            std::copy(transactions.begin(), transactions.end(), std::inserter(new_transactions, new_transactions.end()));
        }
    }
    std::vector<Transaction> intersection;
    set_intersection(buffer_transactions.begin(), buffer_transactions.end(), new_transactions.begin(), new_transactions.end(), std::back_inserter((intersection)));
    new_transactions.clear();
    if (intersection.empty()){
        blockchain.SimpleAddBlock(block);
        
        User admin = User("admin");
        float fee = 10;
        Transaction reward = admin.CreateTransaction(node_user.GetAddress(), fee);
        return true;
    } else {
        int set_size = intersection.size();
        for (int i=0; i<set_size; i++){
            pending_transactions.erase(intersection[i]);
        }
        return false;
    }
}

bool Node::AddHost(std::string address, int port){
    char buffer[16];
    strcpy(buffer, address.c_str());
    int check = inet_pton(AF_INET, address.c_str(), buffer);
    if (port <= 65535 && check!=-1){
        std::pair<std::string, int> host = std::pair(address, port);
        knownHosts.insert(host);
        return true;

    } else{
        std::cout << "Address or port of host invalid" << std::endl;
        return false;
    }
}

//check sig, check amount
bool Node::AddTransaction(Transaction transaction){
    bool sign = Block::CheckSignatureStatic(transaction);
    if (sign){
        std::string sender_address = transaction.senderAddress;
        float sender_balance = 0;
        for (auto &block : blockchain.GetChain()){
            std::vector<Transaction> transactions = block.GetTransactions();
            for (auto &transaction : transactions){
                if(transaction.receiverAddress == sender_address){
                    sender_balance += transaction.amount;
                }
            }
        }
        if (sender_balance > transaction.amount){
            pending_transactions.insert(transaction);
            new_transactions.insert(transaction);
            return true;
        } else {
            std::cout << "Balance of sender not sufficent. Balance: " << sender_balance << ". But required: " << transaction.amount << std::endl;
            return false;
        }
    } else {
        std::cout << "Signature invalid." << std::endl;
        return false;
    }
}

