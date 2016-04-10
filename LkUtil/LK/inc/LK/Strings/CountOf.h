#ifndef LK_STRINGS_COUNTOF_
#define LK_STRINGS_COUNTOF_
namespace LK{
	namespace Strings {
		//查找str在source中的出现次数，注意如"aa"在"aaaa"中只出现2次
		size_t CountOf(std::string const& source, std::string const& str, size_t startIndex = 0);
		//查找ch在source中的出现的次数，允许查询 '我' 这样的或者 '我c' 这样的内容
		size_t CountOf(std::string const& source, int ch, size_t startIndex = 0);
	}
}
#endif