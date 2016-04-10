#ifndef LK_STRINGS_CSTRINGS_
#define LK_STRINGS_CSTRINGS_
#include <cstring>
namespace LK{
	namespace Strings {
		/**
		@brief */
		inline int strnicmp(char const*p1,char const*p2,size_t maxCount) {
#ifdef _VC
			return _strnicmp(p1, p2, maxCount);
#else
			return ::strnicmp(p1, p2, maxCount);
#endif
		}
		/**
		@brief wmemchr，查找某个字符在字符串中的位置
		@param [传入]参数s表示需要进行查找的宽字节字符串
		@param [传入]参数c表示需要查找的字符
		@param [传入]参数n表示进行查找的最大字符数
		@return 返回一个宽字节指针，指向找到的第一个结果的位置。如果找不到，则返回NULL
		@attention 如果传入参数的s为NULL，将会导致程序终止*/
		inline wchar_t const* wmemchr(wchar_t const* s, wchar_t c, size_t n) { for (; n > 0; ++s, --n)if (*s == c)return s; return 0; }
		/**
		@brief wcslwr，将字符串中的大写字母转换为小写字母
		@param [传入]参数s表示需要进行转换的宽字节字符串
		@param [传入]参数len表示进行转换的最大字符数
		@attention 如果传入的参数s为NULL，将会导致程序终止*/
		inline void wcslwr(wchar_t* s, size_t len) { while (len-- != 0 && *s != 0) { if (*s <= 'Z'&&*s >= 'A')*s += 32; ++s; } }
		/**
		@brief wcsuwr*/
		inline void wcsupr(wchar_t* s, size_t len) { while (len-- != 0 && *s != 0) { if (*s <= 'z'&&*s >= 'a')*s -= 32; ++s; } }
		/**
		@brief wcsicmp*/
		int wcsicmp(wchar_t const* s1, wchar_t const* s2);
		/**
		@brief wcsnicmp*/
		int wcsnicmp(wchar_t const* s1, wchar_t const* s2, size_t len);
		/**
		@brief memcmp，宽字节专用*/
		inline int memcmp(wchar_t const* p1, wchar_t const* p2, size_t len)
		{
#ifdef _VC
			return ::memcmp(p1, p2, len*sizeof(wchar_t));
#else
			return ::memcmp(p1, p2, len*sizeof(wchar_t));
#endif
		}
		/**
		@brief memcmp，宽字节专用*/
		inline int memcmp(char const* p1, char const* p2, size_t len)
		{
#ifdef _VC
			return ::memcmp(p1, p2, len);
#else
			return ::memcmp(p1, p2, len);
#endif
		}
		/**
		@brief memicmp，宽字节专用*/
		int memicmp(wchar_t const* p1, wchar_t const* p2, size_t len);
		/**
		@brief 测量多字节字符串字符数，如果传入NULL则返回0*/
		index_t strlen(char const* mString);
		/**
		@brief 测量宽字节字符串字符数，如果传入NULL则返回0*/
		index_t strlen(wchar_t const* wString);
		/**
		@brief 辅助函数，将p所指向字符串转换为一个无符号整数double。plen用以输出读取的的字符数,pdlen用以输出已读有效位数*/
		double ToUIntDouble(char const* lp, index_t* plen, index_t *pdlen);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，并返回写入长度，不足minlen时补0，d为进制数*/
		int WriteNum(long long val, wchar_t*p, int minlen = 0, int d = 10);
		/**
		@brief 将无符号的val转换为字符串并写入p所指定的内存中，并返回写入长度，不足minlen时补0，d为进制数*/
		int WriteUNum(unsigned long long val, wchar_t*p, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用指数方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatE(double val, wchar_t*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatP(double val, wchar_t*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，自动采用指数或者小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloat(double val, wchar_t*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，并返回写入长度，不足minlen时补0，d为进制数*/
		int WriteNum(long long val, char*p, int minlen = 0, int d = 10);
		/**
		@brief 将无符号的val转换为字符串并写入p所指定的内存中，并返回写入长度，不足minlen时补0，d为进制数*/
		int WriteUNum(unsigned long long val, char*p, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用指数方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatE(double val, char*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatP(double val, char*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，自动采用指数或者小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloat(double val, char*p, int prec = 6, int minlen = 0, int d = 10);
		/**
		@brief 默认输出精度，初始值为6*/
		int& DefaultPrecisionRef();
		/**
		@brief 使用指针和长度指定的内容串接字符串*/
		std::string Join(std::vector<std::string >const& strs, char const* ch, size_t chlen);
		/**
		@brief 自付串比较函数*/
		inline int strcmp(char const* src, char const*str) {
			return ::strcmp(src, str);
		}
		/**@brief 初始化local设置一次*/
		char const* localinitonce();
		/**@brief 初始化local设置*/
		char const* localinit();

	}
}
#endif