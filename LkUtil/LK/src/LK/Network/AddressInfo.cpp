#include"stdafx.h"
#ifdef LK_NETWORK_ADDRESSINFO_

namespace LK
{
	namespace Network
	{
		/**
		@brief 获取下一个地址信息的引用*/
		AddressElement& AddressElement::GetNextRef()
		{
			return *CheckPointer<AddressElement, AddressElement>((AddressElement*)info.ai_next);
		}
		/**
		@brief 获取下一个地址信息的引用*/
		AddressElement const& AddressElement::GetNextRef()const
		{
			return *CheckPointer<AddressElement, AddressElement>((AddressElement*)info.ai_next);
		}
		/**
		@brief 获取标志*/
		int AddressElement::GetFlag()const
		{
			return info.ai_flags;
		}
		/**
		@brief 获取协议族*/
		int AddressElement::GetAddressFamily()const
		{
			return info.ai_family;
		}
		/**
		@brief 获取套接字类型*/
		int AddressElement::GetSocketType()const
		{
			return info.ai_protocol;
		}
		/**
		@brief 获取协议类型*/
		int AddressElement::GetProtocolType()const
		{
			return info.ai_socktype;
		}
		/**
		@brief 获取节点名称*/
		std::string AddressElement::GetName()const
		{
			return info.ai_canonname;
		}
		/**
		@brief 获取sockaddr长度*/
		index_t AddressElement::GetAddrLen()const { return info.ai_addrlen; }
		/**
		@brief 获取SocketAddress的引用*/
		SocketAddress const& AddressElement::GetSocketAddressRef()const {
			if(info.ai_addr)return reinterpret_cast<SocketAddress const&>(*info.ai_addr);
			return RThrow<SocketAddress const&, PointerIsNullException>();
		}
		/**
		@brief 转为字符串表示*/
		std::string AddressElement::ToString()const
		{
			switch (info.ai_family)
			{
			case AddressFamily::Internetwork:
			case AddressFamily::InternetworkV6:return ((SocketAddress*)info.ai_addr)->ToString();
			default:break;
			}
			return "UnknowAddress";
		}

		/**
		@brief 构造函数，使用地址和服务字符串（端口号）进行初始化*/
		AddressInfo::AddressInfo(std::string const& addr, std::string const& port) :pInfo(NULL) {
			Asign(addr.c_str(), port.c_str(), NULL);
		}
		/**
		@brief 构造函数，使用地址和服务字符串:端口号 进行初始化*/
		AddressInfo::AddressInfo(std::string const& addrport) :pInfo(NULL) {
			std::string addr, port;
			index_t idx = Strings::LastIndexOf(addrport,':');
			if (idx == -1 || idx == 0 || addrport[idx - 1] == L':') {
				addr = addrport;
				port = "0";
			}
			else {
				addr = addrport.substr(0,idx);
				port = addrport.substr(idx + 1);
			}
			Asign(addr.c_str(), port.c_str(), NULL);
		}
		/**
		@brief 构造函数，使用地址字符串和端口进行初始化*/
		AddressInfo::AddressInfo(std::string const& addr, short port):pInfo(NULL){
			Asign(addr.c_str(), Strings::FromInt64(port).c_str(), NULL);
		}
		/**
		@brief 构造函数，使用地址字符串和端口进行初始化，family指定协议族，socktype指定套接字类型，protocol指定协议类型*/
		AddressInfo::AddressInfo(std::string const& addr, std::string const& port, int family, int socktype, int protocol) : pInfo(NULL)
		{
			addrinfo ad;
			#ifdef _WINDOWS
			ZeroMemory(&ad, sizeof(ad));
			#else
			memset(&ad, 0,sizeof(ad));
			#endif
			ad.ai_family = family;
			ad.ai_socktype = socktype;
			ad.ai_protocol = protocol;
			Asign(addr.c_str(), port.c_str(), &ad);
		}
		/**
		@brief 析构函数，释放地址信息*/
		AddressInfo::~AddressInfo() {//gcc需要添加-lWS32_32指令
			if(pInfo)freeaddrinfo(pInfo);
		}
		/**
		@brief 获取地址信息，并在错误时抛出异常*/
		void AddressInfo::Asign(char const * name, char const * srv, addrinfo const * paddr)
		{
			Initializer::StartupOnce();
			if (pInfo) { freeaddrinfo(pInfo); pInfo = NULL; }
			int result = getaddrinfo(name, srv, paddr, &pInfo);
			if (result != 0)
			{
#ifdef _WINDOWS
				result = WSAGetLastError();
#endif
				freeaddrinfo(pInfo);
				pInfo = NULL;
#ifdef _WINDOWS
				switch (result)
				{
				case WSA_NOT_ENOUGH_MEMORY:
					Throw<NetworkException>("没有足够的内存进行该操作\nThere was insufficient memory to perform the operation.");
				case WSAEAFNOSUPPORT:
					Throw<NetworkException>("正在使用一个与所请求的协议不兼容的地址\nAn address incompatible with the requested protocol was used.This error is returned if the ai_family member of the addrinfoW structure pointed to by the hints parameter is not supported.");
				case WSAEINVAL:
					Throw<NetworkException>("提供了无效的参数\nAn invalid argument was supplied.This error is returned if an invalid value was provided for the ai_flags member of the addrinfoW structure pointed to by the hints parameter.");
				case WSAESOCKTNOSUPPORT:
					Throw<NetworkException>("指定的套接字类型在该协议族中不受支持\nThe support for the specified socket type does not exist in this address family.This error is returned if the ai_socktype member of the addrinfoW structure pointed to by the hints parameter is not supported.");
				case WSAHOST_NOT_FOUND:
					Throw<NetworkException>("找不到主机名\nNo such host is known.This error is returned if the name does not resolve for the supplied parameters or the pNodename and pServicename parameters were not provided.");
				case WSANO_DATA:
					Throw<NetworkException>("找不到数据\nThe requested name is valid, but no data of the requested type was found.");
				case WSANO_RECOVERY:
					Throw<NetworkException>("数据查找时遇到无法恢复的错误\nA nonrecoverable error occurred during a database lookup.This error is returned if nonrecoverable error in name resolution occurred.");
				case WSANOTINITIALISED:
					Throw<NetworkException>("在调用该函数前必须成功执行WSA初始化\nA successful WSAStartup call must occur before using this function.");
				case WSATRY_AGAIN:
					Throw<NetworkException>("认证服务器无响应\nThis is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.This error is returned when a temporary failure in name resolution occurred.");
				case WSATYPE_NOT_FOUND:
					Throw<NetworkException>("找不到指定的类\nThe specified class was not found.The pServiceName parameter is not supported for the specified ai_socktype member of the addrinfoW structure pointed to by the hints parameter.");
				default:
					break;
				}
				Throw<NetworkException>("获取地址信息时发生错误");
#else
				Throw<NetworkException>(gai_strerror(result));
#endif
			}
		}
		/**
		@brief 获取AddressElement的数量*/
		index_t AddressInfo::size()const
		{
			index_t result = 0;
			addrinfo* p = pInfo;
			while (p) { result++; p = p->ai_next; }
			return result;
		}
		/**
		@brief 获取第index个AddressElement的引用*/
		AddressElement const&AddressInfo::operator[](index_t idx)const
		{
			addrinfo const* p = pInfo;
			while (idx--) { if(!(p = p->ai_next))Throw<PointerIsNullException>(); }
			return (AddressElement const&)*p;
		}
	}
}
#endif
