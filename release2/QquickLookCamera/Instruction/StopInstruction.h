#pragma once
#include "InstructionUnit.h"
class StopInstruction :
	public InstructionUnit
{
public:
	StopInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~StopInstruction();
protected:
	virtual void thrSend() override;
};

