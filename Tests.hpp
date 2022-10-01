//
//  Tests.hpp
//  Neww
//
//  Created by Sebastian Stapf on 01.10.22.
//

#ifndef Tests_hpp
#define Tests_hpp

#include <cstdio>
#include "Network/Connection.hpp"
#include "Network/Node.hpp"


bool test_unpack_blocks();
bool test_node_block_creation(bool intersection);
bool test_node_hosts_command();
bool test_node_transactions_command();
bool test_ecdsa();
bool test_uint();
//bool test_node_network_commands();

#endif /* Tests_hpp */
