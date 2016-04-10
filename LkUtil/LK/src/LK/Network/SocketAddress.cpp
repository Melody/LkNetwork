#include"stdafx.h"
#ifdef LK_NETWORK_SOCKETADDRESS_
namespace LK
{
	namespace Network
	{
		/**
		@brief 转换为字符串表示*/
		std::string SocketAddress::ToString()const {
			switch (addr.sa_family){
			case AddressFamily::Internetwork: {
				u8* p = (u8*)&(v4.sin_addr);
				char str[24];
				index_t i = Strings::WriteNum(p[0], str,1); str[i++] = L'.';
				i += Strings::WriteNum(p[1], str + i, 1); str[i++] = L'.';
				i += Strings::WriteNum(p[2], str + i, 1); str[i++] = L'.';
				i += Strings::WriteNum(p[3], str + i, 1);
				if (v4.sin_port != 0) {//端口号非0时输出端口号
					str[i++] = L':';
					p = (u8*)&(v4.sin_port);
					i += Strings::WriteNum((int)p[1] + (int)p[0] * 256, str + i, 1);
				}
				str[i] = 0;
				return str;
			}
			case AddressFamily::InternetworkV6: {
				u8* p = (u8*)&(v6.sin6_addr);
				char str[48];
				index_t i = 0;
				for (index_t k = 0; k < 8; ++k) {
					i += Strings::WriteNum(p[k], str + i, 1, 16);
					if(k!=7)str[i++] = L':';
				}
				if (v6.sin6_port != 0) {//端口号非0时输出端口号
					str[i++] = L':';
					p = (u8*)&(v6.sin6_port);
					i += Strings::WriteNum((int)p[1] + (int)p[0] * 256, str + i, 1);
				}
				str[i] = 0;
				return str;
			}
			default:break;
			}
			return "UnknowAddressType";
		}
		int SocketAddress::Compare(SocketAddress const&s1, SocketAddress const&s2) {
			if (s1.addr.sa_family != s2.addr.sa_family)return s1.addr.sa_family > s2.addr.sa_family ? 1 : -1;
			if (s1.addr.sa_family == AddressFamily::InternetworkV6) {
				for (int i = 0; i < sizeof(s1); ++i)
					if (((u8*)&s1.v6)[i] != ((u8*)&s2.v6)[i])
						return ((u8*)&s1.v6)[i] >((u8*)&s2.v6)[i] ? 1 : -1;
				return 0;
			}
			for (int i = 0; i < sizeof(s1); ++i)
				if (((u8*)&s1.v4)[i] != ((u8*)&s2.v4)[i])
					return ((u8*)&s1.v4)[i] >((u8*)&s2.v4)[i] ? 1 : -1;
			return 0;
		}
	}
}
#endif
