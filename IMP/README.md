#### 间接访存地址识别
* 索引数组地址是等差数列  根据特征先识别索引数组
* 间接访存存在依赖关系，目标地址load操作的cycle > 索引数组load操作的cycle-----》修改为两个Initial Interval内
* 对最近的读写访问情况识别之后 需要对ReadBuffer进行清空  同时将bufferIdx置零



#### BaseAddr的获取

* CGRA.cpp中 parseDMEM 时 map <string,int> spm_base_addr;   **用于存储变量和基地址**
* 索引的检测：cnt差距要在两个Initial Interval之内，地址构成等差数列，两个索引的值不能相同

*   todo:检测到的索引进行存储 同时实现 三个Initial  Interval内如果没发现间接访存模式  删除其cnt地址



#### 间接访存模式的验证
* 12.13  根据识别到的间接访存模式 计算target address,当目标地址和索引数组之间的count差小于2个II为生效  反向限制
* 12.13  根据索引的地址相同和值相同 将RWBUffer中此类索引修改为store操作 来排除数据干扰
* 12.13  将识别间接访存模式 和 间接访存模式的验证采用统一的kII来做判断条件---->解决了运行两次valid 就core dump的问题








#### Cache

* CPU和cache之间传递数据的单位是 word   cache和主存之间传递数据的单位是 block
* 组相联映射
  * 一个cache组中有多个cache line， cache line是从主存中读取数据的基本单位 尽管可能只访问line中的一个byte





* 之前的bank大小 536870912    现在采用 536870912*4





#### 评估传输的信息

* CPU和CGRA的耦合方式一般有松散耦合和紧密耦合
* 传输方式：DMA　　AXI　　　简化流程：外存默认为是主存　　　主存＜－－－－－＞SPM
* 主存和SPM访问延迟影响因素
  * SPM是片上内存  访问速度较快，一般是几纳秒    主存访问的相对延迟较高，一般是几十到百纳秒之间
  * SPM的带宽通常高于内存，SPM用于优化特定应用，主存带宽受到总线宽度和频率的限制，与SPM有较大差距
  * **综上 认为SPM的数据访问延迟为1cycle   当SPM访问发生miss时，需要访问内存，访问延迟假设为15cycle**



