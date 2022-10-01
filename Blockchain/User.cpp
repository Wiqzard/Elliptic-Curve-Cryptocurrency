//
//
//  Blockchain
//
//  Created by Sebastian Stapf on 13.09.22.
//

#include "User.hpp"

//User::User() : userID(currID++), users[userID](this) {}
User::User(){
    users.push_back(this);
}
User::User(const std::string nName){
    this->name = nName;
    AssignPrivatePublicKey();

    //this->address = Utils::genRandom(User::ADDRESS_LENGTH);
    this->AssignPrivatePublicKey();
    userID = currID++;
    //users[userID] = this;
//    users.push_back(this);
}

User::User(const User &user){
    this->name = user.name;
    this->address = user.address;
    this->AssignPrivatePublicKey();
    userID = currID++;
    //users[userID] = this;
}

User& User::operator=(const User &user){
    this->name = user.name;
    this->address = user.address;
    this->privateKey = user.privateKey;
    this->publicKey = user.publicKey;
    userID = currID++;
    users[userID] = this;
    return *this;
}



void User::AssignPrivatePublicKey(){
    if (User::RANDOM_PRIVATE_KEY){
        assert (Uint256::NUM_WORDS == 8);
        std::string randString = Utils::genRandom(64);
        this->address = randString;
        Uint256 nPrivateKey = Uint256(randString);
        CurvePoint nPublicKey = CurvePoint::privateExponentToPublicPoint(nPrivateKey);
        this->privateKey = nPrivateKey;
        this->publicKey = nPublicKey;
    }
}

void User::CreateSignature(const std::string message){
    Uint256 r, s;
    std::string randString = Utils::genRandom(64);
    Uint256 nonce = Uint256(randString);
    Sha256Hash msgHash = Sha256Hash(message);
    bool success = Ecdsa::sign(this->privateKey, msgHash, nonce, r,s);
    if (not success){
        CreateSignature(message);
    }
}

std::vector <Uint256> User::CreateSignature(Sha256Hash msgHash){
    Uint256 r, s;
    std::vector <Uint256> signature;
    //maybe put rand inside
    std::string randString = Utils::genRandom(64);
    //Uint256 nonce = Uint256(randString);
    bool success = Ecdsa::signWithHmacNonce(this->privateKey, msgHash, r, s);
    //bool success = Ecdsa::sign(this->privateKey, msgHash, nonce, r,s);
    if (not success){
        CreateSignature(msgHash);
    }
    signature.push_back(r);
    signature.push_back(s);
    return signature;
}

//Sha256Hash User::HashTransaction(User receiver, float amount){
//    std::string strAmount = std::to_string(amount);
//    strAmount.erase(std::remove_if(strAmount.begin(),
//                                   strAmount.end(),
//                                   [] (char c){return c=='.';}),
//                    strAmount.end());
//    std::string temp = this->User::GetAddress() + receiver.address + strAmount;
//    int pad = 64 - int(temp.length());
//    for (int i = 0; i < pad; i++){
//        temp = temp + "0";
//    }
//    assert(temp.length()==64);
//    return Sha256Hash(temp);
//}
Sha256Hash User::HashTransaction(const std::string senderAddress,const std::string receiverAddress, float amount){
    std::string strAmount = std::to_string(amount);
    strAmount.erase(std::remove_if(strAmount.begin(),
                                   strAmount.end(),
                                   [] (char c){return c=='.';}),
                    strAmount.end());
    std::string temp = senderAddress+ receiverAddress + strAmount;
    int pad = 64 - int(temp.length());
    for (int i = 0; i < pad; i++){
        temp = temp + "0";
    }
    Utils::Bytes bytes = Utils::asciiBytes(temp);
    Sha256Hash thHash = Sha256::getHash(bytes.data(), bytes.size());
    return thHash;
}



//Create transaction from Users
//Transaction User::CreateTransaction(User receiver, float amount){
//    Uint256 r, s;
//    //Create Message from sender, receiver, amount such that hasable
//    Sha256Hash hashedTransaction = User::HashTransaction(receiver, amount);
//    std::vector<Uint256> signature = User::CreateSignature(hashedTransaction);
//    r = signature[0];
//    s = signature[1];
//    Transaction transaction = Transaction(*this, receiver, amount);
//    transaction.SignTransaction(r,s);
//    std::cout << "Transaction created successfully!" << std::endl;
//    return transaction;
//}

Transaction User::CreateTransaction(const std::string receiverAddress, float amount){
    Uint256 r, s;
    Sha256Hash hashedTransaction = User::HashTransaction(this->GetAddress(),receiverAddress, amount);
    std::vector<Uint256> signature = User::CreateSignature(hashedTransaction);
    r = signature[0];
    s = signature[1];
    Transaction transaction = Transaction(this->GetAddress(), receiverAddress, amount);
    transaction.SignTransaction(r,s);
    //std::cout << "Transaction created successfully!" << std::endl;
    return transaction;
}

const std::string User::GetAddress(){return address;}
const std::string User::GetName(){return name;}
const CurvePoint User::GetPublicKey(){return publicKey;}
const bool User::IsNameOfUser(std::string nName){return User::GetName()==nName;}

std::vector<User*>& User::GetUsers(){return users;}

//Create transaction from addresses
//Transaction User::CreateTransaction(std::string receiverAddress, float amount){
//    Uint256 r, s;
//    //Create Message from sender, receiver, amount such that hasable
//    Sha256Hash hashedTransaction = User::HashTransaction(receiver, amount);
//    User::CreateSignature(hashedTransaction);
//    Transaction transaction = Transaction(this->address, receiver.address, amount);
//    transaction.SignTransaction(r,s);
//}



