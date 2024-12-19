//
// Created by onzan_zyu on 2024/12/2.
//

#ifndef IPD_H
#define IPD_H

#include "DataType.h"
#include "../data_structures.h"
#include "../log.h"
#include <math.h>
#include <stdarg.h> // 添加对可变参数的支持
// 存储间接访存模式的特征
struct IPT
{
    // int BaseAddr;    // 基地址
    bool  valid;         // entry是否有效
    uint8_t shift;      // shift数值
    int  count;          //间接访存所属的cnt 用来判断3个InitialInterval识别成功
    DataType hit_cnt;    // 模式命中的次数
    bool prefetch_valid;    //是否已经用于预取
    int last_index_address; // 当前的索引
};
// RWBuffer的信息
struct RWInfo
{
    int cycle;     // 配置指令中的cycle
    bool  IsLoad;// load or store
    DataType value;
    AddrWD address;
    bool   IsIndex;
    int cur_count; // 当前配置指令执行的是多少个count
};

struct cnt_info{
    int cnt;
    int count;
};

// 索引数组的信息  用于验证IPD pattern时存储索引和所处的count 
struct Valid_Idx{
    int value;/// 用于验证的索引的值
    int count;/// 该索引所处的count
    int index_address;  //索引的地址标签
};


extern bool IPDEnable;
extern int InitialInterval;
//  存储捕捉到的load  store的信息  count  RWInfo
extern std::map<int,IPT> IPDentrys;


extern std::map<int,RWInfo> RWBuffers;
extern  int bufferIdx;
// 索引数组和目标数组
extern std::vector<Valid_Idx> index_array;
extern std::map<int,int>target_addr;


int Detect_IMA_rgb();
int Detect_IMA_src2dest();
int Detect_IMA_SPVM();
void print_RWBuffers();
int Index_array_Detect();
int valid_IPDEntry(std::vector<Valid_Idx>& index_array,std::map<int,int>target_addr,int curr_count);
int add_IPDEntry(int BaseAddr,uint8_t shift,int count,int last_idx_address);
int classify_array();
//打印IPD entry
void print_IPD();


#endif //IPD_H
