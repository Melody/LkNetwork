#pragma once
#define LK_MATH_COMPLEX128_
/**@author W意波 编写*/
namespace LK
{
	namespace Math
	{
		/**@brief 表示一个用Float128做成员变量的复数*/
		struct Complex128
		{
			/**@brief 实数部分*/
			Float128 Real;
			//虚数部分
			Float128 Imagine;
			//构造一个未初始化的复数
			Complex128() {}
			//使用一个实数部分的值和一个虚数部分的值构造一个复数
			Complex128(Float128 const& r, Float128 const& i = Float128::Zero()) :Real(r), Imagine(i) {}
			//赋值
			Complex128&operator=(const Complex128& c) { Real = c.Real; Imagine = c.Imagine; return*this; }
			//以下是运算符重载
			Complex128& operator+= (Complex128 const& c)
			{
				Real += c.Real;
				Imagine += c.Imagine;
				return *this;
			}
			Complex128& operator-= (Complex128 const& c) { Real -= c.Real; Imagine -= c.Imagine; return*this; }
			Complex128& operator*= (Complex128 const& c) { Float128 tmp = Real*c.Real - Imagine*c.Imagine; Imagine = Real*c.Imagine + Imagine*c.Real; Real = tmp; return*this; }
			Complex128& operator/= (Complex128 const& c) { Float128 tmp = c.Real *c.Real + c.Imagine *c.Imagine; Float128 t = (Real*c.Real + Imagine *c.Imagine) / tmp; Imagine = (Imagine *c.Real - Real*c.Imagine) / tmp; Real = t; return *this; }
			friend Complex128 operator+(Complex128 const& c1, Complex128 const& c2)
			{
				return Complex128(c1.Real + c2.Real, c1.Imagine + c2.Imagine);
			}
			friend Complex128 operator-(Complex128 const& c1, Complex128 const& c2)
			{
				return Complex128(c1.Real - c2.Real, c1.Imagine - c2.Imagine);
			}
			friend Complex128 operator*(Complex128 const& c1, Complex128 const& c2)
			{
				return Complex128(c1.Real*c2.Real - c1.Imagine*c2.Imagine, c1.Real*c2.Imagine + c1.Imagine*c2.Real);
			}
			friend Complex128 operator/(Complex128 const& c1, Complex128 const& c2)
			{
				Float128 tmp = c2.Real *c2.Real + c2.Imagine *c2.Imagine;
				return Complex128((c1.Real*c2.Real + c1.Imagine *c2.Imagine) / tmp, (c1.Imagine *c2.Real - c1.Real*c2.Imagine) / tmp);
			}
			//比较
			friend bool operator==(const Complex128& d1, const Complex128& d2) { return d1.Real == d2.Real&&d1.Imagine == d2.Imagine; }
			friend bool operator!=(const Complex128& d1, const Complex128& d2) { return d1.Real != d2.Real || d1.Imagine != d2.Imagine; }
			//自身逆时针旋转一个角度（弧度制）
			Complex128&SelfRotate(Float128 const& k)
			{
				Float128 r, i;
				k.SinAndCos(i, r);
				Float128 tmp = Real*r - Imagine*i;
				Imagine = Real*i + Imagine*r;
				Real = tmp;
				return*this;
			}
			//自身逆时针旋转90°
			Complex128&SelfRotate90()
			{
				Float128 tmp = -Imagine;
				Imagine = Real;
				Real = tmp;
				return*this;
			}
			//自身逆时针旋转180°
			Complex128&SelfRotate180()
			{
				Real = -Real;
				Imagine = -Imagine;
				return*this;
			}
			//自身逆时针旋转270°
			Complex128&SelfRotate270()
			{
				Float128 tmp = Imagine;
				Imagine = -Real;
				Real = tmp;
				return*this;
			}
			//逆时针旋转一个角度（弧度制）
			Complex128 Rotate(Float128 const& k)
			{
				Float128 r = k.Cos();
				Float128 i = k.Sin();
				return Complex128(Real*r - Imagine*i, Real*i + Imagine*r);
			}
			//自身逆时针旋转90°
			Complex128 Rotate90()
			{
				return Complex128(-Imagine, Real);
			}
			//自身逆时针旋转180°
			Complex128 Rotate180()
			{
				return Complex128(-Real, -Imagine);
			}
			//自身逆时针旋转270°
			Complex128 Rotate270()
			{
				return Complex128(Imagine, -Real);
			}

			//二进制平摊反转置换
			static void BitRC(Complex128 *p, size_t len);
			//快速傅里叶变换，结果存回p中,要求len是2的整数次方
			static void FFT(Complex128 *p, size_t len);
			//快速傅里叶逆变换，结果存回p中,要求len是2的整数次方
			static void IFFT(Complex128 *p, size_t len);

			//二进制平摊反转置换（高位对齐）
			static void BitRCHigh(cu32*pn, Complex128 *p, size_t nlen, size_t len);
			//快速傅里叶变换（高位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^20
			static void FFTHigh(cu32*pn, Complex128 *p, size_t nlen, size_t len);
			//快速傅里叶逆变换（高位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回值为下限截取掉的32bit数据
			static u32 IFFTHigh(Complex128*p, u32*pn, size_t len, size_t nlen);
			//二进制平摊反转置换（低位对齐）
			static void BitRCLow(cu32*pn, Complex128 *p, size_t nlen, size_t len);
			//快速傅里叶变换（低位对齐），将pn变换结果存回p中,要求len是2的整数次方,nlen最大不应超过2^20
			static void FFTLow(cu32*pn, Complex128 *p, size_t nlen, size_t len);
			//快速傅里叶逆变换（低位对齐），将p逆变换结果存回pn中,要求len是2的整数次方,nlen最大不应超过2^20。返回上限溢出的16bit内容
			static u32 IFFTLow(Complex128*p, u32*pn, size_t len, size_t nlen);

			//傅里叶变换，结果存回pd中
			static void DFT(Complex128 const *ps, Complex128 *pd, size_t len);
			//傅里叶逆变换，结果存回pd中
			static void IDFT(Complex128 const *ps, Complex128 *pd, size_t len);

		};
	}
}