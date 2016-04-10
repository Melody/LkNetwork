#include "stdafx.h"
#ifdef LK_MATH_FLOAT_
namespace LK
{
		ptr<Math::FloatData> const ptr<Math::FloatData>::GetEmpty()
		{
			static ptr<Math::FloatData>* p;
			if (p)return *p;
			GetDefaultMutexRef().lock();
			if (!p)
			{
				static ptr<Math::FloatData> ept = new Math::FloatData();
				ept.UseCount.SetEnableThreadSafe(true);
				p = &ept;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}

		int ptr<Math::FloatData>::SelfPlus(ptr<Math::FloatData> const&x, s64 mov, u64 precision)
		{
			Math::FloatData*pt = get();
			//若有多个对象引用，或者mov为正将导致使用=x1+x2
			if (pt->UseCount != 1 || mov > 0 || &x == this)return FromPlus(*this, x, mov, precision);
			Math::FloatData*px = x.get();
			//计算实际计算比特数
			s64 bits = ((s64)px->bitsCount - mov < (s64)pt->bitsCount) ? pt->bitsCount : px->bitsCount - mov;
			if (bits > (s64)precision)bits = precision;
			size_t mlen = px->Length;
			size_t bmlen = Math::FloatData::GetLengthByBits(px->bitsCount);
			//调整数据部分大小，预留多一个元素
			size_t len = Math::FloatData::GetLengthByBits(bits) + 1;
			pt->Resize(len);
			u64 * p = pt->data;
			u64 * pm = px->data;
			//加法计算
			bool flag = Bits::SelfPlus64(p, len, pm + mlen - bmlen, bmlen, mov + (s64)((s64)len - bmlen) * 64);
			if (flag)
			{
				Bits::Move64(p + pt->Length - len, len, -1);
				//最高位始终为1
				p[pt->Length - 1] |= MinValues::Int64;
			}
			pt->bitsCount = pt->CheckBitsCount();
			return flag;
		}
		int ptr<Math::FloatData>::FromPlus(ptr<Math::FloatData> x1, ptr<Math::FloatData> x2, s64 mov, u64 precision)
		{
			s64 bits;
			if (mov > 0) { mov = -mov; BigObject::Swap(x1, x2); }
			Math::FloatData* p1 = x1.get(), *p2 = x2.get();
			//计算实际计算比特数
			bits = ((s64)p2->bitsCount - mov < (s64)p1->bitsCount) ? p1->bitsCount : p2->bitsCount - mov;
			if (bits > (s64)precision)bits = precision;
			size_t mlen = p2->Length;
			size_t bmlen = Math::FloatData::GetLengthByBits(p2->bitsCount);
			size_t nlen = p1->Length;
			size_t bnlen = Math::FloatData::GetLengthByBits(p1->bitsCount);
			//调整数据部分大小，预留多一个元素用于位移
			size_t len = Math::FloatData::GetLengthByBits(bits) + 1;
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64 * ps = Math::FloatData::Malloc<u64>(size);
			u64*p = ps + size - len;
			*this = new Math::FloatData(bits, len, p, ps);
			u64 * pn = p1->data;
			u64 * pm = p2->data;
			//加法计算
			int flag = Bits::Plus64(p, len, pn + nlen - bnlen, bnlen, pm + mlen - bmlen, bmlen, (s64)((s64)len - bnlen) * 64, (s64)((s64)len - bmlen) * 64 + mov);
			if (flag)
			{
				if (flag == 1)Bits::Move(p, 0, len * 64, -1);
				//最高位始终为1
				p[len - 1] |= MinValues::Int64;
			}
			get()->bitsCount = get()->CheckBitsCount(get()->Length - len);
			return flag;
		}
		s64 ptr<Math::FloatData>::SelfMinus(ptr<Math::FloatData> const&x, s64 mov, u64 precision)
		{
			//若有多个对象引用，将导致使用=x1-x2
			if (get()->UseCount != 1 || &x == this)return FromMinus(*this, x, mov, precision);
			Math::FloatData* pt = get(), *px = x.get();
			//计算实际计算比特数
			s64 bits = ((s64)px->bitsCount - mov < (s64)pt->bitsCount) ? pt->bitsCount : px->bitsCount - mov;
			if (bits > (s64)precision)bits = precision;
			//减法计算
			size_t len = Math::FloatData::GetLengthByBits(bits) + 1;
			pt->Resize(len);
			size_t nlen = pt->Length;
			size_t mlen = Math::FloatData::GetLengthByBits(px->bitsCount);
			u64 *p = pt->data;
			Bits::SelfMinus64(p + nlen - len, len, px->data + px->Length - mlen, mlen, (s64)((s64)len - mlen) * 64 + mov);
			if (p[--nlen] <= MaxValues::Int64)
			{
				while (p[nlen] == 0)if (--nlen == 0)break;
				s64 bm = (s64)(pt->Length - nlen - 1) * 64 + Bits::LeftZeroCount64(p[nlen]);
				Bits::Move64(p + pt->Length - len, len, bm);
				pt->bitsCount = pt->CheckBitsCount(pt->Length - len);
				return -bm;
			}
			pt->bitsCount = pt->CheckBitsCount(pt->Length - len);
			return 0;
		}
		s64 ptr<Math::FloatData>::FromMinus(ptr<Math::FloatData> x1, ptr<Math::FloatData> x2, s64 mov, u64 precision)
		{
			//计算实际计算比特数
			s64 bits = ((s64)x2.get()->bitsCount - mov < (s64)x1.get()->bitsCount) ? x1.get()->bitsCount : x2.get()->bitsCount - mov;
			if (bits > (s64)precision)bits = precision;
			Math::FloatData*p1 = x1.get(), *p2 = x2.get();
			//减法计算
			size_t len = Math::FloatData::GetLengthByBits(bits) + 1;
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64 * ps = Math::FloatData::Malloc<u64>(size);
			u64 * p = ps + size - len;
			size_t nlen = Math::FloatData::GetLengthByBits(p1->bitsCount);
			size_t mlen = Math::FloatData::GetLengthByBits(p2->bitsCount);
			Bits::Minus64(p, len, p1->data + p1->Length - nlen, nlen, p2->data + p2->Length - mlen, mlen, (s64)((s64)len - nlen) * 64, (s64)(len - mlen) * 64 + mov);
			*this = new Math::FloatData(bits, len, p, ps);
			nlen = len;
			if (p[--nlen] <= MaxValues::Int64)
			{
				while (p[nlen] == 0)if (--nlen == 0)break;
				s64 bm = (s64)(len - nlen - 1) * 64 + Bits::LeftZeroCount64(p[nlen]);
				Bits::Move64(p, len, bm);
				return -bm;
			}
			return 0;
		}
		int ptr<Math::FloatData>::SelfMultiply(ptr<Math::FloatData> const& x, u64 precision)
		{
			return FromMultiply(*this, x, precision);
		}
		int ptr<Math::FloatData>::FromMultiply(ptr<Math::FloatData> x1, ptr<Math::FloatData> x2, u64 precision)
		{
			//获取乘数长度和数据信息
			Math::FloatData*p1 = x1.get(), *p2 = x2.get();
			size_t nlen = Math::FloatData::GetLengthByBits(p1->bitsCount);
			size_t mlen = Math::FloatData::GetLengthByBits(p2->bitsCount);
			size_t len = Math::FloatData::GetLengthByBits(precision);
			if (nlen > len) { nlen = len + 1; }
			if (mlen > len) { mlen = len + 1; }
			u64*pn = (p1->data + p1->Length - nlen);
			u64*pm = (p2->data + p2->Length - mlen);

			size_t size = Math::FloatData::GetSizeByLength(len);
			u64 * ps = Math::FloatData::Malloc<u64>(size);
			u64 * p = ps + size - len;
			u64 low;
#ifdef LK_MATH_DOUBLECOMPLEX_
#define _if_DOUBLECOMPLEX_ 1
#else
#define _if_DOUBLECOMPLEX_ 0
#endif
#ifdef LK_MATH_COMPLEX128_
#define _if_COMPLEX128_ 1
#else
#define _if_COMPLEX128_ 0
#endif
#if _if_DOUBLECOMPLEX_ || _if_COMPLEX128_
			//简单评价决定到底采用FFT还是线性相乘
			double mt64 = double(nlen)*mlen;
			double mtFFT = (double)(1ull << (64 - Bits::LeftZeroCount64((nlen > mlen ? nlen : mlen) - 1)));
			mtFFT *= (nlen > mlen ? nlen : mlen)*12;
#endif
#undef _if_DOUBLECOMPLEX_
#undef _if_COMPLEX128_
#ifdef LK_MATH_DOUBLECOMPLEX_
			if (mt64 >= mtFFT && nlen < (65536 / 64) && mlen < (65536 / 64))//采用FFT，这里有个限制：乘数总大小不允许大于1MB
			{
				low = (u64)Bits::MultiplyFFTHigh((u16*)p, len * 4, (u16*)pn, nlen * 4, (u16*)pm, mlen * 4) << (64 - 16);
			}
			else if (mt64 >= mtFFT)
			{
				low = (u64)Bits::MultiplyFFTHigh8((u8*)p, len * 8, (u8*)pn, nlen * 8, (u8*)pm, mlen * 8) << (64 - 8);
			}
			else
#endif/*
#ifdef COMPLEX128_
			if (mt64 >= mtFFT*10 )
			{
				low = (u64)Bits::MultiplyFFTHigh32((u32*)p, len *2, (u32*)pn, nlen *2, (u32*)pm, mlen * 2) << (32);
			}
			else
#endif*/
//采用线性相乘
low = Bits::MultiplyHigh64(p, len, pn, nlen, pm, mlen);
			int flag = 0;
			if ((s64)p[len - 1] > 0) { Bits::Move64(p, len, 1); if ((s64)low < 0)p[0] |= 1; low <<= 1; }
			else flag = 1;
			if ((s64)low < 0)
			{
				while (len != 0 && ++*p == 0) { --len; ++p; }
				if (len == 0)
				{
					*--p = MinValues::Int64;
					len = 1;
					++flag;
				}
			}
			*this = new Math::FloatData(precision, len, p, ps);
			get()->bitsCount = get()->CheckBitsCount();
			return flag;
		}
		int ptr<Math::FloatData>::SelfDivide(ptr<Math::FloatData> const& x, u64 precision)
		{
			return FromDivide(*this, x, precision);
		}
		int ptr<Math::FloatData>::FromDivide(ptr<Math::FloatData> x1, ptr<Math::FloatData> x2, u64 precision)
		{
			//获取乘数长度和数据信息
			Math::FloatData* p1 = x1.get(), *p2 = x2.get();
			size_t nlen = Math::FloatData::GetLengthByBits(p1->bitsCount);
			size_t mlen = Math::FloatData::GetLengthByBits(p2->bitsCount);
			size_t len = Math::FloatData::GetLengthByBits(precision + 1);
			if (nlen > len)nlen = len + 1;
			if (mlen > len)mlen = len + 1;

			u64 *pn = p1->data + p1->Length - nlen;
			u64*pm = p2->data + p2->Length - mlen;
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64 * ps = Math::FloatData::Malloc<u64>(size);
			u64 * p = ps + size - len;
			int rt;
			u64 flag;
			if (Bits::CompareHigh64(pn, pm, nlen, mlen) >= 0) {
				rt = 0;
				Bits::DivideHigh64(p, len, pn, nlen, pm, mlen, true, &flag, 1);
			}
			else {
				rt = -1;
				Bits::DivideHigh64(p, len, pn, nlen, pm, mlen, false, &flag, 1);
			}
			if ((s64)flag < 0)
			{
				while (len != 0 && ++*p == 0) { --len; ++p; }
				if (len == 0) { *--p = MinValues::Int64; len = 1; rt = 1; }
			}
			*this = new Math::FloatData(precision, len, p, ps);
			get()->bitsCount = get()->CheckBitsCount();
			return rt;
		}
		void ptr<Math::FloatData>::SelfInteger(u64 bits)
		{
			Math::FloatData* fp = get();
			if (bits >= fp->bitsCount)return;
			if (UseCount == 1)
			{
				Bits::Set(fp->data, (index_t)(fp->Length * 64ll - fp->bitsCount), (size_t)(fp->bitsCount - bits), false);
				fp->bitsCount = fp->CheckBitsCount(fp->Length - Math::FloatData::GetLengthByBits(bits));
			}
			else FromInteger(*this, bits);
		}
		void ptr<Math::FloatData>::FromInteger(ptr<Math::FloatData> const& x, u64 bits)
		{
			Math::FloatData* fp = x.get();
			if (bits > fp->bitsCount)bits = fp->bitsCount;
			*this = new Math::FloatData(fp->data, 64ull * fp->Length - bits, bits);
		}
		s64 ptr<Math::FloatData>::SelfDecimal(u64 mov)
		{
			Math::FloatData* fp = get();
			if (mov >= fp->bitsCount) { *this = ptr<Math::FloatData>::GetEmpty(); return -(s64)mov; }
			if (UseCount == 1)
			{
				size_t i = fp->Length;
				size_t tlen = Math::FloatData::GetLengthByBits(fp->bitsCount);
				Bits::Move64(fp->data + i - tlen, tlen, mov);
				tlen = Math::FloatData::GetLengthByBits(fp->bitsCount - mov);
				size_t end = i - tlen;
				u64 bmov = 0;
				u64 *p = fp->data;
				while (i-- != end)
				{
					if (p[i])break;
					bmov += 64;
				}
				if (i + 1 == end) { fp->bitsCount = 0; return -(s64)mov; }
				bmov += Bits::LeftZeroCount64(p[i]);
				Bits::Move64(fp->data + fp->Length - tlen, tlen, bmov);
				fp->bitsCount = fp->CheckBitsCount(fp->Length - Math::FloatData::GetLengthByBits(fp->bitsCount - mov - bmov));
				return -(s64)mov - (s64)bmov;
			}
			return FromDecimal(*this, mov);
		}
		s64 ptr<Math::FloatData>::FromDecimal(ptr<Math::FloatData> const& x, u64 mov)
		{
			Math::FloatData* fp = x.get();
			if (mov >= fp->bitsCount) { *this = GetEmpty(); return -(s64)mov; }
			s64 bits = fp->bitsCount - mov;
			size_t len = Math::FloatData::GetLengthByBits(bits);
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64* ps = Math::FloatData::Malloc<u64>(size);
			u64* p = ps + size - len;
			p[0] = 0;
			Bits::Copy(p, (index_t)(len * 64ull - bits), fp->data, (index_t)(64ull * fp->Length - fp->bitsCount), (size_t)bits);
			u64 bmov = 0;
			size_t i = len;
			while (i-- > 0)
			{
				if (p[i])break;
				bmov += 64;
			}
			if (i + 1 == 0) { *this = GetEmpty(); return -(s64)mov; }
			bmov += Bits::LeftZeroCount64(p[i]);
			Bits::Move64(p, len, bmov);
			*this = new Math::FloatData(fp->bitsCount - mov - bmov, len, p, ps);
			return -(s64)mov - (s64)bmov;
		}
		bool ptr<Math::FloatData>::SelfSetBits(u64 bits)
		{
			if (bits == 0)return false;
			Math::FloatData* pp = get();
			if (bits >= pp->bitsCount)return false;
			if (UseCount != 1)*this = pp = Conversion::Convert<ptr<Math::FloatData>>::reference(pp->Clone()).get();
			size_t clen = Math::FloatData::GetLengthByBits(bits);
			bits &= 63;
			size_t lmov = (size_t)(64 - bits);
			u64*p = pp->data + pp->Length - clen;
			bool flag = ((bits & 63) == 0) ? (pp->Length > clen && (s64)*(p - 1) < 0) : ((s64)(*p << bits) < 0);
			if (pp->Length > clen)*(p - 1) = 0;
			if (bits != 0)*p = ((*p >> lmov) << lmov);
			if (flag) {
				if (bits == 0)
					flag = ++*p == 0;
				else flag = ((*p += 1ull << lmov) == 0);
				while (flag&& clen > 1)
				{
					--clen;
					flag = ++(*++p) == 0;
				}
				if (flag) { *--p = MinValues::Int64; clen = 1; }
				pp->Length = clen;
				pp->data = p;
			}
			pp->bitsCount = pp->CheckBitsCount(pp->Length - clen);
			return flag;
		}
		s64 ptr<Math::FloatData>::CompareWith(ptr<Math::FloatData> const& xf, s64 mov)const
		{
			Math::FloatData*fp = get();
			Math::FloatData*xfp = xf.get();
			return Bits::DifferenceIndexHigh(fp->data, 0, (index_t)(fp->Length * 64), xfp->data, 0, (size_t)(xfp->Length * 64), (index_t)mov);
		}

		namespace Math
		{
		u64 Float::MinPrecision = 32;
		u64 Float::MaxPrecision = 102400;
		u64 Float::DefPrecision = 512;
		u64 Float::OblPrecision = 8;
		bool Float::StopCalculating = false;
		//构造
		Float::Float(std::string const& str, u64 prec) : Exp(0), Data(ptr<Math::FloatData>::GetEmpty()), Precision(prec), Sign(0)
		{
			char const* pstr = str.c_str();
			index_t len = str.size();
			ptr<Math::Float> x(0, MaxValues::Int64);
			if (len != 0)
			{
				index_t expindex = 0;
				index_t point = str.size();
				while (expindex != len) { if (pstr[expindex] == '.')point = expindex; else if (pstr[expindex] == 'e' || pstr[expindex] == 'E')break; expindex++; }
				if (point > expindex)point = expindex;
				Exp = expindex < len - 1 ? Float256(str.substr(expindex + 1, len - (expindex + 1))) : Float256::Zero();//指数
				index_t i = 0;
				u64 ull;
				while (i < point - 19)
				{
					ull = 0;
					for (index_t j = 0; j < 19; ++j)ull = ull * 10 + (pstr[i + j] - '0');
					x = x * ptr<Math::Float>(10000000000000000000ull, MaxValues::Int64) + ptr<Math::Float>(ull, prec);
					i += 19;
				}
				if (i < point)
				{
					ull = 0;
					u64 tmp = 1;
					while (i < point) { ull = ull * 10 + (pstr[i] - '0'); tmp *= 10; ++i; }
					x = x * ptr<Math::Float>(tmp, MaxValues::Int64) + ptr<Math::Float>(ull, prec);
				}
				if (point < expindex - 1)
				{
					i = point + 1;
					while (i < expindex - 19)
					{
						ull = 0;
						for (index_t j = 0; j < 19; ++j)ull = ull * 10 + (pstr[i + j] - '0');
						x = x * ptr<Math::Float>(10000000000000000000ull, MaxValues::Int64) + ptr<Math::Float>(ull, prec);
						i += 19;
					}
					if (i < expindex)
					{
						ull = 0;
						u64 tmp = 1;
						while (i < expindex) { ull = ull * 10 + (pstr[i] - '0'); tmp *= 10; ++i; }
						x = x * ptr<Math::Float>(tmp, MaxValues::Int64) + ptr<Math::Float>(ull, prec);
					}
					x *= ptr<Math::Float>(10, prec).Pow(ptr<Math::Float>(Exp - (expindex - point - 1), prec));
				}
				else x *= ptr<Math::Float>(10, prec).Pow(ptr<Math::Float>(Exp, prec));
			}
			SwapWith(*x.get());
		}
		//+=自增
		Float& Float::operator+=(Float const&vf)
		{
			//错误传递
			if (IsError())return *this;
			if (vf.IsError())return MakeError(vf.Sign);
			//+0处理
			if (vf.Equals0())
			{
				s64 precmove = GetPrec(Exp, Precision, vf.Exp, vf.Precision);
				if (precmove <= 0)
					return *this = vf;
				if (precmove < (s64)MinPrecision)Precision = MinPrecision;
				else Precision = precmove;
				if (Data.SelfSetBits(Precision))++Exp;
				return *this;
			}
			if (Equals0())
			{
				s64 precmove = GetPrec(vf.Exp, vf.Precision, Exp, Precision);
				if (precmove > 0)
				{
					*this = vf;
					if (precmove < (s64)MinPrecision)Precision = MinPrecision;
					else Precision = precmove;
					if (Data.SelfSetBits(Precision))++Exp;
				}
				return *this;
			}
			//符号相同
			if (Sign == vf.Sign)
			{
				//指数较大，主导精度高位
				if (Exp >= vf.Exp)
				{
					//如果指数相差超出范围则直接返回
					s64 mov;
					s64 prec = PrecMinus(Exp, vf.Exp, Precision, &mov);
					if (prec > 0)return *this;
					//精度约束修正
					//计算precision为相对于this的最终精度，这里认为相加不会导致精度减少（实际上精度低位相同的数相加，若没有进位，精度将会减少1bit；精度低位不同时，精度也有少量减少）
					if (-prec == (s64)vf.Precision)  --Precision;
					else if ((u64)-prec > vf.Precision) Precision = (s64)vf.Precision + mov;
					//这里将按MaxPrecision和MinPrecision进行精度限制
					if (Precision > MaxPrecision)Precision = MaxPrecision; else if (Precision < MinPrecision)Precision = MinPrecision;
					int flag = Data.SelfPlus(vf.Data, -mov, Precision);
					if (flag != 0)//如果产生进位，指数和精度增加
					{
						Exp += flag;
						Precision += flag;
						if (Precision > MaxPrecision)Precision = MaxPrecision;
					}
				}
				else
				{
					//如果指数相差超出范围则直接返回
					s64 mov;
					s64 prec = PrecMinus(vf.Exp, Exp, vf.Precision, &mov);
					if (prec > 0)return *this = vf;
					//精度约束修正
					//计算precision为相对于this的最终精度，这里认为相加不会导致精度减少（实际上精度低位相同的数相加，若没有进位，精度将会减少1bit；精度低位不同时，精度也有少量减少）
					if (-prec == (s64)Precision)  Precision = vf.Precision - 1;
					else if ((u64)-prec > Precision) Precision += mov;
					else Precision = vf.Precision;
					Exp = vf.Exp;
					//这里将按MaxPrecision和MinPrecision进行精度限制
					if (Precision > MaxPrecision)Precision = MaxPrecision; else if (Precision < MinPrecision)Precision = MinPrecision;
					int flag = Data.FromPlus(vf.Data, Data, -mov, Precision);
					if (flag != 0)//如果产生进位，指数和精度增加
					{
						Exp += flag;
						Precision += flag;
						if (Precision > MaxPrecision)Precision = MaxPrecision;
					}
				}
			}
			else//符号相反
			{
				s64 dif = 0;
				if (Exp == vf.Exp)if ((dif = Data.CompareWith(vf.Data, 0)) == 0) { Sign = 0; Data = Data.GetEmpty(); return*this; }
				if (dif > 0 || Exp > vf.Exp)//保号的情况
				{
					//如果指数相差超出范围则直接返回
					s64 mov;
					s64 prec = PrecMinus(Exp, vf.Exp, Precision, &mov);
					if (prec > 0)return *this;
					//精度约束
					if (-prec == (s64)vf.Precision)  --Precision;
					else if ((u64)-prec > vf.Precision) Precision = (s64)vf.Precision + mov;
					if ((u64)dif > Precision) { Sign = 0; Data = Data.GetEmpty(); return*this; }
					//这里将按MaxPrecision和MinPrecision进行精度限制
					if (Precision - dif > MaxPrecision)Precision = MaxPrecision + dif; else if (Precision - dif < MinPrecision)Precision = MinPrecision + dif;
					//Precision为相对于this的最终精度
					//指数改变
					s64 bmov = Data.SelfMinus(vf.Data, -mov, Precision);
					if (Data.get()->bitsCount == 0) { Sign = 0; return *this; }//置0
					Precision += bmov;
					Exp += bmov;
				}
				else//反转号
				{
					//如果指数相差超出范围则直接返回
					s64 mov;
					s64 prec = PrecMinus(vf.Exp, Exp, vf.Precision, &mov);
					if (prec > 0)return *this = vf;
					//精度约束
					if (-prec == (s64)Precision)  Precision = vf.Precision - 1;
					else if ((u64)-prec > Precision) Precision += mov;
					else Precision = vf.Precision;
					Exp = vf.Exp;
					if ((u64)-dif > Precision) { Sign = 0; Data = Data.GetEmpty(); return*this; }
					//这里将按MaxPrecision和MinPrecision进行精度限制
					if (Precision + dif > MaxPrecision)Precision = MaxPrecision - dif; else if (Precision + dif < MinPrecision)Precision = MinPrecision - dif;
					//Precision为相对于vf的最终精度
					//指数改变
					s64 bmov = Data.FromMinus(vf.Data, Data, -mov, Precision);
					if (Data.get()->bitsCount == 0) { Sign = 0; return *this; }//置0
					Precision += bmov;
					Exp += bmov;
					Sign = vf.Sign;
				}
			}
			if (Data.SelfSetBits(Precision))++Exp;//精确限制
			return *this;
		}
		//相加
		Float Float::operator+(Float const&vf2)const
		{
			return (+*this) += vf2;
		}
		//自增(前置)
		Float& Float::operator++() { return *this += One(); }
		//自增(后置)
		Float Float::operator++(int) { Float tmp(*this); *this += One(); return tmp; }

		//-=自减
		Float& Float::operator-=(Float const&vf)
		{
			return *this += (-vf);
		}
		//相减
		Float Float::operator-(Float const&vf2)const
		{
			return (+*this) -= vf2;
		}
		//自减(前置)
		Float& Float::operator--() { return *this -= One(); }
		//自减(后置)
		Float Float::operator--(int) { Float tmp(*this); *this -= One(); return tmp; }

		//自乘
		Float& Float::operator*=(Float const& vf)
		{
			//错误传递
			if (IsError())return *this;
			if (vf.IsError())return MakeError(vf.Sign);
			Exp += vf.Exp;
			if (vf.Precision < Precision)Precision = vf.Precision;
			//精度限制
			if (Precision > MaxPrecision)Precision = MaxPrecision;
			else if (Precision < MinPrecision)Precision = MinPrecision;

			if (Equals0() || vf.Equals0()) { Sign = 0; return *this; }
			Sign = Sign == vf.Sign ? 1 : -1;
			Exp += Data.SelfMultiply(vf.Data, Precision);
			//精度检查
			if (Data.SelfSetBits(Precision))++Exp;
			return *this;
		}
		//相乘
		Float Float::operator*(Float const&vf2)const
		{
			return (+*this) *= vf2;
		}
		//使用乘法计算倒数
		Float Float::Reciprocal(Float const&num)const
		{
			if (IsError())return *this;
			if (Equals0())return Float().MakeError(MathError::Divide0);
			if (num.IsError())return num;
			Float t = *this;
			t.Precision = num.Precision > Precision ? Precision : num.Precision;
			if (t.Precision > MaxPrecision)t.Precision = MaxPrecision;
			else if (t.Precision < MinPrecision)t.Precision = MinPrecision;
			if (num.Equals0())
			{
				t.Exp = num.Exp - Exp;
				t.Data = ptr<Math::FloatData>::GetEmpty();
				return t;
			}
			t.Exp = Float256::Zero();
			Float tThis(t);
			t = Float(Float256::One() / t.ToFloat256(), tThis.Precision);
			Float one(One(), t.Precision);
			Float tmp = one;
			u64 tp = 256;
			for (;;)
			{
				if (tp > Precision)
				{
					if (tmp.AppEquals(one))break;
				}
				else
				{
					tp <<= 1;
					t.Precision = tp + 32;
				}
				tmp = tThis*t;
				++(++tmp.SelfNegative().SelfHalf()).Exp;
				t *= tmp;
			}
			t.Exp -= Exp;
			return t*num;
		}
		//自除
		Float& Float::operator/=(Float const& vf)
		{
			if (IsError())return *this;
			if (vf.IsError())return *this = vf;
			if (vf.Equals0())return MakeError(MathError::Divide0);
			if (Precision > vf.Precision)Precision = vf.Precision;
			if (Equals0()) { Exp -= vf.Exp; return *this; }
			//精度限制
			if (Precision > MaxPrecision)Precision = MaxPrecision;
			else if (Precision < MinPrecision)Precision = MinPrecision;
			Exp -= vf.Exp;
			Exp += Data.SelfDivide(vf.Data, Precision);
			if (Exp == Exp.MinValue())Sign = 0;
			else Sign = Sign == vf.Sign ? 1 : -1;
			return *this;
		}
		//相除
		Float Float::operator/(Float const&vf2)const
		{
			return (+*this) /= vf2;
		}

		Float Float::Mod(Float const& num, Float &quotient)const
		{
			if (IsError())return *this;
			if (num.IsError())return num;
			if (num.Equals0())return Float().MakeError(MathError::ModDivide0);
			if (Exp<num.Exp)//0.1%2=0...0.1，0.1%-2=-1...-1.9，-0.1%2=-1...1.9，-0.1%-2=0...-0.1
			{
				if (Sign == num.Sign)
				{
					quotient = *this;
					quotient.Exp -= num.Exp;
					if (num.Precision < Precision)
						quotient.Precision = num.Precision;
					quotient.Sign = 0;
					quotient.Data = ptr<Math::FloatData>::GetEmpty();
				}
				else { quotient = One(); quotient.Sign = -1; quotient.Precision = Precision > num.Precision ? num.Precision : Precision; }
				if (quotient.Precision>MaxPrecision)quotient.Precision = MaxPrecision;
				else if (quotient.Precision < MinPrecision)quotient.Precision = MinPrecision;
				if (Sign == num.Sign)return *this;
				return *this + num;
			}
			index_t cmp = (index_t)Data.CompareWith(num.Data, 0);
			if (cmp == 0)
			{
				quotient = One();
				quotient.Sign = Sign == num.Sign ? 1 : -1;
				quotient.Exp = Exp - num.Exp;
				if (quotient.Exp <= Exp.MinValue()) { quotient.Sign = 0; quotient.Data = ptr<Math::FloatData>::GetEmpty(); }
				else if (quotient.Exp >= Exp.MaxValue())quotient.MakeError(MathError::DivideExpOverflow);
				return Float(0, Precision > num.Precision ? num.Precision : Precision);
			}
			if (Exp == num.Exp) {
				if (cmp > 0)//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
				{
					if (Sign == num.Sign) { quotient = One(); }
					else { quotient = Two(); quotient.Sign = -1; }
					quotient.Precision = Precision > num.Precision ? num.Precision : Precision;
					if (quotient.Precision > MaxPrecision)quotient.Precision = MaxPrecision;
					else if (quotient.Precision < MinPrecision)quotient.Precision = MinPrecision;
					if (Sign == num.Sign)return *this - num;
					return *this + num.Half(-1);
				}
				else
				{
					if (Sign == num.Sign)
					{
						quotient = *this;
						quotient.Exp -= num.Exp;
						if (num.Precision < Precision)
							quotient.Precision = num.Precision;
						quotient.Sign = 0;
						quotient.Data = ptr<Math::FloatData>::GetEmpty();
					}
					else { quotient = One(); quotient.Sign = -1; quotient.Precision = Precision > num.Precision ? num.Precision : Precision; }
					if (quotient.Precision > MaxPrecision)quotient.Precision = MaxPrecision;
					else if (quotient.Precision < MinPrecision)quotient.Precision = MinPrecision;
					if (Sign == num.Sign)return *this;
					return *this + num;
				}
			}
			Float rf;
			rf.Precision = Precision > num.Precision ? num.Precision : Precision;
			quotient.Precision = rf.Precision;
			s64 mov = Exp - num.Exp;
			if ((u64)mov > Precision) { quotient = *this / num; return Float(Zero(), rf.Precision); }
			size_t len = Math::FloatData::GetLengthByBits(rf.Precision);
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64* p = Math::FloatData::Malloc<u64>(size);
			quotient.Data = new Math::FloatData(Precision, len, p + size - len, p);
			u64* pf = Math::FloatData::Malloc<u64>(size);
			rf.Data = new Math::FloatData(Precision, len, pf + size - len, pf);
			if (cmp > 0)
			{
				Bits::ModHigh64(p + size - len, len, mov + 1, Data.get()->data, Data.get()->Length, num.Data.get()->data, num.Data.get()->Length, true, pf + size - len, len);
			}
			else
			{
				Bits::ModHigh64(p + size - len, len, mov, Data.get()->data, Data.get()->Length, num.Data.get()->data, num.Data.get()->Length, false, pf + size - len, len);
			}
			s64 rmov = 0;
			for (size_t i = 0; i != len; rmov += 64)
			{
				++i;
				if (pf[size - i] != 0) { rmov += Bits::LeftZeroCount64(pf[size - i]); break; }
			}
			if ((u64)rmov >= rf.Precision)
			{
				quotient.Sign = Sign == num.Sign ? 1 : -1;
				quotient.Exp = Exp - num.Exp - (int)(cmp < 0);
				if (quotient.Exp <= Float256::MinValue()) { quotient.Sign = 0; quotient.Data = ptr<Math::FloatData>::GetEmpty(); }
				else if (quotient.Exp >= Float256::MaxValue())quotient.MakeError(MathError::DivideExpOverflow);
				return Float(Zero(), rf.Precision);
			}
			*rf.Data.get() <<= rmov;
			rf.Exp = num.Exp - rmov;
			if (rf.Exp <= Float256::MinValue())rf.Sign = 0;
			quotient.Sign = 1;
			quotient.Exp = Exp - num.Exp - (int)(cmp < 0);
			if (quotient.Exp <= Float256::MinValue()) { quotient.Sign = 0; quotient.Data = ptr<Math::FloatData>::GetEmpty(); }
			else if (quotient.Exp >= Float256::MaxValue())quotient.MakeError(MathError::DivideExpOverflow);
			rf.Sign = num.Sign;
			//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
			if (Sign != num.Sign)
			{
				++quotient; quotient.Sign = -1;
				rf = num - rf;
			}
			return rf;
		}
		/**
		@brief 求余*/
		Float Float::Mod(Float const& num)const
		{
			if (IsError())return *this;
			if (num.IsError())return num;
			if (num.Equals0())return Float().MakeError(MathError::ModDivide0);
			if (Exp<num.Exp)//0.1%2=0...0.1，0.1%-2=-1...-1.9，-0.1%2=-1...1.9，-0.1%-2=0...-0.1
			{
				if (Sign == num.Sign)return *this;
				return *this + num;
			}
			index_t cmp = (index_t)Data.CompareWith(num.Data, 0);
			if (cmp == 0)
			{
				return Float(0, Precision>num.Precision ? num.Precision : Precision);
			}
			if (Exp == num.Exp) {
				if (cmp > 0)//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
				{
					if (Sign == num.Sign)return *this - num;
					return *this + num.Half(-1);
				}
				else
				{
					if (Sign == num.Sign)return *this;
					return *this + num;
				}
			}
			Float rf;
			rf.Precision = Precision > num.Precision ? num.Precision : Precision;
			s64 mov = Exp - num.Exp;
			if ((u64)mov > Precision) { return Float(Zero(), rf.Precision); }
			size_t len = Math::FloatData::GetLengthByBits(rf.Precision);
			size_t size = Math::FloatData::GetSizeByLength(len);
			u64* pf = Math::FloatData::Malloc<u64>(size);
			rf.Data = new Math::FloatData(Precision, len, pf + size - len, pf);
			if (cmp > 0)
			{
				Bits::ModHigh64(NULL, 0, mov + 1, Data.get()->data, Data.get()->Length, num.Data.get()->data, num.Data.get()->Length, true, pf + size - len, len);
			}
			else
			{
				Bits::ModHigh64(NULL, 0, mov, Data.get()->data, Data.get()->Length, num.Data.get()->data, num.Data.get()->Length, false, pf + size - len, len);
			}
			s64 rmov = 0;
			for (size_t i = 0; i != len; rmov += 64)
			{
				++i;
				if (pf[size - i] != 0) { rmov += Bits::LeftZeroCount64(pf[size - i]); break; }
			}
			if ((u64)rmov >= rf.Precision)
			{
				return Float(Zero(), rf.Precision);
			}
			*rf.Data.get() <<= rmov;
			rf.Exp = num.Exp - rmov;
			if (rf.Exp <= Float256::MinValue())rf.Sign = 0;
			rf.Sign = num.Sign;
			//2.1%2=1...0.1，2.1%-2=-2...-1.9，-2.1%2=-2...1.9，-2.1%-2=1...-0.1
			if (Sign != num.Sign)
			{
				rf = num - rf;
			}
			return rf;
		}
		//自求余
		Float& Float::operator%=(Float const&vf)
		{
			return *this = Mod(vf);
		}
		//求余
		Float Float::operator%(Float const&vf2)const
		{
			return Mod(vf2);
		}
		Float& Float::SelfGetIntegerPart()
		{
			if (Equals0() || IsError())return*this;
			if (Exp < 0) { Sign = 0; return*this; }
			if (Exp - Precision >= -1)return*this;
			Data.SelfInteger(Exp + 1);
			return *this;
		}
		bool Float::IsInteger()const
		{
			if (Equals0())return true;
			if (Exp.Sign < 0)return false;
			if (Exp - Data.get()->bitsCount >= -1)return true;
			return false;
		}
		Float Float::GetIntegerPart()const
		{
			if (Equals0() || IsError())return*this;
			if (Exp.Sign < 0) { return Float(0, Exp, ptr<Math::FloatData>::GetEmpty(), Precision); }
			if (Exp - Precision >= -1)return*this;
			ptr<Math::FloatData> x;
			x.FromInteger(Data, Exp + 1);
			return Float(Sign, Exp, x, Precision);
		}
		Float& Float::SelfGetDecimalPart()
		{
			if (Equals0() || IsError())return*this;
			if (Exp.Sign < 0) { return*this; }
			if (Exp - (s64)Precision >= -1) { Sign = 0; return*this; }
			s64 mov = Data.SelfDecimal(Exp + 1);
			if (Data.get()->bitsCount == 0) { Sign = 0; return*this; }
			//精度
			Float256 prec = Float256(mov) + Precision;
			//精度
			Precision = prec > MaxPrecision ? MaxPrecision : (prec < MinPrecision ? MinPrecision : (u64)prec);
			Exp += mov;
			return *this;
		}
		Float Float::GetDecimalPart()const
		{
			if (Equals0() || IsError())return*this;
			if (Exp.Sign < 0) { return*this; }
			if (Exp - Precision >= -1) { return Float(0, Exp, ptr<Math::FloatData>::GetEmpty(), Precision); }
			ptr<Math::FloatData> x;
			s64 mov = x.FromDecimal(Data, Exp + 1);
			//精度
			Float256 prec = Float256(mov) + Precision;
			u64 pc = prec > MaxPrecision ? MaxPrecision : (prec < MinPrecision ? MinPrecision : (u64)prec);
			if (x.get()->bitsCount == 0)return Float(0, Exp + mov, x, pc);
			return Float(Sign, Exp + mov, x, pc);
		}
		std::string Float::ToString(size_t len, size_t explen)const
		{
			if (Equals0())return "0";
			if (IsError()) { return Sign < -22 ? "未知错误." : MathError::description[-Sign]; }
			if (Precision == 0)return "错误：精度为0 ( ±1*2^" + Exp.ToString() + ")";
			//计算由精度决定的最大显示数值
			s64 prec;
			if (len == 0)
			{
				Float256 fprec = Float256(Precision > OblPrecision ? Precision - OblPrecision : Precision) / Float256::Ln10()*Float256::Ln2();
				if (fprec < Float256::One())len = 1; else len = (size_t)fprec;
				if ((Exp.Sign<0) && len>this->Data.get()->bitsCount)len = (size_t)this->Data.get()->bitsCount;
				else if ((Exp.Sign >= 0) && len>this->Data.get()->bitsCount + Exp)len = size_t(this->Data.get()->bitsCount + Exp);
				prec = Precision;
			}
			else//适当变换精度，减少计算量
			{
				prec = Float256(len) / Float256::Ln2()*Float256::Ln10();
				prec = (s64)(prec + OblPrecision);
				if (prec > (s64)(Precision + OblPrecision))prec = (s64)(Precision + OblPrecision);
			}
			if ((u64)prec > Math::Float::MaxPrecision)prec = Math::Float::MaxPrecision;
			ptr<Math::Float> num(ptr<Math::Float>(this), prec);
			num.SelfAbs();
			//指数计算法
			ptr<Math::Float> expnum(num, 300);
			Float256 rexp = ((expnum.Ln() * ptr<Math::Float>::Calculate1Ln10(expnum.GetPrecision())).ToFloat256()).SelfGetIntegerPart();
			if (*num.get() < One())--rexp;
			num *= ptr<Math::Float>(ptr<Math::Float>::Ten(), num.GetPrecision()).Pow(ptr<Math::Float>(-rexp, num.GetExp().Exp + num.GetPrecision()));//如果计算出来的指数超出常规预期，则说明指数精度不够，有效数字进入随机态
			if (num.Equals0())
			{
				num = (double)(GetDefaultRand()>>1)/((u64)MaxValues::Int64+1);
			}
			else if ((num.GetExp() > Float256::Three() || num.GetExp() < -Float256::One()) && (*num.get() >= Ten() || *num.get() < One()))
			{
				do {
					num.SetExp(GetDefaultRand() % 4);
				} while (*num.get() >= Ten());
			}
			if (*num.get() >= Ten())
			{
				num /= ptr<Math::Float>::Ten(); ++rexp;
			}
			else if (*num.get() < One()) { num.SelfMultiply10(); --rexp; }
			num *= ptr<Math::Float>(1000000000000000000ull, MaxValues::Int64);
			//转换到可计算的十进制量
			static ptr<Math::Float> _1e19 = ptr<Math::Float>(10000000000000000000ull, MaxValues::Int64);
			size_t slen = len / 19 + 1;
			u8* tarr=new u8[sizeof(u64)*slen];
			u64 * pstr = (u64*)tarr;
			size_t i = 0;
			for (;;)
			{
				pstr[i] = num.GetIntegerPart().ToUInt64();
				if (++i >= slen)break;
				num.SelfGetDecimalPart();
				num *= _1e19;
			}
			u64 last = len / 19;
			i = 18 - (len % 19);
			u64 lastplus = 5;
			while (i != 0) { i--; lastplus *= 10; }
			if ((pstr[last] += lastplus) >= 10000000000000000000ull)//进位
			{
				pstr[last] %= 10000000000000000000ull;
				i = (size_t)last;
				while (i-- > 0)
				{
					if (++pstr[i] == 10000000000000000000ull)pstr[i] = 0;
					else break;
				}
				if (++i == 0)
				{
					pstr[last] = 0;
					pstr[0] = 1000000000000000000ull;
					rexp++;
				}
			}
			i = 19 - (len % 19);
			for (size_t j = 0; j < i; ++j)pstr[last] /= 10;
			for (size_t j = 0; j < i; ++j)pstr[last] *= 10;
			while (pstr[last] == 0) { last--; slen--; }
			slen *= 19;
			i = 0; while (pstr[last] % 10 == 0) { pstr[last] /= 10; ++i; }
			slen -= i;
			while (i-- != 0) { pstr[last] *= 10; }
			//指数转字符串
			bool useexp = false;//科学计数法
			u64 pointi;//小数点位置
			size_t beforezero = 0;//前缀0
			size_t backzero = 0;//后缀0
			std::string expstr;
			if (rexp<-4 || rexp>slen + 4)//科学计数法，插入小数点，加上指数
			{
				useexp = true;
				expstr = rexp.ToString();
				pointi = 1;
			}
			else if (rexp.Sign < 0)
			{
				beforezero = -(long)rexp;
				pointi = 0;
			}
			else
			{
				pointi = (s64)rexp + 1;
				backzero = pointi > slen ? (size_t)(pointi - slen) : 0;
			}
			size_t finallen = beforezero + backzero + 1 + slen + ((Sign < 0) ? 1 : 0) + (useexp ? expstr.size() + 1 : 0);
			std::string p;
			p.resize(finallen + 1);
			i = 0;
			if (Sign < 0)p[i++] = '-';
			if (beforezero)
			{
				p[i++] = '0'; p[i++] = '.';
				while (--beforezero>0)p[i++] = '0';
			}
			size_t j = 0;
			u64 k = 1000000000000000000ull;
			while (j < slen)
			{
				p[i++] = '0' + (pstr[j / 19] / k) % 10;
				k /= 10; if (k == 0)k = 1000000000000000000ull;
				if (--pointi == 0 && j != slen - 1)p[i++] = '.';
				j++;
			}
			while (backzero-->0)p[i++] = '0';
			if (useexp)
			{
				p[i++] = 'e';
				memcpy(&p[i], expstr.data(), expstr.size() * sizeof(WCHAR));
				i += expstr.size();
			}
			p.resize(i);
			delete []tarr;

			return p;
		}
	}

		//与字符串的转换
		ptr<Math::Float>::ptr(std::string const& str)
		{
			index_t expindex = Strings::IndexOf(str,'E', 0, true);
			if (expindex < 0)expindex = str.size();
			//如果有两个小数点，则从第二个小数点开始表示精度
			if (Strings::CountOf(str,'.') == 2)
			{
				index_t precp = Strings::LastIndexOf(str,'.');
				if (precp + 1 < expindex)
				{
					s64 vprec = Strings::ToInt64(str.substr(precp + 1, expindex - 1 - precp));
					SetTo(new Math::Float(str.substr(0, precp) + str.substr(expindex, str.size() - expindex), (u64)((vprec + Math::Float::OblPrecision)*3.3219280948874)));
				}
			}
			if (*this==nullptr)
			{
				for (index_t i = 0; i < str.size(); ++i) { if (str[i] != '0')break; expindex--; }
				if (Strings::IndexOf(str,'.')>=0)expindex--;
				SetTo(new Math::Float(str, (u64)((expindex + Math::Float::OblPrecision)*3.3219280948874)));
			}
		}
		ptr<Math::Float>::ptr(std::string const& str, u64 precision) :ptr<BigObject>(new Math::Float(str, precision)) {}
		ptr<Math::Float> ptr<Math::Float>::CalculatePi(u64 prec)
		{
			static std::mutex* pmt;
			if (!pmt)
			{
				GetDefaultMutexRef().lock();
				if (!pmt) {
					static std::mutex mt;
					pmt = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (prec > Math::Float::MaxPrecision)prec = Math::Float::MaxPrecision;
			static u64 lastprec = 0;
			static ptr<Math::Float> pi;
			if (lastprec >= prec)return pi;
			ptr<Math::Float> f;
			if (prec < 65536 || prec - lastprec >= 128)
			{
				ptr<Math::Float> a(1, prec), b(1, prec), t(1, prec);
				long long p = 0;
				b = CalculateSqrt2(prec).Half();
				t.SetExp(-2);
				for (;;)
				{
					f = (a + b);
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					f.SetExp(f.GetExp() - 1);
					b = Sqrt(a*b);
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					if (f.AppEquals(b))break;
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					a -= f;
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					a *= a;
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					a.SetExp(a.GetExp() + p);
					t = t - a;
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
					p++;
					a = f;
					if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
				}
				f += b;
				if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
				f *= f;
				if (Math::Float::StopCalculating)return f.MakeError(Math::MathError::CalculationStop);
				f /= t;
				f.SetExp(f.GetExp() - 2);
			}
			else
			{
				pmt->lock();
				f = Conversion::Convert<ptr>::reference(pi.get()->Clone());
				pmt->unlock();
				s64 exp = 2 - f.GetPrecision();
				for (s64 checkexp = exp;; checkexp += 64)
				{
					u64 num = ((u64)GetPiBBP32((s32)checkexp + 64) << 32) | GetPiBBP32((s32)checkexp + 32);
					if (num != f.GetDat64(checkexp))f.SetDat64(checkexp, num);
					else break;
				}
				while ((u64)exp > 2 - prec)
				{
					exp -= 64;
					f.SetDat64(exp, ((u64)GetPiBBP32((s32)exp + 64) << 32) | GetPiBBP32((s32)exp + 32));
				}
			}
			//线程同步设置
			f.get()->Data.UseCount.SetEnableThreadSafe(true);
			f.UseCount.SetEnableThreadSafe(true);
			/*另一种计算Pi的方法
			f = 48 * (FloatX(1)/ FloatX(49)).Arctan();
			f += 128 * (FloatX(1) / FloatX(57)).Arctan();
			f -= 20 * (FloatX(1) / FloatX(239)).Arctan();
			f += 48 * (FloatX(1) / FloatX(110443)).Arctan();*/
			pmt->lock();
			if (lastprec < prec&&!f.IsError())
			{
				pi = f;
				lastprec = prec;
			}
			pmt->unlock();
			return f;
		}

		//使用指定精度生成e
		ptr<Math::Float> ptr<Math::Float>::CalculateE(u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static u64 lastbits = 0;
			static ptr<Math::Float> _E = 0;
			pm->lock();
			if (lastbits >= bits)
			{
				ptr<Math::Float> p = _E;
				pm->unlock();
				return p;
			}
			ptr<Math::Float> n((bits*Math::Float256::Ln2() + 2).GetIntegerPart(), bits);
			ptr<Math::Float> x(One(), bits);
			while (!(n.GetExp()).Equals0() || n.get()->Data.get()->bitsCount > 1)
			{
				ptr<Math::Float> t = One() / n;
				if (Math::Float::StopCalculating)return t.MakeError(Math::MathError::CalculationStop);
				x *= t;
				if (Math::Float::StopCalculating)return x.MakeError(Math::MathError::CalculationStop);
				++x;
				--n;
			}
			++x;
			x.SetPrecision(bits);
			//重新设置线程同步设置
			x.SetEnableInterlock(true);
			pm->lock();
			if (lastbits < bits&&!x.IsError()) {
				lastbits = bits;
				_E = x;
			}
			pm->unlock();
			return x;
		}
		ptr<Math::Float> ptr<Math::Float>::CalculateLn2(u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static u64 lastbits = 0;
			static ptr<Math::Float> _Ln2 = 0;
			pm->lock();
			if (lastbits >= bits)
			{
				ptr<Math::Float> p = _Ln2;
				pm->unlock();
				return p;
			}
			pm->unlock();
			bits = ((u64)MinValues::Int64) >> (Bits::LeftZeroCount64(bits) - 1);
			ptr<Math::Float> pp = Calculate2_(15, bits);//2^(1/(2^16))
			pp.SetPrecision(bits + 16);
			pp = pp.Ln();
			pp.SetExp(pp.GetExp() + 16);
			pp.SetPrecision(bits);
			//重新设置线程同步设置
			pp.SetEnableInterlock(true);
			pm->lock();
			if (lastbits < bits&&!pp.IsError()) {
				lastbits = bits;
				_Ln2 = pp;
			}
			pm->unlock();
			return pp;
		}
		ptr<Math::Float> ptr<Math::Float>::CalculateLn10(u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static ptr<Math::Float> _Ln10;
			//记录上次计算精度，防止重复计算
			static u64 lastprec = 0;
			pm->lock();
			if (lastprec >= bits)
			{
				ptr<Math::Float> p = _Ln10;
				pm->unlock();
				return p;
			}
			pm->unlock();
			ptr<Math::Float> pln10 = new Math::Float(Math::Float::Ten(), bits);
			pln10 = pln10.Ln();
			//重新设置线程同步设置
			pln10.SetEnableInterlock(true);
			pm->lock();
			if (lastprec < bits&&!pln10.IsError()) {
				lastprec = bits;
				_Ln10 = pln10;
			}
			pm->unlock();
			return pln10;
		}
		ptr<Math::Float> ptr<Math::Float>::Calculate1Ln10(u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static ptr<Math::Float> _1Ln10;
			//记录上次计算精度，防止重复计算
			static u64 lastprec = 0;
			pm->lock();
			if (lastprec >= bits)
			{
				ptr<Math::Float> p = _1Ln10;
				pm->unlock();
				return p;
			}
			pm->unlock();
			ptr<Math::Float> p1ln10 = ptr<Math::Float>::One() / CalculateLn10(bits);
			//重新设置线程同步设置
			p1ln10.SetEnableInterlock(true);
			pm->lock();
			if (lastprec < bits&&!p1ln10.IsError()) {
				lastprec = bits;
				_1Ln10 = p1ln10;
			}
			pm->unlock();
			return p1ln10;
		}
		double ptr<Math::Float>::BBPn(s32 a, s32 b, s32 c, s32 x, bool ife)
		{
			double sum = 0;
			s32 en = x < 0 ? -1 : (x / a);//结束标识
			s32 n = 0;
			while (n <= en)
			{
				//计算(2^(x-a*n))%c
				u64 num = 1;
				s32 t = x;
				s32 mov = (s32)Bits::LeftZeroCount32(c) + 32;
				while (t >= mov)
				{
					num <<= mov;
					t -= mov;
					num %= c;
				}
				num <<= t;
				num %= c;
				//求和
				if (ife && (n & 1))sum -= (double)num / c;
				else sum += (double)num / c;
				x -= a;
				c += b;
				++n;
			}
			u64 tk = 1ull << a;
			double tmp = (double)(tk >> (-x));
			for (int k = 0; k <= 40 / a; ++k)
			{
				if (ife && (n & 1))sum -= (tmp /= tk) / c;
				else sum += (tmp /= tk) / c;
				c += b;
				++n;
			}
			if (sum < 0)sum += (int)(-sum) + 1;
			return sum - (int)sum;
		}
		u32 ptr<Math::Float>::GetPiBBP32(s32 exp)
		{
			exp = -exp;
			double k = -BBPn(10, 4, 1, exp - 1, true) - BBPn(10, 4, 3, exp - 6, true) + BBPn(10, 10, 1, exp + 2, true) - BBPn(10, 10, 3, exp, true) - BBPn(10, 10, 5, exp - 4, true) - BBPn(10, 10, 7, exp - 4, true) + BBPn(10, 10, 9, exp - 6, true);
			//double k = BBPn(4, 8, 1, exp + 2,false) - BBPn(4, 8, 4, exp + 1, false) - BBPn(4, 8, 5, exp, false) - BBPn(4, 8, 6, exp, false);
			if (k < 0)k += 1 + int(-k);
			k -= (int)k;
			return (u32)(k * (double)0x100000000);
		}
		ptr<Math::Float> ptr<Math::Float>::CalculateSqrt2(u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static ptr<Math::Float> _sqrt2 = Math::Float256::Sqrt2();
			//记录上次计算精度，防止重复计算
			static u64 lastprec = 256 < Math::Float::DefPrecision ? 256 : Math::Float::DefPrecision;
			pm->lock();
			if (lastprec >= bits)
			{
				ptr<Math::Float> p = _sqrt2;
				pm->unlock();
				return p;
			}
			pm->unlock();
			ptr<Math::Float> tmp = ptr<Math::Float>(ptr<Math::Float>::Two(), bits).Sqrt();
			//重新设置线程同步设置
			tmp.SetEnableInterlock(true);
			pm->lock();
			if (lastprec < bits&&!tmp.IsError())
			{
				lastprec = bits;
				_sqrt2 = tmp;
			}
			pm->unlock();
			return tmp;
		}
		ptr<Math::Float> ptr<Math::Float>::Calculate2_(size_t x, u64 bits)
		{
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			static ptr<Math::Float> _2_[16];
			static u64 lastbits[16] = { 0 };
			pm->lock();
			if (lastbits[x] >= bits)
			{
				ptr<Math::Float> p = _2_[x];
				pm->unlock();
				return p;
			}
			pm->unlock();
			ptr<Math::Float> p2;
			if (x == 0)
				p2 = ptr<Math::Float>(new Math::Float(Math::Float::Two(), bits)).Sqrt();
			else p2 = Calculate2_(x - 1, bits + 2).Sqrt();
			//重新设置线程同步设置
			p2.SetEnableInterlock(true);
			pm->lock();
			if (lastbits[x] < bits&&!p2.IsError()) {
				lastbits[x] = bits;
				_2_[x] = p2;
			}
			pm->unlock();
			return p2;
		}
		ptr<Math::Float> ptr<Math::Float>::Calculate12_(size_t x, u64 bits)
		{
			static std::mutex* pm;
			if (!pm) {
				GetDefaultMutexRef().lock();
				if (!pm) {
					static std::mutex mt;
					pm = &mt;
				}
				GetDefaultMutexRef().unlock();
			}
			if (bits > Math::Float::MaxPrecision)bits = Math::Float::MaxPrecision;
			static ptr<Math::Float> _2_[16];
			static u64 lastbits[16] = { 0 };
			pm->lock();
			if (lastbits[x] >= bits)
			{
				ptr<Math::Float> p = _2_[x];
				pm->unlock();
				return p;
			}
			pm->unlock();
			ptr<Math::Float> p2 = ptr<Math::Float>(1, bits) / Calculate2_(x, bits);
			//重新设置线程同步设置
			p2.SetEnableInterlock(true);
			pm->lock();
			if (lastbits[x] < bits&&!p2.IsError()) {
				lastbits[x] = bits;
				_2_[x] = p2;
			}
			pm->unlock();
			return p2;
		}

		ptr<Math::Float> ptr<Math::Float>::Ln()const
		{
			if (IsError())return *this;//错误传递
			if (GetSign() <= 0) { Math::Float* p = new Math::Float(); p->MakeError(Math::MathError::LnLessOrEqual0); return p; }
			//根据k = dln(x)/dx = 1/x得知，当x变动一个△x时，传导到lnx的结果表现为变动△x/x。2^(ex-px) -> 2^(-px)
			//因此结果变动位其实就是-GetPrecision();
			s64 resultp = -(s64)GetPrecision();
			//获取该数值副本并增大精度
			ptr<Math::Float> y(*this, (s64)(GetPrecision() + Math::Float::OblPrecision));
			//若≤0，则进入错误状态并传递
			if (GetSign() <= 0)return y.MakeError(Math::MathError::LnLessOrEqual0);
			//_2x用以分离*this的指数出来，因为ln2^x = x*ln2
			ptr<Math::Float> _2x = Zero();
			s64 prec = 0;
			if (!y.GetExp().Equals0()) { _2x = ptr<Math::Float>(y.GetExp(), MaxValues::Int64); y.SetExp(Math::Float256::Zero()); }

			double fd = 0;
			double fdd = 0.5;
			size_t i = 0;
			while (i < 15)
			{
				if (y >= Calculate2_(i, y.GetPrecision()))
				{
					fd += fdd;
					y *= Calculate12_(i, y.GetPrecision());
					if (Math::Float::StopCalculating)return y.MakeError(Math::MathError::CalculationStop);
				}
				fdd /= 2;
				++i;
			}
			//y = (x - 1) / (x + 1)
			//ln(x) = 2y(1+(y^2)/3+(y^4)/5+(y^4)/7+...)
			//此算法收敛较快
			y = (y - One()) / (y + One());
			if (Math::Float::StopCalculating)return y.MakeError(Math::MathError::CalculationStop);
			ptr<Math::Float> y2 = y*y;//y^2
			if (Math::Float::StopCalculating)return y.MakeError(Math::MathError::CalculationStop);
			ptr<Math::Float> y2n = y2*y;//y^2n+1
			if (Math::Float::StopCalculating)return y.MakeError(Math::MathError::CalculationStop);
			ptr<Math::Float> s = y;//y+(y^3)/3+(y^5)/5+(y^7)/7+...
			i = 3;//3，5，7...
			while (!s.IsError())
			{
				//随着计算深入，精度要求会降低。y的指数为0，
				prec = (s64)y.GetPrecision() - (s64)(64 - Bits::LeftZeroCount64(i) - (s64)y2n.GetExp());
				s += y2n / ptr<Math::Float>(i, y2n.GetPrecision());
				if (Math::Float::StopCalculating)return s.MakeError(Math::MathError::CalculationStop);
				if (y2n.Equals0() || prec < 0)break;
				y2.SetPrecision(prec + 2);
				y2n *= y2;
				if (Math::Float::StopCalculating)return s.MakeError(Math::MathError::CalculationStop);
				i += 2;
			}
			s.SetExp(s.GetExp() + Math::Float256::One());
			if (_2x.Equals0())
			{
				if (fd != 0)
				{
					s += CalculateLn2(-resultp)*ptr<Math::Float>(fd, -resultp);
				}
				prec = s.GetExp() - resultp;
				if (prec > (s64)Math::Float::MaxPrecision)prec = Math::Float::MaxPrecision;
				if (prec < (s64)Math::Float::MinPrecision)prec = Math::Float::MinPrecision;
				s.SetPrecision(prec);
				return s;
			}
			Math::Float256 tmp = _2x.GetExp() - resultp;
			if (tmp > Math::Float256(Math::Float::MaxPrecision))prec = Math::Float::MaxPrecision;
			else prec = tmp;
			if (fd != 0)
				return s + CalculateLn2(prec)*(ptr<Math::Float>(fd, -resultp) + _2x);
			return s + CalculateLn2(prec)* _2x;
		}
		ptr<Math::Float> ptr<Math::Float>::Log2()const
		{
			return Ln() / CalculateLn2(GetPrecision() + 4);
		}
		ptr<Math::Float> ptr<Math::Float>::Exp2()const
		{
			if (IsError())return *this;
			ptr<Math::Float> num_x, num_xn, num_factorial, num_1;
			if (GetExp() >= Math::Float256::ExpMaxValue)
			{
				if (GetSign() < 0)
					return Zero();
				num_x = *this;
				return num_x.MakeError(Math::MathError::Exp2Overflow);
			}

			num_x = GetDecimalPart();
			if (num_x.Equals0() && num_x.GetPrecision() < num_x.GetExp() + Math::Float::MinPrecision)//意味着结果的指数会出现误差，有效数字位已经部分失意
			{
				num_x.SetExp(0); num_x.SetPrecision(MinPrecision);
			}
			Math::Float256 expmov = GetIntegerPart().ToFloat256();
			num_x = num_x* CalculateLn2(num_x.GetPrecision());
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			index_t i;
			num_factorial = One();
			num_factorial.SetPrecision(num_x.GetPrecision());
			num_xn = num_x;
			num_1 = One() + num_x;
			if (!num_x.Equals0())for (i = 2;; i++)
			{
				num_xn *= num_x;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_factorial = num_factorial*i;
				num_1 += num_xn / num_factorial;
				if (Math::Float::StopCalculating)return num_1.MakeError(Math::MathError::CalculationStop);
				if ((Math::Float256)num_factorial.GetExp() - (Math::Float256)num_xn.GetExp() > GetPrecision())break;
			}
			if (!expmov.Equals0())num_1.SetExp(num_1.GetExp() + expmov);
			return num_1;
		}
		ptr<Math::Float> ptr<Math::Float>::Pow(ptr<Math::Float> const& number)const
		{
			if (IsError())return(*this);
			if (number.IsError())return number;

			if (number.IsInteger() && number.GetExp() < 64)//为整数计算提高精度
			{
				u64 exp = number.Abs().ToUInt64();
				ptr<Math::Float> df(*this);
				ptr<Math::Float> ret = One();
				for (;;)
				{
					if ((exp & 1) != 0)ret *= df;
					if ((exp >>= 1) == 0)break;
					df *= df;
					if (Math::Float::StopCalculating)return df.MakeError(Math::MathError::CalculationStop);
				}
				if (number.GetSign() < 0)return One() / ret;
				return ret;
			}
			ptr<Math::Float> num(*this, (s64)(GetPrecision()));
			ptr<Math::Float> vf(number, (s64)(number.GetPrecision()));
			if (GetSign() < 0)
			{
				if (vf.IsInteger())//整数次方
				{
					ptr<Math::Float> tmp = (num.SelfAbs().Log2()*vf).Exp2();
					if (Math::Float::StopCalculating)return tmp.MakeError(Math::MathError::CalculationStop);
					if (tmp.IsError())tmp.MakeError(Math::MathError::PowExpOverflow);
					if (!vf.Half().IsInteger())return -tmp;
					return tmp;
				}
				return num.MakeError(Math::MathError::PowLessThan0);
			}
			ptr<Math::Float> tmp = (num.Log2()*vf).Exp2();//使用Exp2来算
			if (Math::Float::StopCalculating)return tmp.MakeError(Math::MathError::CalculationStop);
			if (tmp.IsError())tmp.MakeError(Math::MathError::PowExpOverflow);
			return tmp;
			//return this->Abs().Ln()*(number).Exp();//使用Exp来算
		}
		ptr<Math::Float> ptr<Math::Float>::Sqrt()const
		{
			if (Equals0() || IsError())return *this;
			if (GetSign() < 0)return ptr<Math::Float>(*this).MakeError(Math::MathError::SqrtLessThan0);
			if (*this==One())return *this;
			ptr<Math::Float> num(*this, (s64)(GetPrecision() + Math::Float::OblPrecision));
			ptr<Math::Float> num1 = num;
			Math::Float256 te = num.GetExp().Half().SelfGetIntegerPart();
			if (!te.Equals0())++te.Exp;
			num1.SetExp(num1.GetExp() - te);
			num1 = ptr<Math::Float>(num1.ToFloat256().Sqrt(), num.GetPrecision());//近似值
			num1.SetExp(num1.GetExp() + te.SelfHalf());
			ptr<Math::Float> num2 = num / num1;
			if (Math::Float::StopCalculating)return num2.MakeError(Math::MathError::CalculationStop);
			//			std::cout << num.ToFloat256().ToString() << std::endl;
			//			std::cout << num1.ToFloat256().ToString() << std::endl;
			//			std::cout << num2.ToFloat256().ToString() << std::endl;
			u64 dprec = 256;
			for (;;)//牛顿迭代
			{
				if ((dprec <<= 1) > num2.GetPrecision() && num1.AppEquals(num2))break;
				num1 += num2;
				num1.SelfHalf();
				num2 = num / num1;
				if (Math::Float::StopCalculating)return num2.MakeError(Math::MathError::CalculationStop);
			}
			return num2;
		}
		ptr<Math::Float> ptr<Math::Float>::Sin()const
		{
			if (IsError())return *this;
			ptr<Math::Float> num_x, num_xn, num_factorial, num_2, sum;
			s64 prec = GetPrecision();
			ptr<Math::Float> _pi_half2 = CalculatePi(prec).Half(Math::Float256::Two());
			num_x = Mod(_pi_half2, num_factorial);//除以π/4以判断正负
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			u64 flag = num_factorial.ToUInt64() & 7;
			bool negative = flag > 3;
			switch (flag & 3)
			{
			case 1://45~90°
				if (num_x.Equals0())//返回(根号2)/2
				{
					if (negative)return CalculateSqrt2(prec).Half().SelfNegative();
					return CalculateSqrt2(prec).Half();
				}
				num_x = _pi_half2 - num_x;//cos收敛速度快
				if (negative)return -num_x.Cos();
				return num_x.Cos();
			case 2://90~135°
				if (num_x.Equals0())
				{
					if (negative)return -One();
					return One();
				}
				if (negative)
					return -num_x.Cos();
				return num_x.Cos();
			case 3://135~180°
				num_x = _pi_half2 - num_x;
			default:
				break;
			}
			num_factorial = One();//阶乘
			num_xn = num_x;
			sum = num_x;
			num_2 = num_x;
			num_x *= num_x;
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			u64 i;
			ptr<Math::Float> t;
			for (i = 3;; i += 2)//Sin x = x-x^3/3!+x^5/5!-
			{
				num_factorial = num_factorial*(i*(i - 1));
				num_xn *= num_x;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_xn.SetPrecision(prec);
				num_factorial.SelfNegative();
				t = num_xn / num_factorial;
				sum += t;
				if (Math::Float::StopCalculating)return t.MakeError(Math::MathError::CalculationStop);
				if (t.Equals0())break;
				prec = Math::Float::PrecMinus(sum.GetExp(), t.GetExp(), sum.GetPrecision());
				if (prec > 0)break;
				prec = Math::Float::OblPrecision - prec;
			}
			if (negative)return -sum;
			return sum;
		}
		void ptr<Math::Float>::SinAndCos(ptr<Math::Float> & outsin, ptr<Math::Float> & outcos) const
		{
			if (IsError()) { outsin = outcos = *this; return; }
			ptr<Math::Float> num_x, num_xn, num_factorial, num_2, sum;
			s64 prec = GetPrecision();
			ptr<Math::Float> _pi_half2 = CalculatePi(prec).Half(Math::Float256::Two());
			num_x = Mod(_pi_half2, num_factorial);//除以π/4以判断操作方法
			if (Math::Float::StopCalculating)
			{
				outsin.MakeError(Math::MathError::CalculationStop);
				outcos.MakeError(Math::MathError::CalculationStop);
				return;
			}
			u64 flag = num_factorial.ToUInt64();
			bool swp = false;//交换
			switch ((flag & 7))
			{
			case 1://45~90°
				if (num_x.Equals0()) { outsin = outcos = CalculateSqrt2(prec).Half(); return; }//返回(根号2)/2
				num_x = _pi_half2 - num_x;//cos收敛速度快
				swp = true;
				break;
			case 2://90~135°
				if (num_x.Equals0()) { outsin = One(); outcos = Zero(); return; }
				swp = true;
				break;
			case 3://135~180°
				if (num_x.Equals0()) { outsin = outcos = CalculateSqrt2(prec).Half(); outcos.SelfNegative(); return; }
				num_x = _pi_half2 - num_x;
			case 4://180~225°
				if (num_x.Equals0()) { outcos = -One(); outsin = Zero(); return; }
				break;
			case 5://225~270°
				if (num_x.Equals0()) { outsin = outcos = CalculateSqrt2(prec).Half().SelfNegative(); return; }//返回(根号2)/2
				num_x = _pi_half2 - num_x;//cos收敛速度快
				swp = true;
				break;
			case 6://270~315°
				if (num_x.Equals0()) { outsin = -One(); outcos = Zero(); return; }
				swp = true;
				break;
			case 7://315~360°
				if (num_x.Equals0()) { outsin = outcos = CalculateSqrt2(prec).Half(); outsin.SelfNegative(); return; }
				num_x = _pi_half2 - num_x;
			default:
				break;
			}
			num_factorial = One();//阶乘
			num_xn = num_x;
			sum = num_x;
			num_2 = num_x;
			num_x *= num_x;
			if (Math::Float::StopCalculating)
			{
				outsin.MakeError(Math::MathError::CalculationStop);
				outcos.MakeError(Math::MathError::CalculationStop);
				return;
			}
			u64 i;
			ptr<Math::Float> t;
			for (i = 3;; i += 2)//Sin x = x-x^3/3!+x^5/5!-
			{
				num_factorial *= (i*(i - 1));
				num_xn.SetPrecision(prec);
				num_xn *= num_x;
				if (Math::Float::StopCalculating)
				{
					outsin.MakeError(Math::MathError::CalculationStop);
					outcos.MakeError(Math::MathError::CalculationStop);
					return;
				}
				num_factorial.SelfNegative();
				t = num_xn / num_factorial;
				sum += t;
				if (Math::Float::StopCalculating)
				{
					outsin.MakeError(Math::MathError::CalculationStop);
					outcos.MakeError(Math::MathError::CalculationStop);
					return;
				}
				if (t.Equals0())break;
				prec = Math::Float::PrecMinus(sum.GetExp(), t.GetExp(), sum.GetPrecision());
				if (prec > 0)break;
				prec = Math::Float::OblPrecision - prec;
			}
			if (swp)
			{
				outcos = (flag > 1 && flag < 6) ? -sum : sum;
				outsin = (One() - sum*sum).Sqrt();
				if (flag >= 4)outsin.SelfNegative();
			}
			else
			{
				outsin = flag >= 4 ? -sum : sum;
				outcos = (One() - sum*sum).Sqrt();
				if (flag > 1 && flag < 6)outcos.SelfNegative();
			}
		}
		ptr<Math::Float> ptr<Math::Float>::Cos()const
		{
			if (IsError())return *this;
			ptr<Math::Float> num_x, num_xn, num_factorial, num_2, sum;
			s64 prec = GetPrecision();
			ptr<Math::Float> _pi_half2 = CalculatePi(prec).Half(Math::Float256::Two());
			num_x = Mod(_pi_half2, num_factorial);//除以π/4以判断正负
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			u64 flag = num_factorial.ToUInt64() & 7;
			bool negative = flag > 3;
			switch (flag & 3)
			{
			case 1://45~90°
				if (num_x.Equals0())
				{
					if (negative)return -CalculateSqrt2(prec).Half();
					return CalculateSqrt2(prec).Half();
				}
				num_x = _pi_half2 - num_x;
				if (negative)return -num_x.Sin();
				return num_x.Sin();
			case 2://90~135°
				if (num_x.Equals0())
				{
					return Zero();
				}
				if (negative)return num_x.Sin();
				return -num_x.Sin();
			case 3:
				negative = !negative;
				num_x = _pi_half2 - num_x;
			default:
				break;
			}
			num_factorial = One();//阶乘
			num_xn = One();
			sum = One();
			num_2 = One();
			num_x *= num_x;//变x方
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			u64 i;
			ptr<Math::Float> t;
			for (i = 2;; i += 2)//Cosx = 1-x^2/2!+x^4/4!-
			{
				num_factorial = num_factorial*(i*(i - 1));
				num_xn *= num_x;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_xn.SetPrecision(prec);
				num_factorial.SelfNegative();//变换符号
				t = num_xn / num_factorial;
				sum += t;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				if (t.Equals0())break;
				prec = Math::Float::PrecMinus(sum.GetExp(), t.GetExp(), sum.GetPrecision());
				if (prec > 0)break;
				prec = Math::Float::OblPrecision - prec;
			}
			if (negative)return -sum;
			return sum;
		}
		ptr<Math::Float> ptr<Math::Float>::Arcsin()const
		{
			if (IsError())return *this;
			ptr<Math::Float> num_x, num_xn;//x、xn
			ptr<Math::Float> num1, num2;//分子、分母
			ptr<Math::Float> num_1, num_2;//前一项、后一项
			num_x = Abs();//取绝对值
			if (Abs() > One())
				return num_x.MakeError(Math::MathError::Arcsin);//出错
			s64 prec = GetPrecision();
			if (num_x > CalculateSqrt2(prec).Half())//大于(根号2)/2
			{//为了加速收敛，转换成π/2－。。。
				if (GetSign() < 0)//(π/2)-arcsin(sqrt(1-x^2))
					return(CalculatePi(prec).Half().SelfNegative() + (One() - num_x*num_x).SelfAbs().Sqrt().Arcsin());
				return(CalculatePi(prec).Half() - ((One() - num_x*num_x).SelfAbs().Sqrt().Arcsin()));
			}
			num1 = num2 = One();
			num_xn = num_2 = num_1 = num_x;
			num_x *= num_x;
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			u64 i;
			for (i = 1;; i += 2)//arcsin x = x + 1/2*x^3/3 + 1*3/(2*4)*x^5/5 + ……(|x|<1)
			{
				num1 = num1*ptr<Math::Float>(i, prec);//奇数
				num2 = num2*ptr<Math::Float>(i + 1, prec);//偶数
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_xn.SetPrecision(prec);
				num_xn = num_xn*num_x;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_2 = (num_xn*num1 / num2) / ptr<Math::Float>(i + 2, prec);
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_1 += num_2;
				if (num_2.Equals0())break;
				prec = Math::Float::PrecMinus(num_1.GetExp(), num_2.GetExp(), num_1.GetPrecision());
				if (prec > 0)break;
				prec = Math::Float::OblPrecision - prec;
			}
			if (GetSign() < 0)return -num_1;
			return num_1;
		}
		ptr<Math::Float> ptr<Math::Float>::Arccos()const
		{
			if (IsError())return *this;
			if (Abs() > One())
			{
				ptr<Math::Float> num = *this;
				return num.MakeError(Math::MathError::Arccos);//出错
			}
			if (*this < CalculateSqrt2(GetPrecision()).Half())return CalculatePi(GetPrecision()).Half() - Arcsin();//小于根号2/2，这样计算可以在接近1时获得较高的精度
			return (One() - *this**this).SelfAbs().Sqrt().Arcsin();//取正值
		}
		ptr<Math::Float> ptr<Math::Float>::Arctan()const
		{
			if (IsError())return *this;
			if (GetSign() < 0)return (-*this).Arctan().SelfNegative();
			//配方法：tanx=sinx/cosx，设cosx>0，则sinx = this/sqrt(this平方+1)，转为求arcsin,但此方法在x趋于0时精度大减
			s64 prec = GetPrecision();
			if (*this > CalculateSqrt2(prec).Half())return (*this / ((*this**this + One()).Sqrt())).Arcsin();
			//级数法：arctan x = x - x^3/3 + x^5/5 - ... (x≤1) ，x趋于1时收敛太慢
			ptr<Math::Float> num_x, num_xn;//x、xn
			ptr<Math::Float> num = Three();//分母
			ptr<Math::Float> num_1, num_2;//前一项、后一项
			num_x = -*this**this;
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			num_xn = (*this)*num_x;
			if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			num_2 = num_1 = *this;
			u64 i;
			for (i = 3;; i += 2)
			{
				num_xn.SetPrecision(prec);
				num_2 = num_xn / ptr<Math::Float>(i, prec);
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
				num_1 += num_2;
				if (num_2.Equals0())break;
				prec = Math::Float::PrecMinus(num_1.GetExp(), num_2.GetExp(), num_1.GetPrecision());
				if (prec > 0)break;
				prec = Math::Float::OblPrecision - prec;
				num_xn *= num_x;
				if (Math::Float::StopCalculating)return num_x.MakeError(Math::MathError::CalculationStop);
			}
			return num_1;
		}

		u64& ptr<Math::Float>::MaxPrecision(Math::Float::MaxPrecision);
		u64& ptr<Math::Float>::MinPrecision(Math::Float::MinPrecision);
		u64& ptr<Math::Float>::DefPrecision(Math::Float::DefPrecision);
		bool& ptr<Math::Float>::StopCalculating(Math::Float::StopCalculating);

		//常量
		const Math::Float& Math::Float::Zero()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
			static Math::Float val;
				val = 0;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::One()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 1;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Two()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 2;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Three()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 3;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Four()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 4;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Five()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 5;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Six()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 6;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Seven()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 7;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Eight()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 8;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Nine()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 9;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const Math::Float& Math::Float::Ten()
		{
			static Math::Float* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static Math::Float val;
				val = 10;
				val.Precision = MaxValues::Int64;
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}

		//常量
		const ptr<Math::Float>& ptr<Math::Float>::Zero()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 0;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::One()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 1;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Two()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 2;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Three()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 3;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Four()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 4;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Five()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 5;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Six()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 6;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Seven()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 7;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Eight()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 8;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Nine()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 9;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		const ptr<Math::Float>& ptr<Math::Float>::Ten()
		{
			static ptr<Math::Float>* p;
			if (p) return *p;
			GetDefaultMutexRef().lock();
			if (!p) {
				static ptr<Math::Float> val = 10;
				val.SetPrecision(MaxValues::Int64);
				val.UseCount.SetEnableThreadSafe(true);
				p = &val;
			}
			GetDefaultMutexRef().unlock();
			return *p;
		}
		size_t Conversion::Convert <std::string , ptr<Math::Float> >::DefParam[2] = { 0,6 };
}
#endif
