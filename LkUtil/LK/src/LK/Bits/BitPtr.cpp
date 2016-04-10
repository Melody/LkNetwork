#include"stdafx.h"
#ifdef LK_BITS_BITPTR_
namespace LK{
	namespace Bits {
		std::string BitPtr::ToString(index_t t)
		{
			std::string str;
			if (t == 0)
			{
				str.resize(sizeof(size_t) * 2 + 4);
				str[0] = '0'; str[1] = 'x'; str[sizeof(size_t) * 2 + 2] = '-'; str[sizeof(size_t) * 2 + 3] = '0' + bitMove % 8;
				size_t pv = ptrValue + bitMove / 8;
				for (index_t i = sizeof(size_t) * 2 + 1; i > 1; --i)
				{
					str[i] = pv % 16;
					if (str[i] > 9)str[i] += 'A' - 10;
					else str[i] += '0';
					pv /= 16;
				}
				t = sizeof(size_t) * 2 + 4;
			}
			else if (t > 0)
			{
				str.resize(t); 
				BitPtr tmp(*this);
				for (index_t i = t; i != 0;) { --i; str[i] = '0' + (tmp++).Get(); }
			}
			else
			{
				str.resize(-t);
				BitPtr tmp(*this);
				for (index_t i = 0; i != -t; ++i) { str[i] = '0' + (tmp--).Get(); }
				t = -t;
			}
			return str;
		}
		std::string BitPtr::ToString16(index_t t)
		{
			std::string str;
			if (t == 0) {
				str.resize(sizeof(size_t) * 2 +4);
				str[0] = '0'; str[1] = 'x'; str[sizeof(size_t) * 2 + 2] = '-'; str[sizeof(size_t) * 2 + 3] = '0' + bitMove % 8;
				size_t pv = ptrValue + bitMove / 8;
				for (index_t i = sizeof(size_t) * 2 + 1; i > 1; --i)
				{
					str[i] = pv % 16;
					if (str[i] > 9)str[i] += 'A' - 10;
					else str[i] += '0';
					pv /= 16;
				}
				t = sizeof(size_t) * 2 + 4;
			}
			else if (t > 0) {
				str.resize(((size_t)(t + 3) >> 2));
				BitPtr tmp(*this);
				for (index_t i = ((size_t)t + 3) >> 2; i != 0; ) {
					i--;
					if (i == 0 && (t & 3) != 0)str[i] = (char)tmp.Get64(t & 3);
					else str[i] = (char)tmp.Get64(4);
					tmp += 4;
					if (str[i] < 10)str[i] += L'0';
					else str[i] += L'A' - 10;
				}
			}
			else {
				str.resize(((size_t)(-t + 3) >> 2));
				BitPtr tmp(*this);
				++tmp;
				for (index_t tlen = t, i = 0; tlen <0; i++) {
					tlen += 4;
					tmp -= 4;
					if (tlen > 0) {
						tmp += tlen;
						str[i] = (char)(tmp.Get64(4 - tlen) << tlen);
					}
					else str[i] = (char)tmp.Get64(4);
					if (str[i] < 10)str[i] += '0';
					else str[i] += 'A' - 10;
				}
			}
			return str;
		}
	}
}
#endif