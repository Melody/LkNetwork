#ifndef LK_MINVALUES_
#define LK_MINVALUES_
namespace LK {
	namespace MinValues {
		/**
		@brief char型变量最小值*/
		static const char Char = -128;
		/**
		@brief wchar_t型变量最小值*/
		static const wchar_t WChar = 0;
		/**
		@brief signed char型变量最小值*/
		static const signed char SignedChar = -128;
		/**
		@brief unsigned char型变量最小值*/
		static const unsigned char UnsignedChar = 0;
		/**
		@brief short型变量最小值*/
		static const short Short = -32768;
		/**
		@brief short型变量最小值*/
		static const short Int16 = -32768;
		/**
		@brief unsigned short型变量最小值*/
		static const unsigned short UnsignedShort = 0;
		/**
		@brief unsigned short型变量最小值*/
		static const unsigned short UInt16 = 0;
		/**
		@brief int型变量最小值*/
		static const int Int = (int)0x80000000;
		/**
		@brief int型变量最小值*/
		static const int Int32 = (int)0x80000000;
		/**
		@brief unsigned int型变量最小值*/
		static const unsigned int UnsignedInt = 0;
		/**
		@brief unsigned int型变量最小值*/
		static const unsigned int UInt32 = 0;
		/**
		@brief long型变量最小值*/
		static const long Long = (long)0x80000000;
		/**
		@brief unsigned long型变量最小值*/
		static const unsigned long UnsignedLong = 0;
		/**
		@brief long long型变量最小值*/
		static const long long LongLong = (long long)0x8000000000000000;
		/**
		@brief long long型变量最小值*/
		static const long long Int64 = (long long)0x8000000000000000;
		/**
		@brief unsigned long long型变量最小值*/
		static const unsigned long long UnsignedLongLong = 0;
		/**
		@brief unsigned long long型变量最小值*/
		static const unsigned long long UInt64 = 0;
		/**
		@brief float型变量最小值*/
		extern const float Float;
		/**
		@brief float型变量无限小*/
		extern const float FloatInfinite;
		/**
		@brief double型变量最小值*/
		extern const double Double;
		/**
		@brief double型变量无限小*/
		extern const double DoubleInfinite;
		/**
		@brief long double型变量最小值*/
		extern const long double LongDouble;
		/**
		@brief long double型变量无限小*/
		extern const long double LongDoubleInfinite;
		/**
		@brief long double型变量绝对值最小值*/
		extern const long double LongDoubleAbs;
		/**
		@brief float型变量绝对值最小值*/
		extern const float FloatAbs;
		/**
		@brief double型变量绝对值最小值*/
		extern const double DoubleAbs;
		/**
		@brief 生成double绝对值最小值*/
		double MakeDoubleAbs();
		/**
		@brief 生成float绝对值最小值*/
		float MakeFloatAbs();
		/**
		@brief 生成long double绝对值最小值*/
		long double MakeLongDoubleAbs();
		/**
		@brief 获取某个类型T的最小值*/
		template <class T>inline static T Of(T* p = NULL)
		{
			if (std::is_same<T, float>::value)return (T)Float;
			if (std::is_same<T, double>::value)return (T)Double;
			if (std::is_same<T, long double>::value)return (T)LongDouble;
			if ((T)-1 > (T)0)return (T)0;
			return (T)(1ull << (sizeof(T) * 8 - 1));
		}
	}
}
#endif