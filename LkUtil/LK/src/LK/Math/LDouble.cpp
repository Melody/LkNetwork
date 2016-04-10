#include "stdafx.h"
#ifdef LK_MATH_LDOUBLE_
LK::Math::LDouble::LDouble(double val, long long exp) :Exp(exp), Value(val)
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
		Exp += ((un >> 52) & 0x7ff) - 1022;
		un = (un & 0x800fffffffffffff) | 0x3fe0000000000000;
		Value = dn;
	}
}
LK::Math::LDouble::LDouble(const std::string &str)
{
	char const* pstr = str.c_str();
	int i = 0;
	while (pstr[i]) { if (pstr[i] == 'e' || pstr[i] == 'E')break; i++; }
	Value =Strings::ToDouble(str.substr(0, i));
	Exp = i >= str.size() ? 0 :Strings::ToInt64(str.substr(i + 1, str.size() - (i + 1)));
	Value = std::frexp(Value, &i);
	Exp += i;
	if (Value == 0)
		Exp = 0;
}
LK::Math::LDouble::LDouble(double val) :Exp(0), Value(val)
{
	if (val != 0 && (val >= 1 || (val < 0.5&&val>-0.5) || val <= -1))
	{
		union
		{
			unsigned long long un;
			double dn;
		};
		dn = val;
		Exp = ((un >> 52) & 0x7ff) - 1022;
		un = (un & 0x800fffffffffffff) | 0x3fe0000000000000;
		Value = dn;
	}
}
std::string LK::Math::LDouble::ToString(int len)const
{
	if (Equals0())return "0";
	if (len < 0)Throw<std::invalid_argument>("制定长度小于0");
	if (len > 24)len = 24;
	LDouble ld = Abs();
	int i = 62;
	long long rexp = 0;
	while (i-- > 0)
		if (ld >= _Exp10[i])
		{
			ld /= _Exp10[i];
			rexp += 1ll << i;
		}
	i = 62;
	while (i-- > 0)
		if (ld <= LDouble(1.0) / _Exp10[i])
		{
			ld *= _Exp10[i];
			rexp -= 1ll << i;
		}
	if (ld < LDouble(1.0)) { ld *= *_Exp10; rexp--; }
	else if (ld >= LDouble(10.0)) { ld /= *_Exp10; rexp++; }
	ld.Value = ld.ToDouble();
	std::string str ;
	str.resize(48);
	for (i = 0; i <= len; ++i)
	{
		str[i] = '0' + (char)ld.Value;
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
		if (rexp < 0) { str[++i] = '-'; rexp = -rexp; }
		for (unsigned long long ei = 10000000000000000000ul; ei != 0; ei /= 10)
			if ((unsigned long long)rexp >= ei)
			{
				while (ei != 0)
				{
					str[++i] = '0' + wchar_t((unsigned long long)rexp / ei);
					rexp %= ei;
					ei /= 10;
				}
				break;
			}
		str[i + 1] = 0;
	}
	else if (rexp < 0)
	{
		for (i = len; i >= 0; --i)str[i - rexp + 1] = str[i];
		str[len + 2 - rexp] = 0;
		for (i = 0; i <= (int)-rexp; ++i)str[i] = '0';
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
			str[rexp + 1] = '.';
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
	return str;
}
LK::Math::LDouble LK::Math::operator+(const LDouble& lda, const LDouble& ldb)
{
	LDouble ld1 = lda;
	LDouble ld2 = ldb;
	if (ld1.Value == 0) { return ld2; }
	if (ld2.Value == 0) { return ld1; }
	if ((ld1.Value < 0 && ld2.Value>0) || (ld1.Value > 0 && ld2.Value < 0)) { ld2.Value = -ld2.Value; return ld1 - ld2; }
	if (ld1.Exp < ld2.Exp)BigObject::Swap(ld1, ld2);
	if (((unsigned long long) (ld1.Exp - ld2.Exp))>53)return ld1;
	union {
		unsigned long long un;
		double dn;
	};
	dn = ld2.Value;
	un = (un & 0x800fffffffffffff) | ((ld2.Exp - ld1.Exp + 1022) << 52);
	ld1.Value += dn;
	if (ld1.Value >= 1 || ld1.Value <= -1) { ld1.Value /= 2; ld1.Exp += 1; }
	return LDouble(ld1.Value, ld1.Exp);
}
LK::Math::LDouble LK::Math::operator-(const LDouble& lda, const LDouble& ldb)
{
	LDouble ld1 = lda;
	LDouble ld2 = ldb;
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
	return LDouble(ld1.Value, ld1.Exp);
}
LK::Math::LDouble LK::Math::operator*(const LDouble& lda, const LDouble& ldb)
{
	LDouble v;
	v.Value = lda.Value * ldb.Value;
	if (v.Value<0.5 && v.Value>-0.5)
	{
		v.Exp = lda.Exp + ldb.Exp - 1;
		v.Value *= 2;
	}
	else
	{
		v.Exp = lda.Exp + ldb.Exp;
	}
	return v;
}
LK::Math::LDouble LK::Math::operator/(const LDouble& lda, const LDouble& ldb)
{
	LDouble v;
	v.Value = lda.Value / ldb.Value;
	if (v.Value<1 && v.Value>-1)
		v.Exp = lda.Exp - ldb.Exp;
	else
	{
		v.Value /= 2;
		v.Exp = lda.Exp - ldb.Exp + 1;
	}
	return v;
}
LK::Math::LDouble LK::Math::operator%(const LDouble& dda, const LDouble& ddb)
{
	if (ddb.Equals0())return 0.0 / LDouble(0).ToDouble();
	if (dda.Equals0())return 0;
	LDouble d1 = dda.Abs(), d2 = ddb.Abs();
	if (d1<d2)return dda;
	return (dda / d2).GetDecimalPart()*d2;
}
bool LK::Math::LDouble::operator>(const LDouble& ld)const
{
	if (Value > 0)
	{
		return ld.Value <= 0 || Exp > ld.Exp || (Exp == ld.Exp&&Value > ld.Value);
	}
	if (Value == 0)return ld.Value < 0;
	return ld.Value < 0 && (Exp<ld.Exp || (Exp == ld.Exp&&Value>ld.Value));
}
bool LK::Math::LDouble::operator>=(const LDouble& ld)const
{
	if (Value > 0)
	{
		return ld.Value <= 0 || Exp > ld.Exp || (Exp == ld.Exp&&Value >= ld.Value);
	}
	if (Value == 0)return ld.Value <= 0;
	return ld.Value < 0 && (Exp < ld.Exp || (Exp == ld.Exp&&Value >= ld.Value));
}
LK::Math::LDouble LK::Math::LDouble::Abs()const
{
	if (Value < 0)return -*this;
	return *this;
}
LK::Math::LDouble LK::Math::LDouble::GetIntegerPart()const
{
	if (Exp > 52)return *this;
	if (Exp < 0)return 0;
	return (double)((long long)(ToDouble()));
}
LK::Math::LDouble LK::Math::LDouble::GetDecimalPart()const
{
	if (Exp > 52)return 0;
	if (Exp <= 0)return *this;
	return *this - LDouble((double)(long long)ToDouble());
}

LK::Math::LDouble LK::Math::LDouble::Sqrt()const
{
	if (Exp & 1)return LDouble(std::sqrt(Value * 2), Exp / 2);
	return LDouble(std::sqrt(Value), Exp / 2);
}
LK::Math::LDouble LK::Math::LDouble::Ln()const
{
	return LDouble((double)Exp)*Ln2 + LDouble(log(Value));
}
LK::Math::LDouble LK::Math::LDouble::Log2()const
{
	return LDouble((double)Exp) + LDouble(log(Value)) * _1Ln2;
}
LK::Math::LDouble LK::Math::LDouble::Lg()const
{
	return LDouble((double)Exp)*LDouble(log(Value)) * _1Ln10;
}
LK::Math::LDouble LK::Math::LDouble::Pow(LK::Math::LDouble x)const
{
	if (Value == 0) { if (x.Value > 0)return 0; else return std::sqrt(-1.0); }
	if (Value > 0) return (x*(LDouble((double)Exp) + _1Ln2*LDouble(std::log(Value)))).Exp2();
	if (x.Value > 0)return -(x*(LDouble((double)Exp) + _1Ln2*LDouble(std::log(-Value)))).Exp2();
	if (x.GetDecimalPart().Equals0() && x.Exp < 53)//x为整数
	{
		if (1 & (unsigned long long)x.GetIntegerPart().ToDouble())
			return -(x*(LDouble((double)Exp) + _1Ln2*LDouble(std::log(-Value)))).Exp2();
		else return (x*(LDouble((double)Exp) + _1Ln2*LDouble(std::log(-Value)))).Exp2();
	}
	return std::sqrt(-1.0);
}
LK::Math::LDouble LK::Math::LDouble::Exp2()const
{
	return LDouble(std::pow(2.0, GetDecimalPart().ToDouble()), (long long)GetIntegerPart().ToDouble());
}
LK::Math::LDouble LK::Math::LDouble::Expe()const
{
	LDouble t = *this * _1Ln2;
	return t.Exp2();
}
namespace LK {
	namespace Math {
		const LDouble* _LDoubleMakeExp10()
		{
			static LDouble exp10[64];
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
		const LDouble*const LDouble::_Exp10 = _LDoubleMakeExp10();
	}
}
const LK::Math::LDouble LK::Math::LDouble::E = 2.718281828459045;
const LK::Math::LDouble LK::Math::LDouble::Pi = 3.141592653589793;
const LK::Math::LDouble LK::Math::LDouble::Ln2 = std::log(2.0);
const LK::Math::LDouble LK::Math::LDouble::_1Ln2 = 1 / std::log(2.0);
const LK::Math::LDouble LK::Math::LDouble::Ln10 = std::log(10.0);
const LK::Math::LDouble LK::Math::LDouble::_1Ln10 = 1 / std::log(10.0);
const LK::Math::LDouble LK::Math::LDouble::MaxValue = LK::Math::LDouble(0.5, MaxValues::Int64);
const LK::Math::LDouble LK::Math::LDouble::MinValue = LK::Math::LDouble(-0.5, MaxValues::Int64);

#endif
