// Copyright (c) Bitcoin Finance(BTF)Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//For more information visit:www.btf.finance
// Note: Bitcoin Finance(BTF)Core have diffrent protocol version to Bitcoin core.

#ifndef BITCOIN_VERSION_H
#define BITCOIN_VERSION_H

/**
 * network protocol versioning
 */

static const int PROTOCOL_VERSION = 70035; //70015 for node connection

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209; //209 for old nodes connection

//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 31800; //31800

//! disconnect from peers older than this proto version,for getting blocks
static const int MIN_PEER_PROTO_VERSION = GETHEADERS_VERSION;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31388; //31402 for node connection

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 70000; //60000 for ping pong connection to detect active nodes

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
static const int NO_BLOOM_VERSION = 70035; //70011,>=7035 will not accept bloom filter,if you need to add bloom filter ,Set NODE_BLOOM tag to enable bloom filter

//! "sendheaders" command and announcing blocks with headers starts with this version
static const int SENDHEADERS_VERSION = 70020; //70012,notify new block in all nodes

//! "feefilter" tells peers to filter invs to you by fee starts with this version
static const int FEEFILTER_VERSION = 70020; //70013 filter for inv message

//! short-id-based block download starts with this version
static const int SHORT_IDS_BLOCKS_VERSION = 70019; //70014 improve node's block download  effective & speed

//! not banning for invalid compact blocks starts with this version
static const int INVALID_CB_NO_BAN_VERSION = 70018; //70015 no ban for invalid block 

#endif // BITCOIN_VERSION_H
