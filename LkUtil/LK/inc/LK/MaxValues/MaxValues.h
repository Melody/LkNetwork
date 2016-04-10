#ifndef LK_MAXVALUES_
#define LK_MAXVALUES_
namespace LK {
	namespace MaxValues {
		/**
		@brief char型变量最大值*/
		static const char Char = (char)-1 > (char)0 ? 255 : 127;
		/**
		@brief wchar_t型变量最大值*/
		static const wchar_t WChar = 0xffff;
		/**
		@brief signed char型变量最大值*/
		static const signed char SignedChar = 127;
		/**
		@brief unsigned char型变量最大值*/
		static const unsigned char UnsignedChar = 0xff;
		/**
		@brief short型变量最大值*/
		static const short Short = 0x7fff;
		/**
		@brief short型变量最大值*/
		static const short Int16 = 0x7fff;
		/**
		@brief unsigned short型变量最大值*/
		static const unsigned short UnsignedShort = 0xffff;
		/**
		@brief unsigned short型变量最大值*/
		static const unsigned short UInt16 = 0xffff;
		/**
		@brief int型变量最大值*/
		static const int Int = 0x7fffffff;
		/**
		@brief int型变量最大值*/
		static const int Int32 = 0x7fffffff;
		/**
		@brief unsigned int型变量最大值*/
		static const unsigned int UnsignedInt = 0xffffffff;
		/**
		@brief unsigned int型变量最大值*/
		static const unsigned int UInt32 = 0xffffffff;
		/**
		@brief long型变量最大值*/
		static const long Long = 0x7fffffff;
		/**
		@brief unsigned long型变量最大值*/
		static const unsigned long UnsignedLong = 0xffffffff;
		/**
		@brief long long型变量最大值*/
		static const long long LongLong = 0x7fffffffffffffff;
		/**
		@brief long long型变量最大值*/
		static const long long Int64 = 0x7fffffffffffffff;
		/**
		@brief unsigned long long型变量最大值*/
		static const unsigned long long UnsignedLongLong = 0xffffffffffffffff;
		/**
		@brief unsigned long long型变量最大值*/
		static const unsigned long long UInt64 = 0xffffffffffffffff;
		/**
		@brief float型变量最大值*/
		extern const float Float;
		/**
		@brief float型变量无限大*/
		extern const float FloatInfinite;
		/**
		@brief double型变量最大值*/
		extern const double Double;
		/**
		@brief double型变量无限大*/
		extern const double DoubleInfinite;
		/**
		@brief long double型变量最大值*/
		extern const long double LongDouble;
		/**
		@brief long double型变量无限大*/
		extern const long double LongDoubleInfinite;
		/**
		@brief float型变量最大值*/
		extern const float FloatAbs;
		/**
		@brief double型变量最大值*/
		extern const double DoubleAbs;
		/**
		@brief 生成double最大值*/
		double MakeDouble();
		/**
		@brief 生成float最大值*/
		float MakeFloat();
		/**
		@brief 生成double无穷大*/
		double MakeDoubleInfinite();
		/**
		@brief 生成long double无穷大*/
		long double MakeLongDoubleInfinite();
		/**
		@brief 生成float无穷大*/
		float MakeFloatInfinite();
		/**
		@brief 生成long double最大值*/
		long double MakeLongDouble();
		/**
		@brief 获取某个类型T的最大值*/
		template <class T>inline static T Of(T* p = NULL){
			if (std::is_same<T, float>::value)return (T)Float;
			if (std::is_same<T, double>::value)return (T)Double;
			if (std::is_same<T, long double>::value)return (T)LongDouble;
			if ((T)-1 > (T)0)return (T)-1;
			return (T)(((u64) - 1) >> (64 - sizeof(T) * 8 + 1));
		}
	}
}
#endif