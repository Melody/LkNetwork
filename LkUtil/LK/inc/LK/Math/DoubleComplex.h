#pragma once
#define LK_MATH_DOUBLECOMPLEX_
/**@author W意波 编写*/
namespace LK
{
	namespace Math
	{
		/**@brief 表示一个用double做成员变量的复数*/
		struct DoubleComplex
		{
			/**@brief 实数部分*/
			double Real;
			//虚数部分
			double Imagine;
			/**@brief 构造一个随机的复数*/
			DoubleComplex() {}
			/**@brief 使用一个实数部分的值和一个虚数部分的值构造一个复数*/
			DoubleComplex(double r, double i = 0) :Real(r), Imagine(i) {}
			//以下是运算符重载
			DoubleComplex& operator+= (DoubleComplex const& c)
			{
				Real += c.Real;
				Imagine += c.Imagine;
				return*this;
			}
			DoubleComplex& operator-= (DoubleComplex const& c) { Real -= c.Real; Imagine -= c.Imagine; return*this; }
			DoubleComplex& operator*= (DoubleComplex const& c) { double tmp = Real*c.Real - Imagine*c.Imagine; Imagine = Real*c.Imagine + Imagine*c.Real; Real = tmp; return*this; }
			DoubleComplex& operator/= (DoubleComplex const& c) { double tmp = c.Real *c.Real + c.Imagine *c.Imagine; double t = (Real*c.Real + Imagine *c.Imagine) / tmp; Imagine = (Imagine *c.Real - Real*c.Imagine) / tmp; Real = t; return *this; }
			friend DoubleComplex operator+(DoubleComplex const& c1, DoubleComplex const& c2)
			{
				return DoubleComplex(c1.Real + c2.Real, c1.Imagine + c2.Imagine);
			}
			friend DoubleComplex operator-(DoubleComplex const& c1, DoubleComplex const& c2)
			{
				return DoubleComplex(c1.Real - c2.Real, c1.Imagine - c2.Imagine);
			}
			friend DoubleComplex operator*(DoubleComplex const& c1, DoubleComplex const& c2)
			{
				return DoubleComplex(c1.Real*c2.Real - c1.Imagine*c2.Imagine, c1.Real*c2.Imagine + c1.Imagine*c2.Real);
			}
			friend DoubleComplex operator/(DoubleComplex const& c1, DoubleComplex const& c2)
			{
				double tmp = c2.Real *c2.Real + c2.Imagine *c2.Imagine;
				return DoubleComplex((c1.Real*c2.Real + c1.Imagine *c2.Imagine) / tmp, (c1.Imagine *c2.Real - c1.Real*c2.Imagine) / tmp);
			}
			//比较
			friend bool operator==(const DoubleComplex& d1, const DoubleComplex& d2) { return d1.Real == d2.Real&&d1.Imagine == d2.Imagine; }
			friend bool operator!=(const DoubleComplex& d1, const DoubleComplex& d2) { return d1.Real != d2.Real || d1.Imagine != d2.Imagine; }
			/**@brief 自身逆时针旋转一个角度（弧度制）*/
			DoubleComplex&SelfRotate(double k)
			{
				double r = cos(k);
				double i = sin(k);
				double tmp = Real*r - Imagine*i;
				Imagine = Real*i + Imagine*r;
				Real = tmp;
				return*this;
			}
			/**@brief 自身逆时针旋转90°*/
			DoubleComplex&SelfRotate90()
			{
				double tmp = -Imagine;
				Imagine = Real;
				Real = tmp;
				return*this;
			}
			/**@brief 自身逆时针旋转180°*/
			DoubleComplex&SelfRotate180()
			{
				Real = -Real;
				Imagine = -Imagine;
				return*this;
			}
			/**@brief 自身逆时针旋转270°*/
			DoubleComplex&SelfRotate270()
			{
				double tmp = Imagine;
				Imagine = -Real;
				Real = tmp;
				return*this;
			}
			/**@brief 逆时针旋转一个角度（弧度制）*/
			DoubleComplex Rotate(double k)
			{
				double r = cos(k);
				double i = sin(k);
				return DoubleComplex(Real*r - Imagine*i, Real*i + Imagine*r);
			}
			/**@brief 自身逆时针旋转90°*/
			DoubleComplex Rotate90()
			{
				return DoubleComplex(-Imagine, Real);
			}
			/**@brief 自身逆时针旋转180°*/
			DoubleComplex Rotate180()
			{
				return DoubleComplex(-Real, -Imagine);
			}
			/**@brief 自身逆时针旋转270°*/
			DoubleComplex Rotate270()
			{
				return DoubleComplex(Imagine, -Real);
			}

			/**@brief 二进制平摊反转置换*/
			static void BitRC(DoubleComplex *p, size_t len);
			/**@brief 快速傅里叶变换，结果存回p中,要求len是2的整数次方*/
			static void FFT(DoubleComplex *p, size_t len);
			/**@brief 快速傅里叶逆变换，结果存回p中,要求len是2的整数次方*/
			static void IFFT(DoubleComplex *p, size_t len);

			/**@brief 二进制平摊反转置换（高位对齐）*/
			static void BitRCHigh(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶变换（高位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^20*/
			static void FFTHigh(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶逆变换（高位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回值为下限截取掉的16bit数据*/
			static unsigned short IFFTHigh(DoubleComplex*p, unsigned short*pn, size_t len, size_t nlen);
			/**@brief 二进制平摊反转置换（低位对齐）*/
			static void BitRCLow(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶变换（低位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^16*/
			static void FFTLow(unsigned short const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^16。返回上限溢出的16bit内容*/
			static unsigned short IFFTLow(DoubleComplex*p, unsigned short*pn, size_t len, size_t nlen);

			/**@brief 二进制平摊反转置换（高位对齐）*/
			static void BitRCHigh8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶变换（高位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^32*/
			static void FFTHigh8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶逆变换（高位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^32。返回值为下限截取掉的8bit数据*/
			static unsigned char IFFTHigh8(DoubleComplex*p, unsigned char*pn, size_t len, size_t nlen);
			/**@brief 二进制平摊反转置换（低位对齐）*/
			static void BitRCLow8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶变换（低位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^32*/
			static void FFTLow8(unsigned char const*pn, DoubleComplex *p, size_t nlen, size_t len);
			/**@brief 快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^32。返回上限溢出的8bit内容*/
			static unsigned char IFFTLow8(DoubleComplex*p, unsigned char*pn, size_t len, size_t nlen);

			//傅里叶变换，结果存回pd中
			static void DFT(DoubleComplex const *ps, DoubleComplex *pd, size_t len);
			//傅里叶逆变换，结果存回pd中
			static void IDFT(DoubleComplex const *ps, DoubleComplex *pd, size_t len);

		};
	}
}