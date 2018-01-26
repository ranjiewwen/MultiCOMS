#include "InstructionProcess.h"


InstructionProcess::InstructionProcess(Instruction::CMOSID _cmosId) :
	m_cmosId(_cmosId),
	port_local_send(3954),
	port_far_send(4567),
	port_local_recv(39550),
	ip_local("192.168.1.2"),
	ip_far("192.168.1.1")
{
}


InstructionProcess::~InstructionProcess()
{
}

bool InstructionProcess::AECRun()
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->startRecv();
	instruct->startSendAECRun();

	int i = 0;
	while (!instruct->DONEFLAG)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProcess::SetFPS(int _fps)
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->startSendSetFPS(_fps);
	instruct->startRecv();	

	int i = 0;
	while (!instruct->DONEFLAG)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProcess::setExpoTime(unsigned int _expoTime)
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->startSendsetExpoTime(_expoTime);
	instruct->startRecv();

	int i = 0;
	while (!instruct->DONEFLAG)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProcess::Stop()
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	for (int i = 0; i < 10; ++i)
		instruct->Stop();

	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProcess::SetAGCG(float _total)
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->startSendSetAGCG(_total);
	instruct->startRecv();
	
	int i = 0;
	while (!instruct->DONEFLAG)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProcess::SetDG(float _dg)
{
	instruct = new Instruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->startSendSetDG(_dg);
	instruct->startRecv();
	
	int i = 0;
	while (!instruct->DONEFLAG)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}