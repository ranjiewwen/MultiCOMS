#pragma once
#include "QData/initsock.h"
#include <thread>
#pragma pack (push)
#pragma pack (1)
struct CMD
{
	//16 byte + 8 byte
	unsigned int synWord{ 0 }; // 命令同步字 - 4 byte
	unsigned int dataLength{ 2 }; // 命令数据长度 - 4 byte
	unsigned short counter{ 0 }; // 命令计数器 - 2 byte
	unsigned short form{ 0 }; // 命令类型
	unsigned short timeStamp{ 0 }; // 时间戳
	unsigned short checkSum{ 0 }; // 校验和

	unsigned long long data{ 0 }; // 命令数据
};

#pragma pack (pop)
class InstructionUnit
{
//构造函数&析构函数
public:
	InstructionUnit(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~InstructionUnit();

//数据
public:
	//枚举类型CMOSID
	enum CMOSID{ NoCMOS, CMOSE, CMOS1, CMOS2, CMOS3 };

protected:
	CMOSID cmosId;
	CMD *cmd;
	unsigned short cmdCounter;//发送指令计数，初值为0
	unsigned long long lastSendData{0x0000000000000000};
	bool DONEFLAG = false;
	bool SENDFLAG = false;
	bool m_recving{ false }; //线程退出标识
	bool m_sending{ false }; //线程退出标识

	std::thread m_thrSend; //线程句柄
	std::thread m_thrRecv; //线程句柄

	CInitSock c;//Winsock库的装入和释放
	SOCKET sock_send;
	SOCKET sock_recv;
	sockaddr_in addr_local_send;
	sockaddr_in addr_far_send;
	sockaddr_in addr_local_recv;

public:
	void SetCmosId(CMOSID id)
	{
		cmosId = id;
	}
	void closeSock()
	{
		::closesocket(sock_send);
		::closesocket(sock_recv);
	}
	bool isDone()
	{
		return DONEFLAG;
	}
	bool start()
	{
		if (!startRecv())
			return false;
		if (!startSend())
			return false;
		return true;
	}
	bool stop()
	{
		stopSend();
		stopRecv();
	}
protected:
	bool startSend()
	{
		if (m_sending)
			return false;
		m_sending = true;
		m_thrSend = std::thread([this](){this->thrSend(); });
		return true;
	}
	void stopSend()
	{
		m_sending = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSend.joinable())
			m_thrSend.join();
	}
	bool startRecv()
	{
		if (m_recving)
			return false;
		m_recving = true;
		m_thrRecv = std::thread([this](){this->thrReceive(); });
		return true;
	}
	void stopRecv()
	{
		m_recving = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrRecv.joinable())
			m_thrRecv.join();
	}

protected:
	virtual void thrReceive();
	virtual void thrSend() = 0;
protected:
	void ClearFIFO();

	bool buildCmdReg(int _addr, int _data);
	void sendCmdReg(int _addr, int _data);
	void PowerUp();
	void EnableClockManagement1();
	void EnableClockManagement2();
	void RequiredRegisterUpload();
	void ExtraConfig();
	void SoftPowerUp();
	void EnableSequencer();

	void DisableSequencer();
	void SoftPowerDown();
	void DisableClockManagement2();
	void DisableClockManagement1();
	void PowerDown();
};