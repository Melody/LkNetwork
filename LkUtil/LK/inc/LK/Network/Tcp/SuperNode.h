#ifndef LK_NETWORK_SUPERNODE_
#define LK_NETWORK_SUPERNODE_
#include<map>
namespace LK{
	namespace Network {
		namespace Tcp {
			struct SuperNodeError{
				static const int Prohibited = -1;
				static const int NoSuchSession = -2;
				static const int EstablishFailed = -3;
			};

			/**
			@brief 表示一个超级节点。超级节点使用自定义的协议，实现数据转发、文件传输、远程指令等功能
			总体思路：
			1.超级节点在本机开设一个监听端口，但封包可以通过一个或者多个通道传输到另外的超级节点
			2.第一次连接进行业务和会话号申请，之后每个会话按照第一次连接时申请的协议进行操作
			技术问题：
			要实现会话号到处理过程的映射*/
			struct SuperNode :BigObject{
				IMETHODS(PSuperNode, SuperNode)
				/**
				@brief 使用一个本地监听端（字符串）构造超级节点*/
				SuperNode(std::string const& listenPoint);
				/**
				@brief 接收缓冲块大小*/
				static index_t DefaultReceivingBufferSize;
				/**
				@brief 接收缓存大小*/
				index_t ReceivingBufferSize;
				/**
				@brief 发送缓冲块大小*/
				static index_t DefaultSendingBufferSize;
				/**
				@brief 发送缓存大小*/
				index_t SendingBufferSize;
				/**
				@brief 根据会话号获取会话*/
				PSuperSession GetSessionById(s32 id);
				/**
				@brief 设置指定会话id对应的会话（也用以移除会话）*/
				void SetSessionById(s32 id, PSuperSession ps);
				/**
				@brief 添加会话并返回id*/
				s32 AddSession(PSuperSession ps);
				/**
				@brief 添加地址->收发会话映射*/
				void AddConnectSession(SocketAddress const&addr, PSuperNodeConnectionSession se);
				/**
				@brief 移除会话映射*/
				void RemoveConnectSession(SocketAddress const&addr, PSuperNodeConnectionSession soc);
				/**
				@brief 获取指定地址的会话，如果不存在则建立会话并将其添加到套接字列表中*/
				PSuperNodeConnectionSession FindOrAddConnectSession(std::string addrport);
				/**
				@brief 等待时限，默认是30s*/
				std::chrono::milliseconds timeout;
				/**
				@brief 默认的超时时间*/
				static std::chrono::milliseconds DefaultTimeOut;
			protected:
				//监听套接字
				PSocket listenSocket;
				//当前会话号
				s32 currentSessionId;
				//映射：会话号->处理会话对象
				std::map<s32, PSuperSession>sessionMap;
				//会话map锁
				std::recursive_mutex sessionMapLock;
				//映射：地址->套接字对象
				std::map<SocketAddress, std::vector<PSuperNodeConnectionSession> >socketMap;
				//会话map锁
				std::recursive_mutex socketMapLock;
				//监听线程
				void ListeningThread();
			};
		}
	}
}
#endif