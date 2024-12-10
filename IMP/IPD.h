//
// Created by onzan_zyu on 2024/12/2.
//

#ifndef IPD_H
#define IPD_H

#include "DataType.h"
#include "../data_structures.h"
#include "../log.h"
#include <stdarg.h> // 添加对可变参数的支持
// 存储间接访存模式的特征
struct IPT
{
    AddrWD BaseAddr;    // 基地址
    bool  valid;         // entry是否有效
    uint8_t shift;      // shift数值
    // uint16_t cycle;  // 所属的分析周期
    int  count;          //间接访存所属的cnt 用来判断3个InitialInterval识别成功
    DataType current_index;// 当前的索引是那个
    DataType hit_cnt;    // 模式命中的次数
    AddrWD next_prefetch_addr;  //根据模式生成的预取地址
    bool prefetch_valid;    //是否已经用于预取
};
struct RWInfo
{
    int cycle;     // 配置指令中的cycle
    bool  IsLoad;// load or store
    DataType value;
    AddrWD address;
    bool   IsIndex;
    int cur_count; // 当前配置指令执行的是多少个count
};

extern int InitialInterval;
//  存储捕捉到的load  store的信息  count  RWInfo
extern struct IPT IPDentrys[100];
extern int IPD_index;
extern std::map<int,RWInfo> RWBuffers;
extern  int bufferIdx;


int Detect_IMA_rgb();
int Detect_IMA_src2dest();
int Detect_IMA_SPVM();
void print_RWBuffers();



#endif //IPD_H
