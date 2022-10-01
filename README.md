# Elliptic-Curve-Cryptocurrency

## Description
C++ implementation of a Cryptocurrency, utilizing elliptic curve cryptography together with blockchain technology inspired by Bitcoin. Instead of "Proof of Work" for securing the blockchain, "Proof of Elapsed Time" built in. Hence when a node wants to submit a block that it created from a pool of transactions that it gathered, it gets assigned a random waiting time, after which it can submit the block to the blockchain. \
\
Elliptic curve cryptography (256-bit ECDSA) used for signing transactions and validating transactions. 
  * Uint256 used for 256-bit integers.
  * FieldInt used for prime fields.
  * Sha256 uses SHA-2 algorithm to hash messages. Resulting in:
  * Sha256Hash
  * CurvePoint implements elliptic curve and operations on it.
  * Ecdsa contains the final signing and validation methods.

Blockchain implementation inspired by Bitcoin. 
  * User implemented to assign private/public key pair, creating transactions and assigning signatures.
  * Transaction simply stores sender, receiver, amount and signatures of a transasction.
  * Block contains block index, time, previous hash, current hash and transactions. Validates transactions before adding to block.
  * Blockchain contains the actual chain of blocks. Checks block prev and current hash before adding to blockchain.
 
Simple Peer2Peer network implementation, without use of additional libraries. 
  * Connection contains all the necessary commands, such as getblocks, gethosts, gettransactions, etc. together with methods for sending the requested data.
  * Node has a User associated with it for the rewards, contains the actual blockchain, together with a set of knownHosts. Finally sets for incoming transactions from which the node creates new blocks are built in.
 
Proof of elapsed time, i.e. if a node constructed a block, it gets assigned a waiting time after which it can submit the block to the blockchain. 

## ToDo's
1. Implement a more efficient byte conversion for transmitting data. 
2. Change method/variable names to consistent scheme. 
3. Create more exhaustive code comments.
4. Improve visibility of attributes.
6. Implement multi-threading for handling multiple clients and receiving and processing data simultaneously.
7. Improve constant variable settings.
8. Implement a local database for the blockchain.
9. Create a alpha node that assigns the waiting times.
10. Implement a Merkle-tree for block-transaction security.


## Sources:
* A. M. Antonopoulos: Mastering Bitcoin
* Satoshi Nakamoto: https://bitcoin.org/bitcoin.pdf
* Bitcoin Source Code Project: https://github.com/bitcoin
* Extracted Bitcoin Cryptography: https://github.com/nayuki/Bitcoin-Cryptography-Library
* Elliptic Curve Digital Signature Algorithm: https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm
* Proof of Elapsed Time Information: https://blockchainwelt.de/proof-of-elapsed-time/
* Bitcoin programmed in Python: https://github.com/samrushing/caesure
* Socket Programming in C: https://www.geeksforgeeks.org/socket-programming-cc/

