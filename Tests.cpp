//
//  Tests.cpp
//  Neww
//
//  Created by Sebastian Stapf on 01.10.22.
//

#include "Tests.hpp"

char ip[10] = "127.0.0.1";

/* Creates users
 * Creates transactions
 * Signs transactions
 * Creates blocks
 * Adds transactions to blocks + checks signatures + checks balances
 * Creates Blockchain and adds blocks
 * Creates command blocks together with block metadata
 * Reconstructs blocks from blocks command
 * Checks if reconstructed blocks equal blocks
 */
bool test_unpack_blocks(){
    User sebastian = User("Sebastian");
    User melina = User("Melina");
    Transaction transaction1 = sebastian.CreateTransaction(melina.GetAddress(), 1.2321);
    Transaction transaction2 = melina.CreateTransaction(sebastian.GetAddress(), 0.5);
    Transaction transaction3 = sebastian.CreateTransaction(melina.GetAddress(), 2.4);
    Transaction transaction4 = melina.CreateTransaction(sebastian.GetAddress(), 1.22);
    Block block1 = Block(2);
    Block block2 = Block(3);
    block1.AddTransaction(transaction1);
    block1.AddTransaction(transaction2);
    block2.AddTransaction(transaction3);
    block2.AddTransaction(transaction4);
    Blockchain blockchain = Blockchain();
    blockchain.SimpleAddBlock(block1);
    blockchain.SimpleAddBlock(block2);
    
    std::string command = "getblocks|1";
    std::vector<char> cmd(command.begin(), command.end());
    std::vector<char> blocks_meta_data = Connection::cmd_getblocks_test(blockchain, cmd);
    std::vector<Block> blocks_reconstructed = Connection::unpack_blocks(blocks_meta_data);
    
    Block reconstr1 = blocks_reconstructed[0];
    Block reconstr2 = blocks_reconstructed[1];
    assert(block1.GetBlockIndex() == reconstr1.GetBlockIndex());
    assert(block1.GetBlockTime() == reconstr1.GetBlockTime());
    assert(block1.prevHash == reconstr1.prevHash);
    assert(block1.currHash == reconstr1.currHash);
    assert(block1.TransactionsToString(false) == reconstr1.TransactionsToString(false));
    assert(block2.GetBlockIndex() == reconstr2.GetBlockIndex());
    assert(block2.GetBlockTime() == reconstr2.GetBlockTime());
    assert(block2.prevHash == reconstr2.prevHash);
    assert(block2.currHash == reconstr2.currHash);
    assert(block2.TransactionsToString(false) == reconstr2.TransactionsToString(false));
    std::cout << "unpack_blocks works as expected." << std::endl;
    return true;
}

/*  Creates transactions and inserts them into pending_transactions.
 *  Creates block from transactions in pending transactions.
 *  Submits block if intersection = true:
 *          -> insert duplicate transaction into new_transactions
 *          -> should now trigger assert(added)
 */
bool test_node_block_creation(bool intersection){
    User sebastian = User("Sebastian");
    User melina = User("Melina");
    Transaction transaction1 = sebastian.CreateTransaction(melina.GetAddress(), 1.2321);
    Transaction transaction2 = melina.CreateTransaction(sebastian.GetAddress(), 0.5);
    Node node = Node(ip);

    node.pending_transactions.insert(transaction1);
    node.pending_transactions.insert(transaction2);
    if (intersection){
        node.new_transactions.insert(transaction1);
    }
    Block created_block = node.CreateBlock();
    uint32_t blockchain_length = node.blockchain.GetChainLength();
    assert(blockchain_length == 1);
    bool added = node.SubmitBlock(created_block);
    assert(added);
    assert(node.blockchain.GetChainLength() == blockchain_length+1);
    assert(node.blockchain.GetLastBlock().prevHash == node.blockchain.GetFirstBlock().currHash);
    assert(node.blockchain.GetLastBlock().GetBlockIndex() == node.blockchain.GetChainLength());
    assert(!node.pending_transactions.empty());
    printf("CreateBlock/SubmitBlock works as expected.\n");
    return true;
}

/* Inserts 2 valid, 1 invalid address into knownHosts
 * packs hosts
 */
bool test_node_hosts_command(){
    Node node = Node(ip);
    std::string host1_ip = "192.158.1.38";
    int host_port1 = 0;
    std::string host2_ip = "255.255.255.255";
    int host_port2 = 65000;
    std::string host3_ip = "666.255.255.255";
    int host_port3 = 69000;
    node.AddHost(host1_ip, host_port1);
    node.AddHost(host2_ip, host_port2);
    node.AddHost(host3_ip, host_port3);

    std::string command = "gethosts|";
    std::vector<char> cmd(command.begin(), command.end());
    // Second entry can be ignored here.
    std::vector<char> host_meta_data = Connection::cmd_gethosts_test(node, cmd);
    //cmd_hosts:
    std::vector<std::pair<std::string, int>> host_pairs = Connection::cmd_hosts_test(host_meta_data); // cmd wumpe
    for (auto const &host_pair : host_pairs){
        node.AddHost(host_pair.first, host_pair.second);
        const bool is_in = node.knownHosts.find(host_pair) != node.knownHosts.end();
        assert(is_in);
        assert(node.knownHosts.size() == 3);
    }
    printf("hosts_command works as expected.\n");
    return true;
}

bool test_node_transactions_command(){
    Node node = Node(ip);
    User sebastian = User("Sebastian");
    User melina = User("Melina");
    Transaction transaction1 = sebastian.CreateTransaction(melina.GetAddress(), 1.2321);
    Transaction transaction2 = melina.CreateTransaction(sebastian.GetAddress(), 0.5);
    Block block1 = Block(2);
    block1.AddTransaction(transaction1);
    block1.AddTransaction(transaction2);
    node.blockchain.AddBlock(block1);
    node.pending_transactions.insert(transaction1);
    node.pending_transactions.insert(transaction2);
    // Balance sebastian: 0.5, Balance melina: 1.2321
    Transaction transaction_fail = sebastian.CreateTransaction(melina.GetAddress(), 1);
    node.AddTransaction(transaction1);
    
    std::vector<char> transactions_meta_data = Connection::cmd_gettransaction_test(node);
    std::vector<Transaction> transactions = Connection::cmd_transactions_test(transactions_meta_data);
    assert(transaction1.receiverAddress == transactions[0].receiverAddress);
    assert(transaction2.receiverAddress == transactions[1].receiverAddress);
    assert(transaction1.senderAddress == transactions[0].senderAddress);
    assert(transaction2.senderAddress == transactions[1].senderAddress);
    assert(transaction1.amount == transactions[0].amount);
    assert(transaction2.amount == transactions[1].amount);
    assert(transaction1.signature[0] == transactions[0].signature[0]);
    assert(transaction2.signature[1] == transactions[1].signature[1]);
    printf("transactions_command works as expected.\n");
    return true;
}

bool test_ecdsa(){
    struct SignCase {
        bool matches;
        const char *privateKey;
        const char *msgHash;  // Byte-reversed
        const char *nonce;    // Can be null
        const char *expectedR;
        const char *expectedS;
    };
    const std::vector<SignCase> cases{
        {true, "001201008010802052E810181019014029A50400110020004840808481002400", "F6F77BF7CF37FDB3FE9F7DFEF7FEF5FFD57BEF5FFEEFFBFFF3FFBDDB7FB7CFFD", "6FF1FDEBFEFB76FEBDEE737FBFFBFDE677EBFDB736FE773CFFFCFBFFFFF95AFC", "F4A36C209D0CE07E465000B78B2CB2926C6CA3A81733E41B1D7689C9EF5CBECA", "6FFD7FEFC46BE80C425054544CD0319D31082344065FEB719F7AE8766615A510"},
        {true, "10000000000C80007C0090804204001005922008006050400866004000002100", "880800000C0224050000B1050804020012011001480026404084860008221021", "4813A423C001A0081400A11082000042401865000190102B1403008090020500", "A0356CA71C2963D61C52FE9481FF968CBD3601A53A4B01D86844C3EA693D80A3", "4C8A491953DB7630203B67E84C94D16DD336477BEC98C8B2F3524BCEAE5AC27E"},
        {true, "841809D88359140C061400800A11553002000000104442081000010401520819", "EBBFFEE0FFFBFFC5FEB62A9FBFFFFAE9F45F67DEF9FEEDFFFBFF767BFFFFCCEE", "80000504B95146424F22010009000200085ED80100840016010004581004C008", "32F29FABB1FF6D9FD2AC9C2656C1C9F8A8D08BD02156323C32A9A2231F9C5323", "66C7A5BC1CF71C7E3F940B5DCDBBB49E57F2863B523B7C45E67D2A0C63CA1780"},
        {true, "41FAFE9B8AED4955413045F361506CC58C335DA64450788676844E8267179624", "90A890040202000801E6D040405280A421480008008081041B41002E94202200", "DFFEFFFFEFFDF7DFFFFFFFFFFFFFFFFF7FEFFFBFFFFFFFFFFFFFFFFFBBFFFFFF", "3F702736BAD513FC7FA555A134EA9A06AC9CB45CFB528F976315F0F6CBDC8EEB", "31CD1941849F05B5B93CF3E4DA520904AAACCF4E6C9A0CA77D7E693B78C0953C"}
    };
    for (const SignCase &tc : cases) {
        Uint256 privateKey(tc.privateKey);
        const Sha256Hash msgHash(tc.msgHash);
        Uint256 expectedR(tc.expectedR);
        Uint256 expectedS(tc.expectedS);

        Uint256 r, s;
        bool ok;
        if (tc.nonce != nullptr) {
            Uint256 nonce(tc.nonce);
            ok = Ecdsa::sign(privateKey, msgHash, nonce, r, s);
        } else {
            ok = Ecdsa::signWithHmacNonce(privateKey, msgHash, r, s);
        }
        bool actualMatch = r == expectedR && s == expectedS;
        assert(ok && actualMatch == tc.matches);

        if (Uint256::ZERO < privateKey && privateKey < CurvePoint::ORDER) {
            CurvePoint publicKey = CurvePoint::privateExponentToPublicPoint(privateKey);
            assert(Ecdsa::verify(publicKey, msgHash, r, s));
        }
    }
    printf("ECDSA works as expected. \n");
    return true;
}

bool test_uint(){
    std::string stres = "09F4BE4173C99D20A7AC925A3ED6E95A3D2EFA730B4BF7956168F2DEA24BFC27";
    Uint256 test_int = Uint256(stres);
    std::vector<char> int_vec = test_int.Uint256ToVec();
    std::string int_string(int_vec.begin(), int_vec.end());
    //std::cout << int_string << std::endl;
    assert(int_string == stres);
    printf("Uint works as expected. \n");
    return true;
}
