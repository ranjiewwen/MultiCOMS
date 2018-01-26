#ifndef _INPUT_CMOS_HPP_
#define _INPUT_CMOS_HPP_
#include "IDataProcessUnit.h"
#include "initsock.h"
#include <fstream>

#define NUMOFRECVBUF 1300 //recvbuf的大小 ；函数process()用到

class InputCMOS : public IDataProcessUnit
{
protected:
	CInitSock c;//Winsock库的装入和释放
	SOCKET sock;//接收图像数据套接字
	fd_set fdsock;//套接字集合
	SOCKADDR_IN addr;//PC端地址
	
public:
	InputCMOS(USHORT port_pc, ULONG ip_pc);
	
	~InputCMOS();
	
protected:
	// 数据处理过程
	virtual void process() override;
};
#endif
