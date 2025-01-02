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

int inSPM(AddrWD addr,int kII){
    for(int i=0;i<SPM_BLOCK_NUM;i++){
      // hit
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
  int blockId = inSPM(addr,kII);
  if(blockId>=0){
    MyStatics.numHit++;
    MySPM.blocks[blockId].lastReference = kII;
    if ((MySPM.blocks[blockId].isPrefetch)&& MySPM.blocks[blockId].hit_after_prefetch==0)//预取后首次hit
    {
                MyStatics.hit_after_prefetch++;//  统计总的预取后hit的block数
                MySPM.blocks[blockId].hit_after_prefetch = 1;
                uint32_t base = MySPM.blocks[blockId].prefetchBase;
                  //  反向统计hit信息
                  if(prefetch_Hit.find(base)!=prefetch_Hit.end()) {
                      prefetch_Hit[base]++;
                  }
                  else {
                      prefetch_Hit[base] = 1;
                  }


                char name[20] = "BaseAddr_struct";
                LOG_FILE(LOG_INFO,name,"prefetch hit tag=%d,index=%d,start:%d-end:%d,hit addr=%d,%s\n",
                MySPM.blocks[blockId].tag,MySPM.blocks[blockId].index,MySPM.blocks[blockId].startAddr,MySPM.blocks[blockId].endAddr,addr,MySPM.blocks[blockId].isPrefetch?"Prefetch":"non-Prefetch");
    }
          MySPM.blocks[blockId].lastReference = kII;

        //  空间预取  顺序访存
          if((addr-MySPM.blocks[blockId].last_address==4) && spatial_enable) {
                MySPM.blocks[blockId].spatial_cnt++;
              if((MySPM.blocks[blockId].spatial_cnt>=(BLOCK_SIZE/8-1))&&(MySPM.blocks[blockId].endAddr-addr==4)) {
                  spatial_prefetch(addr+8,kII,blockId);
              }
          }
          MySPM.blocks[blockId].last_address = addr;

  }else if(blockId==-1){
    MyStatics.numMiss++;


    // 间接访存的地址要修改
      //citeseer 536870912-536977376 get 
      //cora   536870912-536957568    get
      // enzymes 320000-400000 536870912-536950912 get
      // pumbed 536870912-537186384  354592-670064 get
      // ogbn   536870912-539580400  4664972-7374460  get
      // rgb 0-2097152   536870912-537919488 
      // src2dest 536870912-537395200
      // pcm 0-524288 536870912-537395200   
      // if( (addr>320000&&addr <400000)  || (addr>536870912&&addr <536950912)) //enzymes
    // if( (addr>354592&&addr <670064)  || (addr>536870912&&addr <537186384))  //pubmed
    // if( (addr>4664972&&addr <7374460)  || (addr>536870912&&addr <539580400))   //ogbn
      // if( (addr>0&&addr <2097152)  || (addr>536870912&&addr <537919488))  //rgb
    // if( (addr>536870912&&addr <537395200)) // src2dest
    // if( (addr>536870912&&addr <537395200)||(addr>0&&addr <524288)) //pcm
    // if(addr>536870912&&addr<536977376) //citeseer
    if(addr>536870912&&addr<536957568)// cora
    {
      MyStatics.IMP_miss++;    
    }

    replaceBlock(addr,kII,false,-1);
  }
  float missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);

  if(missrate>0.1 && kII>20){
    IPDEnable = true;//  启动间接访存检测功能
    // printf("current kII=%d,hitnum=%d,missnum=%d,missrate=%f,need prefetch\n",kII,MyStatics.numHit,MyStatics.numMiss,missrate);
  }
  else if(missrate<0.001){
    IPDEnable = false;
  }
}

//  miss到达阈值  当检测到索引时 开始预取  首先要判定地址不在SPM中
void prefetch(AddrWD addr, int value,int kII){
    for(auto&pair : IPDentrys){

      //  用于根据预取块的accuracy来裁剪预取流量
        if((kII-pair.second.kII>10*InitialInterval)&&(prefetch_Hit.find(pair.first)!=prefetch_Hit.end())) {
            pair.second.prefetch_valid = false;
            pair.second.hit_cnt = 0;
            // pair.second.valid =false;
        }
        if((addr-pair.second.last_index_address<16)&&pair.second.prefetch_valid){
            //  与当前间接访存模式的最新索引地址差较小----和IPDentry绑定

            AddrWD prefetchAddr = value*4+pair.first;
            char name[10] = "prefetch";
            if (inSPM(prefetchAddr,kII)<0)
            {
                MyStatics.num_prefetch++;
                replaceBlock(prefetchAddr,kII,true,pair.first);
                LOG_FILE(LOG_INFO,name,"kII=%d,idx=%d,IPDlastidx=%d,IPD.base=%d,prefetch_addr=%d,block tag=%d,index=%d,%8d-%8d\n",
                kII,addr,pair.second.last_index_address,pair.first,prefetchAddr,getTag(prefetchAddr),getIndex(prefetchAddr),getStartAddr(prefetchAddr),getEndAddr(prefetchAddr));
            }
            pair.second.last_index_address = addr;  // 更新最新的索引

        }
    }
}

// 用于空间预取
bool spatial_prefetch(AddrWD addr,int kII,int block_id) {
    if(inSPM(addr,kII)==-1) {
        Block prev = MySPM.blocks[block_id];
        block_id = getReplacementBlockId();
        char name[10] = "spatial";
        //  记录被替换的块 是否是预取的块
        int prev_start = prev.startAddr;
        int prev_end   = prev.endAddr;
        int last_addr = prev.last_address;
        int spa_cnt =prev.spatial_cnt;

        MySPM.blocks[block_id].tag = getTag(addr);
        MySPM.blocks[block_id].index = getIndex(addr);
        MySPM.blocks[block_id].valid = true;
        MySPM.blocks[block_id].size = BLOCK_SIZE;
        MySPM.blocks[block_id].startAddr = getStartAddr(addr);
        MySPM.blocks[block_id].endAddr = getEndAddr(addr);
        MySPM.blocks[block_id].lastReference = kII;
        MySPM.blocks[block_id].isPrefetch = false;
        MySPM.blocks[block_id].hit_after_prefetch = 0;
        MySPM.blocks[block_id].last_address = 0;
        MySPM.blocks[block_id].spatial_cnt = 0;
        MySPM.blocks[block_id].prefetchBase = -1;
        LOG_FILE(LOG_INFO,name,"pre_kII=%d,cur_kII:%d addr:%10d,replace block %2d,prev_addr:%8d-%8d,addr:%8d-%8d,last_addr=%d,spa_cnt=%d,cur_cnt=%d\n",
         prev.lastReference,kII,addr,block_id,prev_start,prev_end,getStartAddr(addr),getEndAddr(addr),last_addr,spa_cnt,MySPM.blocks[block_id].spatial_cnt);

        return true;
    }
    return false;
}


// 弃用
// void RWBuffersAnalyze(){
//   for(int i=0;i<bufferIdx;i++){
//     if(RWBuffers[i].IsLoad)
//       MyStatics.numRead++;
//     else
//       MyStatics.numWrite++;
//     if(inSPM(RWBuffers[i].address,0)>=0){
//         MyStatics.numHit++;
//         MySPM.blocks[inSPM(RWBuffers[i].address,0)].lastReference = RWBuffers[i].cur_kII;
//     }else{
//       MyStatics.numMiss++;
//       // 将数据置换到SPM中
//       replaceBlock(RWBuffers[i].address,RWBuffers[i].cur_kII,false);
//     }
//   }
// }
// 输出结果
void output(){
  float missrate = 0.0;
  uint32_t delay = MyStatics.numHit+MyStatics.numMiss*15;
   missrate = (float)MyStatics.numMiss/(MyStatics.numHit+MyStatics.numMiss);
    float accuracy = (float)MyStatics.hit_after_prefetch/MyStatics.num_prefetch;
  char name[20] = "../output/output";
  LOG_FILE(LOG_INFO,name,"------------\nblock size=%d\nSPM_SIZE=%d\nspm_block_num=%d\n%s %s\nhit_num=%d\nmiss_num=%d\nmiss_rate=%f\ndelay=%dcycles\nhit_after_prefetch=%d\ntotal_prefetch=%d\naccuracy=%f\nIMP_miss=%d\nnon-IMP_miss=%d\n------------",
        BLOCK_SIZE,SPM_SIZE,SPM_BLOCK_NUM,spatial_enable?"spatial":"no-spatial",prefetch_allow?"IMP":"non-IMP",MyStatics.numHit,MyStatics.numMiss,missrate,delay,MyStatics.hit_after_prefetch,MyStatics.num_prefetch,accuracy,MyStatics.IMP_miss,MyStatics.numMiss-MyStatics.IMP_miss);
  printf("------------\nblock size=%d\nSPM_SIZE=%d\nspm_block_num=%d\n%s %s\nhit_num=%d\nmiss_num=%d\nmiss_rate=%f\ndelay=%dcycles\nhit_after_prefetch=%d\ntotal_prefetch=%d\naccuracy=%f\nIMP_miss=%d\nnon-IMP_miss=%d\n------------",
        BLOCK_SIZE,SPM_SIZE,SPM_BLOCK_NUM,spatial_enable?"spatial":"no-spatial",prefetch_allow?"IMP":"non-IMP",MyStatics.numHit,MyStatics.numMiss,missrate,delay,MyStatics.hit_after_prefetch,MyStatics.num_prefetch,accuracy,MyStatics.IMP_miss,MyStatics.numMiss-MyStatics.IMP_miss);

}
bool replaceBlock(AddrWD addr,int cur_kII,bool isPrefetch,AddrWD prefetchBase){
    uint32_t id = getReplacementBlockId();
    Block prev = MySPM.blocks[id];
//  记录被替换的块 是否是预取的块
    // LOG_Analyze(LOG_INFO,"previous_kII=%d,cur_kII:%d address:%10d,Block %2d is replaced,tag=%6d, index=%4d,addr:%8d-%8d,%s,%s\n",
    //  prev.lastReference,cur_kII,addr,id,prev.tag,prev.index,prev.startAddr,prev.endAddr,prev.isPrefetch?"Prefetch":"",(prev.isPrefetch&&prev.hit_after_prefetch)?"pre_hit":"pre_non_hit");

    Block  b = {};
    b.tag = getTag(addr);
    b.index = getIndex(addr);
    b.valid = true;
    b.size = BLOCK_SIZE;
    b.startAddr = getStartAddr(addr);
    b.endAddr = getEndAddr(addr);
    b.lastReference = cur_kII;
    b.isPrefetch = isPrefetch;
    b.hit_after_prefetch = 0;
    b.prefetchBase = isPrefetch?prefetchBase:-1;
    b.last_address = 0;
    b.spatial_cnt = 0;
    MySPM.blocks[id] = b;
    LOG_Analyze(LOG_INFO,"pre_kII=%d,cur_kII:%d addr:%10d,replace block %2d,prev_addr:%8d-%8d,addr:%8d-%8d,last_addr=%d,prev_spa_cnt=%d,spa_cnt=%d\n",
         prev.lastReference,cur_kII,addr,id,prev.startAddr,prev.endAddr,getStartAddr(addr),getEndAddr(addr),prev.last_address,prev.spatial_cnt,MySPM.blocks[id].spatial_cnt);

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


