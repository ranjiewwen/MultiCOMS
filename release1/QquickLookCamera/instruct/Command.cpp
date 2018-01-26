#include "Command.h"
#include <iostream>
Command::Command(USHORT port_target, ULONG ip_target, USHORT port_local=htons(4567), ULONG ip_local=inet_addr("192.168.1.1"))
{
	sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//填写本地地址信息
	addr_local.sin_family = AF_INET;
	addr_local.sin_addr.S_un.S_addr = ip_local;
	addr_local.sin_port = htons(port_local);

	//填写远程地址信息
	addr_target.sin_family = AF_INET;
	addr_target.sin_addr.S_un.S_addr = ip_target;
	addr_target.sin_port = htons(port_target);

	//将套接字与对应地址绑定
	if (::bind(sock, (LPSOCKADDR)&addr_local, sizeof(addr_local)) == SOCKET_ERROR)
	{
		//printf("failed bind()...\n");
		//return;
	}
}

Command::~Command()
{
	::closesocket(sock);
}

//EB 90	00 00	             XX XX XX XX           	00 00 00 00
//帧头	（实为PC指令计数）	 指令
bool Command::AutoRun()
{
	CMD* cmdhead=new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x02000000);//指令:0200 0000
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::ManulRun()
{
	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x04000000);//指令:0400 0000
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::stopCMOS()
{
	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x0e000000);//指令:0E00 0000
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::setTAI(int tai)
{
	if (tai < 0 || tai > 1023)
	{
		return false;
	}

	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x06000000) + htonl(tai);//指令:0600 00B8（0000-1023）
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::setET(int et)
{
	if (et < 0 || et > 0x2ee0)
	{
		return false;
	}

	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x0C000000) + htonl(et);//指令:0C00 (0000-2EE0)
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::setDG(double dg)
{
	int i = dg;
	int d = (dg - i) * 100;
	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	//int a = 0x0A000000 + i * 128 + d; 验证指令，不能直接看cmd
	cmdhead->cmd = htonl(0x0A000000) + htonl(i * 128) + htonl(d);//指令:0A00 0 (5bit整数.7bit小数)
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::setAG_CG(int gain)
{
	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);

	//固定的11种指令
	switch (gain)
	{
	case  0:cmdhead->cmd = htonl(0x080001E2); break;//1x
	case  1:cmdhead->cmd = htonl(0x080000E2); break;//1.14x
	case  2:cmdhead->cmd = htonl(0x08000062); break;//1.33x
	case  3:cmdhead->cmd = htonl(0x080000A2); break;//1.60x
	case  4:cmdhead->cmd = htonl(0x08000022); break;//2.00x
	case  5:cmdhead->cmd = htonl(0x080000E1); break;//2.29x
	case  6:cmdhead->cmd = htonl(0x08000061); break;//2.67x
	case  7:cmdhead->cmd = htonl(0x080000A1); break;//3.20x
	case  8:cmdhead->cmd = htonl(0x08000021); break;//4.00x
	case  9:cmdhead->cmd = htonl(0x080000C1); break;//5.33x
	case 10:cmdhead->cmd = htonl(0x08000041); break;//8.00x
	default:break;
	}

	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}

bool Command::writeRegister(int addr, int data)
{
	addr *= 65536;//左移16位
	CMD* cmdhead = new CMD;

	cmdhead->frmhead = htons(0xeb90);
	cmdhead->cmdCount = htons(0x0000);
	cmdhead->cmd = htonl(0x10000000) + htonl(addr) + htonl(data);//指令:（2进制）0001000 （9bit addr）（16bit data）
	cmdhead->utility = htonl(0x0000);

	unsigned char* buf = (unsigned char*)cmdhead;
	::sendto(sock, (char*)buf, sizeof(CMD), 0, (sockaddr*)&addr_target, sizeof(addr_target));

	return true;
}