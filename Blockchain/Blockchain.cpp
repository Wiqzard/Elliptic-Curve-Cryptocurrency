//
//  Blockchain.hpp
//
//  Created by Sebastian Stapf on 15.09.22.
//

#include "Blockchain.hpp"
//


std::vector<User*> User::users;
int User::currID;

//
//Blockchain::~Blockchain(){
//    chain.erase(chain.begin()+chain.size());
//}

Block Blockchain::GetLastBlock(){return chain.back();}

const void Blockchain::AddBlock(Block newBlock){
    newBlock.prevHash = Blockchain::GetLastBlock().currHash;
    newBlock.currHash = newBlock.HashBlock();
    chain.push_back(newBlock);
}

const void Blockchain::SimpleAddBlock(Block block){
    chain.push_back(block);
}

bool isNameofUser2(const std::string& nName, User user){
    return user.GetName() == nName;
}

bool Blockchain::AddTransactionB(Block &curBlock, std::string &sender, std::string &receiver, float &amount){
    User uSender;
    User uReceiver;
    assert(curBlock.GetTransactions().size() < Block::MAX_TRANSACTIONS);

    auto it_s = std::find_if(User::GetUsers().begin(), User::GetUsers().end(),
                           [&sender](User *obj)
                           {return obj->GetName() == sender;});
    auto it_r = std::find_if(User::GetUsers().begin(), User::GetUsers().end(),
                                 [&receiver](User *obj)
                                 {return obj->GetName() == receiver;});
    if (it_s != User::GetUsers().end()){
        User uSender = **it_s;
    }
    if (it_r != User::GetUsers().end()){
        User uReceiver = **it_r;
    }
    Transaction transaction = uSender.CreateTransaction(uReceiver.GetAddress(), amount);
    curBlock.AddTransaction(transaction);
    std::random_device rd;
    std::mt19937 rng(rd());
    int MAX_WAITING_TIME = 5;
    std::uniform_int_distribution<int> uni(1, MAX_WAITING_TIME);
    if (curBlock.GetTransactions().size() == Block::MAX_TRANSACTIONS){
        for (auto const& value: User::GetUsers()){
            auto random_integer = uni(rng);
            //value->waitingTime = random_integer;
        }
    }
    return true;
}

const unsigned long Blockchain::GetChainLength(){
    return chain.size();
}

const std::vector<Block> Blockchain::GetSubChain(unsigned long start){
    std::vector<Block>::const_iterator first = chain.begin() + start;
    std::vector<Block>::const_iterator last = chain.begin() + GetChainLength();
    std::vector<Block> sub_chain(first, last);
        return sub_chain;
}



