#ifndef LK_NETWORK_NETPACKAGE_
#define LK_NETWORK_NETPACKAGE_
namespace LK {
	namespace Network {
		/**
		@brief 网络数据包*/
		struct NetPackage :BigObject {
			BMETHODS(ptr<NetPackage>, NetPackage)
			/**
			@brief 从缓冲中获取一个完整数据包内容*/
			static ptr<NetPackage> FromBuffer(Memory::Buffer& buf);
		};
	}
}
#endif