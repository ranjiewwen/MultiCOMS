#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_
#include "../QData/initsock.h"

//PC->FPGA的指令，控制相机参数改变 ；但是先要要个指令触发硬件发数据，然后InputCOMS开始接收数据。
class Command
{
public:
	Command(USHORT port_target, ULONG ip_target, USHORT port_local, ULONG ip_local);
	~Command();

	//自动曝光
	bool AutoRun();

	//手动曝光
	bool ManulRun();

	//停止CMOS Core工作
	bool stopCMOS();

	//设置Target Illumination
	//tai（0-1023）默认值为0xb8-------10进制输入
	bool setTAI(int tai = 0xb8);

	//设置曝光时间
	//et（0-2EE0）-------16进制输入
	bool setET(int et);

	//设置Digital Gain（DG）
	//dg为小数，整数部分为5位，小数部分为7位
	bool setDG(double dg);

	//设置AFE Gain和Column Gain（AG CG）
	//gain为0-10中的任意数，表示增益倍数
	//0--1x
	//1--1.14x
	//2--1.33x
	//3--1.60x
	//4--2.00x
	//5--2.29x
	//6--2.67x
	//7--3.20x
	//8--4.00x
	//9--5.33x
	//10--8.00x
	bool setAG_CG(int gain);

	//设置任意寄存器
	//addr：寄存器地址，9bit
	//data：16bit
	bool writeRegister(int addr, int data);


protected:
	CInitSock c;//Winsock库的装入和释放
	SOCKET sock;//接收图像数据套接字
	sockaddr_in addr_local;//本地地址
	sockaddr_in addr_target;//远程地址

private:
};

#pragma pack (push)
#pragma pack (1)
struct CMD
{
	//12 Bytes
	unsigned short frmhead{ 0xeb90 };//帧头
	unsigned short cmdCount{ 0x0000 };//PC指令计数
	unsigned int cmd;//指令
	unsigned int utility;//对齐
};

#pragma pack (pop)
#endif