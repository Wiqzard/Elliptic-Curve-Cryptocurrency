//
//  User.hpp
//  Blockchain
//
//  Created by Sebastian Stapf on 13.09.22.
//

#ifndef User_hpp
#define User_hpp


//#include <iostream>
//#include <vector>
//#include "Sha256Hash.hpp"
//#include "Utils.hpp"
#include "Ecdsa.hpp"
#include "Sha256.hpp"
#include "Transaction.hpp"




class User{
public:
    const bool RANDOM_PRIVATE_KEY = true;
    static constexpr int ADDRESS_LENGTH = 64;
    
    User();
    
    // Constructs user simply with name, calls AssignPrivatePublicKey()
    User(const std::string nName);
    
    // Copy constructor makes sure that User list hast no duplicates
    User(const User &user);
  
    
    void AssignPrivatePublicKey();

    // Implement Hmac instead of randString/mainly used for testing
    void CreateSignature(const std::string message);
    
    // Implement Hmac instead of randString
    std::vector <Uint256> CreateSignature(Sha256Hash msgHash);
    
    Transaction CreateTransaction(const std::string receiverAddress, float amount);
    
    void SignTransaction();

    //Concatinate both addresses, amount as string int without dot, and rest padded with 0s to 64 string
    //ONLY USED FOR SIGNING AND VERIFICATION
    static Sha256Hash HashTransaction(const std::string senderAddress, const std::string receiverAddress, float amount);


    
    const bool IsNameOfUser(std::string nName);
    
    User& operator=(const User &user);
    
    const std::string GetAddress();
    const std::string GetName();
    const CurvePoint GetPublicKey();
    const Uint256 GetPrivateKey(){return privateKey;}
    static const std::vector<User*>& GetUsers();
    
private:
    std::string name;
    std::string address;
    Uint256 privateKey;
    CurvePoint publicKey;
    static int currID;
    int userID;
    static std::vector<User*> users;
};






#endif /* User_hpp */
