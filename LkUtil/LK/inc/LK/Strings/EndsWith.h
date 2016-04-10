#ifndef LK_STRINGS_ENDSWITH_
#define LK_STRINGS_ENDSWITH_
namespace LK{
	namespace Strings {
		//判断字符串source是否是以字符串str结尾
		inline bool EndsWith(std::string const&source, std::string const&str,bool ignoreCase=false) {
			if (source.length() < str.length()||str.length()==0)return false;
			if (ignoreCase)return strnicmp(&source[source.length() - str.length()], &str[0], str.length()) == 0;
			return memcmp(&source[source.length() - str.length()], &str[0],str.length()) == 0;
		}
	}
}
#endif