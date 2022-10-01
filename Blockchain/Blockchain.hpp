//
//  Blockchain.hpp
//  Neww
//
//  Created by Sebastian Stapf on 15.09.22.
//

#ifndef Blockchain_h
#define Blockchain_h

//#include <cstdint>
//#include <vector>
#include "Block.hpp"


class Blockchain
{
public:
    static const int MAX_WAITING_TIME = 20;
//    Block currBlock;
    Blockchain();
//    ~Blockchain();
    //static bool CheckBlockAddable();
    
    void AddBlock(Block newBlock);
    
    void SimpleAddBlock(Block block);
    
    std::string TransactionsToString(bool byName, Block newBlock);

    Block GetLastBlock();
    Block GetFirstBlock(){return chain[0] ;}
    
    bool AddTransactionB(std::string sender, std::string receiver, float amount);
    
    bool RunBlockProtocol(User blockMaster);
    
    unsigned long GetChainLength();
    
    std::vector<Block> GetSubChain(unsigned long);
    std::vector<Block> GetChain(){return chain; }
private:
//    Block block;
    uint32_t avgWaitingTime;
    std::vector<Block> chain = {Block::CreateGenesisBlock()};
};



#endif /* Blockchain_h */
