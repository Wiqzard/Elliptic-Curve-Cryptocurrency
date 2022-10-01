# send_packet try to go through second function







#  TODO LIST

## 0.P2P NETWORK WITH PROOF BY TIME
## 0.1. IMPLEMENT MULTI-THREADING/ SELECT
## 1.REWRITE WHOLE CRYTPO STUFF WITH VECTORS INSTEAD OF CHAR POINTERS
## 2.TRY, CATCH IN NETWORKING ETC.
## 3.WORK WITH & IN FUNCTIONS
## 4.WORK OUT VISIBILITY STUFF
## 5.WORK OUT MEMORY MANAGEMENT
## 3.TIDY EVERYTHING UP WITH POINTERS/CONSTRUCTOR/DESCRUCTORS
## ADJUST TIME 2 MINE BY NUMBER OF NODES




In peer2peer every node is client/server simulatneously.



Server transmi


#Connection File

##BaseConnection(Class for Networking in general)

    -> connect(self): Connects to other address 
    -> send_packet(self, command, payload): -> writev, sends payload and command to connection
    -> send_version(self): sends address, time, blockheight, ...
    -> getdata(self, items): in principle send_packet(self, "getdata", packed(items))
    -> get_packet(self): in principle simple recv containing command, payload -> returns command, payload
    
    -> go(self): connect(), send_version()
                while(1): command, payload = get_packet()
                if command is None:
                    break
                self.do_command(command, payload)
            finally conn.close()
    -> check_command_name(self, command):  checks if command valid
    -> do_command(slef, cmd, data): check_command_name, returns the matching command method
COMMAND METHODS    (MORE IN NEXT FILE)
    -> cmd_version(self, data):  self.send_packet("verack", "")
    -> cmd_verrack(self, data): pass

#Server File
##BlockHoover (class responsible for downloading blockchain)
    queue
    qset
    requested
    ready
    
###Methods:
    -> go(self, c):
    -> add_block(self, b): 
        -> put b in self.ready
        ->removes from requested if contained
        -> checks own block database if b contained, if not add to db
    -> get_block(self, conn name):
        -> adds name to requested
        -> block = conn.get_block(name)
        -> self.add_block(block)
    -> block_to_db(self, name, b): checks rules of blocks, block_db.add(name, b)


##Connection(BaseConnection) (class responsible for final networking)
    if sock is not None: self.direction = "incominc" else self.direction = "outgoin"
    self.waiting = {}
    self.known = set()
    BaseConnection.__init__(self, my_addr, other_addr, sock)
    
###Methods:
    -> get_our_block_heights: returns block_db.last_block()
    -> getblocks(self): 
        -> hashes = block_db.set_for_getblocks()
        -> hases_append(block_db.ZERO_NAME) (endofsentence token)
        -> self.send_packet("getblocks", hashes(#formatted)
    -> go(self):
        -> connects via G.connection_map
        -> if self.direction == "outgoig" self.connect() with timeout
        -> self.send_version()
        -> while(1): command, payload = self.get_packet()
                -> if command None break
                -> self. do_command(command, payload)
        -> finally release connection
        
    -> wait_for(self, key): synchronization of threads, checks if waiting has key, returns with weight
    -> get_block(self, name): key= (OBJ_BLOCK, name), self.get_data(key), return self.wait_for(key)
    -> get_tx(self, name): 
        -> key = (OBJ_BLOCK, name)
        -> getdata([key])
        reutrn wait_for(key)
    -> do_command(self, cmd, data): similar to above
    -> def getheaders(self, hashes=None):
        -> prepare hashes
        -> chain = [hashes[0]]
        -> while(1): -> send_packet("getheaders", self.version, hashes)
            -> _, data = self.wait_for("headers")
            -> blocks = unpack_headers(data)
            -> for block in blocks: if block.prev_block == chain[-1]: chain.append(block.name)
            -> hashes[0] = chain[-1]
            -> return chain[1:]
        
###Command Methods:
    -> cmd_version(self,data): send_packet("verack", "")
    -> cmd_verrack(self,data): ?
    -> cmd_addr(self,data): 
    -> cmd_getblocks(self,data):
    -> cmd_getheaders(self,data):
    -> cmd_getdata(self, data):
    -> cmd_getaddr(self,data):
    -> cmd_tx(self,data):
    -> cmd_block(self,data):
    -> ...
    
## TransactionPool: (class handling incoming transactions)
    self.missing = {}
    self.pool = {}
    
### Methods
    -> __contains__(self, name): return name in self.pool
    -> add(self, tx):
        -> if tx.name not in self.pool:
            -> 

## General Functions (to implement resulting functionality)
    -> new_block_thread(): establishes new block threads
    -> new_random_address(): for i in range(100):
        -> (ip, port) = G.addr_cache.random()
        -> if (ip_port) not in G.connection_map return (ip_port) else return None
    -> get_my_addr(other): return ('127.0.0.1', 8333)
    -> new_connection_thread(): 
        -> addr1 = new_random_addr(), addr0 = get_my_addr(addr1)
        -> ... Connection(addr0, addr1)
    -> serve(addr):  addr0 = pars..(addr)
        -> s = socket..
        -> s.bind(addr0)
        -> s.listen(100)
        -> while(1): conn, addr1 = s.accept()
            -> G.in_conn_sem.acquire(1)
            Connection(addr0, addr1, sock=conn)
        
    -> connect(addr):  addr1 = parse_add_arg(addr)
        -> G.acquir...
        -> addr0 = get_my_addr(add1)
        -> Connection(addr0, addr1)
        
    -> main1(args, G):
        G.verbose = args.verbose
        G.addr_cache = AddressCache()
        G.block_db = block_db.BlockDB (read_only=False)
        G.hoover = BlockHoover()
        G.txn_pool = TransactionPool()
        G.recent_blocks = ledger.catch_up (G)
        G.connection_map = {}
        
        G.in_conn_sem = coro.semaphore (args.incoming)
        G.out_conn_sem = coro.semaphore (args.outgoing)
        
        if args.serve:
            for addr in args.serve:
                coro.spawn (serve, addr)
        if args.connect:
            for addr in args.connect:
                coro.spawn (connect, addr)
        coro.spawn (G.addr_cache.purge_thread)
        coro.spawn (new_block_thread)
        coro.spawn (new_connection_thread)
        coro.spawn (G.recent_blocks.save_ledger_thread)
    
    -> main(args, global_state)
        global G
        G = global_state
        G.args = args
            coro.spawn (main1, args, G)
        try:
            coro.event_loop()
