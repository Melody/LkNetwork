#ifndef LK_STRINGS_CONVERT_
#define LK_STRINGS_CONVERT_
namespace LK{
	namespace Strings {
		/**
		@brief 辅助函数，计算10的i次方,i的绝对值最大为308*/
		double IntPow10(index_t i);
		/**
		@brief 将字符串转换为double型数值输出*/
		double ToDouble(std::string str);
		/**
		@brief 从double转换到字符串*/
		std::string FromDouble(double dbl, int prec = DefaultPrecisionRef(), int minlen = 0, int d = 10);
		/**
		@brief 从unsigned long long转换到字符串*/
		std::string FromUInt64(u64 u);
		/**
		@brief 从long long转换到字符串*/
		std::string FromInt64(s64 u);
		/**
		@brief 将字符串转换为unsigned long long型数值输出*/
		u64 ToUInt64(std::string const&str);
		/**
		@brief 将字符串转换为long long型数值输出*/
		s64 ToInt64(std::string const&str);
		/**
		@brief 转换为宽字节字符串*/
		std::wstring ToWString(std::string const&);
		inline std::wstring const& ToWString(std::wstring const&str) { return str; }
		/**
		@brief 转换为多宽字节字符串*/
		std::string ToString(std::wstring const&);
		inline std::string const& ToString(std::string const&str) { return str; }
		/**
		@brief 通用转换方法*/
		template<class T>typename std::enable_if<std::is_base_of<ptr<BigObject>,T>::value,std::string >::type ToString(T const&t) { return t->ToString(); }
		template<class T>typename std::enable_if<std::is_base_of<BigObject,T>::value,std::string >::type ToString(T const&t) { return t.ToString(); }
		template<class T>typename std::enable_if<std::is_base_of<ptr<BigObject>,T>::value,std::string >::type ToWString(T const&t) { return ToString(t->ToString()); }
		template<class T>typename std::enable_if<std::is_base_of<BigObject,T>::value,std::string >::type ToWString(T const&t) { return ToString(t.ToString()); }
	}
}
#endif