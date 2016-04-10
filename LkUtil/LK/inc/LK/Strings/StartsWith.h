#ifndef LK_STRINGS_STARTSWITH_
#define LK_STRINGS_STARTSWITH_
namespace LK{
	namespace Strings {
		/**
		@brief 判断str是否是以s开头的*/
		inline bool StartsWith(std::string const& str, std::string const&s) {
			if (str.length() < s.length() || s.length() == 0)return false;
			return memcmp(str.data(), s.data(), s.length()) == 0;
		}
	}
}
#endif