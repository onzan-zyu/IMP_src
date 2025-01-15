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
#include<map>
// 地址的划分问题  32位地址 = tag + block index + block size
#define OFFSET_BIT 4    //
#define BLOCK_SIZE (1 << OFFSET_BIT)
#define BLOCK_INDEX_BIT 4  //SPM中block的数量
#define SPM_BLOCK_NUM  (1 << BLOCK_INDEX_BIT)
// #define SPM_BLOCK_NUM  32
#define SPM_SIZE  (BLOCK_SIZE * SPM_BLOCK_NUM)    //spm划分为多少个block 64B*8=512B

inline bool prefetch_allow = true;   //是否支持IMP预取
inline bool spatial_enable = false;  // 是否支持stride预取
inline bool reduce_traffic_Enable = true;//  流量剪裁
// 存储预取的块的hit信息  反向调节
inline std::map<uint32_t,int> prefetch_Hit;

inline bool flag = true;
inline int Prefetch_Block_Num = 0; 
inline int Prefetch_Block_Hit = 0;
inline bool prefetchEnable = false;  //当检测到间接访存模式后的 预取启动信号
struct Block {
    bool valid;
    bool modified;
    uint32_t tag;
    uint32_t index;
    uint32_t size;
    uint32_t lastReference;
    int startAddr;
    int endAddr;
    bool isPrefetch;
    uint32_t prefetchBase;
    int hit_after_prefetch;
        int last_address;
    int spatial_cnt;
    Block() {
        valid = false;
        modified = false;
        tag = 0;
        index = 0;
        size = 0;
        lastReference = 0;
        startAddr = 0;
        endAddr = 0;
        isPrefetch = false;
        hit_after_prefetch = 0;
        last_address = 0;
        spatial_cnt = 0;
        prefetchBase = -1;
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
    int hit_after_prefetch;
    int num_prefetch;
    int IMP_miss;
    int num_IMP;
    Statics() {
        numRead = 0;
        numWrite = 0;
        numHit = 0;
        numMiss = 0;
        numLatency = 0;
        hit_after_prefetch = 0;
        num_prefetch = 0;
        IMP_miss = 0;
        num_IMP = 0;
    }
};


inline struct SPM MySPM;
inline struct Statics MyStatics;



uint32_t getOffset(AddrWD addr);
uint32_t getIndex(AddrWD addr);
uint32_t getTag(AddrWD addr);
uint32_t getStartAddr(AddrWD addr);
uint32_t getEndAddr(AddrWD addr);
int inSPM(AddrWD addr,int kII);
uint32_t getReplacementBlockId();
// void RWBuffersAnalyze();
void AddressAnalyze(AddrWD addr,int kII,bool IsLoad);

// 替换block
bool replaceBlock(AddrWD addr,int cur_kII,bool isPrefetch,AddrWD prefetchBase);

void printInfo();
void printBlock();
void output();

// used for IMP and normal prefetch
void prefetch(AddrWD addr, int value,int kII);
// 不需要根据信息计算预取的地址 因此不需要value
bool spatial_prefetch(AddrWD addr,int kII,int block_id);
#endif //SPM_H
