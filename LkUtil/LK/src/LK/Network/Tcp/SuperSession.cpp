#include"stdafx.h"
#ifdef LK_NETWORK_SUPERNODE_
namespace LK {
	namespace Network {
		namespace Tcp {
			//构造函数
			SuperSession::SuperSession(weak<PSuperNode> wptr)
				:nd(wptr), remoteId(0),sessionId(0) {
				RefCount.SetEnableThreadSafe(true);
				sessionId = wptr->AddSession(this);
			}
			weak<PSuperNode>& SuperSession::node() { return (weak<PSuperNode>&)nd; }
			//处理线程，将信息解包传到目的地
			void SuperProxyServerSession::SendingThread() {
				PObject st = this;//防侧漏
				std::cout << "启动转接服务发送线程" << std::endl;
				PTime time = node()->timeout==null ? node()->timeout : (node()->timeout + Time::Now);
				Collection::PArray<u8>* p=NULL;
				try {
					int cnt = 0;
					while (sendingstate == 1&&receivingstate==1){
						if (msgList.size() != 0){
							cnt = 0;
							mt.Lock();
							p = msgList.PickOutPointer(0);
							mt.UnLock();
							dest.Send((u8*)*p + 5, p->size()-5);
							delete p;
							p = NULL;
							if(!time==null)time = node()->timeout + Time::Now;
						}
						else cnt++;
						if (cnt != 0) {
							if (cnt > 10) {
								if (Time::Now < time) {
									//检测一下连接是否可用
								}
								Sleep(10);
							}
							else Sleep(0);
						}
					}
				}
				catch (PException) {
					if (p)delete p;
				}
				sendingstate = -1;
				//自关闭
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//接收线程，将目的地的信息打包返回
			void SuperProxyServerSession::ReceivingThread() {
				std::cout << "启动转接服务接收线程" << std::endl;
				PObject st = this;//防侧漏
				u8* p = NULL;
				Collection::DefaultMessageBuffer* sourceRemote=NULL;
				try {
					sourceRemote = &(node()->GetSessionById(returnId).CastTo<PSuperNodeConnectionSession>()->sendingBuffer);
					p = new u8[node()->ReceivingBufferSize];
					*p = 0;
					*(s32*)(p + 1) = remoteId;
					int cnt = 0;
					int len=0;
					while (sendingstate == 1 && receivingstate == 1) {
						if ((len = (int)dest.Receive(p+5, node()->ReceivingBufferSize-5))!=0){
							cnt = 0;
							sourceRemote->PackToBuffer(p, len + 5);
						}
						else cnt++;
						if (cnt < 10)Sleep(0);
						else Sleep(10);
					}
				}
				catch (PException) {
				}
				*p = -1;//关闭会话
				*(s32*)(p + 1) = remoteId;
				if(sourceRemote)sourceRemote->PackToBuffer(p,5);
				receivingstate = -1;
				//自关闭
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
			void SuperProxyServerSession::Close() {
				if (sendingstate == 1)sendingstate = 0;
				if (receivingstate == 1)receivingstate = 0;
				while (sendingstate == 1 || receivingstate == 1){
					Sleep(10);
				}
				dest.Close();
			}
			//构造
			SuperProxyServerSession::SuperProxyServerSession(Collection::PArray<u8>const&msg, s32 id, weak<PSuperNode> wptr)
				:SuperSession(wptr),sendingstate(1),receivingstate(1), returnId(id){
				std::cout << "构建转接服务会话" << std::endl;
				msgList.Append(msg);
			}
			void SuperProxyServerSession::Run() {
				Collection::PArray<u8> msg = msgList[0];
				msgList.RemoveAt((index_t)0);
				u8* p = msg;
				remoteId = *(s32*)(p + 1);//源会话id
				std::cout << "源会话id:" << remoteId << std::endl;
				u8 result[9];
				result[0] = 1;
				*(s32*)(result + 1) = remoteId;
				Collection::DefaultMessageBuffer* buf=NULL;
				try {
					buf = &(node()->GetSessionById(returnId).CastTo<PSuperNodeConnectionSession>()->sendingBuffer);
					std::cout << "解析目标地址：" << (char*)(p + 5) << std::endl;
					AddressInfo af((char*)(p + 5));//解析地址
					dest = new Socket(SocketType::Stream, ProtocolType::TCP);
					std::cout << "连接目标" << std::endl;
					SocketAddress const sa = af[0].GetSocketAddressRef();
					dest.Connect(sa);
					*(s32*)(result + 5) = sessionId;
				}
				catch (PException e) {
					std::cout << "转接服务会话异常：" << e << std::endl;
					sendingstate = -1;
					receivingstate = -1;
					*(s32*)(result + 5) = SuperNodeError::EstablishFailed;
					node()->SetSessionById(sessionId, PSuperSession());
				}
				std::cout << "回复" << std::endl;
				if (buf)buf->PackToBuffer(result,sizeof(result));
				if (sendingstate == 1)
					Threading::StartThread(this, &SuperProxyServerSession::SendingThread);
				if (receivingstate == 1)
					Threading::StartThread(this, &SuperProxyServerSession::ReceivingThread);
			}
			//处理方法
			void SuperProxyClientSession::Run() {
				try {
					 //获取连接会话
					std::cout << "连接远端会话" << std::endl;
					connectSession() = node()->FindOrAddConnectSession(remote);
					//发送连接请求
					std::cout << "发送转接请求，请求对方连接："+remotePortForRemote << std::endl;
					int len = (int)LK::String::Strlen(remotePortForRemote.ToAnsi()) + 1;
					u8* arr = new u8[5+len];
					for (int i = 0; i < len; ++i)arr[i + 5] = remotePortForRemote.ToAnsi()[i];
					*arr = 2;
					*(s32*)(arr + 1) = sessionId;
					connectSession()->sendingBuffer.PackToBuffer(arr, len + 5);
					//启动发送线程
					for (int i = 0; i < 10000; ++i) {
						Sleep(1);
						if (msgList.size() == 0) {
							if (connectSession()->sendingstate != 1)
								Throw<SocketException>(L"节点间连接会话终止");
							continue;
						}
						mt.Lock();
						Collection::PArray<u8> p = msgList.FirstElement();
						msgList.RemoveAt((index_t)0);
						mt.UnLock();
						if(p[0]!=1 || *(s32*)(&p[1])!=sessionId)Throw<SocketException>();
						if(*(s32*)(&p[5])<=0)Throw<PInvalidArgumentException>();
						remoteId = *(s32*)(&p[5]);
						break;
					}
					//启动接收线程
					Threading::StartThread(this, &SuperProxyClientSession::SendingThread);
					Threading::StartThread(this, &SuperProxyClientSession::ReceivingThread);
				}
				catch (PException e) {
					std::cout << e << std::endl;
					node()->SetSessionById(sessionId, PSuperSession());
				}
			}
			//发送线程
			void SuperProxyClientSession::SendingThread() {
				PObject st = this;//防侧漏
				u8* p = new u8[node()->SendingBufferSize];
				*p = 0;
				*(s32*)(p + 1) = remoteId;
				int len;
				try {
					int cnt = 0;
					while (sendingstate == 1 ) {
						if ((len= (int)sendBuffer.ReadLittle(p+5, node()->SendingBufferSize-5))!=0) {
							cnt = 0;
							connectSession()->sendingBuffer.PackToBuffer(p,len+5);
						}
						else cnt++;
						if (cnt < 10)Sleep(0);
						else Sleep(10);
					}
				}
				catch (PException) {
				}
				delete p;
				sendingstate = -1;
				//自关闭
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//发送线程
			void SuperProxyClientSession::ReceivingThread() {
				PObject st = this;//防侧漏
				u8* p = 0;
				int len;
				try {
					int cnt = 0;
					while (receivingstate == 1) {
						if (msgList.size() != 0) {
							mt.Lock();
							Collection::PArray<u8>*parr=NULL;
							if (msgList.size() != 0) {
								cnt = 0;
								parr = msgList.PickOutPointer((index_t)0);
							}
							mt.UnLock();
							if (parr) {
								len = (int)parr->size();
								p = *parr;
								if ((s8)*p == -1)//关闭套接字
									receivingstate = 0;
								else
									recvBuffer.Write(p + 5, len - 5);
								delete parr;
							}
						}
						else cnt++;
						if (cnt < 10)Sleep(0);
						else Sleep(10);
					}
				}
				catch (PException) {
				}
				delete p;
				receivingstate = -1;
				//自关闭
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
			void SuperProxyClientSession::Close() {
				if (sendingstate == 1)sendingstate = 0;
				if (receivingstate == 1)receivingstate = 0;
				while (sendingstate == 1 || receivingstate == 1) {
					Sleep(10);
					recvBuffer.Reset();
					sendBuffer.Reset();
				}
			}
			//构造
			SuperProxyClientSession::SuperProxyClientSession(std::string remoteNode, std::string rPortForRemote, weak<PSuperNode> wptr)
				:SuperSession(wptr),sendingstate(1),receivingstate(1),remote(remoteNode), remotePortForRemote(rPortForRemote){
				std::cout << "转发客户端会话创建" << std::endl;
			}
			PSuperNodeConnectionSession& SuperProxyClientSession::connectSession() { return (PSuperNodeConnectionSession&)cSession; }

			//处理方法，一般在里面启动多个线程
			void SuperNodeConnectionSession::Run() {
				std::cout << "启动节点间收发线程" << std::endl;
				Threading::StartThread(this, &SuperNodeConnectionSession::ReceivingThread);
				Threading::StartThread(this, &SuperNodeConnectionSession::SendingThread);
				Threading::StartThread(this, &SuperNodeConnectionSession::MessageThread);
				Threading::StartThread(this, &SuperNodeConnectionSession::MessageReturnThread);
			}
			//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
			void SuperNodeConnectionSession::Close() {
				if (sendingstate == 1)sendingstate = 0;
				if (receivingstate == 1)receivingstate = 0;
				if (messagestate == 1)messagestate = 0;
				while (sendingstate == 1 || receivingstate == 1 || messagestate == 1 || messagereturnstate == 1) {
					sendingBuffer.buffer.Reset();
					receivingBuffer.buffer.Reset();
					Sleep(10);
				}
				sock.Close();
				node()->RemoveConnectSession(sockAddress, this);
			}
			//接收线程
			void SuperNodeConnectionSession::ReceivingThread() {
				PObject tmpst = this;//自引用防侧漏
				u8* dat = new u8[node()->ReceivingBufferSize];
				receivingBuffer.RefCount.SetEnableThreadSafe(true);
				//启动消息接收线程
				try {
					while (receivingstate==1) {
						index_t len = sock.Receive(dat, node()->ReceivingBufferSize);
						std::cout << "节点收到数据" << std::endl;
						//未经整理的数据放入缓存
						receivingBuffer.buffer.Write(dat, len);
					}
				}
				catch (PException e) {
					std::cout << "节点收信异常：" << e << std::endl;
				}
				delete[]dat;
				receivingstate = -1;
				if(!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//发送线程
			void SuperNodeConnectionSession::SendingThread() {
				PObject tmpst = this;//防侧漏
				sendingBuffer.RefCount.SetEnableThreadSafe(true);
				u8* bf = new u8[node()->SendingBufferSize];
				try {
					int scnt = 0;
					while (sendingstate==1) {
						index_t len = sendingBuffer.buffer.ReadLittle(bf, node()->SendingBufferSize);
						if (len == 0)scnt++;
						else {
							scnt = 0;
							std::cout << "节点检测到数据，开始发送" << std::endl;
							sock.Send(bf, len);
						}
						if (scnt < 10)
							Sleep(0);
						else {
							Sleep(sendingBuffer.buffer.sleepTime);
							continue;
						}
					}
				}
				catch (PException e) {
					std::cout << "节点发信异常：" << e << std::endl;
				}
				delete[]bf;
				sendingstate = -1;//发生异常直接退出
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//消息回传线程，将消息列表中的消息封包到回传列表
			void SuperNodeConnectionSession::MessageReturnThread() {
				PObject tmpst = this;//自引用防侧漏
				int cnt = 0;
				while (messagereturnstate == 1) {
					if (msgList.size() == 0) {
						cnt++;
					}
					else {
						mt.Lock();
						if (msgList.size() == 0)
							mt.UnLock();
						else {
							cnt = 0;
							Collection::PArray<u8>* p = msgList.PickOutPointer(0);
							mt.UnLock();
							try {
								sendingBuffer.PackToBuffer((u8*)*p, (u32)p->size());
							}
							catch (PException e) {
								std::cout << "消息列表处理异常" <<e<< std::endl;
							}
							delete p;
							continue;
						}
					}
					if (cnt < 10)Sleep(0);
					else Sleep(10);
				}
				//关闭回传线程
				messagereturnstate = -1;
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//消息处理线程，解包处理和分发
			void SuperNodeConnectionSession::MessageThread() {
				PObject tmpst = this;//自引用防侧漏
				s32 destSessionId = 0;//目标会话id
				try {
					Collection::PMessagePack pack;
					while (messagestate==1) {
						pack = receivingBuffer.ReadMessage();
						std::cout << "节点收到消息" << std::endl;
						index_t len = pack->data.size();
						u8* p = pack->data;
						len = (int)pack->data.size();
						if (len <= 5) {//内容过少违规直接丢弃
							continue;
						}
						s8 cmd = *p++;
						s32 sourceSessionId = *(s32*)p;
						std::cout << "源会话id：" << sourceSessionId << std::endl;
						p += 4;
						u8 result[9] = { 0 };
						result[0] = 1;//答复
						switch (cmd) {
						case -1: {//关闭会话
							PSuperSession ps = node()->GetSessionById(sourceSessionId);
							if (!ps==null)ps->Close();
						}break;
						case 0: 
						case 1: {//内容包，传递到会话
							PSuperSession ps = node()->GetSessionById(sourceSessionId);
							if (ps==null) { *(s32*)(result + 5) = SuperNodeError::NoSuchSession; goto Err; }
							std::cout << "内容包(答复)分派消息列表" << std::endl;
							ps->mt.Lock();
							ps->msgList.Append(pack->data);
							ps->mt.UnLock();
						}break;
						case 2: {//转发请求
							try {
								std::cout << "收到转接请求" << std::endl;
								PSuperSession ps = new SuperProxyServerSession(pack->data, sessionId, node());
								ps->Run();
							}
							catch (PException e) {
								std::cout <<"转接处理异常:"<< e << std::endl;
								*(s32*)(result + 5) = SuperNodeError::EstablishFailed;
								goto Err;
							}
						}break;
						case 3: {//文件传送请求
						}break;
						case 4: {//测速请求
						}break;
						case 5: {//系统指令请求
						}break;
						default: {//禁止未知会话
							*(s32*)(result + 1) = sourceSessionId;
							*(s32*)(result + 5) = -1;
							sendingBuffer.PackToBuffer(result, sizeof(result));//答复
						}break;
						}
						continue;
					Err:;//异常回复
						*(s32*)(result + 1) = sourceSessionId;
						sendingBuffer.PackToBuffer(result, sizeof(result));//答复
					}
				}
				catch (PException e) {
					std::cout << "消息处理异常" << e << std::endl;
				}
				//关闭接收线程
				messagestate = -1;
				if (!node()==null)node()->SetSessionById(sessionId, PSuperSession());
			}
			//构造函数
			SuperNodeConnectionSession::SuperNodeConnectionSession(SocketAddress const&addr,PSocket soc, weak<PSuperNode>wptr)
			:SuperSession(wptr),sock(soc), messagestate(1),sendingstate(1),receivingstate(1), messagereturnstate(1), sockAddress(addr){
				std::cout << "构建节点间连接会话" << std::endl;
			}
			//处理方法
			void SuperProxyListenSession::Run(){
				try{
					AddressInfo af(local);
					sock = new Socket(SocketType::Stream,ProtocolType::TCP);
					sock.Bind(af[0].GetSocketAddressRef());
					sock.Listen();
					std::cout << local << "监听成功,启动接收线程" << std::endl;
					Threading::StartThread(this, &SuperProxyListenSession::ListeningThread);
				}
				catch (PException e) {
					std::cout << e << std::endl;
				}
			}
			//监听线程
			void SuperProxyListenSession::ListeningThread() {
				PObject tmpst = this;//引用自身，谨防侧漏
				try {
					while (listeingstate == 1) {
						SocketAddress sa;
						PSocket soc = sock.Accept(&sa);
						std::cout << "收到请求:" <<sa.ToString()<<"创建会话"<< std::endl;
						//创建发送客户端会话
						PSuperProxyClientSession pspc = new SuperProxyClientSession(remote, remotePortForRemote, node());
						std::cout << "启动" << sa.ToString() << "的监听端收发线程" << std::endl;
						//创建线程
						Threading::StartThread(this, &SuperProxyListenSession::SendingThread, soc, pspc);
						Threading::StartThread(this, &SuperProxyListenSession::ReceivingThread, soc, pspc);
						pspc->Run();
					}
				}
				catch (PException) {}
			}
			//发送线程
			void SuperProxyListenSession::SendingThread(PSocket soc, PSuperProxyClientSession ses) {
				PObject tmpst = ses;
				PObject tmpthis = this;
				u8* p = NULL;
				try {
					p = new u8[node()->SendingBufferSize];
					int cnt = 0;
					while (sendingstate == 1) {
						int len;
						if ((len = (int)soc.Receive(p, node()->SendingBufferSize)) == 0)++cnt;
						else {
							cnt = 0;
							ses->sendBuffer.Write(p, len);
							continue;
						}
						if (cnt < 10)Sleep(0);
						else Sleep(10);
					}
				}
				catch (PException e) {
					std::cout <<"本地接收错误"<< e << std::endl;
				}
				delete []p;

			}
			//接收线程
			void SuperProxyListenSession::ReceivingThread(PSocket soc, PSuperProxyClientSession ses) {
				PObject tmpst = ses;
				PObject tmpthis = this;
				u8* p = NULL;
				try {
					p = new u8[node()->ReceivingBufferSize];
					int cnt = 0;
					while (receivingstate == 1) {
						int len;
						if ((len = (int)ses->recvBuffer.ReadLittle(p, node()->ReceivingBufferSize)) == 0)++cnt;
						else {
							cnt = 0;
							soc.Send(p, len);
							continue;
						}
						if (cnt < 10)Sleep(0);
						else Sleep(10);
					}
				}
				catch (PException) {}
				delete[]p;
			}
			//关闭处理线程（该函数返回后要保证处理线程不再访问数据）
			void SuperProxyListenSession::Close() {
				if (sendingstate == 1)sendingstate = 0;
				if (receivingstate == 1)receivingstate = 0;
				if (listeingstate == 1)listeingstate = 0;
				sock.Close();
			}
			//构造，clientSessionId表示发往远端的
			SuperProxyListenSession::SuperProxyListenSession(std::string listen, std::string remt, std::string remoteTarget, weak<PSuperNode> wptr)
			:SuperSession(wptr),sendingstate(1),receivingstate(1), listeingstate(1),local(listen),remote(remt),remotePortForRemote(remoteTarget){
				std::cout << "构建监听端："+listen << std::endl;
			}

		}
	}
}
#endif