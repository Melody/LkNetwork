namespace LK{
	namespace Network {
		namespace Tcp {
			//表示一个多功能节点
			struct SuperNode;
			//表示一个多功能节点发起的会话对象
			struct SuperSession;
			//表示一个代理服务端会话
			struct SuperProxyServerSession;
			//表示一个节点连接传输会话
			struct SuperNodeConnectionSession;
			//表示一个转发的本地监听端
			struct SuperProxyListenSession;
			//表示一个代理客户端会话
			struct SuperProxyClientSession;

			typedef ptr<SuperNode> PSuperNode;
			typedef ptr<SuperSession> PSuperSession;
			typedef ptr<SuperProxyServerSession> PSuperProxyServerSession;
			typedef ptr<SuperProxyClientSession> PSuperProxyClientSession;
			typedef ptr<SuperNodeConnectionSession> PSuperNodeConnectionSession;
			typedef ptr<SuperProxyListenSession> PSuperProxyListenSession;
		}
	}
}