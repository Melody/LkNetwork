#ifndef LK_NETWORK_SUPERSESSION_
#define LK_NETWORK_SUPERSESSION_
#include<list>
namespace LK{
	namespace Network {
		namespace Tcp {
			/**
			@brief 会话对象*/
			struct SuperSession :BigObject {
				IMETHODS(PSuperSession, SuperSession)/*
				//会话源消息缓存（返回数据用）
				Collection::PDefaultMessageBuffer sourceRemote;
				//回复包
				Collection::PArray<u8> result;
				//消息列表
				Collection::List<Collection::PArray<u8> > msgList;*/
				//远端会话序号
				s32 remoteId;
				//会话号
				s32 sessionId;
				//消息列表
				std::list<std::vector<u8> >msgList;
				//锁
				std::recursive_mutex mt;
				//处理方法，一般启动几个处理线程
				virtual void Run() = 0;
				//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
				virtual void Close() = 0;
				//构造函数
				SuperSession(weakptr<SuperNode> wptr);
				//获取节点的弱引用
				weakptr<SuperNode>& node();
			private:
				//节点弱引用
				weakptr<BigObject> nd;
			};
			/**
			@brief 转发会话服务端*/
			struct SuperProxyServerSession:SuperSession {
				IMETHODS(PSuperProxyServerSession, SuperProxyServerSession)
				//处理方法
				void Run();
				//发送线程，不断监视消息列表的内容，发现有消息时立即发往目的套接字
				void SendingThread();
				//接收线程，不断把从目的套接字接收到的数据封包，并存入回传会话的消息列表
				void ReceivingThread();
				//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
				void Close();
				//回传会话id
				s32 returnId;
				//构造
				SuperProxyServerSession(std::vector<u8>const&msg,s32 id,weakptr<SuperNode> wptr);
				//状态：1正常，0：终止，-1:已终止
				volatile int sendingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int receivingstate;
				//目标套接字
				PSocket dest;
			};
			/**
			@brief 转发会话客户端*/
			struct SuperProxyClientSession :SuperSession {
				IMETHODS(PSuperProxyClientSession, SuperProxyClientSession)
				//处理方法
				void Run();
				//发送线程
				void SendingThread();
				//接收线程
				void ReceivingThread();
				//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
				void Close();
				//构造，remoteNode表示远端SuperNode的监听地址和端口
				SuperProxyClientSession(std::string remoteNode,std::string remotePortForRemote, weakptr<SuperNode> wptr);
				//状态：1正常，0：终止，-1:已终止
				volatile int sendingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int receivingstate;
				//收缓存
				Memory::Buffer recvBuffer;
				//发缓存
				Memory::Buffer sendBuffer;
				//远端节点地址
				std::string remote;
				//目标地址相对于远端节点的地址
				std::string remotePortForRemote;
				//获取与远端进行联系的会话
				PSuperNodeConnectionSession& connectSession();
			private:
				//与远端进行联系的会话
				ptr<BigObject> cSession;
			};
			/**
			@brief 节点间收发会话*/
			struct SuperNodeConnectionSession :SuperSession {
				IMETHODS(PSuperNodeConnectionSession, SuperNodeConnectionSession)
				//接收数据用缓存
				Collection::DefaultMessageBuffer receivingBuffer;
				//发送数据用缓存
				Collection::DefaultMessageBuffer sendingBuffer;
				//套接字
				PSocket sock;
				//套接字地址
				SocketAddress sockAddress;
				//处理方法，一般在里面启动多个线程
				virtual void Run();
				//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
				virtual void Close();
				//接收线程
				void ReceivingThread();
				//发送线程
				void SendingThread();
				//消息处理线程，处理和分发接收到的消息
				void MessageThread();
				//消息回传线程，将消息列表中的消息封包到回传列表
				void MessageReturnThread();
				//构造函数
				SuperNodeConnectionSession(SocketAddress const& addr,PSocket soc,weakptr<SuperNode> wptr);
				//状态：1正常，0：终止，-1:已终止
				volatile int sendingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int receivingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int messagestate;
				//状态：1正常，0：终止，-1:已终止
				volatile int messagereturnstate;
			};
			/**
			@brief 转发客户端监听会话*/
			struct SuperProxyListenSession :SuperSession {
				IMETHODS(PSuperProxyListenSession, SuperProxyListenSession)
				//处理方法
				void Run();
				//监听线程
				void ListeningThread();
				//发送线程
				void SendingThread(PSocket soc,PSuperProxyClientSession ses);
				//接收线程
				void ReceivingThread(PSocket soc,PSuperProxyClientSession ses);
				//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
				void Close();
				//构造，clientSessionId表示发往远端的
				SuperProxyListenSession(std::string listen, std::string remote,std::string remoteTarget, weakptr<SuperNode> wptr);
				//状态：1正常，0：终止，-1:已终止
				volatile int sendingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int receivingstate;
				//状态：1正常，0：终止，-1:已终止
				volatile int listeingstate;
				//本地套接字
				PSocket sock;
				//本地节点地址
				std::string local;
				//远端节点地址
				std::string remote;
				//目标地址相对于远端节点的地址
				std::string remotePortForRemote;
			};
		}
	}
}
#endif