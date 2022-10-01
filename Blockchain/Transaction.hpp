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

class User;
class Transaction
{
public:
//    User& sender;
//    User& receiver;
    float amount;
    Uint256 signature[2];
    std::string senderAddress;
    std::string receiverAddress;
  
    Transaction();
    //Transaction():Transaction(User{}, User{}, 0){};
//    Transaction(User& nSender, User& nReceiver, float nAmount);
    Transaction(std::string nSenderAddress, std::string nReceiverAddress, float nAmount);
    
    void SignTransaction(Uint256 r, Uint256 s);
    //sign Transaction
    std::vector<char> GetTransactionMetadata();
    // Abstract comparison operator with no meaning. Used to utilize sets
    friend bool operator< (const Transaction &left, const Transaction &right);

};


#endif /* Transaction_hpp */

