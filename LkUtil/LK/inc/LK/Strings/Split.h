#ifndef LK_STRINGS_SPLIT_
#define LK_STRINGS_SPLIT_
namespace LK {
	namespace Strings {
		/**
		@brief 将指定的字符串用指定的字符分割为多个字符串
		@param [传入]参数str为需要分割的字符串
		@param [传入，可选]参数ch为指定的分割字符，默认为空格，可以使用 '擦' 这样的形式，但不能使用 'ab' 或者 ‘我擦’ 或者 L'擦'
		@param [传入，可选]参数removeEmpty指定是否移除空字符串，默认为false
		@param [传入，可选]参数ignoreCase指定是否忽略大小写，默认为false
		@param [传入，可选]参数maxCount指定返回vector的最大元素数，数量超出该数值时将舍去，默认不限制大小
		@return 返回一个包含多个字符串的std::vector*/
		std::vector<std::string > Split(std::string const&str, int ch = ' ',bool removeEmpty=false,bool ignoreCase=false,size_t maxCount=MaxValues::Of((size_t*)0));
		/**
		@brief 将指定的被分割字符串用指定的字符串分割为多个字符串
		@param [传入]参数str为需要分割的源字符串
		@param [传入，可选]参数s为指定的分割字符串
		@param [传入，可选]参数removeEmpty指定是否移除空字符串，默认为false
		@param [传入，可选]参数ignoreCase指定是否忽略大小写，默认为false
		@param [传入，可选]参数maxCount指定返回vector的最大元素数，数量超出该数值时将舍去，默认不限制大小
		@return 返回一个包含多个字符串的std::vector*/
		std::vector<std::string > Split(std::string const&str, std::string const& s,bool removeEmpty=false,bool ignoreCase=false,size_t maxCount=MaxValues::Of((size_t*)0));
		/**
		@brief 将vector中的字符串使用指定分隔符串接起来
		@param [传入]参数strs为要进行串接的字符串vector
		@param [传入，可选]参数ch为指定的分割字符，默认为空格，可以使用 '擦' 这样的形式，但不能使用 'ab' 或者 ‘我擦’ 或者 L'擦'
		@return 返回串接结果*/
		inline std::string Join(std::vector<std::string >const& strs, int ch) {
			size_t chlen = (ch & 0xff000000) ? 4 : ((ch & 0xff0000) ? 3 : ((ch & 0xff00) ? 2 : 1));
			return Join(strs, (char const*)&ch, chlen);
		}
		/**
		@brief 将vector中的字符串使用指定分隔字符串串接起来
		@param [传入]参数strs为要进行串接的字符串vector
		@param [传入]参数s为指定的分割字符串
		@return 返回串接结果*/
		inline std::string Join(std::vector<std::string >const& strs, std::string const& s) {
			return Join(strs, s.data(), s.length());
		}
		//从csv字符串转换为正常字符串
		std::vector<std::string> FromCSV(std::string const& str);
		//打包为csv字符串
		std::string ToCSV(std::vector<std::string>const&strs);
	}
}
#endif