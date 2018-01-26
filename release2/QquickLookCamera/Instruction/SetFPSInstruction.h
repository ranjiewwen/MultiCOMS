#pragma once
#include "InstructionUnit.h"
class SetFPSInstruction :
	public InstructionUnit
{
public:
	SetFPSInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~SetFPSInstruction();
	void setFPS(int _fps)
	{
		m_frLength = 180000 / _fps;
		lastSendData = 0x8000000000000000;
		lastSendData |= 200 * 4294967296;//_addr * 4294967296
		lastSendData |= m_frLength;//data
	}
private:
	unsigned int m_frLength;
protected:
	virtual void thrSend() override;
};

