#ifndef LK_NETWORK_SOCKETADDRESS_
#define LK_NETWORK_SOCKETADDRESS_
namespace LK
{
	namespace Network
	{
		/**
		@brief 表示套接字地址*/
		struct SocketAddress {
			union {
				sockaddr addr;
				sockaddr_in v4;
				sockaddr_in6 v6;
			};
			SocketAddress()
			{
#ifdef _WINDOWS
                ZeroMemory(this, sizeof(*this));
#else
                memset(this,0, sizeof(*this));
#endif
			 }
			/**
			@brief 转换为字符串表示*/
			std::string ToString()const;
			static int Compare(SocketAddress const&s1, SocketAddress const&s2);
			friend bool operator==(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) == 0; }
			friend bool operator!=(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) != 0; }
			friend bool operator>=(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) >= 0; }
			friend bool operator<=(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) <= 0; }
			friend bool operator>(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) > 0; }
			friend bool operator<(SocketAddress const& s1, SocketAddress const&s2) { return Compare(s1, s2) < 0; }
		private:
		};
	}
}
#endif
