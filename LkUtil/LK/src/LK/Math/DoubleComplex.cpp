#include"stdafx.h"
#ifdef LK_MATH_DOUBLECOMPLEX_
namespace LK
{
	namespace Math
	{
		void DoubleComplex::BitRC(DoubleComplex *y, size_t len)
		{
			if (len < 2)return;
			size_t i, j, k;
			size_t tmp = len >> 1;
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				if (i < j) BigObject::Swap(y[i], y[j]);
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		void DoubleComplex::FFT(DoubleComplex *p, size_t len)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = -2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						DoubleComplex t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		void DoubleComplex::IFFT(DoubleComplex *p, size_t len)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = 2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			for (size_t i = 0; i < len; ++i)p[i].Real *= dlen;
		}
		void DoubleComplex::BitRCHigh(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (len != 0)*p = len <= nlen ? pn[nlen - len] : 0;
			if (len == 1)return;
			size_t tmp = len >> 1;
			size_t mov = nlen - len;
			p[len - 1] = nlen == 0 ? 0 : pn[nlen - 1];
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j + mov >= nlen ? 0 : pn[j + mov];
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		void DoubleComplex::FFTHigh(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			BitRCHigh(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = -2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						DoubleComplex t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		unsigned short DoubleComplex::IFFTHigh(DoubleComplex *p, unsigned short*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = 2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			unsigned long long tmp = 0;
			unsigned short ret = 0;
			if (nlen > len)
			{
				memset(pn, 0, (nlen - len)*sizeof(unsigned short));
				pn += nlen - len;
				nlen = len;
			}
			else if (nlen < len)
			{
				if (nlen + 2 < len)(tmp = ((unsigned long long)(p[len - nlen - 3].Real * dlen + 0.5)) >> 32);
				if (nlen + 1 < len)(tmp += ((unsigned long long)(p[len - nlen - 2].Real * dlen + 0.5)) >> 16);
				tmp += ((unsigned long long)(p[len - nlen - 1].Real * dlen + 0.5));
				ret = (unsigned short)tmp;
				tmp >>= 16;
			}
			if (nlen != 0)
			{
				for (size_t i = 0; i < nlen; ++i)//乘法主体
				{
					double rst = p[len - nlen + i].Real * dlen + 0.5;
					u64 rt = (u64)rst;
					double check = rst - rt;
					if (check > 0.9375 || check < 0.0625)Throw<MathException >("精度不足，所得结果可能有误");
					tmp += rt;
					pn[i] = (unsigned short)tmp;
					tmp >>= 16;
				}
			}
			return ret;
		}
		//二进制平摊反转置换（低位对齐）
		void DoubleComplex::BitRCLow(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (nlen != 0)*p = *pn;
			if (len == 1)return;
			size_t tmp = len >> 1;
			p[len - 1] = nlen >= len ? pn[len - 1] : 0;
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j >= nlen ? 0 : pn[j];
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		//快速傅里叶变换（低位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^20
		void DoubleComplex::FFTLow(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			BitRCLow(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = -2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						DoubleComplex t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		//快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回上限溢出的16bit内容
		unsigned short DoubleComplex::IFFTLow(DoubleComplex*p, unsigned short*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = 2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			unsigned long long tmp = 0;
			for (size_t i = 0; i < nlen; ++i)
			{
				if (i < len)tmp += (unsigned long long)(p[i].Real*dlen + 0.5);
				pn[i] = (unsigned short)tmp;
				tmp >>= 16;
			}
			if (nlen < len)tmp += (unsigned long long)(p[nlen].Real*dlen + 0.5);
			return (unsigned short)tmp;
		}
		void DoubleComplex::BitRCHigh8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (len != 0)*p = len <= nlen ? pn[nlen - len] : 0;
			if (len == 1)return;
			size_t tmp = len >> 1;
			size_t mov = nlen - len;
			p[len - 1] = nlen == 0 ? 0 : pn[nlen - 1];
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j + mov >= nlen ? 0 : pn[j + mov];
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		void DoubleComplex::FFTHigh8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			BitRCHigh8(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = -2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						DoubleComplex t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		unsigned char DoubleComplex::IFFTHigh8(DoubleComplex *p, unsigned char*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = 2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			unsigned long long tmp = 0;
			unsigned char ret = 0;
			if (nlen > len)
			{
				memset(pn, 0, nlen - len);
				pn += nlen - len;
				nlen = len;
			}
			else if (nlen < len)
			{
				if (nlen + 5 < len)tmp = (((unsigned long long)(p[len - nlen - 6].Real * dlen + 0.5)) >> 40);
				if (nlen + 4 < len)tmp = (((unsigned long long)(p[len - nlen - 5].Real * dlen + 0.5)) >> 32);
				if (nlen + 3 < len)tmp = (((unsigned long long)(p[len - nlen - 4].Real * dlen + 0.5)) >> 24);
				if (nlen + 2 < len)tmp = (((unsigned long long)(p[len - nlen - 3].Real * dlen + 0.5)) >> 16);
				if (nlen + 1 < len)tmp += (((unsigned long long)(p[len - nlen - 2].Real * dlen + 0.5)) >> 8);
				tmp += ((unsigned long long)(p[len - nlen - 1].Real * dlen + 0.5));
				ret = (unsigned char)tmp;
				tmp >>= 8;
			}
			if (nlen != 0)
			{
				for (size_t i = 0; i < nlen; ++i)//乘法主体
				{
					double rst = p[len - nlen + i].Real * dlen + 0.5;
					u64 rt = (u64)rst;
					double check = rst - rt;
					if (check > 0.9375 || check < 0.0625)Throw<MathException >("精度不足，所得结果可能有误");
					tmp += (u32)rt;
					pn[i] = (unsigned char)tmp;
					tmp >>= 8;
				}
			}
			return ret;
		}
		//二进制平摊反转置换（低位对齐）
		void DoubleComplex::BitRCLow8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (nlen != 0)*p = *pn;
			if (len == 1)return;
			size_t tmp = len >> 1;
			p[len - 1] = nlen >= len ? pn[len - 1] : 0;
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j >= nlen ? 0 : pn[j];
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		//快速傅里叶变换（低位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^20
		void DoubleComplex::FFTLow8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len)
		{
			BitRCLow8(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = -2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						DoubleComplex t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		//快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回上限溢出的16bit内容
		unsigned char DoubleComplex::IFFTLow8(DoubleComplex*p, unsigned char*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				double th = 2 * 3.1415926535897932 / h;
				DoubleComplex wn(cos(th), sin(th)); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					DoubleComplex w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						DoubleComplex t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					DoubleComplex t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			unsigned long long tmp = 0;
			for (size_t i = 0; i < nlen; ++i)
			{
				if (i < len)tmp += (unsigned long long)(p[i].Real*dlen + 0.5);
				pn[i] = (unsigned char)tmp;
				tmp >>= 8;
			}
			if (nlen < len)tmp += (unsigned long long)(p[nlen].Real*dlen + 0.5);
			return (unsigned char)tmp;
		}

		void DoubleComplex::DFT(DoubleComplex const *ps, DoubleComplex *pd, size_t len)
		{
			for (size_t k = 0; k < len; ++k)
			{
				DoubleComplex w(1, 0);
				double t = -2 * 3.1415926535897932*k / len;
				DoubleComplex wn(cos(t), sin(t));
				pd[k] = 0;
				for (size_t i = 0; i < len; ++i)
				{
					pd[k] += ps[i] * w;
					w *= wn;
				}
			}
		}
		void DoubleComplex::IDFT(DoubleComplex const *ps, DoubleComplex *pd, size_t len)
		{
			for (size_t k = 0; k < len; ++k)
			{
				DoubleComplex w(1, 0);
				double t = 2 * 3.1415926535897932*k / len;
				DoubleComplex wn(cos(t), sin(t));
				pd[k] = 0;
				for (size_t i = 0; i < len; ++i) { pd[k] += ps[i] * w; w *= wn; }
				pd[k].Real /= len;
				pd[k].Imagine /= len;
			}
		}

	}
}
#endif