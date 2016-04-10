#pragma once
namespace LK
{
	namespace Network
	{
		/**
		@brief 网络异常*/
		struct NetworkException :std::logic_error {
			//使用一个错误描述字符串来生成异常
			NetworkException(std::string const& str="网络异常") :std::logic_error(str) {}
		};
		/**
		@brief 网络初始化异常*/
		struct NetworkInitializingException :NetworkException {
			//使用一个错误描述字符串来生成异常
			NetworkInitializingException(std::string const& str = "网络初始化异常") :NetworkException(str) {}
		};
		/**
		@brief 套接字异常*/
		struct SocketException :NetworkException {
			//使用一个错误描述字符串来生成异常
			SocketException(std::string const& str = "套接字异常") :NetworkException(str) {}
		};
	}
}