#pragma once
#define LK_MATH_LDOUBLE_
namespace LK
{
	namespace Math
	{
		LDouble operator+(const LDouble& ld1, const LDouble& ld2);
		LDouble operator-(const LDouble& ld1, const LDouble& ld2);
		LDouble operator*(const LDouble& ld1, const LDouble& ld2);
		LDouble operator/(const LDouble& ld1, const LDouble& ld2);
		LDouble operator%(const LDouble& ld1, const LDouble& ld2);
		//表示一个指数加长型双精度浮点数
		struct LDouble
		{
			//指数
			long long Exp;
			//有效数字
			double Value;
			//默认构造
			LDouble() {}
			//从double构造
			LDouble(double d);
			//从字符串构造
			LDouble(const std::string &);
			//用数值和指数构造
			LDouble(double val, long long exp);
			//转换为double
			inline double ToDouble()const { return Equals0() ? 0 : ldexp(Value, Exp > 2048 ? 2048 : Exp < -2048 ? -2048 : (int)Exp); }
			//加法
			friend LDouble operator+(const LDouble& ld1, const LDouble& ld2);
			inline LDouble& operator+=(const LDouble& ld) { return *this = *this + ld; }
			inline LDouble operator + ()const { return *this; }
			inline LDouble& operator++() { return *this = *this + 1; }
			inline LDouble operator++(int) { LDouble ld(*this); ++*this; return ld; }
			//减法
			friend LDouble operator-(const LDouble& ld1, const LDouble& ld2);
			inline LDouble& operator-=(const LDouble& ld) { return *this = *this - ld; }
			inline LDouble operator - ()const { LDouble ld(*this); ld.Value = -ld.Value; return ld; }
			inline LDouble& operator--() { return *this = *this - 1; }
			inline LDouble operator--(int) { LDouble ld(*this); --*this; return ld; }
			//乘法
			friend LDouble operator*(const LDouble& ld1, const LDouble& ld2);
			inline LDouble& operator*=(const LDouble& ld) { return *this = *this * ld; }
			//除法
			friend LDouble operator/(const LDouble& ld1, const LDouble& ld2);
			inline LDouble& operator/=(const LDouble& ld) { return *this = *this / ld; }
			//乘方
			friend LDouble operator^(const LDouble& ld1, const LDouble& ld2) { return ld1.Pow(ld2); }
			inline LDouble& operator^=(const LDouble& ld) { return *this = *this ^ ld; }
			//比较
			bool operator>(const LDouble& ld)const;
			bool operator>=(const LDouble& ld)const;
			inline bool operator<(const LDouble& ld)const { return ld>*this; }
			inline bool operator<=(const LDouble& ld)const { return ld >= *this; }
			bool operator==(const LDouble& ld)const { return Value == ld.Value && Exp == ld.Exp; }
			bool operator!=(const LDouble& ld)const { return Value != ld.Value || Exp != ld.Exp; }
			//判断该数字是否为0
			inline bool Equals0()const { return Value == 0; }
			//获取该数字的绝对值
			LDouble Abs()const;
			//获取整数部分
			LDouble GetIntegerPart()const;
			//获取小数部分
			LDouble GetDecimalPart()const;
			//求开平方
			LDouble Sqrt()const;
			//求以自然数为底数的对数
			LDouble Ln()const;
			//求以2为低的对数
			LDouble Log2()const;
			//求以10为低的对数
			LDouble Lg()const;
			//求该数的x次幂
			LDouble Pow(LDouble x)const;
			//求2的该数次幂
			LDouble Exp2()const;
			//求e的该数次幂
			LDouble Expe()const;
			//求正弦值
			inline LDouble Sin()const { return std::sin(ToDouble()); }
			//求余弦值
			inline LDouble Cos()const { return std::cos(ToDouble()); }
			//求正切值
			inline LDouble Tan()const { return std::tan(ToDouble()); }
			//求余切值
			inline LDouble Cot()const { return 1 / std::tan(ToDouble()); }
			//求反正弦值
			inline LDouble Arcsin()const { return std::asin(ToDouble()); }
			//求反余弦值
			inline LDouble Arccos()const { return std::acos(ToDouble()); }
			//求反正切值
			inline LDouble Arctan()const { return std::atan(ToDouble()); }
			//生成0~1的随机数
			inline LDouble& Rand() { static std::random_device r; u64 t = r(); t ^= (u64)r() << 32; return *this = (double)t / MaxValues::UInt64; }
			//转换到字符串
			std::string ToString(int len = 6)const;
			//常数
			const static LDouble*const _Exp10;
			const static LDouble MaxValue;
			const static LDouble MinValue;
			const static LDouble E;
			const static LDouble Pi;
			const static LDouble Ln2;
			const static LDouble Ln10;
			const static LDouble _1Ln2;
			const static LDouble _1Ln10;
		};
	}
	namespace Conversion {
		template<>struct Convert<std::string , Math::LDouble>
		{
			static inline std::string from(Math::LDouble const&l) { return l.ToString(); }
		};
	}
}
namespace std
{
	inline ::LK::Math::LDouble sin(::LK::Math::LDouble const& f) { return f.Sin(); }
	inline ::LK::Math::LDouble cos(::LK::Math::LDouble const& f) { return f.Cos(); }
	inline ::LK::Math::LDouble tan(::LK::Math::LDouble const& f) { return f.Tan(); }
	inline ::LK::Math::LDouble log(::LK::Math::LDouble const& f) { return f.Ln(); }
	inline ::LK::Math::LDouble log10(::LK::Math::LDouble const& f) { return f.Lg(); }
	inline ::LK::Math::LDouble pow(::LK::Math::LDouble const& f1, ::LK::Math::LDouble const& f2) { return f1.Pow(f2); }
	inline ::LK::Math::LDouble asin(::LK::Math::LDouble const& f) { return f.Arcsin(); }
	inline ::LK::Math::LDouble acos(::LK::Math::LDouble const& f) { return f.Arccos(); }
	inline ::LK::Math::LDouble atan(::LK::Math::LDouble const& f) { return f.Arctan(); }
	inline ::LK::Math::LDouble sqrt(::LK::Math::LDouble const& f) { return f.Sqrt(); }
	inline ::LK::Math::LDouble abs(::LK::Math::LDouble const& f) { return f.Abs(); }
	inline ::LK::Math::LDouble floor(::LK::Math::LDouble const& f) { ::LK::Math::LDouble ff = f.GetIntegerPart(); return f.Value <= 0 ? ((ff == f) ? f : ff - 1) : ff; }
	inline ::LK::Math::LDouble ceil(::LK::Math::LDouble const& f) { ::LK::Math::LDouble ff = f.GetIntegerPart(); return f.Value <= 0 ? ff : ((ff == f) ? f : ff + 1); }
	inline ::LK::Math::LDouble fmod(::LK::Math::LDouble const& f1, ::LK::Math::LDouble const& f2) { return f1%f2; }
}
