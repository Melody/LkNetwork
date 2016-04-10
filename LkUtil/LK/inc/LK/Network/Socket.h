#ifndef LK_NETWORK_SOCKET_
#define LK_NETWORK_SOCKET_
namespace LK
{
	namespace Network
	{
		/**
		@brief 成员记录套接字标记*/
		struct SocketFlag{
			//None
			static const int None = 0;
			//Specifies that the data should not be subject to routing. A Windows Sockets service provider can choose to ignore this flag.
			static const int DontRoute = MSG_DONTROUTE;
			//Sends OOB data (stream-style socket such as SOCK_STREAM only. Receive Processes Out Of Band(OOB) data.
			static const int OOB = MSG_OOB;
			/**Peeks at the incoming data.The data is copied into the buffer, but is not removed from the input queue.The function subsequently returns the amount of data that can be read in a single call to the recv(or recvfrom) function, which may not be the same as the total amount of data queued on the socket.The amount of data that can actually be read in a single call to the recv(or recvfrom) function is limited to the data size written in the send or sendto function call.*/
			static const int Peek = MSG_PEEK;
			/*	The receive request will complete only when one of the following events occurs :
			The buffer supplied by the caller is completely full.
				The connection has been closed.
				The request has been canceled or an error occurred.
				Note that if the underlying transport does not support MSG_WAITALL, or if the socket is in a non - blocking mode, then this call will fail with WSAEOPNOTSUPP.Also, if MSG_WAITALL is specified along with MSG_OOB, MSG_PEEK, or MSG_PARTIAL, then this call will fail with WSAEOPNOTSUPP.This flag is not supported on datagram sockets or message - oriented sockets.*/
//			static const int WaitAll = MSG_WAITALL;
		};
		/**
		@brief 记录关闭选项*/
		struct SocketShutdownFlag {
			static const int Send =
#ifdef _WINDOWS
			SD_SEND;
#else
            SHUT_WR;
#endif
			static const int Receive =
#ifdef _WINDOWS
			SD_RECEIVE;
#else
            SHUT_RD;
#endif
			static const int Both =
#ifdef _WINDOWS
			SD_BOTH;
#else
            SHUT_RDWR;
#endif
		};
		/**
		@brief 套接字*/
		struct Socket :BigObject{
			//套接字
			SOCKET soc;
			/**
			@brief 使用套接字类型、协议类型、协议族来构造一个套接字对象*/
			Socket( int socktype, int protocol,int family=AddressFamily::Unspecified);
			/**
			@brief 复制构造将抛出NotSupportException*/
			inline Socket(const Socket&) { Throw<NotSupportException>(); }
			/**
			@brief 赋值将抛出NotSupportException*/
			inline Socket&operator=(const Socket&) { return RThrow<Socket&,NotSupportException>(); }
			/**
			@brief 重新构造socket，并在发生错误时抛出异常*/
			void Asign(int socktype , int protocol, int family);
			/**
			@brief 连接到指定的套接字地址，连接发生错误将抛出异常*/
			void Connect(SocketAddress const& addr, index_t len = sizeof(sockaddr_in6));
			/**
			@brief 发送内容，返回实际发送的字节数*/
			index_t Send(const char *buf, index_t len,int flag=SocketFlag::None);
			/**
			@brief 发送内容，返回实际发送的字节数*/
			inline index_t Send(const unsigned char *buf, index_t len, int flags = SocketFlag::None) { return Send((const char *)buf, len, flags); }
			/**
			@brief 发送内容，返回实际发送的字节数*/
			inline index_t Send(std::vector<char> const&arr, int flags = SocketFlag::None) { return Send(arr.data(), arr.size(), flags); }
			/**
			@brief 发送内容，返回实际发送的字节数*/
			inline index_t Send(std::vector<u8> const&arr, int flags = SocketFlag::None) { return Send((const unsigned char *)arr.data(),arr.size(), flags); }
			/**
			@brief 发送内容到指定地址，返回实际发送的字节数*/
			index_t SendTo(const char *buf, index_t len, SocketAddress const&addr,int flag = SocketFlag::None,int addrlen = sizeof(sockaddr_in6));
			/**
			@brief 发送内容到指定地址，返回实际发送的字节数*/
			inline index_t SendTo(const unsigned char *buf, index_t len, SocketAddress const&addr, int flags = SocketFlag::None, int addrlen = sizeof(sockaddr_in6)) { return SendTo((const char *)buf, len, addr,flags,addrlen); }
			/**
			@brief 发送内容到指定地址，返回实际发送的字节数*/
			inline index_t SendTo(std::vector<char> const&arr, SocketAddress const&addr, int flags = SocketFlag::None, int addrlen = sizeof(sockaddr_in6)) { return SendTo(arr.data(), arr.size(), addr, flags, addrlen); }
			/**
			@brief 发送内容到指定地址，返回实际发送的字节数*/
			inline index_t SendTo(std::vector<u8> const&arr, SocketAddress const&addr, int flags = SocketFlag::None, int addrlen = sizeof(sockaddr_in6)) { return SendTo((const unsigned char *)arr.data(), arr.size(), addr, flags, addrlen); }
			/**
			@brief 接收内容，写出到buf所指向的空间中，返回实际接收的字节数*/
			index_t Receive(char* buf, index_t len, int flags = SocketFlag::None);
			/**
			@brief 接收内容，写出到buf所指向的空间中，返回实际接收的字节数*/
			inline index_t Receive(unsigned char* buf, index_t len, int flags = SocketFlag::None) { return Receive((char*)buf,len,flags); }
			/**
			@brief 接收内容，写出到buf数组中，返回实际接收的字节数*/
			inline index_t Receive(std::vector<u8>&  buf, int flags = SocketFlag::None) { return Receive((unsigned char*)buf.data(), buf.size(), flags); }
			/**
			@brief 接收内容，写出到buf数组中，返回实际接收的字节数*/
			inline index_t Receive(std::vector<char>&  buf, int flags = SocketFlag::None) { return Receive((char*)buf.data(), buf.size(), flags); }
			/**
			@brief 接收内容，写出到buf所指向的空间中，写出发送端地址，返回实际接收的字节数*/
			index_t ReceiveFrom(char* buf, index_t len, SocketAddress *paddr,int flags = SocketFlag::None,int*paddrlen = NULL);
			/**
			@brief 接收内容，写出到buf所指向的空间中，写出发送端地址，返回实际接收的字节数*/
			inline index_t ReceiveFrom(unsigned char* buf, index_t len, SocketAddress *paddr, int flags = SocketFlag::None, int*paddrlen = NULL) { return ReceiveFrom((char*)buf, len, paddr, flags, paddrlen); }
			/**
			@brief 接收内容，写出到buf数组中，写出发送端地址，返回实际接收的字节数*/
			inline index_t ReceiveFrom(std::vector<u8>& buf, SocketAddress *paddr, int flags = SocketFlag::None, int*paddrlen = NULL) { return ReceiveFrom((unsigned char*)buf.data(), buf.size(), paddr, flags, paddrlen); }
			/**
			@brief 接收内容，写出到buf数组中，写出发送端地址，返回实际接收的字节数*/
			inline index_t ReceiveFrom(std::vector<char>&  buf, SocketAddress *paddr, int flags = SocketFlag::None, int*paddrlen = NULL) { return ReceiveFrom((char*)buf.data(), buf.size(), paddr, flags, paddrlen); }
			/**
			@brief 关闭发送或关闭接收功能*/
			void Shutdown(int flag);
			/**
			@brief 关闭套接字*/
			void Close();
			/**
			@brief 绑定套接字地址*/
			void Bind(SocketAddress const&addr, index_t len = sizeof(sockaddr_in6));
			/**
			@brief 监听*/
			void Listen(index_t backlog = SOMAXCONN);
			/**
			@brief 接收连接申请，成功时返回一个套接字的代理，可选择输出对方的套接字地址*/
			PSocket Accept(SocketAddress*paddr = NULL,int *plen=NULL);
			/**
			@brief 判断该socket是否有数据可读。参数t设置超时时间*/
			bool IsReadable(std::chrono::microseconds ms= std::chrono::microseconds(0))const { return Select(0, ms); }
			/**
			@brief 判断该socket是否已连接，可发送数据。参数t设置超时时间*/
			bool IsWriteable(std::chrono::microseconds ms = std::chrono::microseconds(0))const { return Select(1, ms); }
			/**
			@brief 判断该socket是否处于错误状态。参数t设置超时时间*/
			bool IsError(std::chrono::microseconds ms = std::chrono::microseconds(0))const { return Select(2, ms); }
			//getsockopt
			int GetSockOpt(int level, int optname, void *optval, socklen_t *optlen)const;
			//setsockopt
			int SetSockOpt(int level, int optname, const void *optval, socklen_t optlen);
		private:
			//直接使用套接字构造
			Socket(SOCKET s) :soc(s) {}
			//select
			bool Select(index_t type, std::chrono::microseconds const& t)const;
		};
	}
}
#endif
