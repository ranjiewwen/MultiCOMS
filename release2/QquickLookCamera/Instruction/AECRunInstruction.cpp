#include "AECRunInstruction.h"


AECRunInstruction::AECRunInstruction(USHORT port_local_send, USHORT port_local_recv, USHORT port_far_send, char* ip_local, char* ip_far)
:InstructionUnit(port_local_send, port_local_recv, port_far_send, ip_local, ip_far)
{
	lastSendData = 0x800000C000000001;
}


AECRunInstruction::~AECRunInstruction()
{
}

void AECRunInstruction::thrSend()
{
	PowerUp();
	EnableClockManagement1();
	EnableClockManagement2();
	RequiredRegisterUpload();
	ExtraConfig();
	SoftPowerUp();
	sendCmdReg(160, 0x0011); //Enable AEC
	EnableSequencer();
}
