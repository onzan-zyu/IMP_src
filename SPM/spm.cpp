//
// Created by onzan_zyu on 2024/12/21.
//
#include "spm.h"

uint32_t getOffset(AddrWD addr){
    uint32_t mask = (1 << OFFSET_BIT) - 1;
    return addr & mask;
}
uint32_t getIndex(AddrWD addr){
    uint32_t mask = (1 << BLOCK_INDEX_BIT) - 1;
    return (addr >> OFFSET_BIT) & mask;
}
uint32_t getTag(AddrWD addr){
    uint32_t mask = (1 << (32 - OFFSET_BIT - BLOCK_INDEX_BIT)) - 1;
    return (addr >> (OFFSET_BIT + BLOCK_INDEX_BIT)) & mask;
}
// 采用全相联映射 LRU的置换策略
int inSPM(AddrWD addr){
    uint32_t tag = getTag(addr);
    for(int i=0;i<SPM_BLOCK_NUM;i++){
      // tag相同且valid
      if(MySPM.blocks[i].tag == tag && MySPM.blocks[i].valid){
        return i;
      }
    }
    return -1;
}
// 找到置换出SPM的块
uint32_t getReplacementBlockId(){
      //find invalid block first
      for(uint32_t i = 0;i<SPM_BLOCK_NUM;i++){
        if(!MySPM.blocks[i].valid){
           return i;
        }
      }
      // LRU
        uint32_t resultId = 0;
        uint32_t min = MySPM.blocks[0].lastReference;
        for (uint32_t i = 0; i < SPM_BLOCK_NUM; ++i) {
            if (MySPM.blocks[i].lastReference < min) {
                resultId = i;
                min = MySPM.blocks[i].lastReference;
            }
        }
        return resultId;;
}
void AddressAnalyze(AddrWD addr,int kII,bool IsLoad){
  if(IsLoad) MyStatics.numRead++;
  else MyStatics.numWrite++;
  if(inSPM(addr)>=0){
    MyStatics.numHit+=1;
    MySPM.blocks[inSPM(addr)].lastReference = kII;
  }else{
    MyStatics.numMiss+=1;
      replaceBlock(addr,kII);
  }
  float missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);
  
  if(missrate>0.1&&flag){
    flag = false;
    printf("current kII=%d,missrate=%f,need prefetch\n");
  }
}
void RWBuffersAnalyze(){
  for(int i=0;i<bufferIdx;i++){
    if(RWBuffers[i].IsLoad)
      MyStatics.numRead++;
    else
      MyStatics.numWrite++;
    if(inSPM(RWBuffers[i].address)>=0){
        MyStatics.numHit++;
        MySPM.blocks[inSPM(RWBuffers[i].address)].lastReference = RWBuffers[i].cur_kII;
    }else{
      MyStatics.numMiss++;
      // 将数据置换到SPM中
      replaceBlock(RWBuffers[i].address,RWBuffers[i].cur_kII);
    }
  }
}


bool replaceBlock(AddrWD addr,int cur_kII){
    uint32_t id = getReplacementBlockId();
    Block & b = MySPM.blocks[id];
    b.tag = getTag(addr);
    b.index = getIndex(addr);
    b.valid = true;
    b.size = BLOCK_SIZE;
    b.startAddr = getStartAddr(addr);
    b.endAddr = getEndAddr(addr);
    b.lastReference = cur_kII;
    LOG_Analyze(LOG_INFO,"cur_kII:%d addrese:%x,Block %d is replaced,tag=%x,replace index=%x,addr:%x-%x\n",
         cur_kII,addr,id,b.tag,b.index,b.startAddr,b.endAddr);
    return true;
}
uint32_t getStartAddr(AddrWD addr){
    uint32_t mask = ~(uint32_t)((1 << OFFSET_BIT) - 1);
    return  addr & mask;
}
uint32_t getEndAddr(AddrWD addr){
  return getStartAddr(addr)+BLOCK_SIZE;
}
void printInfo(){
  float hitrate = (float)MyStatics.numHit/(MyStatics.numHit+MyStatics.numMiss);
    float missrate = (float)MyStatics.numHit/(MyStatics.numHit+MyStatics.numMiss);
    printf(" Static Info:\nnumRead:%d\nnumWrite:%d\nnumHit:%d\nnumMiss:%d\nhitRate=%f,missRate=%f\n",
           MyStatics.numRead,MyStatics.numWrite,MyStatics.numHit,MyStatics.numMiss,hitrate,missrate);
    LOG_Analyze(LOG_INFO," Static Info: numRead:%d,numWrite:%d,numHit:%d,numMiss:%d\n",
         MyStatics.numRead,MyStatics.numWrite,MyStatics.numHit,MyStatics.numMiss);
}
void printBlock(){
  for(int i=0;i<SPM_BLOCK_NUM;i++){
      LOG_Analyze(LOG_INFO," Block Info: valid:%s,tag:%x,index:%x,lastReference:%d,startAddr:%x,endAddr:%x\n",
           MySPM.blocks[i].valid?"true":"false",MySPM.blocks[i].tag,MySPM.blocks[i].index,MySPM.blocks[i].lastReference,MySPM.blocks[i].startAddr,MySPM.blocks[i].endAddr);
  }

}