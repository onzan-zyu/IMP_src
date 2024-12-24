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
    // uint32_t tag = getTag(addr);
    // uint32_t index = getIndex(addr);
    for(int i=0;i<SPM_BLOCK_NUM;i++){
      // tag相同 index相同且valid
      if(MySPM.blocks[i].startAddr<=addr && MySPM.blocks[i].valid && MySPM.blocks[i].endAddr>addr){
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
        for (uint32_t i = 1; i < SPM_BLOCK_NUM; ++i) {
            if (MySPM.blocks[i].lastReference < min) {
                resultId = i;
                min = MySPM.blocks[i].lastReference;
            }
        }
        return resultId;;
}

// 正常情况下遇到miss进行取数据的方法
void AddressAnalyze(AddrWD addr,int kII,bool IsLoad){
  if(IsLoad) MyStatics.numRead++;
  else MyStatics.numWrite++;
  if(inSPM(addr)>=0){
    MyStatics.numHit++;
    MySPM.blocks[inSPM(addr)].lastReference = kII;
  }else if(inSPM(addr)==-1){
    MyStatics.numMiss++;
    replaceBlock(addr,kII);
  }
  float missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);
  
  if(missrate>0.1 && kII>20){
    IPDEnable = true;//  启动间接访存检测功能
    printf("current kII=%d,hitnum=%d,missnum=%d,missrate=%f,need prefetch\n",kII,MyStatics.numHit,MyStatics.numMiss,missrate);
  }
  else if(missrate<0.001){
    IPDEnable = false;
  }
}

//  miss到达阈值  当检测到索引时 开始预取  首先要判定地址不在SPM中
void prefetch(AddrWD addr, int value,int kII){
    for(auto&pair : IPDentrys){
        if((addr-pair.second.last_index_address<16)&&pair.second.prefetch_valid){
            //  与当前间接访存模式的最新索引地址差较小----和IPDentry绑定
            AddrWD prefetchAddr = value*4+pair.first;
            replaceBlock(addr,kII);
            char name[10] = "prefetch";
            LOG_FILE(LOG_INFO,name,"idx=%d,IPDlastidx=%d,IPD.base=%d,prefetch_addr=%d\n",
            addr,pair.second.last_index_address,pair.first,prefetchAddr,"prefetch");
            pair.second.last_index_address = addr;  // 更新最新的索引
        }
    }
}



// 弃用
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
// 输出结果
void output(){
  uint32_t delay = MyStatics.numHit+MyStatics.numMiss*15;
  float missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);
  char name[10] = "output";
  LOG_FILE(LOG_INFO,name,"------------\nblock size=%d\nSPM_SIZE=%d\nspm_block_num=%d\nhitnum=%d\nmissnum=%d\nmissrate=%f\ndelay=%dcycles\n------------",
        BLOCK_SIZE,SPM_SIZE,SPM_BLOCK_NUM,MyStatics.numHit,MyStatics.numMiss,missrate,delay);
  printf("------------\nblock size=%d\nSPM_SIZE=%d\nspm_block_num=%d\nhitnum=%d\nmissnum=%d\nmissrate=%f\ndelay=%dcycles\n------------",
        BLOCK_SIZE,SPM_SIZE,SPM_BLOCK_NUM,MyStatics.numHit,MyStatics.numMiss,missrate,delay);
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
    float missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);
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


