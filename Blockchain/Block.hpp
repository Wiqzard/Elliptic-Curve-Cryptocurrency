//
//  Block.hpp
//
//  Created by Sebastian Stapf on 15.09.22.
//

#ifndef Block_hpp
#define Block_hpp

#include "User.hpp"


class Block
{
public:
    static constexpr int MAX_TRANSACTIONS = 2;
    Sha256Hash prevHash;
    Sha256Hash currHash;
    User blockMaster;
    
    Block();
    
    Block(uint32_t nIndexIn);
    
    Block(uint32_t nIndexIn, time_t tTime, Sha256Hash prevHash);
    
    Block(uint32_t nIndex, time_t tTime, Sha256Hash prevHash, Sha256Hash currHash, std::vector<Transaction> transactions): _nIndex(nIndex), _tTime(tTime), prevHash(prevHash), currHash(currHash), transactions(transactions){}
    
    // Creates a block with _nIndex=0, _tTime= *now and fixed prevHash.
    static Block CreateGenesisBlock();
    
    const bool CheckSignature(Transaction transaction);
    const static bool CheckSignatureStatic(Transaction transaction);
    
    // Checks transaction's signature before adding it to the block. (The balance of the sender gets checked when adding the block to the blockchain)
    void AddTransaction(Transaction transaction);
        
    
    /* Returns the transactions in readable form:
     * "sender1address" sends "receiver1address" "amount".
     *                  .......                           */
    std::string TransactionsToString(bool byName);
    
    // Creates a hash of the block containing all of its information.
    Sha256Hash HashBlock();
    
    // Returns block data as byte array.
    std::vector<char> GetBlockMetaData();
    
    
    /*---- Setter/Getter methods ----*/
    
    const uint32_t GetBlockIndex(){ return _nIndex;}
    const time_t GetBlockTime(){ return _tTime; }
    const Sha256Hash GetPrevHash(){ return prevHash;}
    const Sha256Hash GetCurrHash(){ return currHash;}
    std::vector<Transaction> const GetTransactions () const;
    
    void SetBlockTime(time_t time) { _tTime = time;}
    void SetPrevHash(Sha256Hash _prevHash){prevHash=_prevHash;}
    void SetCurrHash(Sha256Hash _currHash){currHash=_currHash;}
    void SetTransactions(std::vector<Transaction> _transasctions){transactions = _transasctions;}
    
private:
    uint32_t _nIndex;
    time_t _tTime;
    std::vector<Transaction> transactions;

};

#endif

