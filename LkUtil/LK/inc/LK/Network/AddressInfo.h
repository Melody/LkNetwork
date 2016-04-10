#ifndef LK_NETWORK_ADDRESSINFO_
#define LK_NETWORK_ADDRESSINFO_
namespace LK
{
	namespace Network
	{
		/**
		@brief 成员记录地址族枚举*/
		struct AddressFamily
		{
			static const int Unspecified = AF_UNSPEC;// unspecified
			static const int Unix = AF_UNIX;// local to host (pipes, portals)
			static const int Internetwork = AF_INET;// internetwork: UDP, TCP, etc.
			//static const int Arpanet = AF_IMPLINK;// arpanet imp addresses
			//static const int Pup = AF_PUP;// pup protocols: e.g. BSP
			//static const int CHAOS = AF_CHAOS;// mit CHAOS protocols
			//static const int NS = AF_NS;// XEROX NS protocols
			static const int IPX = AF_IPX;// IPX protocols: IPX, SPX, etc.
			//static const int ISO = AF_ISO;// ISO protocols
			//static const int OSI = AF_OSI;// OSI is ISO
			//static const int ECMA = AF_ECMA;// european computer manufacturers
			//static const int Datakit = AF_DATAKIT;// datakit protocols
			//static const int CCITT = AF_CCITT;// CCITT protocols, X.25 etc
			static const int SNA = AF_SNA;// IBM SNA
			static const int DECnet = AF_DECnet;// DECnet
			//static const int DirectDataLink = AF_DLI;// Direct data link interface
			//static const int LAT = AF_LAT;// LAT
			//static const int NSCHyperchannel = AF_HYLINK;// NSC Hyperchannel
			static const int AppleTalk = AF_APPLETALK;// AppleTalk
			//static const int NetBios = AF_NETBIOS;// NetBios-style addresses
			//static const int VoiceView = AF_VOICEVIEW;// VoiceView
			//static const int Firefox = AF_FIREFOX;// Protocols from Firefox
			//static const int Unknow1 = AF_UNKNOWN1;// Somebody is using this!
			//static const int Banyan = AF_BAN;// Banyan
			//static const int ATM = AF_ATM;// Native ATM Services
			static const int InternetworkV6 = AF_INET6;// Internetwork Version 6
			//static const int CLUSTER = AF_CLUSTER;// Microsoft Wolfpack
			//static const int IEEE12844 = AF_12844;// IEEE 1284.4 WG AF
			static const int IrDA = AF_IRDA;// IrDA
			//static const int NETDES = AF_NETDES;// Network Designers OSI & gateway
		};
		/**
		@brief 成员记录套接字类型枚举*/
		struct SocketType {
			static const int Any = 0;/* unspecified*/
			static const int Stream = SOCK_STREAM;/* stream socket */
			static const int Datagram = SOCK_DGRAM;/* datagram socket */
			static const int Raw = SOCK_RAW;/* raw-protocol interface */
			static const int ReliablyDelivered = SOCK_RDM;/* reliably-delivered message */
			static const int SequencedPacket = SOCK_SEQPACKET;/* sequenced packet stream */
		};
		/**
		@brief 成员记录协议类型枚举*/
		struct ProtocolType {
			static const int Any = 0;
//#if(_WIN32_WINNT >= 0x0501)
			static const int HOPOPTS = IPPROTO_HOPOPTS;  // IPv6 Hop-by-Hop options
//#endif//(_WIN32_WINNT >= 0x0501)
			static const int ICMP = IPPROTO_ICMP;
			static const int IGMP = IPPROTO_IGMP;
//			static const int GGP = IPPROTO_GGP;
#if(_WIN32_WINNT >= 0x0501)
			static const int IPV4 = 4;
#endif//(_WIN32_WINNT >= 0x0501)
#if(_WIN32_WINNT >= 0x0600)
			static const int ST = 5;
			static const int EGP = 8;
#endif//(_WIN32_WINNT >= 0x0600)
			static const int TCP = IPPROTO_TCP;
#ifdef _LINUX
			static const int EGP = IPPROTO_EGP;
#endif
#if(_WIN32_WINNT >= 0x0600)
            static const int CBT = 7;
			//static const int IGP = IPPROTO_IGP;
#endif//(_WIN32_WINNT >= 0x0600)
			static const int PUP = IPPROTO_PUP;
			static const int UDP = IPPROTO_UDP;
			static const int IDP = IPPROTO_IDP;
#if(_WIN32_WINNT >= 0x0600)
			static const int RDP = 27;
#endif//(_WIN32_WINNT >= 0x0600)

//#if(_WIN32_WINNT >= 0x0501)
			static const int IPV6 = IPPROTO_IPV6;
			static const int Routing = IPPROTO_ROUTING;
			static const int Fragment = IPPROTO_FRAGMENT;
			static const int ESP = IPPROTO_ESP;
			static const int AH = IPPROTO_AH;// authentication header
			static const int ICMPV6 = IPPROTO_ICMPV6;// ICMPv6
			static const int None = IPPROTO_NONE;// IPv6 no next header
			static const int DSTOPTS = IPPROTO_DSTOPTS;// IPv6 Destination options
//#endif//(_WIN32_WINNT >= 0x0501)

#if(_WIN32_WINNT >= 0x0501)
			static const int ND = IPPROTO_ND;
			static const int ICLFXBM = 78;
#endif//(_WIN32_WINNT >= 0x0501)
//#if(_WIN32_WINNT >= 0x0600)
//			static const int PIM = IPPROTO_PIM;
//			static const int PGM = IPPROTO_PGM;
//			static const int L2TP = IPPROTO_L2TP;
//			static const int SCTP = IPPROTO_SCTP;
//#endif//(_WIN32_WINNT >= 0x0600)
			static const int Raw = IPPROTO_RAW;
			static const int Max = IPPROTO_MAX;
		};
		/**
		@brief 记录单个地址信息*/
		struct AddressElement{
			/**
			@brief 获取下一个地址信息的引用*/
			AddressElement& GetNextRef();
			/**
			@brief 获取下一个地址信息的引用*/
			AddressElement const& GetNextRef()const;
			/**
			@brief 获取标志*/
			int GetFlag()const;
			/**
			@brief 获取协议族*/
			int GetAddressFamily()const;
			/**
			@brief 获取套接字类型*/
			int GetSocketType()const;
			/**
			@brief 获取协议类型*/
			int GetProtocolType()const;
			/**
			@brief 获取节点名称*/
			std::string GetName()const;
			/**
			@brief 获取sockaddr长度*/
			index_t GetAddrLen()const;
			/**
			@brief 获取SocketAddress的引用*/
			SocketAddress const& GetSocketAddressRef()const;
			/**
			@brief 转为字符串表示*/
			std::string ToString()const;
		private:
			addrinfo info;
		};
		/**
		@brief 地址信息类*/
		struct AddressInfo{
			/**
			@brief 构造函数，使用地址和服务字符串（端口号）进行初始化*/
			AddressInfo(std::string const& addr, std::string const& port);
			/**
			@brief 构造函数，使用地址和服务字符串:端口号 进行初始化*/
			AddressInfo(std::string const& addrport);
			/**
			@brief 构造函数，使用地址字符串和端口进行初始化*/
			AddressInfo(std::string const& addr, short port);
			/**
			@brief 构造函数，使用地址字符串和端口进行初始化，family指定协议族，socktype指定套接字类型，protocol指定协议类型*/
			AddressInfo(std::string const& addr, std::string const& port,int family,int socktype=SocketType::Any,int protocol = ProtocolType::TCP);
			/**
			@brief 析构函数，释放地址信息*/
			~AddressInfo();
			/**
			@brief 默认构造函数，构造空地址信息*/
			AddressInfo():pInfo(NULL) {}
			/**
			@brief 重新获取地址信息，并在错误时抛出异常*/
			void Asign(char const*name, char const*srv=NULL, addrinfo const*paddr=NULL);
			/**
			@brief 获取AddressElement的数量*/
			index_t size()const;
			/**
			@brief 获取第index个AddressElement的引用*/
			AddressElement const&operator[](index_t idx)const;
		private:
			/**
			@brief 禁止复制构造*/
			AddressInfo(const AddressInfo&);
			/**
			@brief 禁止赋值*/
			AddressInfo&operator=(const AddressInfo&);
			/**
			@brief 地址信息*/
			addrinfo* pInfo;
		};
	}
}
#endif
