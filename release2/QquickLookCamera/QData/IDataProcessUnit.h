#ifndef _I_DATA_PROCESS_UNIT_HPP_
#define _I_DATA_PROCESS_UNIT_HPP_

#include <thread>
#include <memory>
#include "CirQueue.h"
class SaveProxy;
class QString;

//使用模板是因为无法为数组类型的变量指定数组大小，vector和map之类的变量有性能损失
//template<int MaxInputChannel, int MaxOutputChannel>
#define MaxInputChannel 8
#define MaxOutputChannel 128
class IDataProcessUnit
{
protected:
	//描述缓冲区单位长度，标识以多少字节为一组来处理数据，有些处理单元不需要使用此数值
	//一般只有输入缓冲区需要了解长度信息
	typedef std::pair<int, std::shared_ptr<IBuffer>> IntBufferPair; 
	
	IntBufferPair m_inputBuffer[MaxInputChannel]; //输入缓冲区
	IntBufferPair m_outputBuffer[MaxOutputChannel]; //输出缓冲区
	bool m_discardable{ false }; //在数据出来来不及的时候是否允许丢弃数据，丢新的还是旧的自行处理
	bool m_processing{ false }; //线程退出标识
	std::thread m_thrProcess; //线程句柄
protected:
	IDataProcessUnit(){
		for (int i = 0; i < MaxInputChannel; ++i) {
			m_inputBuffer[i].first = -1;
			m_inputBuffer[i].second = nullptr;
		}
	}

public:
	//数据处理流程在此
	virtual void process() {
	}

public:
	virtual ~IDataProcessUnit() { 
		stop(); 
	}
	int maxInputChannel() const { return MaxInputChannel; }
	int maxOutputChannel() const { return MaxOutputChannel; }



	//注册输入缓冲区
	virtual bool registerInputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize = -1);
	//注册输出缓冲区
	virtual bool registerOutputBuffer(int channel, std::shared_ptr<IBuffer>& bufPtr, int unitSize = -1);
	////重设输入缓冲区数据单元大小(字节数)
	//virtual bool resetInputUnitSize(int channel, int unitSize);
	////重设输出缓冲区数据单元大小(字节数)
	//virtual bool resetOutputUnitSize(int channel, int unitSize);

	//virtual bool saveToFile(SaveProxy& /*saveProxy*/, const QString& /*savePath*/) { return false; }
	//virtual void stopSaving(){}

public:
	//启动
	virtual bool start() {
		if (m_processing)
			return false;
		m_processing = true;
		m_thrProcess = std::thread([this](){this->process(); });
		return true;
	}
	//停止
	virtual void stop() {
		m_processing = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (m_thrProcess.joinable())
			m_thrProcess.join();
		//复位操作
		//reset();
	}
};

#endif