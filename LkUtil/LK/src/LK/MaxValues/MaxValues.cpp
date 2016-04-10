#include"stdafx.h"
#ifdef LK_MAXVALUES_
namespace LK{
	namespace MaxValues {
		/**
		@brief 生成double无穷大*/
		double MakeDoubleInfinite() {
			return 1e308*1e308;
		}
		/**
		@brief 生成long double无穷大*/
		long double MakeLongDoubleInfinite() {
			return 1e308*1e308;
		}
		/**
		@brief 生成float无穷大*/
		float MakeFloatInfinite() {
			return (float)1e300;
		}
		/**
		@brief 生成double最大值*/
		double MakeDouble()
		{
			double d = 1;
			while (d - 1 != d)d = d * 2;
			d -= 2;
			while (d * 4 != d * 2)d *= 2;
			return d;
		}
		/**
		@brief 生成float最大值*/
		float MakeFloat()
		{
			float d = 1;
			while (d - 1 != d)d = d * 2;
			d -= 2;
			while (d * 4 != d * 2)d *= 2;
			return d;
		}
		/**
		@brief 生成long double最大值*/
		long double MakeLongDouble() {
			long double d = 1;
			while (d - 1 != d)d = d * 2;
			d -= 2;
			while (d * 4 != d * 2)d *= 2;
			return d;
		}

		const float Float = MakeFloat();
		const double Double = MakeDouble();
		const float FloatInfinite = MakeFloatInfinite();
		const float FloatAbs = MakeFloat();
		const double DoubleAbs = MakeDouble();
		const double DoubleInfinite = MakeDoubleInfinite();
		const long double LongDouble = MakeLongDouble();
		const long double LongDoubleInfinite = MakeLongDoubleInfinite();
	}
}
#endif