#include "InputCMOS.h"
//#include <QDebug>
InputCMOS::InputCMOS(USHORT port, ULONG ip)
{
	//创建套接字对象，并设为非阻塞模式
	ULONG ul = 1;
	sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	::ioctlsocket(sock, FIONBIO, (ULONG*)&ul);//设为非阻塞模式

	//填充地址结构
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = ip;
	addr.sin_port = htons(port);

	//将套接字与对应地址绑定
	if (::bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
			//printf("failed bind()...\n");
			return;
	}
	int recvSize = 512 * 1024 * 1024;
	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvSize, 4);
}

InputCMOS::~InputCMOS()
{
	::closesocket(sock);
}

#include <QFile>
void InputCMOS::process()
{
	UCHAR *recvbuf = new UCHAR[NUMOFRECVBUF];//接收UDP数据
	sockaddr_in addr_far;
	int alen = sizeof(addr_far);
	//select模型
	//添加套接字句柄到套接字集合
	FD_ZERO(&fdsock); // 先清空
	FD_SET(sock, &fdsock);
	struct timeval timeout;
	timeout.tv_sec = 2;

	//unsigned char* buf = new unsigned char[512 * 1024 * 1024];
	//long long cnt = 0;

	while (m_processing)
	{
		fd_set fdread = fdsock;//fdsock_img的copy
		int nret = ::select(0, &fdread, NULL, NULL, &timeout);//select()返回值：有未决I/O的套接字句柄的个数
		;
		if (nret == SOCKET_ERROR)
		{
			//printf("failed select()...\n");
			continue;
		}
		if (nret == 0)//超时
		{
			//qDebug() << ("overtime...\n");
			continue;
		}

		if (nret > 0)
		{
			//for (int i = 0; i<(int)fdsock.fd_count; i++)
			//{
				//memset(recvbuf, 0, sizeof(*recvbuf));//清空接收缓冲区
				if (FD_ISSET(sock, &fdread))//若该套接字可读
				{
					int nrecv = ::recvfrom(sock, (char*)recvbuf, NUMOFRECVBUF, 0, (LPSOCKADDR)&addr_far, &alen);
					if (nrecv > 0)
					{
		/*				memcpy(buf + cnt, recvbuf, nrecv);
						cnt += nrecv;

						if (cnt > 500 * 1024 * 1024)
						{
							cnt = 0;
							static int fnum = 0;
							QFile f(QObject::tr("C:/recv_%1.dat").arg(++fnum)); f.open(QIODevice::WriteOnly); f.write((char*)buf, 500 * 1024 * 1024); f.close();
						}*/

						if (m_outputBuffer[0].second)
							m_outputBuffer[0].second->push_back(recvbuf, nrecv);//将接受的数据放入输出缓冲区
						//qDebug() << ("\n收到%d个字节", nrecv);
					}
				}
			//}
		}
	}
}