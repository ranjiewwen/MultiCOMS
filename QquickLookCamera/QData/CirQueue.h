#ifndef CCIRQUEUE_H
#define CCIRQUEUE_H

#include <memory>
#include <atomic>

#include "IBuffer.h"

class CCirQueue : public IBuffer
{
    //基本操作
public:
    ///
    /// \brief push
    /// 数据入队,内存拷贝
    /// \param _buf,输入数据
    /// \param _size,入队数据量
    /// \return, 0=success
    ///
    int push_back(const unsigned char* _buf, int _size);

    ///
    /// \brief Pop
    /// 出队
    /// \param _len,出队数据量
    /// \return
    ///
    int pop_front(int _len);

    ///
    /// \brief Front
    /// 取队首的数据，返回内部数据指针
    /// \param _buf,返回地址
    /// \param _size,提取数据量
    /// \return,0=success
    ///
    int front(unsigned char*& _buf, int _size);

    ///
    /// \brief ValidSize
    /// 返回队列中有效数据量
    /// \return
    ///
    long long validSize();

    ///
    /// \brief FreeSize
    /// 返回队列中剩余数据量
    /// \return
    ///
    long long freeSize();

	long long totalSize() { return m_length; }

    void reset() {
        m_incount  = 0;
        m_outcount = 0;
        m_validNum = 0;
        m_curBlockPoint = 0;
    }

    //扩展操作
public:
    CCirQueue();
    ~CCirQueue();

    ///
    /// \brief Initial
    /// 初始化，分配内存，重置状态
    /// \param _total,队列容量
    /// \param _outMaxSize,最大出队数据量
    /// \return, 0=success, others=failed
    ///
    int Initial(long long _total, int _outMaxSize);

    ///
    /// \brief Memset
    /// \param _val
    /// \return
    ///
    int Memset(char _val=0);

    ///
    /// \brief GetInCount
    /// \return,返回入队数据量
    ///
    long long GetInCount() {return m_incount;}

    ///
    /// \brief GetOutCount
    /// \return,返回出队数据量
    ///
    long long GetOutCount() {return m_outcount;}

    ///
    /// \brief QueueSize
    /// \return,返回队列大小
    ///
    long long QueueSize() {return m_length;}

    ///
    /// \brief GetMaxUnitSize
    /// \return,返回最大出队数据量
    ///
    int GetMaxUnitSize() {return m_unitmaxsize;}

    ///
    /// \brief GetBlockPointer
    /// 一次取出一大块数据，不做出队操作,一次取出的数据量需要能被队列容量整除
    /// \param _size,取出的数据量，需要能被队列容量整除
    /// \param _nRet,返回对视的数据量
    /// \return,若参数无误且有足够的数据，返回数据指针,否则返回nullptr
    ///
    unsigned char* GetBlockPointer(int _size, long long &_nRet);

private:
    std::shared_ptr<unsigned char>	m_dataBuf{nullptr};	//队列总缓存数据区
    std::shared_ptr<unsigned char>	m_tmpBuf{nullptr};	//临时缓存Front函数用
    long long	m_length{0};	//内存队列长度
    int	m_unitmaxsize{0};	//front函数返回内存块最大长度
    std::atomic<long long>	m_validNum{0};	//队列中有效数据个数
    std::atomic<long long>	m_incount{0};	//总入队字节数
    std::atomic<long long>	m_outcount{0};	//总出队字节数
    std::atomic<long long>	m_curBlockPoint{0};	//当前数据缓存获取位置
};

#endif // CCIRQUEUE_H
