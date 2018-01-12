// Copyright (c) 2009-2012 The Darkcoin developers
// Copyright (c) 2015-2017 The DTC developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "bignum.h"
#include "sync.h"
#include "net.h"
#include "key.h"

#include "util.h"
#include "script.h"
#include "base58.h"
#include "main.h"

using namespace std;
using namespace boost;

class CSporkMessage;
class CSporkManager;

#include "bignum.h"
#include "net.h"
#include "key.h"
#include "util.h"
#include "protocol.h"
#include "darksend.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/
#define SPORK_START                                           10001
#define SPORK_END                                             10005

#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT               10001
#define SPORK_2_MASTERNODE_WINNER_ENFORCEMENT                 10002
#define SPORK_3_DEVELOPER_PAYMENTS_ENFORCEMENT                10003
#define SPORK_4_PAYMENT_ENFORCEMENT_DOS_VALUE                 10004
#define SPORK_5_ENFORCE_NEW_PROTOCOL_V200                     10005

#define SPORK_1_MASTERNODE_PAYMENTS_ENFORCEMENT_DEFAULT       1527811200 //OFF
#define SPORK_2_MASTERNODE_WINNER_ENFORCEMENT_DEFAULT         4070908800 //OFF
#define SPORK_3_DEVELOPER_PAYMENTS_ENFORCEMENT_DEFAULT        1519862400 //OFF
#define SPORK_4_PAYMENT_ENFORCEMENT_DOS_VALUE_DEFAULT         0          //By default do not add to peer banscore
#define SPORK_5_ENFORCE_NEW_PROTOCOL_V200_DEFAULT             4070908800 //OFF

extern std::map<uint256, CSporkMessage> mapSporks;
extern CSporkManager sporkManager;

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
private:
    std::vector<unsigned char> vchSig;

public:
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    CSporkMessage(int nSporkID, int64_t nValue, int64_t nTimeSigned) :
        nSporkID(nSporkID),
        nValue(nValue),
        nTimeSigned(nTimeSigned)
        {}

    CSporkMessage() :
        nSporkID(0),
        nValue(0),
        nTimeSigned(0)
        {}


    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
	unsigned int nSerSize = 0;
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
	}

    uint256 GetHash(){
        uint256 n = Hash(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    bool Sign(std::string strSignKey);
    bool CheckSignature();
    void Relay();
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;
    std::map<int, CSporkMessage> mapSporksActive;

public:
    std::string strTestPubKey;
    std::string strMainPubKey;

    CSporkManager() {
        strMainPubKey = "043ea5053bb95e29a5a0fc57f1f6fd44677cba712e578991f32090931e9adc4c202829c8147d1a4874e1b381978b92d4f31928eab1e2fb1f20aa5c87725ae5d284";
        strTestPubKey = "043ea5053bb95e29a5a0fc57f1f6fd44677cba712e578991f32090931e9adc4c202829c8147d1a4874e1b381978b92d4f31928eab1e2fb1f20aa5c87725ae5d284";
    }

    void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
    void ExecuteSpork(int nSporkID, int nValue);
    bool UpdateSpork(int nSporkID, int64_t nValue);

    bool IsSporkActive(int nSporkID);
    int64_t GetSporkValue(int nSporkID);
    int GetSporkIDByName(std::string strName);
    std::string GetSporkNameByID(int id);

    bool SetPrivKey(std::string strPrivKey);
};

#endif
