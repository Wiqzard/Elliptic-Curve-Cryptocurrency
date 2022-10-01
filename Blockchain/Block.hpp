//
//  Block.hpp
//  Neww
//
//  Created by Sebastian Stapf on 15.09.22.
//

#ifndef Block_hpp
#define Block_hpp

//#include <cstdint>
//#include <iomanip>
//#include <iostream>
//#include <sstream>
//#include <algorithm>
//#include <vector>
//#include <array>
//#include <time.h>
//#include <stdio.h>
//#include "Transaction.hpp"
//#include "Sha256Hash.hpp"
//#include "Sha256.hpp"
//#include "Ecdsa.hpp"
//#include "Utils.hpp"
#include "User.hpp"


class Block
{
public:

//    std::vector<Transaction> transactions(1);
//    Block(uint32_t nIndexIn, const std::string &sDataIn);
    
    Sha256Hash prevHash;
    Sha256Hash currHash;
    User blockMaster;
    static const int nMaxTransactions = 2;
    
    
    Block();
    Block(uint32_t nIndexIn);
    Block(uint32_t nIndex, time_t tTime, Sha256Hash prevHash, Sha256Hash currHash, std::vector<Transaction> transactions): _nIndex(nIndex), _tTime(tTime), prevHash(prevHash), currHash(currHash), transactions(transactions){}
    
    static Block CreateGenesisBlock();
    
    int CheckBalance();
    
    bool CheckSignature(Transaction transaction);
    static bool CheckSignatureStatic(Transaction transaction);

    void AddTransaction(Transaction transaction);
    
    void MineBlock(uint32_t nDifficulty); /* Add Miner Reward at End*/
    
    std::string TransactionsToString(bool byName);
    Sha256Hash HashBlock();
    std::vector<char> GetBlockMetaData();
    
    uint32_t GetBlockIndex(){ return _nIndex;}
    time_t GetBlockTime(){ return _tTime; }
    std::vector<Transaction> GetTransactions();
    void SetBlockTime(time_t time) { _tTime = time;}
    
    //string of headers, transactions, delimeted with |
private:
    uint32_t _nIndex;
    uint32_t _nNonce;
    time_t _tTime;
    std::string sHash;
    //Sha256Hash currentHash;
    std::string sPrevHash;
    
    std::vector<Transaction> transactions;
    
//    std::array<Transaction, 2> transactions;
    
    std::string _CalculateHash() const;
};

#endif

