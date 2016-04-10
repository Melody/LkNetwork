#include "stdafx.h"
#ifdef LK_STRINGS_SPLIT_
namespace LK {
	namespace Strings {
		std::vector<std::string > Split(std::string const&str, int ch, bool removeEmpty, bool ignoreCase, size_t maxCount) {
			std::vector<std::string > strs;
			if (maxCount == 0)return strs;
			if (ignoreCase)ignoreCase = (ch&~32) <= 'Z' && (ch&~32) >= 'A';
			size_t left = 0;
			for (size_t i = 0; i < str.length();++i) {
				if (str[i] < 0) {
					if ((u8)str[i] * 256 + (u8)str[i+1] == ch) {
						if (!removeEmpty || i != left) {
							strs.push_back(str.substr(left, i - left));
							if (strs.size() == maxCount)return strs;
						}
						left = i+2;
					}
					++i;
				}
				else if (str[i] == ch ||(ignoreCase&&((str[i]&~32)==(ch&~32)))) {
					if (!removeEmpty || i != left) {
						strs.push_back(str.substr(left, i - left));
						if (strs.size() == maxCount)return strs;
					}
					left = i + 1;
				}
			}
			if(!removeEmpty || left!=str.length())strs.push_back(str.substr(left));
			return strs;
		}
		std::vector<std::string > Split(std::string const&str, std::string const& s, bool removeEmpty, bool ignoreCase, size_t maxCount) {
			std::vector<std::string > strs;
			if (maxCount == 0)return strs;
			char ch = s[0];
			if (ignoreCase)ch = (ch<='z'&&ch>='a')?(ch&~32):ch;
			size_t left = 0;
			for (size_t i = 0; i < str.length(); ++i) {
				if (str[i] == ch || (ignoreCase && ch>='A'&&ch<='Z' &&((str[i] & ~32) == ch))) {
					size_t k = 0;
					while (k < s.length()) {
						if (str[i + k] != s[k] && (!ignoreCase || (str[i + k] & ~32) != (s[k] & ~32) || (s[k] & ~32) < 'A' || (s[k] & ~32) > 'Z'))break;
						k++;
					}
					if (k == s.length()) {
						if (!removeEmpty || i != left) {
							strs.push_back(str.substr(left, i - left));
							if (strs.size() == maxCount)return strs;
						}
						left = i + s.length();
					}
				}
			}
			if(!removeEmpty || left!=str.length())strs.push_back(str.substr(left));
			return strs;
		}
		//从csv字符串转换为正常字符串
		std::vector<std::string> FromCSV(std::string const& str) {
			if (str == "")return std::vector<std::string>();
			int i = 0, last = -1;
			bool ins = false;
			std::vector<std::string> strlist;
			int cnt = 0;//双引号计数
			while (i <= str.length()) {
				if (ins == false) {//非双引号内部
					if (i == str.length() || str[i] == ',') {//i为分隔位置
						int lst = last + 1;
						std::string s = str.substr(lst, i - lst);//隔间字符串
																 //节点之间为双引号引起来的模式
						if (s.length() != 0 && s[0] == '"') {
							s = s.substr(1, s.length() - 2);
							for (size_t k = 0; k < s.length() - 1; ++k)
								if (s[k] == '\"'&&s[k + 1] == '\"')s.erase(k + 1, 1);
						}
						strlist.push_back(s);
						last = i;//记录上一个逗号
					}
					else if (str[i] == '"' && last == i - 1) {//逗号紧接双引号，进入双引号内部模式
						ins = true;
						cnt = 0;//内部双引号计数
					}
				}
				else {
					if (i + 1 >= str.length()) { if (str.length() == i)i--; ins = false; }
					else if (str[i] == '"') {
						if (((++cnt & 1) != 0) && str[i + 1] == ',') ins = false;//退出双引号内部
					}
				}
				i++;
			}
			return strlist;
		}
		//打包为csv字符串
		std::string ToCSV(std::vector<std::string>const&strs) {
			if (strs.size() == 0)return "";
			std::string str = "";
			for (int i = 0; i < strs.size(); ++i) {
				if (strs[i].length() != 0) {
					if (strs[i].find(",") != -1 || strs[i].find("\n") != -1 ||strs[i][0] == '\"')
					{
						std::string s = strs[i];
						for (size_t k = 0; k < s.length(); ++k)
							if (s[k] == '\"')s.insert(++k, 1, '\"');
						str += "\"" + s + "\"";
					}
					else str += strs[i];
				}
				if (i + 1 != strs.size())str += ",";
			}
			return str;
		}

	}
}
#endif