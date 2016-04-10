#include"stdafx.h"
#ifdef LK_NETWORK_SUPERNODE_
namespace LK {
	namespace Network {
		namespace Tcp {
			/**
			@brief 使用一个本地监听端（字符串）构造超级节点*/
			SuperNode::SuperNode(std::string addrport)
				:listenSocket(new Socket(SocketType::Stream, ProtocolType::TCP)),
				currentSessionId(1),
				ReceivingBufferSize(DefaultReceivingBufferSize),
				SendingBufferSize(DefaultSendingBufferSize),
				timeout(DefaultTimeOut)
			{
				AddressInfo af(addrport);
				AddressElement const ae = af[0];
				listenSocket->Bind(ae.GetSocketAddressRef());
				//开始监听
				listenSocket->Listen();
				//启动监听线程
				Threading::StartThread(this, &SuperNode::ListeningThread);
			}
			index_t SuperNode::DefaultReceivingBufferSize = 8 * 1024;
			index_t SuperNode::DefaultSendingBufferSize = 8 * 1024;
			PTime SuperNode::DefaultTimeOut = 30;
			//监听线程
			void SuperNode::ListeningThread(){
				try {
					for (;;) {
						SocketAddress sa;
						PSocket s = listenSocket.Accept(&sa);
						//创建一个节点收发会话
						PSuperNodeConnectionSession ps= new SuperNodeConnectionSession(sa,s,this);
						//将会话添加到会话列表，获得会话id
						ps->sessionId =  AddSession(ps);
						ps->Run();
					}
				}
				catch (PException e) {
					std::cout << "监听异常退出:"<<e << std::endl;
				}
			}
			/**
			@brief 根据会话号获取会话*/
			PSuperSession SuperNode::GetSessionById(s32 id) {
				PSuperSession t;
				if (id < 0)return t;
				sessionMapLock.Lock();
				std::map<s32, PSuperSession>::iterator it = sessionMap.find(id);
				if (it != sessionMap.end())t = it->second;
				sessionMapLock.UnLock();
				return t;
			}
			/**
			@brief 设置指定会话id对应的会话（用以移除会话）*/
			void SuperNode::SetSessionById(s32 id, PSuperSession ps) {
				if (id < 0)Throw<PIndexOutOfRangeException>();
				sessionMapLock.Lock();
				if (ps==null){
					std::map<s32, PSuperSession>::iterator it = sessionMap.find(id);
					if (it != sessionMap.end())ps = it->second;
					sessionMap.erase(id);
					sessionMapLock.UnLock();
					if(!ps==null)ps->Close();
				}
				else {
					sessionMap[id] = ps;
					sessionMapLock.UnLock();
				}
			}
			/**
			@brief 添加会话并返回id*/
			s32 SuperNode::AddSession(PSuperSession ps) {
				s32 id;
				for (;;) {
					sessionMapLock.Lock();
					id = currentSessionId++;
					if (currentSessionId < 0)currentSessionId = 1;
					std::map<s32, PSuperSession>::iterator it = sessionMap.find(id);
					if (it == sessionMap.end())sessionMap[id]=ps;
					sessionMapLock.UnLock();
					if (it == sessionMap.end())break;
				}
				return id;
			}
			/**
			@brief 添加地址->收发会话映射*/
			void SuperNode::AddConnectSession(SocketAddress const&addr, PSuperNodeConnectionSession se) {
				socketMapLock.Lock();
				std::map<SocketAddress, Collection::PArray<PSuperNodeConnectionSession> >::iterator it = socketMap.find(addr);
				if (it == socketMap.end())socketMap[addr] = Collection::PArray<PSuperNodeConnectionSession >(1, se);
				else socketMap[addr].Append(se);
				socketMapLock.UnLock();
			}
			/**
			@brief 移除会话映射*/
			void SuperNode::RemoveConnectSession(SocketAddress const&addr, PSuperNodeConnectionSession soc) {
				socketMapLock.Lock();
				std::map<SocketAddress, Collection::PArray<PSuperNodeConnectionSession> >::iterator it = socketMap.find(addr);
				if (it != socketMap.end()) {
					Collection::PArray<PSuperNodeConnectionSession> arr = socketMap[addr];
					for (index_t i = 0; i < arr.size(); ++i)if (arr[i].Equals(soc))arr.RemoveAt(i);
					if (arr.size() == 0)
						socketMap.erase(addr);
				}
				socketMapLock.UnLock();
			}
			/**
			@brief 获取指定地址的会话，如果不存在则建立会话并将其添加到套接字列表中*/
			PSuperNodeConnectionSession SuperNode::FindOrAddConnectSession(std::string addrport) {
				AddressInfo af(addrport);
				SocketAddress const& sf = af[0].GetSocketAddressRef();
				socketMapLock.Lock();
				std::map<SocketAddress, Collection::PArray<PSuperNodeConnectionSession> >::iterator it = socketMap.find(sf);
				PSuperNodeConnectionSession ps;
				if (it == socketMap.end()) {
					PSocket soc = new Socket(SocketType::Stream, ProtocolType::TCP);
					std::cout << "连接远端节点" << std::endl;
					soc.Connect(sf);
					ps = new SuperNodeConnectionSession(sf, soc, this);
					ps->sessionId = AddSession(ps);
					AddConnectSession(sf,ps);
					ps->Run();
				}
				else ps = it->second[PRandom::GetDefaultRef().NextInt(0, (int)it->second.size() - 1)];//有多个连接时随机选一个
				socketMapLock.UnLock();
				return ps;
			}
		}
	}
}
#endif