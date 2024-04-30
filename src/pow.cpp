// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"

//BTF Network Difficulty ctl:BtfDiffEnable setup for 212700 forking and init a new Difficulty for all new miners..
static bool BtfDiffEnable(const CBlockIndex* pindexPrev){
 const int nHeight = pindexPrev->nHeight + 1;
    if (nHeight>=212701){
        return true;
    }else if(nHeight<=212700){
        return false;
    }else{
        return true;
    }
}


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();
    if (!BtfDiffEnable(pindexLast)){
        std::cout<<"BTF Network Difficulty ctl:BtfDiffEnable disabled for BTC chain forking."<<"\n";
        return pindexLast->nBits;
    }
    // Only change once per difficulty adjustment interval
    //if ((pindexLast->nHeight+1) % 2 != 0) //BTF Network debug remarked 2block -> adjustment debug
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {   
        std::cout<<"Bitcoin Finance (BTF) Network: New Block mined found - - - > Notify."<<"\n";
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            std::cout<<"BTF Network: Special difficulty rule for testnet - > Notify."<<"\n";
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block  
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                    std::cout<<"BTF Network:Return the last non-special-min-difficulty-rules-block ."<<"\n";
                return pindex->nBits;
            }
        }

        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    //BTF network's params.DifficultyAdjustmentInterval()=2016 blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    //int nHeightFirst = pindexLast->nHeight - (2-1);  //BTF Network debug remarked 2block -> adjustment
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);
    std::cout<<"BTF Network:Bitcoin Finance(BTF) network's params.DifficultyAdjustmentInterval Retargeting -> Retarget."<<"\n";
    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
            if (params.fPowNoRetargeting)
                return pindexLast->nBits;
            // Limit adjustment step
            int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
            //setting nActualTimespan-> 2016 = 168 * 12 = 2 * 1008
            //nActualTimespan*=1008; //BTF Network debug remarked 2*1008=2016 -> adjustment debug
            if (nActualTimespan < params.nPowTargetTimespan/4){
                nActualTimespan = params.nPowTargetTimespan/4;
            std::cout<<"BTF Network:Bitcoin Finance(BTF) network difficulty adjustment decrease[-]."<<"\n";
            }
            if (nActualTimespan > params.nPowTargetTimespan*4){
                nActualTimespan = params.nPowTargetTimespan*4;
            std::cout<<"BTF Network:Bitcoin Finance(BTF) network difficulty adjustment increase[+]."<<"\n";
            }
            // Retarget
            const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
            const arith_uint256 btfFlexPow = UintToArith256(uint256S("0000000000000000000000000000000000000000000000000000000000000003"));
            arith_uint256 bnNew;
            bnNew.SetCompact(pindexLast->nBits);
            if (bnNew<=btfFlexPow){
                //avoid zerx difficulty in Btf network forks
            const arith_uint256 btfInitedPow = UintToArith256(uint256S("0000000000000000000000000000000000000000000000000000000000000003"));
                bnNew *= nActualTimespan*btfInitedPow;
                bnNew /= params.nPowTargetTimespan;
                bnNew +=btfInitedPow;
                std::cout<<"BTF Network:Bitcoin Finance(BTF) network difficulty btfInitedPow inited -> adjustment [+3]."<<"\n";
            }else{
                //Start a new difficulty regarting
                bnNew *= nActualTimespan;               //
                bnNew /= params.nPowTargetTimespan;   //1209600
                std::cout<<"BTF Network:Bitcoin Finance(BTF) network -> new difficulty regarting setup."<<"\n";
            }
            if (bnNew > bnPowLimit){
                bnNew = bnPowLimit;
                std::cout<<"BTF Network:Bitcoin Finance(BTF) network -> new difficulty reached Btf.bnPowLimit -> setup (bnNew = bnPowLimit)."<<"\n";
            }
            return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
