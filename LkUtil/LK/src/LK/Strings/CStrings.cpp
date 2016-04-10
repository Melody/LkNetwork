#include "stdafx.h"
#ifdef LK_STRINGS_CSTRINGS_
#include<locale>
#include<iostream>
namespace LK {
	namespace Strings {
		/**
		@brief 辅助函数，将p所指向字符串转换为一个无符号整数double。plen用以输出读取的的字符数,pdlen用以输出已读有效位数*/
		double ToUIntDouble(char const* lp, index_t* plen, index_t *pdlen) {
			double num = 0;
			char const* p = lp;
			char const* pd = NULL;
			while (*p >= L'0' && *p <= L'9') {//十进制表示
				if (!pd&&*p != L'0')pd = p;
				if (num > 1e17)break;
				num = num * 10 + *p - L'0';
				++p;
			}
			if (pd) {
				if (pdlen)*pdlen = p - pd;
				while (*p >= L'0' && *p <= L'9')++p;
			}
			else if (pdlen)*pdlen = p - lp;
			if (plen)*plen = p - lp;
			return num;
		}
		/**
		@brief 默认输出精度，初始值为6*/
		int& DefaultPrecisionRef() { static int val = 6; return val; }
		int WriteNum(long long v, wchar_t*p, int minlen, int d) {
			if (v < 0) { *p++ = L'-'; return WriteUNum(-v, p, minlen - 1, d) + 1; }
			return WriteUNum(v, p, minlen, d);
		}
		int WriteUNum(unsigned long long val, wchar_t*p, int minlen, int d)
		{
			int result = 0;
			do {
				p[result] = (wchar_t)(val % d);
				if (p[result] > 9)p[result] += L'A' - 10;
				else p[result] += L'0';
				++result;
				val /= d;
			} while (val != 0);
			while (result < minlen) p[result++] = L'0';
			for (int i = 0; i < result / 2; ++i)
				std::swap(p[i], p[result - i - 1]);
			return result;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用指数方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatE(double val, wchar_t*p, int prec, int minlen, int d)
		{
			if (val == 0) { int i = 1; *p++ = L'0'; if (minlen > 1) { *p++ = '.'; i++; } while (i < minlen)p[i++] = L'0'; return i; }
			if (val != val) { int i = 0; do { *p++ = L'#'; } while (++i<minlen); return i; }
			bool nagtive = false;
			if (val < 0) { val = -val; if (minlen > 0)minlen--; *p++ = L'-'; nagtive = true; }
			int exp = (int)(log(val) / log((double)d));
			if (exp != 0 && -exp == exp) { int i = 0; do { *p++ = L'∞'; } while (++i<minlen);  return nagtive + i; }
			val *= pow((double)d, -exp);
			if (val >= d) { val /= d; exp++; }
			else if (val < 1) { val *= d; exp--; }
			int len = 1;
			*p++ = (wchar_t)val;
			val = (val - (int)val)*d;
			if (prec > 1) {
				if (val != 0) {
					*p++ = L'.';
					do {
						*p++ = (wchar_t)val;
						val = (val - (int)val)*d;
						++len;
					} while (val != 0 && len < prec);
					if (val * 2 >= d) {//舍入
						wchar_t* pp = p - 1;
						while (*pp != L'.'&&++*pp == d) { *pp = 0; pp--; }
						if (*pp == L'.') { --p; --len; if (++*--pp == d) { *pp = 1;  exp++; --p; } }
					}
					if (len != 1)++len;
				}
				for (wchar_t* pp = p - len; pp != p; ++pp) {
					if (*pp == L'.')continue;
					if (*pp >= 10)
						*pp += L'A';
					else *pp += L'0';
				}
			}
			wchar_t tmps[16];
			int testlen = WriteNum(exp, tmps, exp < 0 ? 2 : 1, d);
			while (len + testlen + 1<minlen) { if (len++ != 1)*p++ = L'0'; else *p++ = L'.'; }
			*p++ = L'e';
			for (minlen = 0; minlen < testlen; ++minlen)p[minlen] = tmps[minlen];
			return len + testlen + 1 + nagtive;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用小数点方式，并返回写入长度，
		不足minlen时小数点后补0，d为进制数*/
		int WriteFloatP(double val, wchar_t*p, int prec, int minlen, int d) {
			int cnt = 0;
			if (val == 0) { cnt = 1; *p++ = L'0'; if (minlen > 1) { *p++ = L'.'; cnt++; } while (cnt < minlen&&cnt <= prec)p[cnt++] = L'0'; return cnt; }
			if (val != val) { cnt = 0; do { *p++ = L'#'; } while (++cnt<minlen); return cnt; }
			if (val < 0) { val = -val; cnt++; *p++ = L'-'; }
			int exp = (int)(log(val) / log((double)d));
			if (exp != 0 && -exp == exp) { do { *p++ = L'∞'; } while (++cnt<minlen);  return cnt; }
			val *= pow((double)d, -exp);
			if (val >= d) { val /= d; exp++; }
			else if (val < 1) { val *= d; exp--; }
			//生商
			for (int i = 0; i < prec; ++i) {
				int md = (int)val;
				p[i] = (wchar_t)md;
				val -= md;
				val *= d;
			}
			if (val * 2 > d) {//四舍五入
				for (int i = prec; i != 0;) {
					i--;
					if (++p[i] < d)break;
					p[i] = 0;
				}
				if (p[0] == 0) { exp++; p[0] = 1; }
			}
			for (int i = 0; i < prec; ++i)p[i] = (p[i] < 10 ? (p[i] + L'0') : (p[i] + L'A' - 10));
			//添加小数点
			if (exp < 0) {
				cnt += prec + 1 - exp;
				int i = prec + 1 - exp;
				while (i-- > 1 - exp)
					p[i] = p[i - (1 - exp)];
				while (i != 0)p[i--] = L'0';
				p[0] = L'0';
				p[1] = L'.';
				p += prec + 1 - exp;
			}
			else if (exp < prec) {
				bool pt = (exp + 1 < prec) || (cnt + prec + 1 <= minlen);
				cnt += prec + pt;
				if (pt) {
					for (int i = prec; i > exp; i--)p[i] = p[i - 1];
					p[exp + 1] = L'.';
				}
				p += prec + pt;
			}
			else {
				cnt += exp + 1;
				for (int i = prec; i <= exp; ++i)p[i] = L'0';
				if (cnt < minlen)(p++)[exp + 1] = L'.';
				p += exp + 1;
			}
			while (cnt < minlen) { *p++ = L'0'; cnt++; }
			return cnt;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，自动采用指数或者小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloat(double val, wchar_t*p, int prec, int minlen, int d) {
			double td = d*d;
			td = td*td*td;
			if (val >= td || val <= -td || (val>0 && val<d*d / td) || (val<0 && val>-d*d / td))return WriteFloatE(val, p, prec, minlen, d);
			return WriteFloatP(val, p, prec, minlen, d);
		}
		int WriteNum(long long v, char*p, int minlen, int d)
		{
			if (v < 0) { *p++ = '-'; return WriteUNum(-v, p, minlen - 1, d) + 1; }
			return WriteUNum(v, p, minlen, d);
		}
		int WriteUNum(unsigned long long val, char*p, int minlen, int d)
		{
			int result = 0;
			do {
				p[result] = (char)(val % d);
				if (p[result] > 9)p[result] += 'A' - 10;
				else p[result] += '0';
				++result;
				val /= d;
			} while (val != 0);
			while (result < minlen) p[result++] = '0';
			for (int i = 0; i < result / 2; ++i)
				std::swap(p[i], p[result - i - 1]);
			return result;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用指数方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloatE(double val, char*p, int prec, int minlen, int d)
		{
			if (val == 0) { int i = 1; *p++ = '0'; if (minlen > 1) { *p++ = '.'; i++; } while (i < minlen)p[i++] = '0'; return i; }
			if (val != val) { int i = 0; do { *p++ = '#'; } while (++i<minlen); return i; }
			bool nagtive = false;
			if (val < 0) { val = -val; if (minlen > 0)minlen--; *p++ = '-'; nagtive = true; }
			int exp = (int)(log(val) / log((double)d));
			if (exp != 0 && -exp == exp) { int i = 0; do { *p++ = "∞"[0];*p++ = "∞"[1]; } while ((i+=2)<minlen);  return nagtive + i; }
			val *= pow((double)d, -exp);
			if (val >= d) { val /= d; exp++; }
			else if (val < 1) { val *= d; exp--; }
			int len = 1;
			*p++ = (char)val;
			val = (val - (int)val)*d;
			if (prec > 1) {
				if (val != 0) {
					*p++ = '.';
					do {
						*p++ = (char)val;
						val = (val - (int)val)*d;
						++len;
					} while (val != 0 && len < prec);
					if (val * 2 >= d) {//舍入
						char* pp = p - 1;
						while (*pp != '.'&&++*pp == d) { *pp = 0; pp--; }
						if (*pp == '.') { --p; --len; if (++*--pp == d) { *pp = 1;  exp++; --p; } }
					}
					if (len != 1)++len;
				}
				for (char* pp = p - len; pp != p; ++pp) {
					if (*pp == '.')continue;
					if (*pp >= 10)
						*pp += 'A';
					else *pp += '0';
				}
			}
			char tmps[16];
			int testlen = WriteNum(exp, tmps, exp < 0 ? 2 : 1, d);
			while (len + testlen + 1<minlen) { if (len++ != 1)*p++ = '0'; else *p++ = '.'; }
			*p++ = 'e';
			for (minlen = 0; minlen < testlen; ++minlen)p[minlen] = tmps[minlen];
			return len + testlen + 1 + nagtive;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，采用小数点方式，并返回写入长度，
		不足minlen时小数点后补0，d为进制数*/
		int WriteFloatP(double val, char*p, int prec, int minlen, int d) {
			int cnt = 0;
			if (val == 0) { cnt = 1; *p++ = '0'; if (minlen > 1) { *p++ = '.'; cnt++; } while (cnt < minlen&&cnt <= prec)p[cnt++] = '0'; return cnt; }
			if (val != val) { cnt = 0; do { *p++ = '#'; } while (++cnt<minlen); return cnt; }
			if (val < 0) { val = -val; cnt++; *p++ = '-'; }
			int exp = (int)(log(val) / log((double)d));
			if (exp != 0 && -exp == exp) { do { *p++ = "∞"[0]; *p++ = "∞"[1]; } while ((cnt+=2)<minlen);  return cnt; }
			val *= pow((double)d, -exp);
			if (val >= d) { val /= d; exp++; }
			else if (val < 1) { val *= d; exp--; }
			//生商
			for (int i = 0; i < prec; ++i) {
				int md = (int)val;
				p[i] = (char)md;
				val -= md;
				val *= d;
			}
			if (val * 2 > d) {//四舍五入
				for (int i = prec; i != 0;) {
					i--;
					if (++p[i] < d)break;
					p[i] = 0;
				}
				if (p[0] == 0) { exp++; p[0] = 1; }
			}
			for (int i = 0; i < prec; ++i)p[i] = (p[i] < 10 ? (p[i] + '0') : (p[i] + 'A' - 10));
			//添加小数点
			if (exp < 0) {
				cnt += prec + 1 - exp;
				int i = prec + 1 - exp;
				while (i-- > 1 - exp)
					p[i] = p[i - (1 - exp)];
				while (i != 0)p[i--] = '0';
				p[0] = '0';
				p[1] = '.';
				p += prec + 1 - exp;
			}
			else if (exp < prec) {
				bool pt = (exp + 1 < prec) || (cnt + prec + 1 <= minlen);
				cnt += prec + pt;
				if (pt) {
					for (int i = prec; i > exp; i--)p[i] = p[i - 1];
					p[exp + 1] = '.';
				}
				p += prec + pt;
			}
			else {
				cnt += exp + 1;
				for (int i = prec; i <= exp; ++i)p[i] = '0';
				if (cnt < minlen)(p++)[exp + 1] = '.';
				p += exp + 1;
			}
			while (cnt < minlen) { *p++ = '0'; cnt++; }
			return cnt;
		}
		/**
		@brief 将val转换为字符串并写入p所指定的内存中，自动采用指数或者小数点方式，并返回写入长度，不足minlen时小数点后补0，d为进制数*/
		int WriteFloat(double val, char*p, int prec, int minlen, int d) {
			double td = d*d;
			td = td*td*td;
			if (val >= td || val <= -td || (val>0 && val<d*d / td) || (val<0 && val>-d*d / td))return WriteFloatE(val, p, prec, minlen, d);
			return WriteFloatP(val, p, prec, minlen, d);
		}
		int wcsicmp(wchar_t const* s1, wchar_t const* s2)
		{
			if (s1 == s2)return 0;
			if (s1 == 0 || s2 == 0)return s1 ? 1 : -1;
			while (*s1 && *s2)
			{
				if (*s1 != *s2)
				{
					if (((*s1 <= 'z'&&*s1 >= 'a') || (*s1 <= 'Z'&&*s1 >= 'A')) && ((*s2 <= 'z'&&*s2 >= 'a') || (*s2 <= 'Z'&&*s2 >= 'A')))
					{
						if ((*s1^*s2) != 32)break;
					}
					else break;
				}
				s1++;
				s2++;
			}
			return *s1 > *s2 ? 1 : (*s1 < *s2 ? -1 : 0);
		}
		int wcsnicmp(wchar_t const* s1, wchar_t const* s2, size_t len)
		{
			if (s1 == s2)return 0;
			if (s1 == 0 || s2 == 0)return s1 ? 1 : -1;
			while (len-- > 0)
			{
				if (*s1 != *s2)
				{
					if (((*s1 <= 'z'&&*s1 >= 'a') || (*s1 <= 'Z'&&*s1 >= 'A')) && ((*s2 <= 'z'&&*s2 >= 'a') || (*s2 <= 'Z'&&*s2 >= 'A')))
					{
						if ((*s1^*s2) != 32)break;
					}
					else break;
				}
				s1++;
				s2++;
			}
			return len != 0 ? (*s1 > *s2 ? 1 : (*s1 < *s2 ? -1 : 0)) : 0;
		}
		int memicmp(wchar_t const* p1, wchar_t const* p2, size_t len)
		{
			while (len-- > 0)
			{
				if ((*p1 <= L'z'&&*p1 >= L'a') || (*p1 <= L'Z'&&*p1 >= L'A'))
				{
					if (((*p1^*p2)&(((wchar_t)-1) ^ 0x20)) != 0)
						return *p1 > *p2 ? 1 : -1;
				}
				else if (*p1 != *p2)return *p1 > *p2 ? 1 : -1;
				++p1; ++p2;
			}
			return 0;
		}
		index_t strlen(char const* str)
		{
			if (str == NULL)return 0;
			if (sizeof(char)>2)
			{
				index_t i = 0;
				while (str[i] != 0)
					++i;
				return i;
			}
			static const size_t magic_bits = size_t(sizeof(char) == 1 ? (0xfefefefefefefeff ^ (1ull << (sizeof(size_t) * 8 - 1))) : (0xfffefffefffeffff ^ (1ull << (sizeof(size_t) * 8 - 1))));
			size_t* sp = (size_t*)str;
			switch ((size_t)str & (sizeof(size_t) - 1))
			{
			case sizeof(char) : if (*((char*)sp) == 0)return 0; ++((char*&)sp);
			case sizeof(char) * 2:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
			case sizeof(char) * 3:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
#ifdef _X64
			case sizeof(char) * 4:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
			case sizeof(char) * 5:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
			case sizeof(char) * 6:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
			case sizeof(char) * 7:if (*((char*)sp) == 0)return (char*)sp - str; ++((char*&)sp);
#endif
			default:break;
			}
			for (;;) {
				size_t longword = *sp;
				if ((((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0)
				{
					const char*cp = (const char*)(sp);
					if (cp[0] == 0)
						return cp - str;
					if (cp[1] == 0)
						return cp - str + 1;
					if (cp[2] == 0)
						return cp - str + 2;
					if (cp[3] == 0)
						return cp - str + 3;
#ifdef _X64
					if (cp[4] == 0)
						return cp - str + 4;
					if (cp[5] == 0)
						return cp - str + 5;
					if (cp[6] == 0)
						return cp - str + 6;
					if (cp[7] == 0)
						return cp - str + 7;
#endif
				}
				++sp;
			}

		}
		index_t strlen(wchar_t const* str)
		{
			if (str == NULL)return 0;
			if (sizeof(wchar_t) != 2)
			{
				index_t i = 0;
				while (str[i] != 0)
					++i;
				return i;
			}
#ifdef _X64
			static const size_t magic_bits = 0xfffefffefffeffff ^ (1ull << (sizeof(size_t) * 8 - 1));
#endif
			size_t* sp = (size_t*)str;
			switch ((size_t)str & (sizeof(size_t) - 1))
			{
			case 2:if (*((wchar_t*)sp) == 0)return 0; ++((wchar_t*&)sp);
#ifdef _X64
			case 4:if (*((wchar_t*)sp) == 0)return (wchar_t*)sp - str; ++((wchar_t*&)sp);
			case 6:if (*((wchar_t*)sp) == 0)return (wchar_t*)sp - str; ++((wchar_t*&)sp);
#endif
			default:break;
			}
			for (;;) {
#ifdef _X64
				size_t longword = *sp;
				if ((((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0)
				{
					const wchar_t*cp = (const wchar_t*)(sp);
					if (cp[0] == 0)
						return cp - str;
					if (cp[1] == 0)
						return cp - str + 1;
					if (cp[2] == 0)
						return cp - str + 2;
					if (cp[3] == 0)
						return cp - str + 3;
				}
#else
				if ((*sp & 0xffff) == 0)return (const wchar_t*)(sp)-str;
				if (*sp <= 0xffff)return (const wchar_t*)(sp)-str + 1;
#endif
				++sp;
			}

		}
		std::string Join(std::vector<std::string >const& strs, char const* ch, size_t chlen) {
			size_t alllen = 0;
			for (size_t i = 0;;) {
				alllen += strs[i].length();
				if (++i == strs.size())break;
				alllen += chlen;
			}
			std::string str;
			str.resize(alllen);
			alllen = 0;
			for (size_t i = 0;;) {
				memcpy(&str[alllen], strs[i].data(), strs[i].length());
				alllen += strs[i].length();
				if (++i == strs.size())break;
				memcpy(&str[alllen], ch, chlen);
				alllen += chlen;
			}
			return str;
		}
		/**@brief 初始化local设置一次*/
		char const* localinitonce()
		{
			static char const* p;
			if (p)return p;
			static  std::mutex mt;
			mt.lock();
			if (!p) {
				p = localinit();
			}
			mt.unlock();
			return p;
		}
		/**@brief 初始化local设置*/
		char const* localinit() {
			static std::locale loc = std::locale("");
			std::cout.imbue(loc);
			std::wcout.imbue(loc);
			std::cin.imbue(loc);
			std::wcin.imbue(loc);
			std::cerr.imbue(loc);
			return loc.name().c_str();
		}
	}
}
#endif