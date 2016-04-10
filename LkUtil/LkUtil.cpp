// Ⱥ�����Һ���.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace LK;
using namespace LK::Network;
//�׽��־��
ptr<Socket> sock;
//���ݽ��
std::string unpack(std::vector<u8> const&arr) { std::string str; str.resize((int&)arr[0]); LK::memcpy(&str[0], &arr[4], str.size()); return str; }
//���ݷ����ͷ���ֽڱ�ʾ���ݳ���
std::vector<u8> pack(std::string const& str) { std::vector<u8> tmp(str.size() + 4); ((int&)tmp[0]) = str.size(); LK::memcpy(&tmp[4], str.data(), str.size()); return tmp; }
//�������ṩ�Ľӿ�
void login(std::string qun, std::string name = "");//����������¼ָ����Ⱥ�ţ��Խ��ܸ�Ⱥ��Ϣ��nameΪ�û�����ȱʡֵΪip�š������Ⱥ�ű�����Ⱥ���Ѿ����е�Ⱥ���У�������潫�޷�����
void message(std::string msg, std::string qun, std::string qq);//��������������Ϣʱ���ú����ᱻ���ã�����msgΪ��Ϣ���ݣ�qun����ϢȺ�ţ�qq�Ƿ���Ϣ�˵�QQ��.�ú������ݿ����Լ���д
void speak(std::string str, std::string qun = "");//ͨ����ʽ��������ָ��Ⱥ�﷢�ԣ����qunΪ�գ�����������login��Ⱥ���ԡ�strΪ�������ݡ�
//���ݽ����߳�
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
		if (len > 100000)throw std::logic_error("�յ�����Ϣ����");
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
			//������֮���ȵ�¼
			login("244953928", "�ʴ�������");
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

void message(std::string msg, std::string qun, std::string qq){//�ú����������б�д
	std::cout << "��QȺ" << qun << "�յ�" << qq << "��������Ϣ��\n" << msg << std::endl;
	static int val = 0;
	speak("�� �� ����", qun);
}
void speak(std::string str, std::string qun) {
	std::cout << "��QȺ" << qun << "������" << str << std::endl;
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
