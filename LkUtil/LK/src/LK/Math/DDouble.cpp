#include "stdafx.h"
#ifdef LK_MATH_DDOUBLE_
int LK::Math::DDouble::DefOutPrec = 6;
LK::Math::DDouble::DDouble(double val, double ep) : Exp(ep), Value(val)
{
	if (val == 0)Exp = 0;
	else if (val >= 1 || (val < 0.5&&val>-0.5) || val <= -1)
	{
		union
		{
			double dn;
			unsigned long long un;
		};
		dn = val;
		Exp += (long)((un >> 52) & 0x7ff) - 1022.0;
		un = (un & 0x800fffffffffffff) | 0x3fe0000000000000;
		Value = dn;
	}
}
LK::Math::DDouble::DDouble(const std::string &str)
{
	char const* pstr = str.c_str();
	int i = 0;
	while (pstr[i]) { if (pstr[i] == 'e' || pstr[i] == 'E')break; i++; }
	double et = i < str.size() ? Strings::ToDouble(str.substr(i + 1, (int)str.size() - (i + 1))) : 0;
	Value = Strings::ToDouble(str.substr(0, i));
	Value = std::frexp(Value, &i);
	Exp = i;
	if (Value == 0)Exp = 0;
	else
	{
		*this *= DDouble(10.0).Pow(et);
		if (IsError())MakeError(MathError::InputOverflow);
	}

}
LK::Math::DDouble::DDouble(double val) :Exp(0), Value(val)
{
	if (val != 0 && (val >= 1 || (val < 0.5&&val>-0.5) || val <= -1))
	{
		union
		{
			unsigned long long un;
			double dn;
		};
		dn = val;
		Exp = long(((un >> 52) & 0x7ff) - 1022);
		un = (un & 0x800fffffffffffff) | 0x3fe0000000000000;
		Value = dn;
	}
}
std::string LK::Math::DDouble::ToString(int len)const
{
	if (IsError())
	{
		int ec = GetErrorCode();
		if (ec > 0 || ec < -20)return "未初始化错误";
		return MathError::description[-ec];
	}
	if (Equals0())return "0";
	if (len < 0)Throw<std::invalid_argument>("指定长度小于0");
	if (len > 24)len = 24;
	DDouble ld = Abs();
	int i = 62;
	double rexp = 0;
	if (ld.Exp > (double)MinValues::Int64&&ld.Exp < (double)MaxValues::Int64)//较小时使用精确计算法
	{
		while (i-- > 0)
			if (ld >= _Exp10[i])
			{
				ld /= _Exp10[i];
				rexp += 1LL << i;
			}
		i = 62;
		while (i-- > 0)
			if (ld <= DDouble(1.0) / _Exp10[i])
			{
				ld *= _Exp10[i];
				rexp -= 1LL << i;
			}
		if (ld < DDouble(1.0)) { ld *= *_Exp10; rexp--; }
		else if (ld >= DDouble(10.0)) { ld /= *_Exp10; rexp++; }
		ld.Value = ld.ToDouble();
	}
	else//过大将使用对数计算
	{
		rexp = (ld.Ln() / Ln10).GetIntegerPart().ToDouble();
		DDouble tv = ld / DDouble(10).Pow(rexp);
		if (tv >= DDouble(10.0) || tv < DDouble(1.0)) { tv.Exp = 1; }
		ld.Value = tv.ToDouble();
		if (ld.Value < 1.0) { ld.Value *= 10.0; rexp--; }
		else if (ld.Value >= 10.0) { ld.Value /= 10.0; rexp++; }
	}
	std::string str;
	str.resize(64);
	for (i = 0; i <= len; ++i)
	{
		str[i] = '0' + (wchar_t)ld.Value;
		ld.Value = (ld.Value - (int)ld.Value) * 10;
	}
	str[len + 1] = 0;
	if (str[len] >= '5')//进位
	{
		str[len] = 0;
		for (i = len; i > 0; i--)
			if (str[i - 1] < '9') { str[i - 1]++; break; }
			else str[i - 1] = 0;
			if (str[0] == 0) { str[0] = '1'; rexp++; }
	}
	else str[len] = 0;
	while (len > 0 && (str[len] == 0 || str[len] == '0'))str[len--] = 0;
	if (rexp<-4 || rexp>len + 4)//科学计数法，插入小数点，加上指数
	{
		i = 0;
		if (str[1] != 0) {//有必要插入小数点
			char tc = '.';
			while (str[++i] != 0)
				BigObject::Swap(tc, str[i]);
			str[i] = tc;
		}
		str[++i] = 'e';
		std::string ss=Strings::FromDouble((rexp > -1000000000000000000ll && rexp < 1000000000000000000ll ? (double)(long long)(rexp + (rexp >= 0 ? 0.5 : -0.5)) : rexp, 0));
		++i;
		for (int ii = 0; (str[i + ii] = ss[ii]) != 0; ++ii);
	}
	else if (rexp < 0)
	{
		for (i = len; i >= 0; --i)str[i - (long long)rexp + 1] = str[i];
		str[len + 2 - (long long)rexp] = 0;
		for (i = 0; i <= (long long)-rexp; ++i)str[i] = '0';
		str[1] = '.';
	}
	else
	{
		for (i = 1; i <= rexp; ++i)if (i > len)str[i] = '0';
		if (i > len)str[i] = 0;
		else
		{
			for (i = len; i > rexp; --i)str[i + 1] = str[i];
			str[len + 2] = 0;
			str[(long long)rexp + 1] = '.';
		}
	}
	if (Value < 0)//插入“-”号
	{
		i = 0;
		char tc = '-';
		while (str[i] != 0)
			BigObject::Swap(tc, str[i++]);
		str[i] = tc; str[i + 1] = 0;
	}
	str.resize(Strings::strlen(str.c_str()));
	;	return str;
}
LK::Math::DDouble LK::Math::operator+(const DDouble& dda, const DDouble& ddb)
{
	if (dda.IsError())return dda;
	if (ddb.IsError())return ddb;
	if (dda.Value == 0) { return ddb; }
	if (ddb.Value == 0) { return dda; }
	DDouble ld1 = dda;
	DDouble ld2 = ddb;
	if ((ld1.Value < 0 && ld2.Value>0) || (ld1.Value > 0 && ld2.Value < 0)) { ld2.Value = -ld2.Value; return ld1 - ld2; }
	if (ld1.Exp < ld2.Exp)BigObject::Swap(ld1, ld2);
	if (ld1.Exp > ld2.Exp + 53)return ld1;
	union
	{
		unsigned long long un;
		double dn;
	};
	dn = ld2.Value;
	un = (un & 0x800fffffffffffff) | (((unsigned long long)(ld2.Exp - ld1.Exp + 1022)) << 52);
	ld1.Value += dn;
	if (ld1.Value >= 1 || ld1.Value <= -1) {
		ld1.Value /= 2; ld1.Exp += 1;
	}
	return ld1;
}
LK::Math::DDouble LK::Math::operator-(const DDouble& dda, const DDouble& ddb)
{
	if (dda.IsError())return dda;
	if (ddb.IsError())return ddb;
	DDouble ld1 = dda;
	DDouble ld2 = ddb;
	if (ld1.Value == 0) { ld2.Value = -ld2.Value; return ld2; }
	if (ld2.Value == 0) { return ld1; }
	if ((ld1.Value < 0 && ld2.Value>0) || (ld1.Value > 0 && ld2.Value < 0)) { ld2.Value = -ld2.Value; return ld1 + ld2; }
	if (ld1.Abs() < ld2.Abs()) {
		BigObject::Swap(ld1.Exp, ld2.Exp); double v = -ld1.Value; ld1.Value = -ld2.Value; ld2.Value = v;
	}
	if (ld1.Exp > ld2.Exp + 53)return ld1;
	union {
		unsigned long long un;
		double dn;
	};
	dn = ld2.Value;
	un = (un & 0x800fffffffffffff) | ((unsigned long long)(ld2.Exp - ld1.Exp + 1022) << 52);
	ld1.Value -= dn;
	return DDouble(ld1.Value, ld1.Exp);
}
LK::Math::DDouble LK::Math::operator*(const DDouble& dda, const DDouble& ddb)
{
	if (dda.IsError())return dda;
	if (ddb.IsError())return ddb;
	DDouble v;
	v.Value = dda.Value * ddb.Value;
	if (v.Value<0.5 && v.Value>-0.5)
	{
		v.Exp = dda.Exp + ddb.Exp - 1;
		v.Value *= 2;
	}
	else
	{
		v.Exp = dda.Exp + ddb.Exp;
	}
	if (v.Exp > MaxValues::Double)return v.MakeError(MathError::MultiplyExpOverflow);
	if (v.Exp < MinValues::Double)return 0;
	return v;
}
LK::Math::DDouble LK::Math::operator/(const DDouble& dda, const DDouble& ddb)
{
	if (dda.IsError())return dda;
	if (ddb.IsError())return ddb;
	if (ddb.Equals0())return DDouble(dda).MakeError(MathError::Divide0);
	DDouble v;
	v.Value = dda.Value / ddb.Value;
	if (v.Value<1 && v.Value>-1)
		v.Exp = dda.Exp - ddb.Exp;
	else
	{
		v.Value /= 2;
		v.Exp = dda.Exp - ddb.Exp + 1;
	}
	if (v.Exp > MaxValues::Double)return v.MakeError(MathError::DivideExpOverflow);
	if (v.Exp < MinValues::Double)return 0;
	return v;
}
LK::Math::DDouble LK::Math::operator%(const DDouble& dda, const DDouble& ddb)
{
	if (dda.IsError())return dda;
	if (ddb.IsError())return ddb;
	if (ddb.Equals0())return DDouble(dda).MakeError(MathError::ModDivide0);
	if (dda.Equals0())return 0;
	DDouble d1 = dda.Abs(), d2 = ddb.Abs();
	if (d1<d2)return dda;
	return (dda / d2).GetDecimalPart()*d2;
}
bool LK::Math::DDouble::operator>(const DDouble& ld)const
{
	if (Value > 0)
	{
		return ld.Value <= 0 || Exp > ld.Exp || (Exp == ld.Exp&&Value > ld.Value);
	}
	if (Value == 0)return ld.Value < 0;
	return ld.Value < 0 && (Exp<ld.Exp || (Exp == ld.Exp&&Value>ld.Value));
}
bool LK::Math::DDouble::operator>=(const DDouble& ld)const
{
	if (Value > 0)
	{
		return ld.Value <= 0 || Exp > ld.Exp || (Exp == ld.Exp&&Value >= ld.Value);
	}
	if (Value == 0)return ld.Value <= 0;
	return ld.Value < 0 && (Exp < ld.Exp || (Exp == ld.Exp&&Value >= ld.Value));
}
LK::Math::DDouble LK::Math::DDouble::Abs()const
{
	if (Value < 0)return -*this;
	return *this;
}
LK::Math::DDouble LK::Math::DDouble::GetIntegerPart()const
{
	if (Exp > 52)return *this;
	if (Exp < 0)return 0;
	return (double)(long long)ToDouble();
}
LK::Math::DDouble LK::Math::DDouble::GetDecimalPart()const
{
	if (Exp > 52)return 0;
	if (Exp <= 0)return *this;
	return *this - (double)(long long)ToDouble();
}

LK::Math::DDouble LK::Math::DDouble::Sqrt()const
{
	if (IsError())return *this;
	if (Value < 0)return DDouble(*this).MakeError(MathError::SqrtLessThan0);
	if (((int)Exp) & 1)return DDouble(std::sqrt(Value * 2), Exp / 2);
	return DDouble(std::sqrt(Value), Exp / 2);
}
LK::Math::DDouble LK::Math::DDouble::Ln()const
{
	if (IsError())return *this;
	if (Value <= 0)return DDouble(*this).MakeError(MathError::LnLessOrEqual0);
	return (Exp)*Ln2 + std::log(Value);
}
LK::Math::DDouble LK::Math::DDouble::Log2()const
{
	if (IsError())return *this;
	if (Value <= 0)return DDouble(*this).MakeError(MathError::LnLessOrEqual0);
	return (Exp)+log(Value) * _1Ln2;
}
LK::Math::DDouble LK::Math::DDouble::Lg()const
{
	if (IsError())return *this;
	if (Value <= 0)return DDouble(*this).MakeError(MathError::LnLessOrEqual0);
	return Exp*log(Value) * _1Ln10;
}
LK::Math::DDouble LK::Math::DDouble::Pow(LK::Math::DDouble x)const
{
	if (IsError())return *this;
	if (x.IsError())return x;
	if (Value == 0) { if (x.Value > 0)return 0; else if (x.Value == 0) return DDouble(*this).MakeError(MathError::Pow00); }
	if (Value > 0)
	{
		DDouble result = (x*(DDouble(Exp) + _1Ln2*DDouble(std::log(Value)))).Exp2();
		if (result.IsError() || result.Exp > MaxValues::Double)return result.MakeError(MathError::PowExpOverflow);
		if (result.Exp < MinValues::Double)return 0;
		return result;
	}
	if (x.Value > 0)
	{
		DDouble result = -(x*(DDouble(Exp) + _1Ln2*DDouble(std::log(-Value)))).Exp2();
		if (result.IsError() || result.Exp > MaxValues::Double)return result.MakeError(MathError::PowExpOverflow);
		if (result.Exp < MinValues::Double)return 0;
		return result;
	}
	if (x.GetDecimalPart().Equals0() && x.Exp < 53)//x为整数
	{
		if (1 & (unsigned long long)x.GetIntegerPart().ToDouble())
		{
			DDouble result = -(x*(DDouble(Exp) + _1Ln2*DDouble(std::log(-Value)))).Exp2();
			if (result.IsError() || result.Exp > MaxValues::Double)return result.MakeError(MathError::PowExpOverflow);
			if (result.Exp < MinValues::Double)return 0;
			return result;
		}
	}
	return DDouble(*this).MakeError(MathError::PowLessThan0);
}
LK::Math::DDouble LK::Math::DDouble::Exp2()const
{
	if (IsError())return *this;
	DDouble result = DDouble(std::pow(2, GetDecimalPart().ToDouble()), GetIntegerPart().ToDouble());
	if (result.Exp > MaxValues::Double)return result.MakeError(MathError::Exp2Overflow);
	if (result.Exp < MinValues::Double)return 0;
	return result;
}
LK::Math::DDouble LK::Math::DDouble::Expe()const
{
	if (IsError())return *this;
	DDouble t = *this * _1Ln2;
	if (t.IsError())return t.MakeError(MathError::ExpEOverflow);
	return t.Exp2();
}
namespace LK {
	namespace Math {
		const DDouble* _DDoubleMakeExp10()
		{
			static DDouble exp10[64];
			exp10[0] = 10.0;
			int i = 0;
			while (i < 62)
			{
				exp10[i + 1] = exp10[i] * exp10[i];
				i++;
			}
			while (i++ < 62)
			{
				exp10[i] = exp10->Pow((double)(1ull << i));
			}
			return exp10;
		}
		const DDouble*const DDouble::_Exp10 = _DDoubleMakeExp10();
	}
}
const LK::Math::DDouble LK::Math::DDouble::E = 2.718281828459045;
const LK::Math::DDouble LK::Math::DDouble::Pi = 3.141592653589793;
const LK::Math::DDouble LK::Math::DDouble::Ln2 = std::log(2.0);
const LK::Math::DDouble LK::Math::DDouble::_1Ln2 = 1 / std::log(2.0);
const LK::Math::DDouble LK::Math::DDouble::Ln10 = std::log(10.0);
const LK::Math::DDouble LK::Math::DDouble::_1Ln10 = 1 / std::log(10.0);
const LK::Math::DDouble LK::Math::DDouble::MaxValue = LK::Math::DDouble(0.5, MaxValues::Double);
const LK::Math::DDouble LK::Math::DDouble::MinValue = LK::Math::DDouble(-0.5, MaxValues::Double);
#endif
