#pragma once
#include "InstructionUnit.h"

class AECRunInstruction : public InstructionUnit
{
public:
	AECRunInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far);
	~AECRunInstruction();
protected:
	virtual void thrSend() override;
};

