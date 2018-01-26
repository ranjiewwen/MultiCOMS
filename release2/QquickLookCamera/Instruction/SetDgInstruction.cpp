#include "SetDgInstruction.h"


SetDgInstruction::SetDgInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far)
: InstructionUnit(port_local_send, port_local_recv, port_far_send, ip_local, ip_far)
{
}


SetDgInstruction::~SetDgInstruction()
{
}

void SetDgInstruction::thrSend()
{
	sendCmdReg(160, 0x0010);//设为手动曝光
	sendCmdReg(161, 0x60B8);
	sendCmdReg(205, m_dg);//DG
}