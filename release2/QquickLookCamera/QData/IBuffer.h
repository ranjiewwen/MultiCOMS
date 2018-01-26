#ifndef _I_BUFFER_H_
#define _I_BUFFER_H_

//缓冲区接口抽象类

class IBuffer
{
public:
	IBuffer();
	virtual ~IBuffer();

	//fifo:
	virtual int push_back(const unsigned char* buf, int validSize) = 0;
	virtual int pop_front(int popSize) = 0;
	virtual int front(unsigned char*& ptr, int frontSize) = 0;
	virtual void reset() = 0;

};

#endif