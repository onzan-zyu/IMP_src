//
// Created by onzan_zyu on 2024/12/21.
//

#ifndef SPM_H
#define SPM_H
#include<cstdio>
#include<cstdlib>
#include "../log.h"
#include "../IMP/DataType.h"
#include "../IMP/IPD.h"
// 地址的划分问题  32位地址 = tag + block index + block size
#define OFFSET_BIT 6    //64B = 16 * 4B
#define BLOCK_SIZE (1 << OFFSET_BIT)
#define BLOCK_INDEX_BIT 4
#define SPM_BLOCK_NUM  (1 << BLOCK_INDEX_BIT)
#define SPM_SIZE  (BLOCK_SIZE * SPM_BLOCK_NUM)    //spm划分为多少个block 64B*8=512B

inline bool flag = true;

struct Block {
    bool valid;
    bool modified;
    uint32_t tag;
    uint32_t index;
    uint32_t size;
    uint32_t lastReference;
    int startAddr;
    int endAddr;
    Block() {
        valid = false;
        modified = false;
        tag = 0;
        index = 0;
        size = 0;
        lastReference = 0;
        startAddr = 0;
        endAddr = 0;
    }
};
struct SPM {
    int size;
    int BlockNum;
    struct Block blocks[SPM_BLOCK_NUM];
    SPM() : size(SPM_SIZE),BlockNum(SPM_BLOCK_NUM),blocks{} {}
};
struct Statics {
    uint32_t numRead;
    uint32_t numWrite;
    uint32_t numHit;
    uint32_t numMiss;
    uint32_t numLatency;
    Statics() {
        numRead = 0;
        numWrite = 0;
        numHit = 0;
        numMiss = 0;
        numLatency = 0;
    }
};

inline struct SPM MySPM;
inline struct Statics MyStatics;

uint32_t getOffset(AddrWD addr);
uint32_t getIndex(AddrWD addr);
uint32_t getTag(AddrWD addr);
uint32_t getStartAddr(AddrWD addr);
uint32_t getEndAddr(AddrWD addr);
int inSPM(AddrWD addr);
uint32_t getReplacementBlockId();
void RWBuffersAnalyze();
void AddressAnalyze(AddrWD addr,int kII,bool IsLoad);

// 替换block
bool replaceBlock(AddrWD addr,int cur_kII);

void printInfo();
#endif //SPM_H
