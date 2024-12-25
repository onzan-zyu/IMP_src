#### 间接访存地址识别
* 索引数组地址是等差数列  根据特征先识别索引数组
* 间接访存存在依赖关系，目标地址load操作的cycle > 索引数组load操作的cycle-----》修改为两个Initial Interval内
* 对最近的读写访问情况识别之后 需要对ReadBuffer进行清空  同时将bufferIdx置零

* todo: 索引数组标注地址信息
* IPD entry中记录上一次使用的索引地址  IPD entry中上一次计算的索引地址和用于验证的索引数组 差值两个元素单位   addIPDEntry()的时候添加索引的地址  valid_IPD的时候每次计算完要更新上一次计算的索引地址


#### BaseAddr的获取

* CGRA.cpp中 parseDMEM 时 map <string,int> spm_base_addr;   **用于存储变量和基地址**
* 索引的检测：cnt差距要在两个Initial Interval之内，地址构成等差数列，两个索引的值不能相同

*   todo:检测到的索引进行存储 同时实现 三个Initial  Interval内如果没发现间接访存模式  删除其cnt地址



#### 间接访存模式的验证
* 12.13  根据识别到的间接访存模式 计算target address,当目标地址和索引数组之间的count差小于2个II为生效  反向限制
* 12.13  根据索引的地址相同和值相同 将RWBUffer中此类索引修改为store操作 来排除数据干扰
* 12.13  将识别间接访存模式 和 间接访存模式的验证采用统一的kII来做判断条件---->解决了运行两次valid 就core dump的问题
* 12.15  判断IPDEntry.size()作为识别IMP和valid的条件时，需要先获取size，否则会产生core dump 执行次数168902 额外20次 = 168921


* 12.22  完成了地址hit miss的识别功能   todo----如何预取  什么时机插入合适的预取  预取和判断预取的时机   怎么样预取才不会导致miss
* 12.24 new idea:考虑在运行时直接去验证IPD















