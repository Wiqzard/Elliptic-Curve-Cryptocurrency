//
//  Block.cpp
//  Neww
//
//  Created by Sebastian Stapf on 15.09.22.
//

#include "Block.hpp"


Block::Block(){
    _tTime = time(nullptr);
}
Block::Block(uint32_t nIndexIn) : _nIndex(nIndexIn){
    _tTime = time(nullptr);
}

Block Block::CreateGenesisBlock(){
    return Block(0);
}

//bool Block::CheckSignature(Transaction transaction){
//    Uint256 r = transaction.signature[0];
//    Uint256 s = transaction.signature[1];
//    CurvePoint publicKey = transaction.sender.GetPublicKey();
//    Sha256Hash msgHash = transaction.sender.HashTransaction(transaction.receiver, transaction.amount);
//    bool check = Ecdsa::verify(publicKey, msgHash, r,s);
//    return check;
//}
bool Block::CheckSignature(Transaction transaction){
    Uint256 r = transaction.signature[0];
    Uint256 s = transaction.signature[1];
    Uint256 pubk = Uint256(transaction.senderAddress);
    CurvePoint publicKey = CurvePoint::privateExponentToPublicPoint(pubk);
    Sha256Hash msgHash = User::HashTransaction(transaction.senderAddress, transaction.receiverAddress, transaction.amount);
    bool check = Ecdsa::verify(publicKey, msgHash, r,s);
    return check;
}

bool Block::CheckSignatureStatic(Transaction transaction){
    Uint256 r = transaction.signature[0];
    Uint256 s = transaction.signature[1];
    Uint256 pubk = Uint256(transaction.senderAddress);
    CurvePoint publicKey = CurvePoint::privateExponentToPublicPoint(pubk);
    Sha256Hash msgHash = User::HashTransaction(transaction.senderAddress, transaction.receiverAddress, transaction.amount);
    bool check = Ecdsa::verify(publicKey, msgHash, r,s);
    return check;
}

void Block::AddTransaction(Transaction transaction){
    assert(transactions.size()<=10);
    assert(CheckSignature(transaction));
    transactions.push_back(transaction);
}

std::vector<Transaction> Block::GetTransactions(){
    return transactions;
}

std::string Block::TransactionsToString(bool byName){
    std::string temp = "";
    if (byName){
        for (auto const& value: GetTransactions()){
            temp += value.senderAddress + " sends " + value.receiverAddress  + " " + std::to_string(value.amount)+ "HoboCoin\n";
                //.sender.GetName().receiver.GetName()
        }
    }else{
        for (auto const& value: GetTransactions()){
            temp += value.senderAddress + " sends " + value.receiverAddress + " " + std::to_string(value.amount)+ "HoboCoin\n";
            //.sender.GetName().receiver.GetAddress()
        }
    }
    return temp;
}

Sha256Hash Block::HashBlock(){
    std::string temp =  prevHash.Sha256HashToString() + currHash.Sha256HashToString() + Block::TransactionsToString(false);
    std::vector<std::uint8_t> bytes(temp.begin(), temp.end());
    Sha256Hash hash = Sha256::getHash(bytes.data(), bytes.size());
    return hash;
}

std::vector<char> Block::GetBlockMetaData(){
    //nindex, tTime, prevHash, curr hash, transactions into char delimited with |
    std::vector <char> meta_data;
    meta_data.push_back('|');
    char strInd[16];
    sprintf(strInd, "%u", _nIndex);
    std::string strInt = std::to_string(_nIndex);
    for (char const c: strInt){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    char strTime[16];
    sprintf(strTime, "%ld", _tTime);
    for (char const &c: strTime){
        meta_data.push_back(c);
    }
    meta_data.push_back('|');
    std::vector<char> prevHashVec = prevHash.Sha256HashToVec();
    meta_data.insert(meta_data.end(), prevHashVec.begin(), prevHashVec.end());
    meta_data.push_back('|');
    std::vector<char> currHashVec = currHash.Sha256HashToVec();
    meta_data.insert(meta_data.end(), currHashVec.begin(), currHashVec.end());
    meta_data.push_back('|');
    
    for (int i=0; i<nMaxTransactions; i++){
        std::vector<char> transaction_meta_data = transactions[i].GetTransactionMetadata();
        meta_data.insert(meta_data.end(), transaction_meta_data.begin(), transaction_meta_data.end());
    }
    return  meta_data;
}

//reinterpret_cast<char*>(meta_data.data())
//Block::Block(uint32_t nIndexIn, const std::string &sDataIn) : _nIndex(nIndexIn), _sData(sDataIn)
//{
//    _nNonce = 0;
//    _tTime = time(nullptr);
//
//    sHash = _CalculateHash();
//}
//
//void Block::MineBlock(uint32_t nDifficulty)
//{
//    char cstr[nDifficulty + 1];
//    for (uint32_t i = 0; i < nDifficulty; ++i)
//    {
//        cstr[i] = '0';
//    }
//    cstr[nDifficulty] = '\0';
//
//    std::string str(cstr);
//
//    do
//    {
//        _nNonce++;
//        sHash = _CalculateHash();
//    } while (sHash.substr(0, nDifficulty) != str);
//    std::cout << "Block mined: " << sHash << std::endl;
//}
//
//inline std::string Block::_CalculateHash() const
//{
//    std::stringstream ss;
//    ss << _nIndex << sPrevHash << _tTime << _sData << _nNonce;
//    return Sha256(ss.str());
//}
