//#include "Blockchain.h"
//#include "sha256.h"
#include <iostream>
#include "Utils.hpp"
#include "User.hpp"
#include "Block.hpp"
//#include "TestHelper.hpp"
//#include "Sha256.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include "Transaction.hpp"
std::vector<User*> User::users;
int User::currID;
int main()
{
    Block::CreateGenesisBlock();
    std::vector<User*>& jp = User::GetUsers();
//    Bytes test = asciiBytes("PMrX2]fs0pr^oF;03dA:Gu=!.Tp~Yf<{X^Dj#~'OMz0.a$]ti#UH:3MlX`\\f8uC%rp.`={\"AmdAee6>h8`=Iaw");
//    Sha256Hash actualHash = Sha256::getHash(test.data(), test.size());
//
//

//    int len = 64;
//    std::string number = Utils::genRandom(len);
//    std::cout << number << std::endl << number.length()<< std::endl;
//
//    User sebastian = User("Sebastian");
//    User bernd = User("Bernd");
//    sebastian.AssignPrivatePublicKey();
////    sebastian.CreateSignature("YOYO");
//    std::string address = sebastian.GetAddress();
//    std::cout << address << std::endl;
//    std::string test_string = Utils::genRandom(64);
//    Sha256Hash test_hash = Sha256Hash(test_string);
//    sebastian.CreateSignature(test_string);
//    sebastian.CreateSignature(test_hash);
//    Sha256Hash test_transaction_hash = sebastian.HashTransaction(bernd, 10);
//    std::cout << "Hello " << test_transaction_hash.value[10] << std::endl;
//    sebastian.CreateTransaction(bernd, 10);
//    std::string test_hash2string = test_hash.Sha256HashToString();
//    std::cout << test_hash2string << std::endl;

//    string a = sha256("Hello World");
//    std::cout << a;


    /* Blockchain bChain = Blockchain();

    cout << "Mining block 1..." << endl;
    bChain.AddBlock(Block(1, "Block 1 Data"));

    cout << "Mining block 2..." << endl;
    bChain.AddBlock(Block(2, "Block 2 Data"));

    cout << "Mining block 3..." << endl;
    bChain.AddBlock(Block(3, "Block 3 Data"));
*/
    return 0;
}
