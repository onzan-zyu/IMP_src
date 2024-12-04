//
// Created by onzan_zyu on 2024/12/2.
//
#include "IPD.h"
std::map<int,RWInfo> RWBuffers;
int bufferIdx=0;
struct IPT IPDentrys[100];// 间接访存模式存储结构体数组
// base_addr count  when count==2,base addr valid
std::map<int,int> BaseAddr_cnt;//when shift ==2
std::map<int,int> BaseAddr_cnt1;//when shift ==3
std::map<int,int>BaseAddr_cnt2;  //when shift ==4



//打印load store buffer中的数据
void print_RWBuffers(){
    for(const auto& pair : RWBuffers){
        if(pair.second.cycle==-1){
            // LOG_Buffer(LOG_INFO,"\n\n-------------------------------------execute cycle finished-----------------------------------------------\n\n");
        }
        else{
            LOG_Buffer(LOG_INFO,"cycle:%d,%s,address:%d,value:%d",pair.second.cycle,pair.second.IsLoad?"load":"store",pair.second.address,pair.second.value);
        }

    }
}

// 用于rgb  由于rgb中存在 数据依赖关系的操作 cycle相同  删除了cycle限制的依赖
int Detect_IMA_rgb(){
    std::vector<int> diff(RWBuffers.size()+1, 0);
    
    int temp;
    int prev = 0;
    int count;
    int flag  =0;
    // 计算地址中的等差数列 并 标记为索引
    printf("标记索引\n");
    for (int i = 1; i < RWBuffers.size(); i++)
    {
       for (int j=i;j>=0;j--)
       {
           temp = RWBuffers[i].address-RWBuffers[j].address;
           //加载操作 且地址差距在数据的size大小内
           if (temp>0 && temp<=8 && RWBuffers[i].IsLoad)
           {
               if (prev==temp)
               {
                   count++;
                   if (count>=3 && flag==0)
                   {
                        flag ==1;
                    //    printf("find indirect memory access pattern! size = %d\n",prev);
                       LOG_Buffer(LOG_INFO,"find indirect memory access pattern!");
                   }
               }
               prev = temp;
               diff[i] = temp;
               diff[j] = diff[j]!=temp?temp:diff[j];
               // printf("RWBuffer[%d]-RWBuffer[%d] = diff[%d] = %d\n",i,j,i,diff[i]);
               break; //第一次发现等差就停止遍历 防止差额变为2d

           }
       }

    }

    // 根据索引计算base addr
    printf("计算base addr\n");
    for (int i = 1; i < RWBuffers.size(); i++)
    {
        if (diff[i]==0 && RWBuffers[i].IsLoad) // 处理hit  miss addr 且是load操作
        {
            for (int j=i-1;j>=0;j--)
            {//  索引数组  rgb中 不能根据cycle识别依赖 && (RWBuffers[i].cycle>RWBuffers[j].cycle) 
                if (diff[j]!=0 )
                {
                    int temp_addr = RWBuffers[i].address - RWBuffers[j].value*4;
                    // int temp_addr1 = RWBuffers[i].address - RWBuffers[j].value*8;
                    ///////////////////有待进一步商榷 当value为0时 访存地址为本身
                    ///// to do 需要过滤 还是 根据识别到的访存模式进一步去检测未来的地址是否正确
                    // if (temp_addr>0&&RWBuffers[j].value!=0)
                    if (temp_addr>=0)
                    {
                        BaseAddr_cnt[temp_addr]++;
                        LOG_CALCULATE(LOG_INFO,"RWBUffers[%d]=%d,RWBUffers[%d]=%d,RWBUffers[%d]-RWBuffers[%d]*4=%d cnt=%d\n",
                                                    i,RWBuffers[i].address,j,RWBuffers[j].value,i,j,temp_addr,BaseAddr_cnt[temp_addr]);
                        if (BaseAddr_cnt[temp_addr]==2)
                        {
                            LOG_IMA(LOG_INFO,"diff[%d]=%d,RWBuffer[%d] load address=%d,load value=%d base addr=%d,shift=%d\n",
                                                  i,diff[i],i,RWBuffers[i].address,RWBuffers[j].value,temp_addr,2);
                            // printf("RWBuffer[%d] load address=%d,load value=%d ",i,RWBuffers[i].address,RWBuffers[j].value);
                            // printf("base addr=%d,shift=%d\n",temp_addr,2);
                            break;
                        }
                    }                       
                    
                }
            }
        }
    }
    // return flag;
    if(BaseAddr_cnt.size()!=0){
        BaseAddr_cnt.clear();
    }
    
    return 1;

}


int Detect_IMA_src2dest(){
    std::vector<int> diff(RWBuffers.size()+1, 0);
    
    int temp;
    int prev = 0;
    int count;
    int flag  =0;
    // 计算地址中的等差数列 并 标记为索引
    printf("标记索引\n");
    for (int i = 1; i < RWBuffers.size(); i++)
    {
       for (int j=i;j>=0;j--)
       {
           temp = RWBuffers[i].address-RWBuffers[j].address;
           //加载操作 且地址差距在数据的size大小内
           if (temp>0 && temp<=8 && RWBuffers[i].IsLoad)
           {
               if (prev==temp)
               {
                   count++;
                   if (count>=3 && flag==0)
                   {
                        flag ==1;
                    //    printf("find indirect memory access pattern! size = %d\n",prev);
                       LOG_Buffer(LOG_INFO,"find indirect memory access pattern!");
                   }
               }
               prev = temp;
               diff[i] = temp;
               diff[j] = diff[j]!=temp?temp:diff[j];
               // printf("RWBuffer[%d]-RWBuffer[%d] = diff[%d] = %d\n",i,j,i,diff[i]);
               break; //第一次发现等差就停止遍历 防止差额变为2d

           }
       }

    }

    // 根据索引计算base addr
    printf("计算base addr\n");
    for (int i = 1; i < RWBuffers.size(); i++)
    {
        if (diff[i]==0 && RWBuffers[i].IsLoad) // 处理hit  miss addr 且是load操作
        {
            for (int j=i-1;j>=0;j--)
            {//  索引数组
                if (diff[j]!=0 && (RWBuffers[i].cycle>RWBuffers[j].cycle) )
                {
                    int temp_addr = RWBuffers[i].address - RWBuffers[j].value*4;
                    // int temp_addr1 = RWBuffers[i].address - RWBuffers[j].value*8;
                    ///////////////////有待进一步商榷 当value为0时 访存地址为本身
                    ///// to do 需要过滤 还是 根据识别到的访存模式进一步去检测未来的地址是否正确
                    // if (temp_addr>0&&RWBuffers[j].value!=0)
                    if (temp_addr>0)
                    {
                        BaseAddr_cnt[temp_addr]++;
                        LOG_CALCULATE(LOG_INFO,"RWBUffers[%d]=%d,RWBUffers[%d]=%d,RWBUffers[%d]-RWBuffers[%d]*4=%d cnt=%d\n",
                                                    i,RWBuffers[i].address,j,RWBuffers[j].address,i,j,temp_addr,BaseAddr_cnt[temp_addr]);
                        if (BaseAddr_cnt[temp_addr]==2)
                        {
                            LOG_IMA(LOG_INFO,"diff[%d]=%d,RWBuffer[%d] load address=%d,load value=%d base addr=%d,shift=%d\n",
                                                  i,diff[i],i,RWBuffers[i].address,RWBuffers[j].value,temp_addr,2);
                            // printf("RWBuffer[%d] load address=%d,load value=%d ",i,RWBuffers[i].address,RWBuffers[j].value);
                            // printf("base addr=%d,shift=%d\n",temp_addr,2);
                            break;
                        }
                    }                       
                    
                }
            }
        }
    }
    // return flag;
    if(BaseAddr_cnt.size()!=0){
        BaseAddr_cnt.clear();
    }
    
    return 1;

}