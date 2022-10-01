//
//  Blockchain.hpp
//
//  Created by Sebastian Stapf on 15.09.22.
//

#ifndef Blockchain_h
#define Blockchain_h

#include <random>
#include "Block.hpp"


class Blockchain
{
public:
    
    Blockchain(){};
    ~Blockchain(){};
    
    // Assigns currHash of last block in chain to newBlocks prevHash and assigns newBlock currHash the hash of newBlock. Then adds to blockchain.
    const void AddBlock(Block newBlock);
    
    // Adds block to blockchain.
    const void SimpleAddBlock(Block block);
    
    // Ignore for now.
    bool AddTransactionB(Block &curBlock, std::string &sender, std::string &receiver, float &amount);
    
    const unsigned long GetChainLength();
    Block GetLastBlock();
    const Block GetFirstBlock(){return chain[0];}
    //Returns chain from starting in chain to end.
    const std::vector<Block> GetSubChain(unsigned long);
    const std::vector<Block> GetChain(){return chain; }
    
private:
    std::vector<Block> chain = {Block::CreateGenesisBlock()};
};



#endif /* Blockchain_h */
