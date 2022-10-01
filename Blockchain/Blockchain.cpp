#include "Blockchain.hpp"
#include <random>

//Blockchain::Blockchain()
//{
//    chain.emplace_back(Block::CreateGenesisBlock());
//}
//Blockchain::Blockchain() : chain(Block::CreateGenesisBlock()){}
std::vector<User*> User::users;
int User::currID;
Blockchain::Blockchain(){}

//
//Blockchain::~Blockchain(){
//    chain.erase(chain.begin()+chain.size());
//}

//std::string Blockchain::TransactionsToString(bool byName, Block newBlock){
//    std::string temp = "";
//    if (byName){
//        for (auto const& value: newBlock.GetTransactions()){
//            temp += value.sender.GetName() + "sends " + value.receiver.GetName() + " " + std::to_string(value.amount)+ "HoboCoin\n";
//        }
//    }else{
//        for (auto const& value: newBlock.GetTransactions()){
//            temp += value.sender.GetAddress() + "sends " + value.receiver.GetAddress() + " " + std::to_string(value.amount)+ "HoboCoin\n";
//        }
//    }
//    return temp;
//}
std::string Blockchain::TransactionsToString(bool byName, Block newBlock){
    std::string temp = "";
    if (byName){
        for (auto const& value: newBlock.GetTransactions()){
            temp += value.senderAddress + "sends " + value.receiverAddress + " " + std::to_string(value.amount)+ "HoboCoin\n";
        }
    }else{
        for (auto const& value: newBlock.GetTransactions()){
            temp += value.senderAddress + "sends " + value.receiverAddress + " " + std::to_string(value.amount)+ "HoboCoin\n";
        }
    }
    return temp;
}

//Block Blockchain::GetCurrBlock(){return currBlock;}
Block Blockchain::GetLastBlock(){return chain.back();}

void Blockchain::AddBlock(Block newBlock){
//    assert(Blockchain::CheckBlockAddable());
    newBlock.prevHash = Blockchain::GetLastBlock().currHash;
    newBlock.currHash = newBlock.HashBlock();
    chain.push_back(newBlock);
}

void Blockchain::SimpleAddBlock(Block block){
    chain.push_back(block);
}

bool isNameofUser2(const std::string& nName, User user){
    return user.GetName() == nName;
}

bool AddTransactionB(Block &curBlock, std::string &sender, std::string &receiver, float &amount){
    User uSender;
    User uReceiver;
    assert(curBlock.GetTransactions().size() < Block::nMaxTransactions);

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
    std::uniform_int_distribution<int> uni(1, Blockchain::MAX_WAITING_TIME);
    if (curBlock.GetTransactions().size() == Block::nMaxTransactions){
        for (auto const& value: User::GetUsers()){
            auto random_integer = uni(rng);
            value->waitingTime = random_integer;
        }
    //HERE TRIGGER THE COUNTDOWN EVENT
    }
    return true;
}

unsigned long Blockchain::GetChainLength(){
    return chain.size();
}

// Subchain starting at position start, ending at last element of chain.
std::vector<Block> Blockchain::GetSubChain(unsigned long start){
    std::vector<Block>::const_iterator first = chain.begin() + start;
    std::vector<Block>::const_iterator last = chain.begin() + GetChainLength();
    std::vector<Block> sub_chain(first, last);
        return sub_chain;
}

//bool Blockchain::RunBlockProtocol(User blockMaster){
//    currBlock.blockMaster = blockMaster;
//    Blockchain::AddBlock(currBlock);
//    //initialize block with prev hash etc..
//    currBlock = Block();
//    return true;
//}


