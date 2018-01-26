#pragma once
#include "InstructionUnit.h"
class SetDgInstruction :
	public InstructionUnit
{
public:
	SetDgInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~SetDgInstruction();
	//void setDg(float _dg)
	//{
	//	m_dg = (int)_dg * 128;
	//	_dg = _dg - (int)_dg;
	//	if (_dg >= 0.5)
	//	{
	//		m_dg += 64;
	//		_dg -= 0.5;
	//	}
	//	if (_dg >= 0.25)
	//	{
	//		m_dg += 32;
	//		_dg -= 0.25;
	//	}
	//	if (_dg >= 0.125)
	//	{
	//		m_dg += 16;
	//		_dg -= 0.125;
	//	}
	//	if (_dg >= 0.0625)
	//	{
	//		m_dg += 8;
	//		_dg -= 0.0625;
	//	}
	//	if (_dg >= 0.03125)
	//	{
	//		m_dg += 4;
	//		_dg -= 0.03125;
	//	}
	//	if (_dg >= 0.015625)
	//	{
	//		m_dg += 2;
	//		_dg -= 0.015625;
	//	}
	//	if (_dg >= 0.0078125)
	//	{
	//		m_dg += 1;
	//		_dg -= 0.0078125;
	//	}
	//}
	void setDg(float _dg)
	{
		m_dg = (int)_dg * 128 + (_dg - (int)_dg) * 100;
		lastSendData = 0x8000000000000000;
		lastSendData |= 205 * 4294967296;//_addr * 4294967296
		lastSendData |= m_dg;//data
	}
private:
	unsigned int m_dg;
protected:
	virtual void thrSend() override;
};

