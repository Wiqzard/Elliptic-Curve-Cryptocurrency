//
//  Transaction.hpp
//  Blockchain
//
//  Created by Sebastian Stapf on 13.09.22.
//

#ifndef Transaction_hpp
#define Transaction_hpp



//#include <iostream>

#include "Uint256.hpp"

class Transaction
{
public:

    float amount;
    Uint256 signature[2];
    std::string senderAddress;
    std::string receiverAddress;
  
    Transaction();
    Transaction(std::string nSenderAddress, std::string nReceiverAddress, float nAmount);
    
    //Only assigns given signatures.
    void SignTransaction(Uint256 r, Uint256 s);
    
    //Returns byte array metadata of transaction.
    std::vector<char> GetTransactionMetadata();
    
    // Abstract comparison operator with no meaning. Used to utilize sets
    friend bool operator< (const Transaction &left, const Transaction &right);

};


#endif

