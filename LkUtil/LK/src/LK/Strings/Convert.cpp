#include "stdafx.h"
#ifdef LK_STRINGS_CONVERT_
namespace LK {
	namespace Strings {
		/**
		@brief 辅助函数，计算10的i次方,i的绝对值最大为308*/
		double IntPow10(index_t i) {
			if (i > DBL_MAX_10_EXP)return DBL_MAX;
			if (i < -DBL_MAX_10_EXP)return 0;
			static double pw10[1 + DBL_MAX_10_EXP] = { 0 };
			if (!*pw10) { *pw10 = 1; for (index_t k = 1; k <= DBL_MAX_10_EXP; ++k)pw10[k] = pw10[k - 1] * 10; }
			return i >= 0 ? pw10[i] : 1 / pw10[-i];
		}
		double ToDouble(std::string str)
		{
			char const* p = str.c_str();
			if (!p)return 0;
			bool nagtive = false;//负号
			while (*p == '+' || *p == '-') {
				if (*p == '-')nagtive = !nagtive;
				++p;
			}
			while (*p == '0') { ++p; }
			index_t len;//记录已读数量
			index_t dlen;//记录精度数
			double before = ToUIntDouble(p, &len, &dlen);//获取小数点转换之前的部分
			double point = 0;//记录小数部分
			index_t lenPoint = 0;
			p += len;
			if (*p == '.') {//继续获取小数部分
				p++;
				char const* lp = p;
				while (*p >= '0' && *p <= '9') {//十进制表示
					if (point > 1e17)break;
					point = point * 10 + *p - '0';
					++p;
				}
				lenPoint = p - lp;
				while (*p >= '0' && *p <= '9')++p;
			}
			index_t exp = 0;//记录指数
			if (*p == 'e' || *p == 'E') {//继续获取指数部分
				bool enagtive = false;//指数负号
				p++;
				while (*p == '+' || *p == '-') {
					if (*p == '-')enagtive = !enagtive;
					++p;
				}
				while (*p >= '0' && *p <= '9') {//十进制表示
					if (exp > 65536)return enagtive ? 0 : nagtive ? MinValues::Double : MaxValues::Double;
					exp = exp * 10 + *p - '0';
					++p;
				}
				if (enagtive)exp = -exp;
			}
			before = before*IntPow10(exp + len - dlen) + point*IntPow10(exp - lenPoint);
			return nagtive ? -before : before;
		}
		u64 ToUInt64(std::string const& str)
		{
			bool nagtive = false;
			unsigned long long num = 0;
			char const* p = str.c_str();
			while (*p == '+' || *p == '-')
			{
				if (*p == '-')nagtive = !nagtive;
				++p;
			}
			if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X'))//十六进制表示
			{
				p += 2;
				while ((*p >= '0' && *p <= '9') || (*p >= 'A'&&*p <= 'F') || (*p >= 'a'&&*p <= 'f'))
				{
					num = num * 16 + ((*p >= '0' && *p <= '9') ? *p - '0' : ((*p >= 'A'&&*p <= 'F') ? *p - 'A' + 10 : *p - 'a' + 10));
					++p;
				}
			}
			else    while (*p >= '0' && *p <= '9')//十进制表示
			{
				num = num * 10 + *p - '0';
				++p;
			}
			return nagtive ? -(signed long long)num : num;
		}
		s64 ToInt64(std::string const& str)
		{
			bool nagtive = false;
			long long num = 0;
			char const* p =str.c_str();
			while (*p == '+' || *p == '-')
			{
				if (*p == '-')nagtive = !nagtive;
				++p;
			}
			if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X'))//十六进制表示
			{
				p += 2;
				while ((*p >= '0' && *p <= '9') || (*p >= 'A'&&*p <= 'F') || (*p >= 'a'&&*p <= 'f'))
				{
					num = num * 16 + ((*p >= '0' && *p <= '9') ? *p - '0' : ((*p >= 'A'&&*p <= 'F') ? *p - 'A' + 10 : *p - 'a' + 10));
					++p;
				}
			}
			else    while (*p >= '0' && *p <= '9')//十进制表示
			{
				num = num * 10 + *p - '0';
				++p;
			}
			return nagtive ? -num : num;
		}

		std::string FromDouble(double number, int prec, int minlen , int d ) {
			std::string str;
			str.resize(64);
			str.resize(WriteFloat(number, &str[0], prec,minlen,d));
			return str;
		}
		std::string FromUInt64(u64 u)
		{
			std::string str;
			str.resize(64);
			str.resize(WriteUNum(u, &str[0]));
			return str;
		}
		/**
		@brief 从long long转换到字符串*/
		std::string FromInt64(s64 u)
		{
			std::string str;
			str.resize(64);
			str.resize(WriteNum(u, &str[0]));
			return str;
		}
		char const* __init_str_cvt = localinitonce();
		std::wstring ToWString(std::string const&str ){
			index_t length = str.size();
			index_t len = 0;
			for (index_t i = 0; i < length; ++i, ++len)//统计字符数
				if (str[i] < 0)++i;
			size_t mSize = 0;
			std::wstring wstr;
			wstr.resize(len + 2);
#ifdef _WINDOWS
			if (len)mSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)length, &wstr[0], (int)(len + 1));
#else
			if (len)mbstowcs(&wstr[0], str.c_str(), length);
			mSize = len + strlen(wstr.c_str() + len);
#endif
			wstr.resize(mSize);
			return wstr;
		}
		std::string ToString(std::wstring const&wstr) {
			size_t len = wstr.length()*sizeof(wchar_t);
			for (size_t i = 0; i <wstr.length(); ++i)//统计字符数
				if ((wstr[i] & 0xffffff80) == 0)len -= (sizeof(WCHAR) - 1);
			std::string str;
			//忽略需要三个及以上字节表示的字符
			str.resize(len + 2);
			size_t mSize=0;
#ifdef _WINDOWS
			CHAR c = '_';//无法转换的字符使用下划线替换
			if (len)mSize = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.length(), &str[0], (int)len + 1, &c, NULL);
#else
			if (len)mSize = wcstombs(&str[0], source, len);
#endif
			str.resize(mSize);
			return str;
		}
	}
}
#endif
