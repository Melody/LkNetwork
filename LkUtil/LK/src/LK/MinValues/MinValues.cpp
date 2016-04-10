#include "stdafx.h"
#ifdef LK_MINVALUES_
#define LK_MINVALUES_
namespace LK{
	namespace MinValues {
		/**
		@brief 生成double绝对值最小值*/
		double MakeDoubleAbs() {
			double d = 1;
			while (d*0.5 != 0)d *= 0.5;
			return d;
		}
		/**
		@brief 生成float绝对值最小值*/
		float MakeFloatAbs() {
			float d = 1;
			while (d*0.5 != 0)d *= 0.5;
			return d;
		}
		/**
		@brief 生成long double绝对值最小值*/
		long double MakeLongDoubleAbs() {
			long double d = 1;
			while (d*0.5 != 0)d *= 0.5;
			return d;
		}
		const float Float = -MaxValues::MakeFloat();
		const double Double = -MaxValues::MakeDouble();
		const float FloatAbs = MakeFloatAbs();
		const double DoubleAbs = MakeDoubleAbs();
		const long double LongDouble = -MaxValues::MakeLongDouble();
		const long double LongDoubleAbs = MakeLongDoubleAbs();
		const float FloatInfinite = -MaxValues::MakeFloatInfinite();
		const double DoubleInfinite = -MaxValues::MakeDoubleInfinite();
		const long double LongDoubleInfinite = -MaxValues::MakeLongDoubleInfinite();
	}
}
#endif