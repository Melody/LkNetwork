#include "stdafx.h"
#ifdef LK_STRINGS_INDEXOF_
namespace LK {
	namespace Strings {
		index_t IndexOf(std::string const& source, char ch,size_t startIndex,bool ignoreCase) {
			if (source.length() == 0)return -1;
			if (startIndex > source.length())Throw<std::out_of_range>("起始位置大于源字符串长");
			char const* p;
			if (ignoreCase&&((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))){
				if ((ch >= 'a'&&ch <= 'z'))ch -= 32;
				for (size_t i = startIndex; i < source.length(); ++i)
				{
					if (source[i] == ch)return i;
					if (source[i] == ch + 32&&source[i] >= 'a'&&source[i] <= 'z')return i;
				}
				return -1;
			}
			p = strchr(source.c_str()+startIndex, ch);
			if (p == nullptr)return -1;
			return p - source.c_str();
		}		//查找str在source中第一次出现的位置，找不到返回-1
		index_t LastIndexOf(std::string const& source, std::string const& str) {
			size_t hash = 0;
			size_t i;
			for (i = 0; i != str.length();++i) {
				hash += str[i];
			}
			size_t h = 0;
			for (i = source.length() - str.length(); i != source.length(); ++i)
				h += source[i];
			i = source.length() - str.length();
			do {
				if (h == hash&&source[i] == str[0] && memcmp(&source[i], &str[0], str.length()) == 0)return i;
				h -= source[i + str.length() - 1];
				h += source[i - 1];
			} while (i-- != 0);
			return -1;
		}
		//查找ch在source中第一次出现的位置，找不到返回-1
		index_t LastIndexOf(std::string const& source, char ch) {
			for (size_t i = source.length(); i != 0;) {
				if(source[--i]==ch)return i;
			}
			return -1;
		}

	}
}
#endif