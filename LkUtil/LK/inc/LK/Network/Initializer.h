#ifndef LK_NETWORK_INITIALIZER_
#define LK_NETWORK_INITIALIZER_
namespace LK
{
	namespace Network
	{
		/**
		@brief 包含WSAPI静态初始化和清理方法*/
		struct Initializer {
			/**
			@brief 初始化*/
			static void Startup();
			/**
			@brief 反初始化*/
			static void Cleanup();
			/**
			@brief 初始化，初始化成功返回true，否则返回false。
			@note 一旦初始化成功，再次调用该函数时就会直接返回true，除非调用了UnInitialize*/
			static bool StartupOnce();
			/**
			@brief 清理，成功返回true，否则返回false。
			@note 一旦清理成功，再次调用该函数时就会直接返回true，除非调用了Initialize*/
			static bool CleanupOnce();
		private:
			static bool& hasinit();
		};
	}
}
#endif