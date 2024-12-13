//
// Created by onzan_zyu on 2024/12/2.
//
#include "IPD.h"
std::map<int,RWInfo> RWBuffers;
int bufferIdx=0;
// struct IPT IPDentrys[100];// 间接访存模式存储结构体数组
// std::vector<IPT> IPDentrys;
std::map<int,IPT> IPDentrys;

int InitialInterval;
// base_addr count  when count==2,base addr valid
std::map<int,int> BaseAddr_cnt;//when shift ==2
std::vector<int> diff(RWBuffers.size()+1, 0);

//用于验证IPD pattern的索引数组和验证数组
std::map<int,int>target_addr;
std::vector<Valid_Idx> index_array;
// std::map<int,int> BaseAddr_cnt1;//when shift ==3
// std::map<int,int>BaseAddr_cnt2;  //when shift ==4

bool IPDEnable= true;



//打印load store buffer中的数据
void print_RWBuffers(){
    int cnt = 0;
    for(const auto& pair : RWBuffers){
        if(pair.second.cycle==-1){
            // LOG_Buffer(LOG_INFO,"\n\n-------------------------------------execute cycle finished-----------------------------------------------\n\n");
        }
        else{
            LOG_Buffer(LOG_INFO,"RWBuffers[%d],count:%d,cycle:%d,%s,address:%d,value:%d,%s",
                      cnt,pair.second.cur_count,pair.second.cycle,pair.second.IsLoad?"load":"store",
                      pair.second.address,pair.second.value,pair.second.IsIndex?"Index Array":" ");
            cnt=cnt+1;
        }

    }
    LOG_Buffer(LOG_INFO,"\n\n-------------------------------------execute cycle finished-----------------------------------------------\n\n");
}

// 用于rgb  由于rgb中存在 数据依赖关系的操作 cycle相同  删除了cycle限制的依赖
int Detect_IMA_rgb(){
    // std::vector<int> diff(RWBuffers.size()+1, 0);
    
    int temp;
    int prev = 0;
    int count;
    int flag  =0;
    // 计算地址中的等差数列 并 标记为索引
    printf("标记索引\n");
    if(RWBuffers.size()==0){
        printf(" IPD detect RWBuffer size==0\n");
        return 0;
   }
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
    if(RWBuffers.size()==0){
        printf(" idx detect RWBuffer size==0\n");
        return 0;
   }
    for (int i = 1; i < RWBuffers.size(); i++)
    {
        if (diff[i]==0 && RWBuffers[i].IsLoad) // 处理hit  miss addr 且是load操作
        {
            for (int j=i-1;j>=0;j--)
            {//  索引数组  rgb中 不能根据cycle识别依赖 && (RWBuffers[i].cycle>RWBuffers[j].cycle) 
                if (diff[j]!=0 )
                {
                    RWBuffers[j].IsIndex = true;   //标记为索引数组
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
    // std::vector<int> diff(RWBuffers.size()+1, 0);
    
    int temp;
    int prev = 0;
    int count;
    int flag  =0;
    // 计算地址中的等差数列 并 标记为索引
    if(RWBuffers.size()==0){
        printf(" idx detect RWBuffer size==0\n");
        return 0;
   }
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
                    //    LOG_Buffer(LOG_INFO,"find indirect memory access pattern!");
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
                    RWBuffers[j].IsIndex = true;   //标记为索引数组
                    int temp_addr = RWBuffers[i].address - RWBuffers[j].value*4;
                    // int temp_addr1 = RWBuffers[i].address - RWBuffers[j].value*8;
                    ///////////////////有待进一步商榷 当value为0时 访存地址为本身
                    ///// to do 需要过滤 还是 根据识别到的访存模式进一步去检测未来的地址是否正确
                    // if (temp_addr>0&&RWBuffers[j].value!=0)
                    if (temp_addr>0)
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

// 尝试新的BaseAddr_cnt
std::map<int,cnt_info> BaseAddr_struct;//when shift ==2

int Detect_IMA_SPVM(){
      // 根据索引计算base addr
    printf("IMA  detect\n");
    LOG_CALCULATE(LOG_INFO,"InitialInterval=%d\n",InitialInterval);
   if(RWBuffers.size()==0){
        printf("RWBuffer size==0\n");
        return 0;
   }
    for (int i = 1; i < RWBuffers.size(); i++)
    {
        if ( (!RWBuffers[i].IsIndex) && RWBuffers[i].IsLoad) // 处理hit  miss addr 且是load操作
        {
            for (int j=i-1;j>=0;j--)
            {//  索引数组        两个InitialInterval内的计算   控制寻找索引的范围
                if (RWBuffers[j].IsIndex  &&  RWBuffers[i].cur_count-RWBuffers[j].cur_count<2*InitialInterval)//  是索引数组
                {
                    int temp_addr = RWBuffers[i].address - RWBuffers[j].value*4;
                    // int temp_addr1 = RWBuffers[i].address - RWBuffers[j].value*8;
                    ///////////////////有待进一步商榷 当value为0时 访存地址为本身
                    ///// to do 需要过滤 还是 根据识别到的访存模式进一步去检测未来的地址是否正确
                    // if (temp_addr>0&&RWBuffers[j].value!=0)
                    if (temp_addr>0)
                    {
                        if(BaseAddr_struct.find(temp_addr)==BaseAddr_struct.end()){// 创建新的
                            struct cnt_info temp_info;
                            temp_info.cnt = 1;
                            temp_info.count = RWBuffers[i].cur_count;
                            BaseAddr_struct[temp_addr]=  temp_info;
                        }else{
                            BaseAddr_struct[temp_addr].cnt++;
                            BaseAddr_struct[temp_addr].count = RWBuffers[i].cur_count;
                        }
                        
                        //过了7个InitialInterval 后 hit数不到5的BaseAddr  移除
                        // printf("erase start\n");
                        if((RWBuffers[i].cur_count>BaseAddr_struct[temp_addr].count+7*InitialInterval)&&BaseAddr_struct[temp_addr].count<5){
                                 BaseAddr_struct.erase(temp_addr);
                                continue;      
                        }
                        // printf("erase finish\n");
                        // BaseAddr_cnt[temp_addr]++;
                        LOG_CALCULATE(LOG_INFO,"RWBUffers[%d]=%d,RWBUffers[%d]=%d,RWBUffers[%d]-RWBuffers[%d]*4=%d cnt=%d\n",
                                                    i,RWBuffers[i].address,j,RWBuffers[j].value,i,j,temp_addr,BaseAddr_struct[temp_addr].cnt);
                        if (BaseAddr_struct[temp_addr].cnt>=5)//  提高 间接访存模式的要求
                        {   
                            add_IPDEntry(temp_addr,2,BaseAddr_struct[temp_addr].cnt);
                            //    识别到了间接访存模式  将间接访存模式存储到 IPDentrys数组
                            LOG_IMA(LOG_INFO,"diff[%d]=%d,RWBuffer[%d] load address=%d,load value=%d base addr=%d,shift=%d  cnt=%d\n",
                                                  i,diff[i],i,RWBuffers[i].address,RWBuffers[j].value,temp_addr,2,BaseAddr_struct[temp_addr].cnt);
                            // break;
                        }
                    }                       
                    
                }
            }
        }
    }
    return 0;
}

//用于检测索引数组 并在diff中标注地址差
int Index_array_Detect(){
    int temp;
    int prev = 0;
    int count = 0;
    int flag  =0;
    std::vector<int> idx;//  用于存储当前的索引数组再RWBuffers中的index
    int pre_addr = 0;
    // printf("index array detect\n");
    if(RWBuffers.size()==0){
        printf(" idx detect RWBuffer size==0\n");
        return 0;
   }
    
    for (int i = 1; i < RWBuffers.size(); i++)
    {
        pre_addr = RWBuffers[i].address;
        idx.push_back(i);

        if(!RWBuffers[i].IsLoad){////    确保是load操作
            continue;
        }
       for (int j=i-1;j>=0;j--)
       {
            if(!RWBuffers[j].IsLoad){ ////    确保是load操作
                continue;
            }
           temp = pre_addr-RWBuffers[j].address;  
           ///////  索引当前值和前一个值不同  排除都是0的output
           if ((temp==4 || temp==8) && RWBuffers[j].value!=RWBuffers[idx.back()].value)
           {
               idx.push_back(j);
               pre_addr = RWBuffers[j].address;
               if (idx.size()>=14) // 当存在10个及以上时 才会认定为索引
               {
                   for (int i=0;i<idx.size();i++)
                   {
                        diff[idx[i]] = temp;
                        RWBuffers[idx[i]].IsIndex = true;
                        // if(idx.size()>10)
                        //     LOG_Index(LOG_INFO,"idx[%d]:RWBuffers[%d].address=%d\n",i,idx[i],RWBuffers[idx[i]].address);
                   }
               }
           }
           else if (RWBuffers[j].value==RWBuffers[idx.back()].value && RWBuffers[j].address==RWBuffers[idx.back()].address)
            {
                idx.push_back(j);
                if (idx.size()>=3) // 4个
                {
                    for (int i=0;i<idx.size();i++)
                    {
                        RWBuffers[idx[i]].cycle = -1;
                        RWBuffers[idx[i]].IsLoad = 0;
                    }
                }
            }




       }
        idx.clear();
    }
    if(BaseAddr_struct.size()!=0){
        BaseAddr_struct.clear();
    }
    LOG_CALCULATE(LOG_INFO,"\n\n----------------------------one cycle calculate finished-------------------\n\n");
    
    return 1;

}

int add_IPDEntry(int BaseAddr,uint8_t shift,int count){
    //  找到entry
    if(IPDentrys.find(BaseAddr)!=IPDentrys.end())
    {
        // printf("IPDentry already exists\n");
        return 0;
    }
    else{
        struct IPT temp ={true,shift,count,0,false};
        IPDentrys[BaseAddr] = temp;
        // printf("add IPDentry\n");
        return 1;
    }
    
}

// 用检测到的索引和target来 验证IPD的entry    如何判断 IPDEntry 和index之间的count差在两个II之内
int valid_IPDEntry(std::vector<Valid_Idx>& index_array,std::map<int,int>target_addr,int curr_count){
    int temp; int cnt = 0;
    printf("validate entry\n");
    LOG_Index(LOG_INFO,"IPD entry num:%d, index_array size=%d,target array size = %d\n",IPDentrys.size(),index_array.size(),target_addr.size());
    for(auto it = IPDentrys.begin(); it != IPDentrys.end();it++){
        for(int j=0;j<index_array.size();j++){
            //IPD和索引之间的 count差要小于2倍的Initial Interval  pow(2,it->second.shift)
            LOG_Index(LOG_INFO,"IPD count:%d,index[%d],count=%d\n",it->second.count,j,index_array[j].count);
            // it->second.count+2*InitialInterval<=index_array[j].count   valid entry的时候不应该判断count差
            if(it->second.valid){
                temp = index_array[j].value * 4 + it->first;
                LOG_Index(LOG_INFO,"index_array[%d]=%d base=%d,idx*4+base = %d,count=%d\n",j,index_array[j],it->first,temp,it->second.count);    
                // 根据模式找到了匹配的目标地址且目标地址和索引地址之间的差距小于2*InitialIntervald 
                if(target_addr.find(temp)!=target_addr.end()&& target_addr[temp]-index_array[j].count<2*InitialInterval){
                    it->second.hit_cnt++;
                    cnt++;
                    // if(it->second.hit_cnt>=30){
                    //     IPDEnable = false;
                    // }
                    LOG_Validate(LOG_INFO,"pattern hit! index:%d shift:%d baseaddr:%d  target:%d  hitcount:%d\n",
                    index_array[j],it->second.shift,it->first,temp,it->second.hit_cnt);
                }
            }
            //   标记entry中过了20个II且hit次数较少的
            if(curr_count - it->second.count>40*InitialInterval && it->second.hit_cnt<6){
                it->second.valid = false;
            }
        }
    } 
    LOG_Index(LOG_INFO,"target addr num=%d, hitcount sum=%d\n",
                    target_addr.size(),cnt);
    //  遍历间接访存的entry 移除invalid的entry
    for(auto it = IPDentrys.begin(); it != IPDentrys.end();){
        // (curr_count - it->second.count>5*InitialInterval&&it->second.hit_cnt<3) 
        if(  !it->second.valid){
            LOG_Index(LOG_INFO,"remove baseadr:%d,hit_cnt:%d\n",it->first,it->second.hit_cnt);
            it = IPDentrys.erase(it);
        }
        else 
            it++;
    }
    printf("validate finished\n");
    return 0;


}

 // 标注用于 validate 的索引数组 和目标数组
int classify_array(){
    printf("calssify array  RWBuffer size=%d\n",RWBuffers.size());   // 要排除store操作
    int idx=0;
    int target=0;
    for(int i=0;i<RWBuffers.size();i++){    //  load操作  索引数组
        if(RWBuffers[i].IsIndex && RWBuffers[i].IsLoad){
            Valid_Idx temp = {int(RWBuffers[i].value),RWBuffers[i].cur_count};
            index_array.push_back(temp);
            idx++;
            LOG_TXT(LOG_INFO,"index,idx:%d,addr:%d,value:%d\n",i,RWBuffers[i].address,RWBuffers[i].value);
        }
        else if(RWBuffers[i].IsLoad){//  load数组
            target_addr[RWBuffers[i].address] = RWBuffers[i].cur_count;
            target++;
            LOG_TXT(LOG_INFO,"target,idx:%d,addr:%d,value:%d\n",i,RWBuffers[i].address,RWBuffers[i].value);
        }
    }
    printf("classify finished index size=%d,target num=%d,target_addr size=%d\n",idx,target,target_addr.size());
    return 1;
}



void print_IPD(){
    for(const auto&pair:IPDentrys){
        LOG_IPDentry(LOG_INFO,"baseaddr:%d,valid:%s,shift:%d,count:%d,hit_cnt:%d\n",
            pair.first,pair.second.valid?"valid":"invalid",pair.second.shift,pair.second.count,
            pair.second.hit_cnt);
    }
}





























// int Detect_IMA_rgb(){
//     std::vector<int> diff(RWBuffers.size()+1, 0);
    
//     int temp;
//     int prev = 0;
//     int count;
//     int flag  =0;
//     // 计算地址中的等差数列 并 标记为索引
//     printf("标记索引\n");
//     for (int i = 1; i < RWBuffers.size(); i++)
//     {
//        for (int j=i;j>=0;j--)
//        {
//            temp = RWBuffers[i].address-RWBuffers[j].address;
//            //加载操作 且地址差距在数据的size大小内
//            if (temp>0 && temp<=8 && RWBuffers[i].IsLoad)
//            {
//                if (prev==temp)
//                {
//                    count++;
//                    if (count>=3 && flag==0)
//                    {
//                         flag ==1;
//                     //    printf("find indirect memory access pattern! size = %d\n",prev);
//                        LOG_Buffer(LOG_INFO,"find indirect memory access pattern!");
//                    }
//                }
//                prev = temp;
//                diff[i] = temp;
//                diff[j] = diff[j]!=temp?temp:diff[j];
//                // printf("RWBuffer[%d]-RWBuffer[%d] = diff[%d] = %d\n",i,j,i,diff[i]);
//                break; //第一次发现等差就停止遍历 防止差额变为2d

//            }
//        }

//     }

//     // 根据索引计算base addr
//     printf("计算base addr\n");
//     for (int i = 1; i < RWBuffers.size(); i++)
//     {
//         if (diff[i]==0 && RWBuffers[i].IsLoad) // 处理hit  miss addr 且是load操作
//         {
//             for (int j=i-1;j>=0;j--)
//             {//  索引数组  rgb中 不能根据cycle识别依赖 && (RWBuffers[i].cycle>RWBuffers[j].cycle) 
//                 if (diff[j]!=0 )
//                 {
//                     RWBuffers[j].IsIndex = true;   //标记为索引数组
//                     int temp_addr = RWBuffers[i].address - RWBuffers[j].value*4;
//                     // int temp_addr1 = RWBuffers[i].address - RWBuffers[j].value*8;
//                     ///////////////////有待进一步商榷 当value为0时 访存地址为本身
//                     ///// to do 需要过滤 还是 根据识别到的访存模式进一步去检测未来的地址是否正确
//                     // if (temp_addr>0&&RWBuffers[j].value!=0)
//                     if (temp_addr>=0)
//                     {
//                         BaseAddr_cnt[temp_addr]++;
//                         LOG_CALCULATE(LOG_INFO,"RWBUffers[%d]=%d,RWBUffers[%d]=%d,RWBUffers[%d]-RWBuffers[%d]*4=%d cnt=%d\n",
//                                                     i,RWBuffers[i].address,j,RWBuffers[j].value,i,j,temp_addr,BaseAddr_cnt[temp_addr]);
//                         if (BaseAddr_cnt[temp_addr]==2)
//                         {
//                             LOG_IMA(LOG_INFO,"diff[%d]=%d,RWBuffer[%d] load address=%d,load value=%d base addr=%d,shift=%d\n",
//                                                   i,diff[i],i,RWBuffers[i].address,RWBuffers[j].value,temp_addr,2);
//                             // printf("RWBuffer[%d] load address=%d,load value=%d ",i,RWBuffers[i].address,RWBuffers[j].value);
//                             // printf("base addr=%d,shift=%d\n",temp_addr,2);
//                             break;
//                         }
//                     }                       
                    
//                 }
//             }
//         }
//     }
//     // return flag;
//     if(BaseAddr_cnt.size()!=0){
//         BaseAddr_cnt.clear();
//     }
    
//     return 1;

// }
