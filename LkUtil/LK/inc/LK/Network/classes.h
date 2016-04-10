#ifdef _WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#ifdef _LINUX
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#endif
#ifndef LK_NETWORK_
#define LK_NETWORK_
namespace LK
{
	/**
	@brief 网络命名空间*/
	namespace Network
	{
		/**@brief 网络异常*/
		struct NetworkException;
		/**@brief 套接字类型*/
		struct Socket;
		/**@brief 套接字代理*/
		typedef ptr<Socket> PSocket;
		#ifndef _WINDOWS
		typedef int SOCKET;
		#endif
	}
}
#endif
