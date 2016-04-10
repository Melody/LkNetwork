// 群主帮我喊话.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace LK;
using namespace LK::Network;
//套接字句柄
ptr<Socket> sock;
//数据解包
std::string unpack(std::vector<u8> const&arr) { std::string str; str.resize((int&)arr[0]); LK::memcpy(&str[0], &arr[4], str.size()); return str; }
//数据封包：头四字节表示数据长度
std::vector<u8> pack(std::string const& str) { std::vector<u8> tmp(str.size() + 4); ((int&)tmp[0]) = str.size(); LK::memcpy(&tmp[4], str.data(), str.size()); return tmp; }
//以下是提供的接口
void login(std::string qun, std::string name = "");//（主动）登录指定的群号，以接受该群消息。name为用户名，缺省值为ip号。这里的群号必须是群主已经加有的群才行，否则后面将无法发言
void message(std::string msg, std::string qun, std::string qq);//（被动）当有消息时，该函数会被调用，其中msg为消息内容，qun是消息群号，qq是发消息人的QQ号.该函数内容可以自己编写
void speak(std::string str, std::string qun = "");//通过算式计算器在指定群里发言，如果qun为空，则在所有已login的群发言。str为发言内容。
//数据接收线程
void receiveThread() {
	Memory::Buffer buf;
	buf.SetEnableThreadSafe(false);
	for (;;) {
		char tmp[1024];
		int len = sock->Receive(tmp, 1024);
		if (buf.GetUnreadSize() + buf.GetBackMinimum() + len > buf.GetLength())
			buf.Resize(buf.GetUnreadSize() + buf.GetBackMinimum() + len);
		buf.Write(tmp, len);
		if (buf.GetUnreadSize() < 4)continue;
		buf.Read(&len, 4);
		if (len > 100000)throw std::logic_error("收到的消息过长");
		buf.SkipBack(4);
		if (buf.GetUnreadSize() < 4 + len)continue;
		std::vector<u8> t(len + 4);
		buf.Read(t, t.size());
		std::vector<std::string> cmds = Strings::FromCSV(unpack(t));
		if (cmds[0] == "message")
			message(cmds[1], cmds[3], cmds[4]);
	}
}

int main()
{
	sock = new Socket(SocketType::Stream, ProtocolType::TCP);
	AddressInfo ai("febrime.top:80");
	if (ai.size() > 0) {
		try {
			sock->Connect(ai[0].GetSocketAddressRef(), ai[0].GetAddrLen());
			//连接上之后先登录
			login("244953928", "笔代机器人");
			receiveThread();
		}
		catch (SocketException e) {
			std::cout << e.what() << std::endl;
			std::cin.get();
		}
		try {
			sock->Close();
		}
		catch (...) {}
	}
	return 0;
}

void login(std::string qun, std::string name) {
	std::vector<std::string> cmds = { "login", qun, name };
	std::vector<u8> tmp = pack(Strings::ToCSV(cmds));
	int len = 0;
	while (len < tmp.size())
	{
		int val = sock->Send(&tmp[len], tmp.size() - len);
		if (val == 0)break;
		len += val;
	}
}

void message(std::string msg, std::string qun, std::string qq){//该函数可以自行编写
	std::cout << "从Q群" << qun << "收到" << qq << "发来的消息：\n" << msg << std::endl;
	static int val = 0;
	speak("真 · 好玩", qun);
}
void speak(std::string str, std::string qun) {
	std::cout << "向Q群" << qun << "喊话：" << str << std::endl;
	std::vector<std::string> cmds = { "speak", str, qun };
	std::vector<u8> tmp = pack(Strings::ToCSV(cmds));
	int len = 0;
	while (len < tmp.size())
	{
		int val = sock->Send(&tmp[len], tmp.size() - len);
		if (val == 0)break;
		len += val;
	}

}
