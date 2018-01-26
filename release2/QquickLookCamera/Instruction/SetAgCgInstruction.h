#pragma once
#include "InstructionUnit.h"
class SetAgCgInstruction :
	public InstructionUnit
{
public:
	SetAgCgInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~SetAgCgInstruction();
	void setTotalGain(float _gainTotal)
	{
		if (fabs(_gainTotal - 1.00) < 0.1)
			m_ag_cg = 0x01E2;
		else
		if (fabs(_gainTotal - 1.14) < 0.1)
			m_ag_cg = 0x00E2;
		else
		if (fabs(_gainTotal - 1.33) < 0.1)
			m_ag_cg = 0x0062;
		else
		if (fabs(_gainTotal - 1.60) < 0.1)
			m_ag_cg = 0x00A2;
		else
		if (fabs(_gainTotal - 2.00) < 0.1)
			m_ag_cg = 0x0022;
		else
		if (fabs(_gainTotal - 2.29) < 0.1)
			m_ag_cg = 0x00E1;
		else
		if (fabs(_gainTotal - 2.67) < 0.1)
			m_ag_cg = 0x0061;
		else
		if (fabs(_gainTotal - 3.20) < 0.1)
			m_ag_cg = 0x00A1;
		else
		if (fabs(_gainTotal - 4.00) < 0.1)
			m_ag_cg = 0x0021;
		else
		if (fabs(_gainTotal - 5.33) < 0.1)
			m_ag_cg = 0x00C1;
		else
		if (fabs(_gainTotal - 8.00) < 0.1)
			m_ag_cg = 0x0041;
		lastSendData = 0x8000000000000000;
		lastSendData |= 204 * 4294967296;//_addr * 4294967296
		lastSendData |= m_ag_cg;//data
	}
private:
	unsigned int m_ag_cg;
protected:
	virtual void thrSend() override;
};

