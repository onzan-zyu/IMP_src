/*
 * CGRA.cpp
 *
 *  Created on: 6 Mar 2017
 *      Author: manupa
 */

#include "CGRA.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <assert.h>

namespace HyCUBESim {

CGRA::CGRA(int SizeX, int SizeY,int type, int MemSize) {
	// TODO Auto-generated constructor stub
	sizeX = SizeX;
	sizeY = SizeY;
	MEM_SIZE = MemSize;

	for (int y = 0; y < SizeY; ++y) {
		for (int x = 0; x < SizeX; ++x) {
			if(type == 1)
				CGRATiles[y][x] = new CGRATile(x,y,(x==0),&dmem,MemSize);
			else if(type == 2)
				CGRATiles[y][x] = new CGRATile(x,y,(x==0 or (x==SizeX-1)),&dmem,MemSize);
		}
	}

	for (int y = 0; y < SizeY; ++y) {
		for (int x = 0; x < SizeX; ++x) {

			if(x+1 < SizeX){
				CGRATiles[y][x]->connectedTiles[EAST]=CGRATiles[y][x+1];
			}

			if(y+1 < SizeY){
				CGRATiles[y][x]->connectedTiles[SOUTH]=CGRATiles[y+1][x];
			}

			if(x-1 >= 0){
				CGRATiles[y][x]->connectedTiles[WEST]=CGRATiles[y][x-1];
			}

			if(y-1 >= 0){
				CGRATiles[y][x]->connectedTiles[NORTH]=CGRATiles[y-1][x];
			}

		}
	}

	for (int i = 0; i < MEM_SIZE; ++i) {
		dmem[i]=0;
	}

}

// void printBinary(unsigned int number) {
//     unsigned int mask = 1U << (sizeof(number) * 8 - 1);
//     for (int i = 0; i < sizeof(number) * 8; ++i) {
//         printf("%d", (number & mask) ? 1 : 0); 
//         number <<= 1; 
//     }
//     printf(" constant print\n");
// }
void isEqual(unsigned int number,std::string binaryStr){
	 int result = 0;
    int length = binaryStr.length();

    for (int i = 0; i < length; ++i) {
        // 将当前字符转换为数值（'0' 或 '1'）
        int bitValue = binaryStr[i] - '0';

        // 计算当前位的权重（2的幂）
        int power = length - 1 - i;

        // 将当前位的值乘以其权重并累加到结果中
        result += bitValue * (1 << power);
    }
	if(result!=number){
		printf("str:%s number:%d\n",binaryStr,number);
		LOG_Missmatch(LOG_INFO,"str:%s number:%d\n",binaryStr,number);
	}
}

//所属Initial Interval 中的cycle
int CGRA::configCGRA(std::string CMEMFileName,int xdim, int ydim) {

	std::ifstream cmemfile(CMEMFileName.c_str());
	std::string line;
	LOG(SIMULATOR) << CMEMFileName << "\n";
	assert(cmemfile.is_open() && "Error opening cmem file");

	//ignoring the first line
	std::getline(cmemfile,line);
	int t;
	while(1){
		if(!std::getline(cmemfile,line)){
			break;
		}

		if(line.empty()){
			break;
		}

	    std::istringstream iss(line);
	    

	    iss >> t;   // cycle   增加每个配置所属的cycle
		// printf("read cycle:%d\n",t);
		std::string phyloc;
		    std::string op;
	    for (int i = 0; i < xdim*ydim; ++i) {
		    std::getline(cmemfile,line);
		    iss.str(line);
			iss.clear();
		    

		    std::getline(iss,phyloc,',');
		    std::getline(iss,op,',');
			iss.str("");
		    int y=i/xdim;
		    int x=i%xdim;

		    LOG(SIMULATOR) << "T=" << t << ",Y=" << y << ",X=" << x << "," << op << "\n";

		    HyIns currIns;
			currIns.current_cycle = t;
			// printf("instruction %d parse start\n",t);
			// printf("op=%s,NPB:%s\n",op.c_str(),op.substr(0,1).c_str());
		    if(atoi(op.substr(0,1).c_str())==1){
		    	currIns.NPB=true;
		    }
		    else{
		    	currIns.NPB=false;
		    }
			// printf("op=%s,NPB:%s\n",op.c_str(),op.substr(1,1).c_str());
		    if(atoi(op.substr(1,1).c_str())==1){
		    	currIns.constValid=true;
		    }
		    else{
		    	currIns.constValid=false;
		    }
			// printf("constant parse %s",op.substr(2,32).c_str());
			currIns.constant = std::stoll(/*op.substr(46,3) +*/ op.substr(2,27+5),nullptr,2);
			// printf(" constant parse success\n");
			/////////////////////////////////////////////
			// printBinary(currIns.constant);
			isEqual(currIns.constant,op.substr(2,32).c_str());
			////////////////////////////////////////////
			// if((currIns.constant >> 32) == 1){ //negative number identification
			// 	currIns.constant = currIns.constant | 0b11111000000000000000000000000000;
			// }

			// printf("opcode=%d\n",currIns.opcode);
			currIns.opcode = std::stoi(op.substr(29+5,5),nullptr,2);
			
			// printf("regwen=%d %d %d %d\n",currIns.regwen[Reg0],currIns.regwen[Reg1],currIns.regwen[Reg2],currIns.regwen[Reg3]);
		    currIns.regwen[Reg0] = std::stoi(op.substr(34+5,1),nullptr,2);
		    currIns.regwen[Reg2] = std::stoi(op.substr(35+5,1),nullptr,2);
		    currIns.regwen[Reg3] = std::stoi(op.substr(36+5,1),nullptr,2);
		    currIns.regwen[Reg1] = std::stoi(op.substr(37+5,1),nullptr,2);
			// printf("tregwen=%d\n",currIns.tregwen);
		    currIns.tregwen = std::stoi(op.substr(38+5,1),nullptr,2);
			// printf("regbypass=%d %d %d %d\n",currIns.regbypass[Reg0],currIns.regbypass[Reg1],currIns.regbypass[Reg2],currIns.regbypass[Reg3]);
		    currIns.regbypass[Reg3] = std::stoi(op.substr(39+5,1),nullptr,2);
		    currIns.regbypass[Reg0] = std::stoi(op.substr(40+5,1),nullptr,2);
		    currIns.regbypass[Reg2] = std::stoi(op.substr(41+5,1),nullptr,2);
		    currIns.regbypass[Reg1] = std::stoi(op.substr(42+5,1),nullptr,2);

		    LOG(SIMULATOR) << "XbConfig : " << op.substr(43+5,21) << "\n";
		    LOG(SIMULATOR) << "xB.I2 : " << op.substr(49+5,3) << "\n";
			// printf("currIns.xB.P\n");
		    currIns.xB.P = convertStrtoXBI(op.substr(43+5,3));

//		    if(currIns.constValid){
//		    	currIns.xB.I2 = INV;
//		    }
//		    else{
		    	currIns.xB.I2 = convertStrtoXBI(op.substr(46+5,3));
//		    }
		    currIns.xB.I1 = convertStrtoXBI(op.substr(49+5,3));
			// printf("NWSE_O\n");
		    currIns.xB.NORTH_O = convertStrtoXBI(op.substr(52+5,3));
		    currIns.xB.WEST_O = convertStrtoXBI(op.substr(55+5,3));
		    currIns.xB.SOUTH_O = convertStrtoXBI(op.substr(58+5,3));
		    currIns.xB.EAST_O = convertStrtoXBI(op.substr(61+5,3));
			// printf("instruction %d parse finished\n",t);
		    CGRATiles[y][x]->printIns(currIns);

		    CGRATiles[y][x]->configMem.push_back(currIns);
			phyloc.clear();
			op.clear();
			line.clear();
		}
	    std::getline(cmemfile,line);
	}
	InitialInterval = t+1;
	cmemfile.close();
}

int CGRA::parseDMEM(std::string DMEMFileName) {

	std::ifstream dmemfile(DMEMFileName.c_str());
	std::string line;

	assert(dmemfile.is_open() && "Error opening dmem file");

	//ignore the first line
	std::getline(dmemfile,line);

	for (int i = 0; i < 4096; ++i) {
		dmem[i]=0;
	}

	while(std::getline(dmemfile,line)){
		std::istringstream iss(line);

		std::string addr;
		std::string pre;
		std::string post;

		std::getline(iss,addr,',');
		std::getline(iss,pre,',');
		std::getline(iss,post,',');

		LOG(SIMULATOR) << addr << "," << pre << "\n";
		InterestedAddrList.push_back(atoi(addr.c_str()));

		dmem[(DataType)atoi(addr.c_str())]=atoi(pre.c_str());
	}
}

int CGRA::parseDMEM(std::string DMEMFileName,std::string memallocFileName) {
	std::ifstream dmemfile(DMEMFileName.c_str());
	std::ifstream memallocfile(memallocFileName.c_str());
	std::string line;
	assert(dmemfile.is_open() && "Error opening dmem file");
	assert(memallocfile.is_open() && "Error opening memalloc file");
	//ignore the first line
	std::getline(dmemfile,line);
	std::getline(memallocfile,line);

	// for (int i = 0; i < MEM_SIZE; ++i) {
	// 	dmem[i]=0;
	// }
	std::map<std::string, int> spm_base_addr;
	printf("Start parse memalloc file\n");
	std::istringstream iss2;
	while(std::getline(memallocfile,line)){
		iss2.str(line);
		iss2.clear();

		std::string var_name;
		std::string base_addr;
		std::getline(iss2,var_name,',');
		std::getline(iss2,base_addr,',');

		spm_base_addr[var_name]= atoi(base_addr.c_str());
		LOG(SIMULATOR) << var_name << "," << spm_base_addr[var_name] << "\n";
		var_name.clear();
		base_addr.clear();
		iss2.str("");
	}
	memallocfile.close();
	
	printf("Start parse data mem file\n");
	std::istringstream iss;
	while(std::getline(dmemfile,line)){
		iss.str(line);
		iss.clear();

		std::string var_name;
		std::string offset;
		int addr;
		std::string pre;
		std::string post;

		std::getline(iss,var_name,',');
		std::getline(iss,offset,',');
		std::getline(iss,pre,',');
		std::getline(iss,post,',');

		addr = spm_base_addr[var_name]+atoi(offset.c_str());

		//LOG(SIMULATOR) << addr << "," << pre << "\n";
		InterestedAddrList.push_back(addr);
		// printf("addr=%d,data=%d",addr,pre.c_str());
		dmem[(DataType)addr]=atoi(pre.c_str());
		dmem_post[(DataType)addr]=atoi(post.c_str());
		dmem_pre[(DataType)addr]=atoi(post.c_str());
		var_name.clear();
		offset.clear();
		pre.clear();
		post.clear();
		iss.str("");
	}
	dmemfile.close();
	printf("data mem parse finished\n");
#ifndef ARCHI_16BIT
	dmem[MEM_SIZE-2]=1;//dmem[4094]=1;
	InterestedAddrList.push_back(MEM_SIZE-2);//InterestedAddrList.push_back(4094);
#endif
//	LOG(SIMULATOR) << "Data Memory Content\n";
//	for (int i = 0; i < 4096; ++i) {
//		LOG(SIMULATOR) << i << "," << (int)dmem[i] << "\n";
//	}
}

//dmem is byte addressable memory, base_addr is the byte address
//data size should be number of bytes
void CGRA::writeDMEM(HyCUBESim::CGRA& cgraInstance, int base_addr, uint8_t* data, int data_size) {
    for (int i = 0; i < data_size; ++i) {
        cgraInstance.dmem[base_addr + i] = data[i];
    }
}

void CGRA::readDMEM(HyCUBESim::CGRA& cgraInstance, int base_addr, uint8_t* data, int data_size) {
    for (int i = 0; i < data_size; ++i) {
        data[i] = cgraInstance.dmem[base_addr + i];
    }
}

void CGRA::invokeCGRA(HyCUBESim::CGRA& cgraInstance) {
    int count = 0;

#ifdef ARCHI_16BIT
    while (cgraInstance.dmem[MEM_SIZE - 2] == 0) {
        cgraInstance.executeCycle(count);
        count++;
    }
#else
    while (cgraInstance.dmem[MEM_SIZE / 2 - 1] == 0) {
        cgraInstance.executeCycle(count);
		// LOG_TXT(LOG_INFO,"count=%d\n",count);
        count++;
    }
#endif

    // 20 cycles for epilogue
    for (int i = 0; i < 20; i++) {
        cgraInstance.executeCycle(count);
		// LOG_TXT(LOG_INFO,"count=%d\n",count);
        count++;
    }
}

int CGRA::executeCycle(int kII) {
	int temp = MyStatics.numMiss;
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CGRATiles[y][x]->execute(kII);
		}
	}
	
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CGRATiles[y][x]->updateRegisters();
			CGRATiles[y][x]->updatePC();
		}
	}
	if(MyStatics.numMiss==temp){
		char *name = (char *)(malloc(40 * sizeof(char)));
		if(name==NULL){
			printf("utilization malloc malloc error \n");
		}
        sprintf(name,"../output/uti_%s",prefetch_allow?"pf":"no_pf");
        LOG_FILE(LOG_INFO,name,"%d\n",kII);
	}
	// printf("executeCycle finish\n");
	//  执行一次循环将进行一次间接访存分析和依次间接访存模式匹配
	// src2dest count等于40 可识别到间接访存模式kII%120==119
	// printf("IPD size=%d\n",IPDentrys.size());
	if(kII %10000==9999) {
		printInfo();
		printBlock();
	}

	if(kII%100==99 && IPDEnable){
		int num = IPDentrys.size();
		// printf("kII=%d,IPD entry size=%d,RWBuffer.size=%d ",kII,IPDentrys.size(),RWBuffers.size());
		if(num<3){//  间接访存模式较少   后面修改为SPM miss相关的数据
			printf("detect buffer size:%d\n",RWBuffers.size());
			Index_array_Detect();   // 检测Index数组
			Detect_IMA_SPVM();
			print_RWBuffers();
			print_IPD();
			LOG_IPDentry(LOG_INFO,"Detect IPD kII=%d,IPD analyse finised,IPD entry size=%d\n",kII,IPDentrys.size());
			// LOG_Buffer(LOG_INFO,"\n\n-------------------------------------one detect IMA cycle finished-----------------------------------------------\n\n");
		}
		// else if(num>=4){
		// 	printf("classify buffer size:%d\n",RWBuffers.size());
		// 	LOG_IPDentry(LOG_INFO,"valid IPD  kII=%d IPD entry size=%d,RWBUffer size=%d\n",kII,IPDentrys.size(),RWBuffers.size());
		// 	Index_array_Detect();
		// 	classify_array(); // 对应index target的clear
		// 	//kII传入的是当前的count  用于移除与当前count相差较远且hit次数少的pattern
		// 	valid_IPDEntry(index_array,target_addr,kII);
		// 	print_RWBuffers();
		// 	print_IPD();
		// 	LOG_IPDentry(LOG_INFO,"validate IPD kII=%d,validate finised,IPD entry size=%d\n",kII,IPDentrys.size());

		// 	if(index_array.size()>0){
		// 		index_array.clear();
		// 	}
		// 	if(target_addr.size()>0){
		// 		target_addr.clear();
		// 	}

		// }



		if(RWBuffers.size()!=0){
				RWBuffers.clear();
				bufferIdx = 0;
		}
		// printf("kII=%d,IPD entry size=%d \n",kII,IPDentrys.size());
	}
}
// 
void CGRA::dumpRawData(){
	printf("start dump raw data\n");
	std::ofstream myfile;
 	myfile.open ("dumped_raw_data.txt");
	int overall = dmem.size();
	for(int addr  = 0; addr < overall; addr++){
		myfile<< addr << "," <<(int)dmem_pre[addr] << "," << (int)dmem_post[addr] << "\n";
	}
	printf("dump raw data finished\n");
	 myfile.close();
}


void CGRA::printInterestedAddrOutcome() {
	LOG(SIMULATOR) << "Interested Addresses :: \n";
	for(int addr : InterestedAddrList){
		LOG(SIMULATOR) << addr << "," << (int)dmem[addr] << "\n";
	}
	int correct_count = 0;
	int wrong_count = 0;
	for(int addr : InterestedAddrList){
		if((int)dmem[addr] == (int)dmem_post[addr]){
			correct_count++;
		}else{
			wrong_count++;
			LOG_Missmatch(LOG_INFO,"Data mismatch at address: %d result:%d ,expected:%d\n",addr,(int)dmem[addr],(int)dmem_post[addr]);
			// printf("Data mismatch at address: %d result:%d ,expected:%d\n",addr,(int)dmem[addr],(int)dmem_post[addr]);
			LOG(SIMULATOR) << "Data mismatch at address: "<< addr << ", result:" << (int)dmem[addr]<<", expected:" << (int)dmem_post[addr] << "\n";
		}
	}
	std::cout << "Simulation Result: Matches::"<<correct_count<<", Mismatches::"<<wrong_count<< "\n";
	std::ofstream rsltfile;
	rsltfile.open ("sim_result.txt");
	rsltfile<<correct_count<<","<<wrong_count<< "\n";
	rsltfile.close();
}

XBarInput CGRA::convertStrtoXBI(std::string str) {

	LOG(SIMULATOR) << "convertStr called : " << std::stoi(str,nullptr,2) << "\n";
	// printf("stoi:%s\n",str.c_str());
	switch(std::stoi(str,nullptr,2)){
		case 0:
			return EAST_I;
		case 1:
			return SOUTH_I;
		case 2:
			return WEST_I;
		case 3:
			return NORTH_I;
		case 4:
			return ALU_I;
		case 5:
			return RES_I;
		case 7:
			return INV;
		default:
			LOG(SIMULATOR) << str;
			assert(false);
	}
}

} /* namespace HyCUBESim */