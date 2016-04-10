#include"stdafx.h"
#ifdef LK_NETWORK_SOCKET_
#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~0
#endif
namespace LK
{
	namespace Network
	{
		/**
		@brief 使用协议族、套接字类型、协议类型来构造一个套接字对象*/
		Socket::Socket( int socktype, int protocol,int family){
			Asign( socktype, protocol,family);
		}
		/**
		@brief 重新构造socket，并在发生错误时抛出异常*/
		void Socket::Asign( int socktype, int protocol,int family) {
			Initializer::StartupOnce();
			soc = socket(family, socktype, protocol);
			if (soc == INVALID_SOCKET)
			{
#ifdef _WINDOWS
				switch (WSAGetLastError())
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem or the associated service provider has failed.");
				case WSAEAFNOSUPPORT:
					Throw<SocketException>("The specified address family is not supported.For example, an application tried to create a socket for the AF_IRDA address family but an infrared adapter and device driver is not installed on the local computer.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEMFILE:
					Throw<SocketException>("No more socket descriptors are available.");
				case WSAEINVAL:
					Throw<SocketException>("An invalid argument was supplied.This error is returned if the af parameter is set to AF_UNSPEC and the type and protocol parameter are unspecified.");
				case WSAEINVALIDPROVIDER:
					Throw<SocketException>("The service provider returned a version other than 2.2.");
				case WSAEINVALIDPROCTABLE:
					Throw<SocketException>("The service provider returned an invalid or incomplete procedure table to the WSPStartup.");
				case WSAENOBUFS:
					Throw<SocketException>("No buffer space is available.The socket cannot be created.");
				case WSAEPROTONOSUPPORT:
					Throw<SocketException>("The specified protocol is not supported.");
				case WSAEPROTOTYPE:
					Throw<SocketException>("The specified protocol is the wrong type for this socket.");
				case WSAEPROVIDERFAILEDINIT:
					Throw<SocketException>("The service provider failed to initialize.This error is returned if a layered service provider(LSP) or namespace provider was improperly installed or the provider fails to operate correctly.");
				case WSAESOCKTNOSUPPORT:
					Throw<SocketException>("The specified socket type is not supported in this address family.");
				default:break;
				}
				Throw<SocketException>("构造套接字时发生异常");
#else
                Throw<SocketException>(std::string ("构造套接字时发生异常\n")+strerror(errno));
#endif
			}
		}
		/**
		@brief 连接到指定的套接字地址，连接发生错误将抛出异常*/
		void Socket::Connect(SocketAddress const& addr,index_t len )
		{
			index_t result = connect(soc, &addr.addr, (int)len);
			if (result < 0) {
#ifdef _WINDOWS
				result = WSAGetLastError();
				closesocket(soc);
				soc = INVALID_SOCKET;
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEADDRINUSE:
					Throw<SocketException>("The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until the connect function if the bind was to a wildcard address (INADDR_ANY or in6addr_any) for the local IP address. A specific address needs to be implicitly bound by the connect function.");
				case WSAEINTR:
					Throw<SocketException>("The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEALREADY:
					Throw<SocketException>("A nonblocking connect call is in progress on the specified socket."
						"Note  In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll.");
				case WSAEADDRNOTAVAIL:
					Throw<SocketException>("The remote address is not a valid address(such as INADDR_ANY or in6addr_any) .");
				case WSAEAFNOSUPPORT:
					Throw<SocketException>("Addresses in the specified family cannot be used with this socket.");
				case WSAECONNREFUSED:
					Throw<SocketException>("The attempt to connect was forcefully rejected.");
				case WSAEFAULT:
					Throw<SocketException>("The sockaddr structure pointed to by the name contains incorrect address format for the associated address family or the namelen parameter is too small.This error is also returned if the sockaddr structure pointed to by the name parameter with a length specified in the namelen parameter is not in a valid part of the user address space.");
				case WSAEINVAL:
					Throw<SocketException>("The parameter s is a listening socket.");
				case WSAEISCONN:
					Throw<SocketException>("The socket is already connected(connection - oriented sockets only).");
				case WSAENETUNREACH:
					Throw<SocketException>("The network cannot be reached from this host at this time.");
				case WSAEHOSTUNREACH:
					Throw<SocketException>("A socket operation was attempted to an unreachable host.");
				case WSAENOBUFS:
					Throw<SocketException>("Note  No buffer space is available.The socket cannot be connected.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor specified in the s parameter is not a socket.");
				case WSAETIMEDOUT:
					Throw<SocketException>("An attempt to connect timed out without establishing a connection.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and the connection cannot be completed immediately.");
				case WSAEACCES:
					Throw<SocketException>("An attempt to connect a datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled.");
				default:break;
				}
				Throw<SocketException>("尝试连接时发生异常");
#else
                Throw<SocketException>(std::string ("尝试连接时发生异常\n")+strerror(errno));
#endif

			}
		}
		/**
		@brief 发送内容，返回实际发送的字节数*/
		index_t Socket::Send(const char *buf, index_t len, int flag)
		{
			int result = send(soc, buf, (int)len, flag);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEACCES:
					Throw<SocketException>("The requested address is a broadcast address, but the appropriate flag was not set.Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address.");
				case WSAEINTR:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEFAULT:
					Throw<SocketException>("The buf parameter is not completely contained in a valid part of the user address space.");
				case WSAENETRESET:
					Throw<SocketException>("The connection has been broken due to the keep - alive activity detecting a failure while the operation was in progress.");
				case WSAENOBUFS:
					Throw<SocketException>("No buffer space is available.");
				case WSAENOTCONN:
					Throw<SocketException>("The socket is not connected.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("MSG_OOB was specified, but the socket is not stream - style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.");
				case WSAESHUTDOWN:
					Throw<SocketException>("The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and the requested operation would block.");
				case WSAEMSGSIZE:
					Throw<SocketException>("The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.");
				case WSAEHOSTUNREACH:
					Throw<SocketException>("The remote host cannot be reached from this host at this time.");
				case WSAEINVAL:
					Throw<SocketException>("The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.");
				case WSAECONNABORTED:
					Throw<SocketException>("The virtual circuit was terminated due to a time - out or other failure.The application should close the socket as it is no longer usable.");
				case WSAECONNRESET:
					Throw<SocketException>("The virtual circuit was reset by the remote side executing a hard or abortive close.For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet.The application should close the socket as it is no longer usable.");
				case WSAETIMEDOUT:
					Throw<SocketException>("The connection has been dropped, because of a network failure or because the system on the other end went down without notice.");
				default:break;
				}
				Throw<SocketException>("试图发送数据时出现异常");
#else
				Throw<SocketException>(std::string ("试图发送数据时出现异常\n")+strerror(errno));
#endif
			}
			return result;
		}
		/**
		@brief 发送内容到指定地址，返回实际发送的字节数*/
		index_t Socket::SendTo(const char *buf, index_t len, SocketAddress const&addr, int flag, int addrlen)
		{
			int result = sendto(soc, buf, (int)len,flag, &addr.addr,addrlen);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEACCES:
					Throw<SocketException>("The requested address is a broadcast address, but the appropriate flag was not set.Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address.");
				case WSAEINVAL:
					Throw<SocketException>("An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.");
				case WSAEINTR:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEFAULT:
					Throw<SocketException>("The buf or to parameters are not part of the user address space, or the tolen parameter is too small.");
				case WSAENETRESET:
					Throw<SocketException>("The connection has been broken due to keep - alive activity detecting a failure while the operation was in progress.");
				case WSAENOBUFS:
					Throw<SocketException>("No buffer space is available.");
				case WSAENOTCONN:
					Throw<SocketException>("The socket is not connected(connection - oriented sockets only).");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("MSG_OOB was specified, but the socket is not stream - style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.");
				case WSAESHUTDOWN:
					Throw<SocketException>("The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and the requested operation would block.");
				case WSAEMSGSIZE:
					Throw<SocketException>("The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.");
				case WSAEHOSTUNREACH:
					Throw<SocketException>("The remote host cannot be reached from this host at this time.");
				case WSAECONNABORTED:
					Throw<SocketException>("The virtual circuit was terminated due to a time - out or other failure.The application should close the socket as it is no longer usable.");
				case WSAECONNRESET:
					Throw<SocketException>("The virtual circuit was reset by the remote side executing a hard or abortive close.For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet.The application should close the socket as it is no longer usable.");
				case WSAEADDRNOTAVAIL:
					Throw<SocketException>("The remote address is not a valid address, for example, ADDR_ANY.");
				case WSAEAFNOSUPPORT:
					Throw<SocketException>("Addresses in the specified family cannot be used with this socket.");
				case WSAEDESTADDRREQ:
					Throw<SocketException>("A destination address is required.");
				case WSAENETUNREACH:
					Throw<SocketException>("The network cannot be reached from this host at this time.");
				case WSAETIMEDOUT:
					Throw<SocketException>("The connection has been dropped, because of a network failure or because the system on the other end went down without notice.	");
				default:break;
				}
				Throw<SocketException>("试图发送数据时出现异常");
#else
				Throw<SocketException>(std::string ("试图发送数据时出现异常\n")+strerror(errno));
#endif
			}
			return result;
		}
		/**
		@brief 接收内容*/
		index_t Socket::Receive(char* buf, index_t len, int flags)
		{
			int result = recv(soc,buf, (int)len, flags);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEFAULT:
					Throw<SocketException>("The buf parameter is not completely contained in a valid part of the user address space.");
				case WSAENOTCONN:
					Throw<SocketException>("The socket is not connected.");
				case WSAEINTR:
					Throw<SocketException>("The(blocking) call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAENETRESET:
					Throw<SocketException>("For a connection - oriented socket, this error indicates that the connection has been broken due to keep - alive activity that detected a failure while the operation was in progress.For a datagram socket, this error indicates that the time to live has expired.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("MSG_OOB was specified, but the socket is not stream - style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.");
				case WSAESHUTDOWN:
					Throw<SocketException>("The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and the receive operation would block.");
				case WSAEMSGSIZE:
					Throw<SocketException>("The message was too large to fit into the specified buffer and was truncated.");
				case WSAEINVAL:
					Throw<SocketException>("The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
				case WSAECONNABORTED:
					Throw<SocketException>("The virtual circuit was terminated due to a time - out or other failure.The application should close the socket as it is no longer usable.");
				case WSAETIMEDOUT:
					Throw<SocketException>("The connection has been dropped because of a network failure or because the peer system failed to respond.");
				case WSAECONNRESET:
					Throw<SocketException>("The virtual circuit was reset by the remote side executing a hard or abortive close.The application should close the socket as it is no longer usable.On a UDP - datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message.");
				default:break;
				}
				Throw<SocketException>("试图接收数据时出现异常");
#else
				Throw<SocketException>(std::string ("试图接收数据时出现异常\n")+strerror(errno));
#endif
			}
			return result;
		}
		/**
		@brief 接收内容，写出到buf所指向的空间中，写出发送端地址，返回实际接收的字节数*/
		index_t Socket::ReceiveFrom(char* buf, index_t len, SocketAddress *paddr, int flags, int*paddrlen)
		{
			int result = recvfrom(soc, buf, (int)len, flags,(sockaddr*)paddr, (socklen_t*)paddrlen);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEFAULT:
					Throw<SocketException>("The buf parameter is not completely contained in a valid part of the user address space.");
				case WSAENOTCONN:
					Throw<SocketException>("The socket is not connected.");
				case WSAEINTR:
					Throw<SocketException>("The(blocking) call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAENETRESET:
					Throw<SocketException>("For a connection - oriented socket, this error indicates that the connection has been broken due to keep - alive activity that detected a failure while the operation was in progress.For a datagram socket, this error indicates that the time to live has expired.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("MSG_OOB was specified, but the socket is not stream - style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.");
				case WSAESHUTDOWN:
					Throw<SocketException>("The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and the receive operation would block.");
				case WSAEMSGSIZE:
					Throw<SocketException>("The message was too large to fit into the specified buffer and was truncated.");
				case WSAEINVAL:
					Throw<SocketException>("The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
				case WSAECONNABORTED:
					Throw<SocketException>("The virtual circuit was terminated due to a time - out or other failure.The application should close the socket as it is no longer usable.");
				case WSAETIMEDOUT:
					Throw<SocketException>("The connection has been dropped because of a network failure or because the peer system failed to respond.");
				case WSAECONNRESET:
					Throw<SocketException>("The virtual circuit was reset by the remote side executing a hard or abortive close.The application should close the socket as it is no longer usable.On a UDP - datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message.");
				default:break;
				}
				Throw<SocketException>("试图接收数据时出现异常");
#else
				Throw<SocketException>(std::string ("试图接收数据时出现异常\n")+strerror(errno));
#endif
			}
			return result;
		}
		/**
		@brief 关闭发送或关闭接收功能*/
		void Socket::Shutdown(int flag)
		{
			int result = shutdown(soc, flag);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSAECONNABORTED:
					Throw<SocketException>("The virtual circuit was terminated due to a time - out or other failure.The application should close the socket as it is no longer usable."
						"This error applies only to a connection - oriented socket.");
				case WSAECONNRESET:
					Throw<SocketException>("The virtual circuit was reset by the remote side executing a hard or abortive close.The application should close the socket as it is no longer usable."
						"This error applies only to a connection - oriented socket.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEINVAL:
					Throw<SocketException>("The how parameter is not valid, or is not consistent with the socket type.For example, SD_SEND is used with a UNI_RECV socket type.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAENOTCONN:
					Throw<SocketException>("The socket is not connected.This error applies only to a connection - oriented socket.");
				case WSAENOTSOCK:
					Throw<SocketException>("Note  The descriptor is not a socket.");
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				default:break;
				}
				Throw<SocketException>("试图停止发送或接收时出现异常");
#else
				Throw<SocketException>(std::string ("试图停止发送或接收时出现异常\n")+strerror(errno));
#endif
			}
		}
		/**
		@brief 关闭套接字*/
		void Socket::Close()
		{
#ifdef _WINDOWS
			int result = closesocket(soc);
#else
			int result = close(soc);
#endif
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEINTR:
					Throw<SocketException>("The(blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking, but the l_onoff member of the linger structure is set to nonzero and the l_linger member of the linger structure is set to a nonzero timeout value.");
				default:break;
				}
				Throw<SocketException>("试图关闭套接字时出现异常");
#else
				Throw<SocketException>(std::string ("试图关闭套接字时出现异常\n")+strerror(errno));
#endif
			}
		}
		/**
		@brief 绑定套接字地址*/
		void Socket::Bind(SocketAddress const&adr, index_t len)
		{
			int result = bind(soc, &adr.addr, (int)len);
			if (result< 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("Note  A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEACCES:
					Throw<SocketException>("An attempt was made to access a socket in a way forbidden by its access permissions."
						"This error is returned if nn attempt to bind a datagram socket to the broadcast address failed because the setsockopt option SO_BROADCAST is not enabled.");
				case WSAEADDRINUSE:
					Throw<SocketException>("Only one usage of each socket address(protocol / network address / port) is normally permitted."
						"This error is returned if a process on the computer is already bound to the same fully qualified address and the socket has not been marked to allow address reuse with SO_REUSEADDR.For example, the IP address and port specified in the name parameter are already bound to another socket being used by another application.For more information, see the SO_REUSEADDR socket option in the SOL_SOCKET Socket Options reference, Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE, and SO_EXCLUSIVEADDRUSE.");
				case WSAEADDRNOTAVAIL:
					Throw<SocketException>("The requested address is not valid in its context."
						"This error is returned if the specified address pointed to by the name parameter is not a valid local IP address on this computer.");
				case WSAEFAULT:
					Throw<SocketException>("The system detected an invalid pointer address in attempting to use a pointer argument in a call."
						"This error is returned if the name parameter is NULL, the name or namelen parameter is not a valid part of the user address space, the namelen parameter is too small, the name parameter contains an incorrect address format for the associated address family, or the first two bytes of the memory block specified by name do not match the address family associated with the socket descriptor s.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEINVAL:
					Throw<SocketException>("An invalid argument was supplied."
						"This error is returned of the socket s is already bound to an address.");
				case WSAENOBUFS:
					Throw<SocketException>("An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full."
						"This error is returned of not enough buffers are available or there are too many connections.");
				case WSAENOTSOCK:
					Throw<SocketException>("An operation was attempted on something that is not a socket."
						"This error is returned if the descriptor in the s parameter is not a socket.");
				default:break;
				}
				Throw<SocketException>("试图绑定套接字时发生异常"+Strings::FromInt64(result));
#else
				Throw<SocketException>(std::string ("试图绑定套接字时发生异常\n")+strerror(errno));
#endif
			}
		}
		/**
		@brief 监听*/
		void Socket::Listen(index_t backlog)
		{
			int result = listen(soc, (int)backlog);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEADDRINUSE:
					Throw<SocketException>("The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs during execution of the bind function, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEINVAL:
					Throw<SocketException>("The socket has not been bound with bind.");
				case WSAEISCONN:
					Throw<SocketException>("The socket is already connected.");
				case WSAEMFILE:
					Throw<SocketException>("No more socket descriptors are available.");
				case WSAENOBUFS:
					Throw<SocketException>("No buffer space is available.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("The referenced socket is not of a type that supports the listen operation.");
				default:break;
				}
				Throw<SocketException>("试图监听套接字时发生异常");
#else
				Throw<SocketException>(std::string ("试图监听套接字时发生异常\n")+strerror(errno));
#endif
			}
		}
		/**
		@brief 接收连接申请，成功时返回一个套接字的代理，可选择输出对方的套接字地址*/
		ptr<Socket> Socket::Accept(SocketAddress*paddr,int* plen)
		{
			SOCKET s = accept(soc, (sockaddr*)paddr, (socklen_t*)plen);
			if (s < 0)
			{
#ifdef _WINDOWS
				s = WSAGetLastError();
				switch (s)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAECONNRESET:
					Throw<SocketException>("An incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call.");
				case WSAEFAULT:
					Throw<SocketException>("The addrlen parameter is too small or addr is not a valid part of the user address space.");
				case WSAEINTR:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEINVAL:
					Throw<SocketException>("The listen function was not invoked prior to accept.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEMFILE:
					Throw<SocketException>("The queue is nonempty upon entry to accept and there are no descriptors available.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAENOBUFS:
					Throw<SocketException>("No buffer space is available.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				case WSAEOPNOTSUPP:
					Throw<SocketException>("The referenced socket is not a type that supports connection - oriented service.");
				case WSAEWOULDBLOCK:
					Throw<SocketException>("The socket is marked as nonblocking and no connections are present to be accepted.");
				default:break;
				}
				Throw<SocketException>("尝试获取接收套接字时发生异常");
#else
				Throw<SocketException>(std::string ("尝试获取接收套接字时发生异常\n")+strerror(errno));
#endif
			}
			return new Socket(s);
		}
		//select
		bool Socket::Select(index_t type,	std::chrono::microseconds const& t)const
		{
			fd_set s;
			FD_ZERO(&s);
			FD_SET(soc, &s);
			timeval time={ (long)t.count()/1000000, (long)t.count() %1000000};
			int result = select(0, type == 0 ? &s : NULL, type == 1 ? &s : NULL, type == 2 ? &s : NULL, &time);
			if (result <0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAEFAULT:
					Throw<SocketException>("The Windows Sockets implementation was unable to allocate needed resources for its internal operations, or the readfds, writefds, exceptfds, or timeval parameters are not part of the user address space.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEINVAL:
					Throw<SocketException>("The time - out value is not valid, or all three descriptor parameters were null.");
				case WSAEINTR:
					Throw<SocketException>("A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAENOTSOCK:
					Throw<SocketException>("One of the descriptor sets contains an entry that is not a socket.");
				default:break;
				}
				Throw<SocketException>("对套接字进行select判断操作时发生异常");
#else
				Throw<SocketException>(std::string ("对套接字进行select判断操作时发生异常\n")+strerror(errno));
#endif
			}
			return result != 0;
		}
		//getsockopt
		int Socket::GetSockOpt(int level, int optname, void *optval, socklen_t *optlen)const
		{
			int result = getsockopt(soc, level, optname, (char*)optval, optlen);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEFAULT:
					Throw<SocketException>("One of the optval or optlen parameters is not a valid part of the user address space, or the optlen parameter is too small.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Winsock call is in progress, or the service provider is still processing a callback function.");
				case WSAEINVAL:
					Throw<SocketException>("The level parameter is unknown or invalid.");
				case WSAENOPROTOOPT:
					Throw<SocketException>("The option is unknown or unsupported by the indicated protocol family.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				default:break;
				}
				Throw<SocketException>("获取套接字信息时发生异常");
#else
				Throw<SocketException>(std::string ("获取套接字信息时发生异常\n")+strerror(errno));
#endif
			}
			return result;
		}
		//setsockopt
		int Socket::SetSockOpt(int level, int optname, const void *optval, socklen_t optlen){
			int result = setsockopt(soc, level, optname, (const char*)optval, optlen);
			if (result < 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
				switch (result)
				{
				case WSANOTINITIALISED:
					Throw<SocketException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					Throw<SocketException>("The network subsystem has failed.");
				case WSAEFAULT:
					Throw<SocketException>("The buffer pointed to by the optval parameter is not in a valid part of the process address space or the optlen parameter is too small.");
				case WSAEINPROGRESS:
					Throw<SocketException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				case WSAEINVAL:
					Throw<SocketException>("The level parameter is not valid, or the information in the buffer pointed to by the optval parameter is not valid.");
				case WSAENETRESET:
					Throw<SocketException>("The connection has timed out when SO_KEEPALIVE is set.");
				case WSAENOPROTOOPT:
					Throw<SocketException>("The option is unknown or unsupported for the specified provider or socket(see SO_GROUP_PRIORITY limitations).");
				case WSAENOTCONN:
					Throw<SocketException>("The connection has been reset when SO_KEEPALIVE is set.");
				case WSAENOTSOCK:
					Throw<SocketException>("The descriptor is not a socket.");
				default:break;
				}
				Throw<SocketException>("设置套接字信息时发生异常");
#else
				Throw<SocketException>(std::string ("设置套接字信息时发生异常\n")+strerror(errno));
#endif
			}
			return result;
		}

	}
	
}
#endif
