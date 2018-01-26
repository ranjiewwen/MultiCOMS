#include "CirQueue.h"
//#include <QDebug>

CCirQueue::CCirQueue()
{

}

CCirQueue::~CCirQueue()
{
	//qDebug() << "CCirQueue::~CCirQueue";
}

int CCirQueue::Initial(long long _total, int _outMaxSize) {
    if ((_total>0) && (_outMaxSize>0))
    {
        m_length = _total;
        m_incount	= 0;
        m_outcount	= 0;
        m_validNum	= 0;
        m_curBlockPoint = 0;
        m_unitmaxsize	= _outMaxSize;

		m_dataBuf.reset(new unsigned char[m_length]);
        if (!m_dataBuf)
            return -1;
        m_tmpBuf.reset(new unsigned char[m_unitmaxsize]);
        if (!m_tmpBuf) {
            m_dataBuf = std::move(nullptr);
            return -1;
        }

        return 0;
    }
    else
    {
        return -1;
    }
}

int CCirQueue::push_back(const unsigned char* _buf, int _size) {
    if (!m_dataBuf) {
        return -1;
    }

    if (_size <= 0)	{
        return -2;
    }

	if (m_validNum + _size > m_length - m_unitmaxsize) {
        return -3;
    }

    unsigned char* dataBuf = (&(*m_dataBuf));
    if (m_incount%m_length + _size <= m_length) {
        memcpy(dataBuf + m_incount%m_length, _buf, _size);
    }
    else {
        memcpy(dataBuf+m_incount%m_length, _buf, m_length-m_incount%m_length);
        memcpy(dataBuf, _buf+m_length-m_incount%m_length, _size-(m_length-m_incount%m_length));
    }

    m_incount += _size;

    m_validNum.fetch_add(_size);//???m_validNum += _size;???

    return 0;
}

int CCirQueue::pop_front(int _len) {
    if (!m_dataBuf) {
        return -1;
    }

    if (_len <= 0) {
        return -1;
    }

    if (_len > m_validNum) {
        return -2;
    }

    m_outcount += _len;

    m_validNum.fetch_sub(_len);//???m_validNum -= _size;???

    return 0;
}

int CCirQueue::front(unsigned char*& _buf, int _size) {
    if (!m_dataBuf) {
        return -1;
    }

    long long validnum = m_validNum.load(); //memory order ?
    if ((validnum < _size) || (_size > m_unitmaxsize)) {
		//printf("\nvalidnum = %l , size = %l , m_unitmaxsize = %l", validnum, _size, m_unitmaxsize);
        return -1;
    }
    else {
        validnum = _size;
    }

    unsigned char* dataBuf = (&(*m_dataBuf));
    unsigned char* tempBuf = (&(*m_tmpBuf));
    if ((m_outcount%m_length)+validnum <= m_length) {
        _buf = dataBuf + (m_outcount%m_length);
    }
    else {
        memcpy(tempBuf, dataBuf+(m_outcount%m_length), m_length-(m_outcount%m_length));
        memcpy(tempBuf+m_length-(m_outcount%m_length), dataBuf, validnum-(m_length-(m_outcount%m_length)));
        _buf = tempBuf;
    }

    return 0;
}


long long CCirQueue::validSize() {
    return m_incount-m_outcount;
}

long long CCirQueue::freeSize() {
    return (m_length-m_unitmaxsize)-(m_incount-m_outcount);
}


//**********************扩展操作***************************

int CCirQueue::Memset(char _val/*=0*/) {
    if (!m_dataBuf)
        return 1;
    memset(&(*m_dataBuf), _val, m_length);
    if (m_tmpBuf)
        memset(&(*m_tmpBuf), _val, m_unitmaxsize);
    return 0;
}

unsigned char* CCirQueue::GetBlockPointer(int _size, long long &_nRet) {
    if (!m_dataBuf)
        return nullptr;
    //_pos和_size必须是单位大小
    if (0 != m_length%_size) {
        return nullptr;
    }

    if (m_incount != 0 && m_curBlockPoint != _size) {
        //TRACE("has ...... %lld bytes, need %d\n", m_incount, _size);
    }

    if (m_incount-m_curBlockPoint > m_length-m_unitmaxsize) {
        _nRet = m_incount - m_curBlockPoint - (m_length-m_unitmaxsize) ;
        m_curBlockPoint = m_incount - (m_length-m_unitmaxsize);
    }
    else {
        _nRet = 0;
    }

    if ((m_curBlockPoint>m_incount-_size) || (_size>m_length/2))
    {
        return nullptr;
    }
    else
    {
        m_curBlockPoint += _size;
        return (&(*m_dataBuf)) + (m_curBlockPoint-_size)%m_length;
    }
}

