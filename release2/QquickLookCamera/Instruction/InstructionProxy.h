#pragma once
#include "InstructionUnit.h"
class InstructionProxy
{
public:
	InstructionProxy(InstructionUnit::CMOSID _cmosId);
	~InstructionProxy();
	bool AECRun();
	bool setExpoTime(unsigned int _expoTime);
	bool SetFPS(int _fps);
	bool Stop();
	bool SetAGCG(float _total);
	bool SetDG(float _dg);
	//设置：发送数据时——本地端口
	void setPortLocalSend(USHORT tmp){ port_local_send = tmp; }
	//设置：接收数据时——本地端口
	void setPortLocalRecv(USHORT tmp){ port_local_recv = tmp; }
	//设置：发送数据时——目标端口
	void setPortFarSend(USHORT tmp){ port_far_send = tmp; }
	//设置本地IP
	void setIPLocal(char *tmp){ ip_local=tmp; }
	//设置目标IP
	void setIPFar(char *tmp){ ip_far=tmp; }
private:
	USHORT port_local_send;
	USHORT port_local_recv;
	USHORT port_far_send;
	char* ip_local;
	char* ip_far;
	InstructionUnit::CMOSID m_cmosId;
};

