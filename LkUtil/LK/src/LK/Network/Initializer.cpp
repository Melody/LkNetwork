#include"stdafx.h"
#ifdef LK_NETWORK_INITIALIZER_
namespace LK
{
	namespace Network
	{
		//初始化
		void Initializer::Startup()
		{
#ifdef _WINDOWS
			WSADATA wsaData;
			index_t iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				switch (WSAGetLastError())
				{
				case WSASYSNOTREADY:
					return Throw<NetworkInitializingException>("The underlying network subsystem is not ready for network communication.");
				case WSAVERNOTSUPPORTED:
					return Throw<NetworkInitializingException>("The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
				case WSAEINPROGRESS:
					return Throw<NetworkInitializingException>("A blocking Windows Sockets 1.1 operation is in progress.");
				case WSAEPROCLIM:
					return Throw<NetworkInitializingException>("A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
				default:break;
				}
				return Throw<NetworkInitializingException>();
			}
#endif
			GetDefaultMutexRef().lock();
			hasinit() = true;
			GetDefaultMutexRef().unlock();
		}
		/**
		@brief 初始化，初始化成功返回true，否则返回false。
		@note 一旦初始化成功，再次调用该函数时就会直接返回true，除非遇到清理*/
		bool Initializer::StartupOnce()
		{
			if (hasinit())return true;
			GetDefaultMutexRef().lock();
			if (!hasinit())
			{
				try { Startup();}
				catch (NetworkInitializingException) { }
			}
			bool rt= hasinit();
			GetDefaultMutexRef().unlock();
			return rt;
		}
		/**
		@brief 清理*/
		void Initializer::Cleanup()
		{
#ifdef _WINDOWS
			index_t iResult = WSACleanup();
			if (iResult != 0) {
				switch (WSAGetLastError())
				{
				case WSANOTINITIALISED:
					return Throw<NetworkException>("A successful WSAStartup call must occur before using this function.");
				case WSAENETDOWN:
					return Throw<NetworkException>("The network subsystem has failed.");
				case WSAEINPROGRESS:
					return Throw<NetworkException>("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
				default:break;
				}
				return Throw<NetworkException>("清理WSA网络组件时发生异常");
			}
#endif
			GetDefaultMutexRef().lock();
			hasinit() = false;
			GetDefaultMutexRef().unlock();
		}
		/**
		@brief 清理，清理成功返回true，否则返回false。
		@note 一旦清理成功，再次调用该函数时就会直接返回true，除非重新初始化*/
		bool Initializer::CleanupOnce(){
			if (!hasinit())return true;
			GetDefaultMutexRef().lock();
			if (hasinit())
			{
				try { Cleanup();}
				catch (NetworkException) { }
			}
			bool rt= !hasinit();
			GetDefaultMutexRef().unlock();
			return rt;
		}
		bool& Initializer::hasinit()
		{
			static bool has;
			return has;
		}
	}
}
#endif