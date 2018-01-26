#include "SetFPSInstruction.h"


SetFPSInstruction::SetFPSInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far)
: InstructionUnit(port_local_send, port_local_recv, port_far_send, ip_local, ip_far)
{

}


SetFPSInstruction::~SetFPSInstruction()
{
}

void SetFPSInstruction::thrSend()
{
	for (int i = 0; i < 10;++i)
		sendCmdReg(200, m_frLength);//ÉèÖÃÖ¡ÂÊ
}