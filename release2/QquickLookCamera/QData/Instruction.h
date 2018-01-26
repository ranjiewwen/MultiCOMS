#pragma once
#include "initsock.h"
#include <thread>
#pragma pack (push)
#pragma pack (1)
struct CMD
{
	//16 byte + 8 byte
	unsigned int synWord{ 0 }; // 命令同步字 - 4 byte
	unsigned int dataLength{ 0 }; // 命令数据长度 - 4 byte
	unsigned short counter{ 0 }; // 命令计数器 - 2 byte
	unsigned short form{ 0 }; // 命令类型
	unsigned short timeStamp{ 0 }; // 时间戳
	unsigned short checkSum{ 0 }; // 校验和

	unsigned long long data{ 0 }; // 命令数据
};

#pragma pack (pop)
class Instruction
{
//构造函数&析构函数
public:
	Instruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~Instruction();
	void closeSock();

//数据
public:
	//枚举类型CMOSID
	enum CMOSID{ NoCMOS, CMOSE, CMOS1, CMOS2, CMOS3 };
private:
	CMOSID cmosId;
	CMD *cmd;
	bool SENDFLAG = false;

	unsigned int m_ag_cg{ 0x01e2 };//寄存器值
	unsigned int m_dg{ 0x0080 };//寄存器值
	int m_FPS{ 18 };
	unsigned int m_frLength{ 10000 };//帧长
	unsigned int m_frExpoTime{ 2000 };//曝光时间，小于帧长

	bool m_recving{ false }; //线程退出标识
	bool m_sendingAECRun{ false }; //线程退出标识
	bool m_sendingsetExpoTime{ false }; //线程退出标识
	bool m_sendingSetFPS{ false }; //线程退出标识
	bool m_sendingSetDG{ false };
	bool m_sendingSetAGCG{ false };

	std::thread m_thrSendSetFPS; //线程句柄
	std::thread m_thrRecv; //线程句柄
	std::thread m_thrSendsetExpoTime; //线程句柄
	std::thread m_thrSendAECRun; //线程句柄
	std::thread m_thrSendSetDG;
	std::thread m_thrSendSetAGCG;
//socket
protected:
	CInitSock c;//Winsock库的装入和释放
	SOCKET sock_send;
	SOCKET sock_recv;
	sockaddr_in addr_local_send;
	sockaddr_in addr_far_send;
	sockaddr_in addr_local_recv;
//
public:
	//
	bool startSendAECRun()
	{
		if (m_sendingAECRun)
			return false;
		m_sendingAECRun = true;
		m_thrSendAECRun = std::thread([this](){this->AECRun(); });
		return true;
	}
	void stopSendAECRun()
	{
		m_sendingAECRun = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendAECRun.joinable())
			m_thrSendAECRun.join();
	}
	//
	bool startSendsetExpoTime(unsigned int _expoTime)
	{
		m_frExpoTime = _expoTime;
		if (m_sendingsetExpoTime)
			return false;
		m_sendingsetExpoTime = true;
		m_thrSendsetExpoTime = std::thread([this](){this->ManualExposure(m_frExpoTime); });
		return true;
	}
	void stopSendsetExpoTime()
	{
		m_sendingsetExpoTime = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendsetExpoTime.joinable())
			m_thrSendsetExpoTime.join();
	}
	//
	bool startSendSetFPS(int _fps)
	{
		m_FPS = _fps;
		if (m_sendingSetFPS)
			return false;
		m_sendingSetFPS = true;
		m_thrSendSetFPS = std::thread([this](){this->SetFPS(m_FPS); });
		return true;
	}
	void stopSendSetFPS()
	{
		m_sendingSetFPS = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetFPS.joinable())
			m_thrSendSetFPS.join();
	}
	//
	bool startSendSetAGCG(int _total)
	{
		m_ag_cg = _total;
		if (m_sendingSetAGCG)
			return false;
		m_sendingSetAGCG = true;
		m_thrSendSetAGCG = std::thread([this](){this->SetAGCG(m_ag_cg); });
		return true;
	}
	void stopSendSetAGCG()
	{
		m_sendingSetAGCG = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetAGCG.joinable())
			m_thrSendSetAGCG.join();
	}
	//
	bool startSendSetDG(int _dg)
	{
		if (m_sendingSetDG)
			return false;
		m_sendingSetDG = true;
		m_thrSendSetDG = std::thread([this](){this->SetDG(m_dg); });
		return true;
	}
	void stopSendSetDG()
	{
		m_sendingSetDG = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrSendSetDG.joinable())
			m_thrSendSetDG.join();
	}
	//
	bool startRecv()
	{
		if (m_recving)
			return false;
		m_recving = true;
		m_thrRecv = std::thread([this](){this->receive(); });
		return true;
	}
	void stopRecv()
	{
		m_recving = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrRecv.joinable())
			m_thrRecv.join();
	}
//发送
private:
	unsigned short cmdCounter;//发送指令计数，初值为0
public:
	void SetCmosId(CMOSID id);
	void Stop()
	{
		PowerDown();
	}
protected:
	void ClearFIFO();
	//开始上传图像，自动曝光
	void AECRun();
	//手动曝光
	void ManualExposure(unsigned int _expoTime);
	void SetFPS(int _fps);
	void SetAGCG(float _gainTotal);
	void SetDG(float _dg);

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
//接收
public:
	bool DONEFLAG = false;
protected:
	void receive();
};