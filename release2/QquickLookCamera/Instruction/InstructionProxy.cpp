#include "InstructionProxy.h"
#include "AECRunInstruction.h"
#include "SetAgCgInstruction.h"
#include "SetDgInstruction.h"
#include "SetExpoTimeInstruction.h"
#include "SetFPSInstruction.h"
#include "StopInstruction.h"


InstructionProxy::InstructionProxy(InstructionUnit::CMOSID _cmosId) :
	m_cmosId(_cmosId),
	port_local_send(3954),
	port_far_send(4567),
	port_local_recv(3955),
	ip_local("192.168.1.2"),
	ip_far("192.168.1.1")
{
}


InstructionProxy::~InstructionProxy()
{
}

bool InstructionProxy::AECRun()
{
	AECRunInstruction* instruct = new AECRunInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProxy::SetFPS(int _fps)
{
	SetFPSInstruction *instruct{ new SetFPSInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far) };
	instruct->SetCmosId(m_cmosId);
	instruct->setFPS(_fps);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	//delete instruct;
	return true;
}

bool InstructionProxy::setExpoTime(unsigned int _expoTime)
{
	SetExpoTimeInstruction *instruct = new SetExpoTimeInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->setExpoTime(_expoTime);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	delete instruct;
	return true;
}

bool InstructionProxy::Stop()
{
	StopInstruction* instruct = new StopInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	delete instruct;
	return true;
}

bool InstructionProxy::SetAGCG(float _total)
{
	SetAgCgInstruction* instruct = new SetAgCgInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->setTotalGain(_total);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	delete instruct;
	return true;
}

bool InstructionProxy::SetDG(float _dg)
{
	SetDgInstruction* instruct = new SetDgInstruction(port_local_send, port_local_recv, port_far_send, ip_local, ip_far);
	instruct->SetCmosId(m_cmosId);
	instruct->setDg(_dg);
	instruct->start();

	int i = 0;
	while (!instruct->isDone())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (i >= 40)
		{
			instruct->closeSock();
			//delete instruct;
			return false;
		}
		i++;

	}
	instruct->closeSock();
	delete instruct;
	return true;
}