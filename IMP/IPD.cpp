//
// Created by onzan_zyu on 2024/12/2.
//
#include "IPD.h"
std::map<int,RWInfo> RWBuffers;
int bufferIdx=0;
void print_RWBuffers(){
    for(const auto& pair : RWBuffers){
LOG_Buffer(LOG_INFO,"cycle:%d,%s,value:%d,address:%d",pair.second.cycle,pair.second.IsLoad?"load":"store",pair.second.value,pair.second.address);
    }
}