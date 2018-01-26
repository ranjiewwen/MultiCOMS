#include "InstructionUnit.h"
#include "semaphore.h"
#include <QDebug>
InstructionUnit::InstructionUnit(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far)
{
	cmd = new CMD;
	cmdCounter = 0;

	//用于接收的socket
	addr_local_recv.sin_family = AF_INET;
	addr_local_recv.sin_addr.S_un.S_addr = inet_addr(ip_local);
	addr_local_recv.sin_port = htons(port_local_recv);
	sock_recv = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ULONG ulr = 1;
	::ioctlsocket(sock_recv, FIONBIO, (ULONG*)&ulr);//设为非阻塞模式
	if (::bind(sock_recv, (LPSOCKADDR)&addr_local_recv, sizeof(addr_local_recv)) == SOCKET_ERROR)
	{
		qDebug() << "Recv绑定失败\n";
	}

	//用于发送的socket
	addr_local_send.sin_family = AF_INET;
	addr_local_send.sin_addr.S_un.S_addr = inet_addr(ip_local);
	addr_local_send.sin_port = htons(port_local_send);
	addr_far_send.sin_family = AF_INET;
	addr_far_send.sin_addr.S_un.S_addr = inet_addr(ip_far);
	addr_far_send.sin_port = htons(port_far_send);
	sock_send = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ULONG uls = 1;
	::ioctlsocket(sock_send, FIONBIO, (ULONG*)&uls);//设为非阻塞模式
	if (::bind(sock_send, (LPSOCKADDR)&addr_local_send, sizeof(addr_local_send)) == SOCKET_ERROR)
	{
		qDebug() << "Send绑定失败\n";
	}
}

InstructionUnit::~InstructionUnit()
{
	stopSend();
	stopRecv();
	::closesocket(sock_send);
	::closesocket(sock_recv);
}

bool InstructionUnit::buildCmdReg(int _addr, int _data)
{
	if (cmosId == NoCMOS)
		return false;
	cmd->synWord = htonl(0x03CCF0FF);
	cmd->dataLength = htonl(0x00000002);
	cmd->counter = htons(cmdCounter);
	switch (cmosId)
	{
	case CMOSE:
		cmd->form = htons(0x0300);
		break;
	case CMOS1:
		cmd->form = htons(0x0400);
		break;
	case CMOS2:
		cmd->form = htons(0x0500);
		break;
	case CMOS3:
		cmd->form = htons(0x0600);
		break;
	default:
		break;
	}
	cmd->timeStamp = htons(0x0000);
	cmd->checkSum = htons(0x0000);
	unsigned long long cmdData{ 0x8000000000000000 };
	unsigned long long a = _addr * 4294967296;
	cmdData |= a;
	cmdData |= _data;
	cmd->data = htonll(cmdData);
	return true;
}

void InstructionUnit::sendCmdReg(int _addr, int _data)
{
	//if (m_sending)
	{
		if (SENDFLAG == true) 
			sendSemaphore.wait();
		SENDFLAG = true;

		if (!buildCmdReg(_addr, _data))
		{
			qDebug() << "——————建立指令失败！——————\n";
		}
		::sendto(sock_send, (char*)cmd, sizeof(CMD), 0, (sockaddr*)&addr_far_send, sizeof(addr_far_send));
		cmdCounter++;
	}
}

void InstructionUnit::ClearFIFO()
{
	//if (m_sending)
	{
		if (SENDFLAG == true) 
			sendSemaphore.wait();
		SENDFLAG = true;

		cmd->synWord = htonl(0x03CCF0FF);
		cmd->dataLength = htonl(0x00000002);
		cmd->counter = htons(cmdCounter);
		cmd->form = htons(0x0B00);
		cmd->timeStamp = htons(0x0000);
		cmd->checkSum = htons(0x0000);
		switch (cmosId)
		{
		case NoCMOS:
			break;
		case CMOSE:
			cmd->data = htonll(0x0000000800000000);
			break;
		case CMOS1:
			cmd->data = htonll(0x0000000400000000);
			break;
		case CMOS2:
			cmd->data = htonll(0x0000000200000000);
			break;
		case CMOS3:
			cmd->data = htonll(0x0000000100000000);
			break;
		default:
			break;
		}

		::sendto(sock_send, (char*)cmd, sizeof(CMD), 0, (sockaddr*)&addr_far_send, sizeof(addr_far_send));
		cmdCounter++;
	}
}


void InstructionUnit::PowerDown()
{
	//if (m_sending)
	{
		if (SENDFLAG == true) 
			sendSemaphore.wait();
		SENDFLAG = true;

		cmd->synWord = htonl(0x03CCF0FF);
		cmd->dataLength = htonl(0x00000002);
		cmd->counter = htons(cmdCounter);
		cmd->form = htons(0x0200);
		cmd->timeStamp = htons(0x0000);
		cmd->checkSum = htons(0x0000);
		cmd->data = htonll(0x0000000200000000);

		::sendto(sock_send, (char*)cmd, sizeof(CMD), 0, (sockaddr*)&addr_far_send, sizeof(addr_far_send));
		cmdCounter++;
	}
}

void InstructionUnit::PowerUp()
{
	//if (m_sending)
	{
		if (SENDFLAG == true)
			sendSemaphore.wait();
		SENDFLAG = true;

		cmd->synWord = htonl(0x03CCF0FF);
		cmd->dataLength = htonl(0x00000002);
		cmd->counter = htons(cmdCounter);
		cmd->form = htons(0x0200);
		cmd->timeStamp = htons(0x0000);
		cmd->checkSum = htons(0x0000);
		cmd->data = htonll(0x0000000100000000);

		for (int i = 0; i < 10; ++i)
			::sendto(sock_send, (char*)cmd, sizeof(CMD), 0, (sockaddr*)&addr_far_send, sizeof(addr_far_send));
		cmdCounter++;
	}
}

void InstructionUnit::EnableClockManagement1()
{
	//Part1
	if (cmosId == CMOSE)
	{
		sendCmdReg(2, 0x0003);//Color sensor parallel mode selection
	}
	else
	{
		sendCmdReg(2, 0x0002);//Monochrome sensor parallel mode selection
	}
	sendCmdReg(32, 0x200C);//Configure clock management
	sendCmdReg(20, 0x0000);//Configure clock management
	sendCmdReg(16, 0x0007);//Configure PLL bypass mode
}

void InstructionUnit::EnableClockManagement2()
{
	//Part2
	sendCmdReg(9, 0x0000);//Release clock generator soft reset
	sendCmdReg(32, 0x200E);//Enable logic clock
	sendCmdReg(34, 0x0001);//Enable logic blocks
}

void InstructionUnit::RequiredRegisterUpload()
{
	sendCmdReg(41, 0x085A);
	sendCmdReg(129, 0x0 << 13 | 0x0001);//129[13]-0x0
	sendCmdReg(65, 0x288B);//
	sendCmdReg(66, 0x53C5);//
	sendCmdReg(67, 0x0344);//
	sendCmdReg(68, 0x0085);//
	sendCmdReg(70, 0x4800);//
	sendCmdReg(128, 0x4710);//
	sendCmdReg(197, 0x0103);//
	sendCmdReg(176, 0x00F5);//
	sendCmdReg(180, 0x00FD);//
	sendCmdReg(181, 0x0144);
}

void InstructionUnit::SoftPowerUp()
{
	sendCmdReg(32, 0x200F);
	sendCmdReg(10, 0x0000);
	sendCmdReg(64, 0x0001);
	sendCmdReg(72, 0x0203);
	sendCmdReg(40, 0x0003);
	sendCmdReg(48, 0x0001);
	sendCmdReg(112, 0x0000);
}
void InstructionUnit::ExtraConfig()
{
	sendCmdReg(130, 0x0001);
	sendCmdReg(129, 0x0001);
	sendCmdReg(194, 0x00e4);
	sendCmdReg(199, 100);
	sendCmdReg(200, 10000);//设置帧率为18FPS,则帧长为10000
						   //15FPS则为12000
						   //10FPS则为18000
}
void InstructionUnit::EnableSequencer()
{
	//ClearFIFO();
	sendCmdReg(192, 0x1 << 0);//192[0]-0x1
}
void InstructionUnit::DisableSequencer()
{
	sendCmdReg(192, 0x0 << 0);//192[0]-0x1
	//ClearFIFO();
}
void InstructionUnit::SoftPowerDown()
{
	sendCmdReg(112, 0x0000);
	sendCmdReg(48, 0x0000);
	sendCmdReg(40, 0x0000);
	sendCmdReg(72, 0x0200);
	sendCmdReg(64, 0x0000);
	sendCmdReg(10, 0x0999);
}
void InstructionUnit::DisableClockManagement2()
{
	//Part2
	sendCmdReg(34, 0x0000);//Disable logic blocks
	sendCmdReg(32, 0x200C);//Disable logic clock
	sendCmdReg(9, 0x0009);//Soft reset clock generator
}
void InstructionUnit::DisableClockManagement1()
{
	//Part1
	sendCmdReg(16, 0x0000);//Disable PLL
	sendCmdReg(8, 0x0099);//Soft reset PLL
	sendCmdReg(20, 0x0000);//Configure clock management
}

void InstructionUnit::thrReceive()
{
	CMD* cmdRecv = new CMD; //接收指令数据
	sockaddr_in addr;
	int alen = sizeof(addr);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;

	while (m_recving)
	{
		//recvSemaphore.wait();
		//select模型
		//添加套接字句柄到套接字集合
		fd_set fdsock;//套接字集合
		FD_ZERO(&fdsock); // 先清空
		FD_SET(sock_recv, &fdsock);
		int nret = ::select(0, &fdsock, NULL, NULL, &timeout);//select()返回值：有未决I/O的套接字句柄的个数
		if (nret == SOCKET_ERROR)
		{
			//printf("failed select()...\n");
			continue;
		}
		if (nret == 0)//超时
		{
			//qDebug() << ("overtime...\n");
			if (cmd->synWord != 0 && DONEFLAG==false)
			{
				cmd->counter = htons(cmdCounter);
				::sendto(sock_send, (char*)cmd, sizeof(CMD), 0, (sockaddr*)&addr_far_send, sizeof(addr_far_send));
				cmdCounter++;
			}
			continue;
		}

		if (nret > 0)
		{
			if (FD_ISSET(sock_recv, &fdsock))//若该套接字可读
			{
				int nrecv = ::recvfrom(sock_recv, (char*)cmdRecv, sizeof(CMD), 0, (LPSOCKADDR)&addr, &alen);
				if (nrecv == 24)
				if (cmdRecv->synWord == htonl(0x05ccf0ff))
				if ((cmdRecv->data & 0x0111111111111111) == (cmd->data & 0x0111111111111111))
				{
					qDebug() << ("\n收到%d个字节\n", nrecv);
					if (cmd->data == htonll(lastSendData))
					{
						qDebug() << "成功\n";
						DONEFLAG = true;
						break;
					}
					sendSemaphore.signal();
				}
			}
		}
	}
}