#include "IDataProcessUnit.h"
#include "CirQueue.h"

//template<int MaxInputChannel, int MaxOutputChannel>
bool IDataProcessUnit/*<MaxInputChannel, MaxOutputChannel>*/::registerInputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize) {
	if (channel < 0 || channel >= MaxInputChannel)
		return false;
	m_inputBuffer[channel].first = unitSize;
	m_inputBuffer[channel].second = bufPtr;
	return true;
}

//template<int MaxInputChannel, int MaxOutputChannel>
bool IDataProcessUnit/*<MaxInputChannel, MaxOutputChannel>*/::registerOutputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize) {
	if (channel < 0 || channel >= MaxOutputChannel)
		return false;
	m_outputBuffer[channel].first = unitSize;
	m_outputBuffer[channel].second = bufPtr;
	return true;
}

////template<int MaxInputChannel, int MaxOutputChannel>
//bool IDataProcessUnit/*<MaxInputChannel, MaxOutputChannel>*/::resetInputUnitSize(int channel, int unitSize) {
//	if (channel < 0 || channel >= MaxInputChannel)
//		return false;
//	m_inputBuffer[channel].first = unitSize;
//	if (m_inputBuffer[channel].second) {
//		m_inputBuffer[channel].second->reset();
//	}
//	return true;
//}

////template<int MaxInputChannel, int MaxOutputChannel>
//bool IDataProcessUnit/*<MaxInputChannel, MaxOutputChannel>*/::resetOutputUnitSize(int channel, int unitSize) {
//	if (channel < 0 || channel >= MaxOutputChannel)
//		return false;
//	m_outputBuffer[channel].first = unitSize;
//	if (m_outputBuffer[channel].second) {
//		m_outputBuffer[channel].second->reset();
//	}
//	return true;
//}
