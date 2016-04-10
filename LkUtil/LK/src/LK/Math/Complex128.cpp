#include"stdafx.h"
#ifdef LK_MATH_COMPLEX128_
namespace LK
{
	namespace Math
	{
		void Complex128::BitRC(Complex128 *y, size_t len)
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
		void Complex128::FFT(Complex128 *p, size_t len)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h)).SelfNegative();
				Complex128 wn(th.Cos(), th.Sin()); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						Complex128 t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		void Complex128::IFFT(Complex128 *p, size_t len)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h));
				Complex128 wn(th.Cos(), th.Sin()); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			double dlen = 1.0 / len;
			for (size_t i = 0; i < len; ++i)p[i].Real *= dlen;
		}
		void Complex128::BitRCHigh(cu32*pn, Complex128 *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (len != 0)*p = len <= nlen ? Float128(pn[nlen - len]) : Float128::Zero();
			if (len == 1)return;
			size_t tmp = len >> 1;
			size_t mov = nlen - len;
			p[len - 1] = nlen == 0 ? Float128::Zero() : Float128(pn[nlen - 1]);
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j + mov >= nlen ? Float128::Zero() : Float128(pn[j + mov]);
				k = tmp;
				while (j >= k)
				{
					j ^= k;
					k >>= 1;
				}
				j |= k;
			}
		}
		void Complex128::FFTHigh(cu32*pn, Complex128 *p, size_t nlen, size_t len)
		{
			BitRCHigh(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h)).SelfNegative();
				Float128 thw1, thw2;
				th.SinAndCos(thw1, thw2);
				Complex128 wn(thw2, thw1); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 &rp = p[k + hh];
						if (rp.Real.Sign == 0 && rp.Imagine.Sign == 0) { rp = p[k]; continue; }
						Complex128 t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		u32 Complex128::IFFTHigh(Complex128 *p, u32*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h));
				Float128 thw1, thw2;
				th.SinAndCos(thw1, thw2);
				Complex128 wn(thw2, thw1); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			Float128 dlen = Float128::One() / len;
			u64 tmp[2] = { 0 };
			u32 ret = 0;
			if (nlen > len)
			{
				memset(pn, 0, (nlen - len)*sizeof(u32));
				pn += nlen - len;
				nlen = len;
			}
			else if (nlen < len)
			{
				if (nlen + 2 < len)
				{
					Float128 ft = (++(p[len - nlen - 3].Real * dlen).Half(-1)).SelfHalf();
					ft.Data >>= (191 - ft.Exp);
					tmp[0] = ft.Data.ull[0];
				}
				if (nlen + 1 < len)
				{
					Float128 ft = (++(p[len - nlen - 2].Real * dlen).Half(-1)).SelfHalf();
					ft.Data >>= (159 - ft.Exp);
					if (tmp[0] > ~ft.Data.ull[0])
						tmp[1] = ft.Data.ull[1] + 1;
					else tmp[1] = ft.Data.ull[1];
					tmp[0] += ft.Data.ull[0];
				}
				Float128 ft = (++(p[len - nlen - 1].Real * dlen).Half(-1)).SelfHalf();
				ft.Data >>= (127 - ft.Exp);
				if (tmp[0] > ~ft.Data.ull[0])
					tmp[1] += ft.Data.ull[1] + 1;
				else tmp[1] += ft.Data.ull[1];
				tmp[0] += ft.Data.ull[0];
				ret = (u32)tmp[0];
				tmp[0] = Bits::ShiftRight(tmp[1], tmp[0], 32);
				tmp[1] >>= 32;
			}
			if (nlen != 0)
			{
				for (size_t i = 0; i < nlen; ++i)//乘法主体
				{
					Float128 ft = (++(p[len - nlen + i].Real * dlen).Half(-1)).SelfHalf();
					ft.Data >>= (127 - ft.Exp);
					if (tmp[0] > ~ft.Data.ull[0])
						tmp[1] += ft.Data.ull[1] + 1;
					else tmp[1] += ft.Data.ull[1];
					tmp[0] += ft.Data.ull[0];
					pn[i] = (u32)tmp[0];
					tmp[0] = Bits::ShiftRight(tmp[1], tmp[0], 32);
					tmp[1] >>= 32;
				}
			}
			return ret;
		}
		//二进制平摊反转置换（低位对齐）
		void Complex128::BitRCLow(cu32*pn, Complex128 *p, size_t nlen, size_t len)
		{
			size_t i, j, k;
			if (len == 0)return;
			if (nlen != 0)*p = Float128(*pn);
			if (len == 1)return;
			size_t tmp = len >> 1;
			p[len - 1] = nlen >= len ? Float128(pn[len - 1]) : Float128::Zero();
			for (i = 1, j = tmp; i < len - 1; ++i)
			{
				p[i] = j >= nlen ? Float128::Zero() : Float128(pn[j]);
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
		void Complex128::FFTLow(cu32*pn, Complex128 *p, size_t nlen, size_t len)
		{
			BitRCLow(pn, p, nlen, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h)).SelfNegative();
				Complex128 wn(th.Cos(), th.Sin()); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k, w *= wn)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 &rp = p[k + hh];
						if (rp.Real == 0 && rp.Imagine == 0) { rp = p[k]; continue; }
						Complex128 t = w*rp;
						rp = p[k] - t;
						p[k] += t;
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
		}
		//快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回上限溢出的16bit内容
		u32 Complex128::IFFTLow(Complex128*p, u32*pn, size_t len, size_t nlen)
		{
			BitRC(p, len);
			for (size_t h = 2; h <= len; h <<= 1)
			{
				Float128 th = th.Pi().Half(30 - Bits::LeftZeroCount32((u32)h));
				Float128 thw1, thw2;
				th.SinAndCos(thw1, thw2);
				Complex128 wn(thw2, thw1); // h次单位复根 
				size_t hh = h >> 1;
				for (size_t j = 0; j < len; j += h)
				{ // 原序列被分成了L/h段h长序列 
					Complex128 w(1, 0); // 旋转因子 
					size_t k;
					for (k = j; k < j + hh - 1; ++k)
					{ // 因为折半定理，只需要计算枚举一半的长度即可 
						Complex128 t = w*p[k + hh];
						p[k + hh] = p[k] - t;
						p[k] += t;
						w *= wn; // 更新旋转因子 
					}
					Complex128 t = w*p[k + hh];
					p[k + hh] = p[k] - t;
					p[k] += t;
				}
			}
			Float128 dlen = Float128::One() / len;
			u64 tmp[2] = { 0 };
			for (size_t i = 0; i < nlen; ++i)
			{
				if (i < len) {
					Float128 ft = (++(p[i].Real * dlen).Half(-1)).SelfHalf();
					ft.Data >>= (127 - ft.Exp);
					if (tmp[0]>~ft.Data.ull[0])
						tmp[1] += ft.Data.ull[1] + 1;
					else tmp[1] += ft.Data.ull[1];
					tmp[0] += ft.Data.ull[0];
				}
				pn[i] = (u32)tmp[0];
				tmp[0] = Bits::ShiftRight(tmp[1], tmp[0], 32);
				tmp[1] >>= 32;
			}
			if (nlen < len)
			{
				Float128 ft = (++(p[nlen].Real * dlen).Half(-1)).SelfHalf();
				ft.Data >>= (127 - ft.Exp);
				tmp[0] += ft.Data.ull[0];
			}
			return (u32)tmp[0];
		}

		void Complex128::DFT(Complex128 const *ps, Complex128 *pd, size_t len)
		{
			for (size_t k = 0; k < len; ++k)
			{
				Complex128 w(1, 0);
				Float128 th = th.Pi().Half(-1).SelfNegative()*k / len;
				Float128 thw1, thw2;
				th.SinAndCos(thw1, thw2);
				Complex128 wn(thw2, thw1); // h次单位复根 
				pd[k] = Float128::Zero();
				for (size_t i = 0; i < len; ++i)
				{
					pd[k] += ps[i] * w;
					w *= wn;
				}
			}
		}
		void Complex128::IDFT(Complex128 const *ps, Complex128 *pd, size_t len)
		{
			for (size_t k = 0; k < len; ++k)
			{
				Complex128 w(1, 0);
				Float128 th = th.Pi().Half(-1)*k / len;
				Float128 thw1, thw2;
				th.SinAndCos(thw1, thw2);
				Complex128 wn(thw2, thw1); // h次单位复根 
				pd[k] = Float128::Zero();
				for (size_t i = 0; i < len; ++i) { pd[k] += ps[i] * w; w *= wn; }
				pd[k].Real /= len;
				pd[k].Imagine /= len;
			}
		}

	}
}
#endif
