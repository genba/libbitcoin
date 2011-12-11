#ifndef LIBBITCOIN_BLOCKCHAIN_BERKELEYDB_BLOCKCHAIN_H
#define LIBBITCOIN_BLOCKCHAIN_BERKELEYDB_BLOCKCHAIN_H

#include <bitcoin/blockchain/blockchain.hpp>

#ifdef DB_CXX_HEADER
    #include DB_CXX_HEADER
#else
    #include <db_cxx.h>
#endif

#include <bitcoin/util/threads.hpp>

#include "bdb_detail.hpp"

namespace libbitcoin {

class bdb_blockchain
 : public blockchain, public threaded_service,
    public std::enable_shared_from_this<bdb_blockchain>
{
public:
    bdb_blockchain(const std::string& prefix);
    ~bdb_blockchain();

    static bool setup(const std::string& prefix);

    void store(const message::block& block, store_block_handler handle_store);

    void fetch_block(size_t depth, fetch_handler_block handle_fetch);
    void fetch_block(const hash_digest& block_hash,
        fetch_handler_block handle_fetch);
    void fetch_block_locator(fetch_handler_block_locator handle_fetch);
    void fetch_balance(const short_hash& pubkey_hash,
        fetch_handler_balance handle_fetch);

private:
    bdb_blockchain();
    void initialize(const std::string& prefix);

    void do_store(const message::block& block,
        store_block_handler handle_store);

    void fetch_block_by_depth(size_t depth, 
        fetch_handler_block handle_fetch);
    void fetch_block_by_hash(const hash_digest& block_hash, 
        fetch_handler_block handle_fetch);
    void do_fetch_block_locator(fetch_handler_block_locator handle_fetch);

    bool save_block(size_t depth, const message::block serial_block);
    uint32_t save_transaction(const message::transaction& block_tx);

    bdb_guard<DbEnv> env_;
    bdb_guard<Db> db_blocks_;
    bdb_guard<Db> db_blocks_hash_;
    bdb_guard<Db> db_txs_;
    bdb_guard<Db> db_txs_hash_;
};

} // libbitcoin

#endif
