#pragma once
/**@author 由W意波编写*/
#define LK_MATH_DDOUBLE_
namespace LK
{
	namespace Math
	{
		DDouble operator+(const DDouble& lda, const DDouble& ldb);
		DDouble operator-(const DDouble& lda, const DDouble& ldb);
		DDouble operator*(const DDouble& lda, const DDouble& ldb);
		DDouble operator/(const DDouble& lda, const DDouble& ldb);
		DDouble operator%(const DDouble& lda, const DDouble& ldb);
		/**
		@brief 表示一个浮点指数型双精度浮点数*/
		struct DDouble
		{
			/**
			@brief 指数*/
			double Exp;
			/**
			@brief 有效数字，正常情况下应该在±[0.5,1)区间，小于0.5时认为是发生了错误*/
			double Value;
			/**
			@brief 默认输出有效数字，默认为6*/
			static int DefOutPrec;
			/**
			@brief 默认构造一个未初始化的DDouble数值*/
			DDouble() {}
			/**
			@brief 从双精度浮点数构造
			@param [传入]参数d表示双精度浮点数*/
			DDouble(double d);
			/**
			@brief 从字符串构造
			@param [传入]参数str表示一个字符串*/
			DDouble(const std::string &str);
			/**
			@brief 用数值和指数构造*/
			DDouble(double val, double exp);
			/**
			@brief 转换为double*/
			inline double ToDouble()const { return Equals0() ? 0 : ldexp(Value, Exp > 2048 ? 2048 : Exp < -2048 ? -2048 : (int)Exp); }
			//加法
			friend DDouble operator+(const DDouble& lda, const DDouble& ldb);
			inline DDouble& operator+=(const DDouble& ld) { return *this = *this + ld; }
			inline DDouble operator + ()const { return *this; }
			inline DDouble& operator++() { return *this = *this + 1; }
			inline DDouble operator++(int) { DDouble ld(*this); ++*this; return ld; }
			//减法
			friend DDouble operator-(const DDouble& lda, const DDouble& ldb);
			inline DDouble& operator-=(const DDouble& ld) { return *this = *this - ld; }
			inline DDouble operator - ()const { DDouble ld(*this); ld.Value = -ld.Value; return ld; }
			inline DDouble& operator--() { return *this = *this - 1; }
			inline DDouble operator--(int) { DDouble ld(*this); --*this; return ld; }
			//乘法
			friend DDouble operator*(const DDouble& lda, const DDouble& ldb);
			inline DDouble& operator*=(const DDouble& ld) { return *this = *this * ld; }
			//求余
			friend DDouble operator%(const DDouble& lda, const DDouble& ldb);
			inline DDouble& operator%=(const DDouble& ld) { return *this = *this % ld; }
			//除法
			friend DDouble operator/(const DDouble& lda, const DDouble& ldb);
			inline DDouble& operator/=(const DDouble& ld) { return *this = *this / ld; }
			//乘方
			inline friend DDouble operator^(const DDouble& lda, const DDouble& ldb) { return lda.Pow(ldb); }
			inline DDouble& operator^=(const DDouble& ld) { return *this = *this ^ ld; }
			//比较
			bool operator>(const DDouble& ld)const;
			bool operator>=(const DDouble& ld)const;
			inline bool operator<(const DDouble& ld)const { return ld>*this; }
			inline bool operator<=(const DDouble& ld)const { return ld >= *this; }
			bool operator==(const DDouble& ld)const { return Value == ld.Value && Exp == ld.Exp; }
			bool operator!=(const DDouble& ld)const { return Value != ld.Value || Exp != ld.Exp; }

			/**
			@brief 判断该数字是否为0*/
			inline bool Equals0()const { return Value == 0; }
			/**
			@brief 判断该数字是否有错误*/
			inline bool IsError()const { return Value <= -1; }
			/**
			@brief 获取错误码*/
			inline int GetErrorCode()const { return (int)Value; }
			/**
			@brief 使用一个错误码产生一个错误,并返回该数字*/
			inline DDouble& MakeError(int code) { Value = code; return *this; }
			/**
			@brief 获取该数字的绝对值*/
			DDouble Abs()const;
			/**
			@brief 求d的绝对值*/
			inline static DDouble Abs(DDouble const& d) { return d.Abs(); }
			/**
			@brief 获取整数部分*/
			DDouble GetIntegerPart()const;
			/**
			@brief 获取小数部分*/
			DDouble GetDecimalPart()const;
			/**
			@brief 求开平方*/
			DDouble Sqrt()const;
			/**
			@brief 求d的平方根*/
			inline static DDouble Sqrt(DDouble const& d) { return d.Sqrt(); }
			/**
			@brief 求以该数的自然对数*/
			DDouble Ln()const;
			/**
			@brief 求d的自然对数*/
			inline static DDouble Ln(DDouble const& d) { return d.Ln(); }
			/**
			@brief 求以2为底的对数*/
			DDouble Log2()const;
			/**
			@brief 求d以2为底的对数*/
			inline static DDouble Log2(DDouble const& d) { return d.Log2(); }
			/**
			@brief 求以10为底的对数*/
			DDouble Lg()const;
			/**
			@brief 求d以10为底的对数*/
			inline static DDouble Lg(DDouble const& d) { return d.Lg(); }
			/**
			@brief 求该数的x次幂*/
			DDouble Pow(DDouble x)const;
			/**
			@brief 求d的x次幂*/
			inline static DDouble Pow(DDouble const& d, DDouble const& x) { return d.Pow(x); }
			/**
			@brief 求2的该数次幂*/
			DDouble Exp2()const;
			/**
			@brief 求2的d次幂*/
			inline static DDouble Exp2(DDouble const& d) { return d.Exp2(); }
			/**
			@brief 求e的该数次幂*/
			DDouble Expe()const;
			/**
			@brief 求e的d次幂*/
			inline static DDouble Expe(DDouble const& d) { return d.Expe(); }
			/**
			@brief 求正弦值*/
			inline DDouble Sin()const { return std::sin(ToDouble()); }
			/**
			@brief 求d的余弦值*/
			inline static DDouble Sin(DDouble const& d) { return d.Sin(); }
			/**
			@brief 求余弦值*/
			inline DDouble Cos()const { return std::cos(ToDouble()); }
			/**
			@brief 求d的余弦值*/
			inline static DDouble Cos(DDouble const& d) { return d.Cos(); }
			/**
			@brief 求正切值*/
			inline DDouble Tan()const { return std::tan(ToDouble()); }
			/**
			@brief 求d的正切值*/
			inline static DDouble Tan(DDouble const& d) { return d.Tan(); }
			/**
			@brief 求余切值*/
			inline DDouble Cot()const { return 1 / std::tan(ToDouble()); }
			/**
			@brief 求d的余切值*/
			inline static DDouble Cot(DDouble const& d) { return d.Cot(); }
			/**
			@brief 求正弦值*/
			inline DDouble Arcsin()const { return std::asin(ToDouble()); }
			/**
			@brief 求d的正弦值*/
			inline static DDouble Arcsin(DDouble const& d) { return d.Arcsin(); }
			/**
			@brief 求余弦值*/
			inline DDouble Arccos()const { return std::acos(ToDouble()); }
			/**
			@brief 求d的余弦值*/
			inline static DDouble Arccos(DDouble const& d) { return d.Arccos(); }
			/**
			@brief 求该数的正切值*/
			inline DDouble Arctan()const { return std::atan(ToDouble()); }
			/**
			@brief 求d的正切值*/
			inline static DDouble Arctan(DDouble const& d) { return d.Arctan(); }
			/**
			@brief 获取一个0~1范围内的DDouble型随机数*/
			inline static DDouble Rand() { static std::random_device r; u64 t = r(); t ^= (u64)r() << 32; return (double)t / MaxValues::UInt64; }
			/**
			@brief 转换为字符串
			@param [传入]参数len表示有效数字长度
			@return 返回一个ptr<String>类型的字符串*/
			std::string ToString(int len = DefOutPrec)const;
			//常数
			const static DDouble*const _Exp10;
			const static DDouble MaxValue;
			const static DDouble MinValue;
			const static DDouble E;
			const static DDouble Pi;
			const static DDouble Ln2;
			const static DDouble Ln10;
			const static DDouble _1Ln2;
			const static DDouble _1Ln10;
		};
	}
	namespace Conversion {
		template<>struct Convert<std::string , Math::DDouble>
		{
			static inline std::string from(Math::DDouble const&l) { return l.ToString(); }
		};
	}
}
namespace std
{
	inline ::LK::Math::DDouble sin(::LK::Math::DDouble const& f) { return f.Sin(); }
	inline ::LK::Math::DDouble cos(::LK::Math::DDouble const& f) { return f.Cos(); }
	inline ::LK::Math::DDouble tan(::LK::Math::DDouble const& f) { return f.Tan(); }
	inline ::LK::Math::DDouble log(::LK::Math::DDouble const& f) { return f.Ln(); }
	inline ::LK::Math::DDouble log10(::LK::Math::DDouble const& f) { return f.Lg(); }
	inline ::LK::Math::DDouble pow(::LK::Math::DDouble const& f1, ::LK::Math::DDouble const& f2) { return f1.Pow(f2); }
	inline ::LK::Math::DDouble asin(::LK::Math::DDouble const& f) { return f.Arcsin(); }
	inline ::LK::Math::DDouble acos(::LK::Math::DDouble const& f) { return f.Arccos(); }
	inline ::LK::Math::DDouble atan(::LK::Math::DDouble const& f) { return f.Arctan(); }
	inline ::LK::Math::DDouble sqrt(::LK::Math::DDouble const& f) { return f.Sqrt(); }
	inline ::LK::Math::DDouble abs(::LK::Math::DDouble const& f) { return f.Abs(); }
	inline ::LK::Math::DDouble floor(::LK::Math::DDouble const& f) { ::LK::Math::DDouble ff = f.GetIntegerPart(); return f.Value <= 0 ? ((ff == f) ? f : ff - 1) : ff; }
	inline ::LK::Math::DDouble ceil(::LK::Math::DDouble const& f) { ::LK::Math::DDouble ff = f.GetIntegerPart(); return f.Value <= 0 ? ff : ((ff == f) ? f : ff + 1); }
	inline ::LK::Math::DDouble fmod(::LK::Math::DDouble const& f1, ::LK::Math::DDouble const& f2) { return f1%f2; }
}
