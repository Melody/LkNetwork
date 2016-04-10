#include "stdafx.h"
#ifdef LK_STRINGS_COUNTOF_
namespace LK {
	namespace Strings {
		size_t CountOf(std::string const& source, std::string const& str, size_t startIndex) {
			size_t cnt = 0;
			if (startIndex > source.length())Throw<std::out_of_range>("起始位置超过源字符串长");
			if (str.length() == 0)return 0;
			char const* ps = source.c_str()+startIndex;
			char const*p;
			while(p = strstr(ps, str.c_str())){
				ps = p + str.length();
				cnt++;
			}
			return cnt;
		}
		size_t CountOf(std::string const& source, int ch, size_t startIndex) {
			char str[8] = { ch&0xff,(ch>>8)&0xff,(ch>>16)&0xff,(ch>>24)&0xff };
			if (str[3] == 0xff) {
				str[3] = 0;
				if (str[2] == 0xff) {
					str[2] = 0;
					if (str[1] == 0xff) {
						str[1] = 0;
					}
				}
			}
			return CountOf(source, str,startIndex);
		}
	}
}
#endif