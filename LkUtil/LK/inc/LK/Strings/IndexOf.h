#ifndef LK_STRINGS_INDEXOF_
#define LK_STRINGS_INDEXOF_
namespace LK{
	namespace Strings {
		//查找str在source中第一次出现的位置，找不到返回-1
		inline index_t IndexOf(std::string const& source, std::string const& str){
			if (source.length() < str.length() || str.length() == 0)return -1;
			char const* p = strstr(source.c_str(), str.c_str());
			if (p == nullptr)return -1;
			return p - source.c_str();
		}
		//查找ch在source中第一次出现的位置，找不到返回-1
		inline index_t IndexOf(std::string const& source, char ch){
			if (source.length()  == 0)return -1;
			char const* p = strchr(source.c_str(), ch);
			if (p == nullptr)return -1;
			return p - source.c_str();
		}
		index_t IndexOf(std::string const& source, char ch, size_t startIndex,bool ignoreCase=false);
		//查找str在source中第一次出现的位置，找不到返回-1
		index_t LastIndexOf(std::string const& source, std::string const& str);
		//查找ch在source中第一次出现的位置，找不到返回-1
		index_t LastIndexOf(std::string const& source, char ch);
	}
}
#endif