//
//  Transaction.cpp
//  Blockchain
//
//  Created by Sebastian Stapf on 13.09.22.
//

#include "Transaction.hpp"

//Transaction::Transaction(User& nSender, User& nReceiver, float nAmount)
//:sender(nSender), receiver(nReceiver), amount(nAmount) {}

Transaction::Transaction(std::string nSenderAddress, std::string nReceiverAddress, float nAmount) : senderAddress(nSenderAddress), receiverAddress(nReceiverAddress), amount(nAmount){};


void Transaction::SignTransaction(Uint256 r, Uint256 s){
    this->signature[0] = r;
    this->signature[1] = s;
}
bool operator< (const Transaction &left, const Transaction &right){
    return left.amount < right.amount;
}
std::vector<char> Transaction::GetTransactionMetadata(){
    std::vector <char> meta_data;
    std::string sender = senderAddress;
    std::string receiver = receiverAddress;
    for (char const &c: sender){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    for (char const &c: receiver){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%f", amount);
    for (const char& c: buffer){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    std::vector<char> r_vec = signature[0].Uint256ToVec();
    std::vector<char> s_vec = signature[1].Uint256ToVec();
    for (const char &c : r_vec){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    for (const char &c : s_vec){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');

    return  meta_data;
}

//Make sure the sender signs the transaction with private key
//void Transaction::SignTransaction(){
//    this->signature = nSignature;
//}
