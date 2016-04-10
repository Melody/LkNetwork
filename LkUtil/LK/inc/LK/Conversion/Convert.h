#ifndef LK_CONVERSION_CONVERT_
#define LK_CONVERSION_CONVERT_
namespace LK {
	namespace Conversion {
		//默认允许From未指定
		template<class To,class From=void>struct Convert {
			static To from(From f) { return (To)f; }
			static To reference(typename std::add_lvalue_reference<typename std::add_const<From>::type>::type f) { return (typename std::add_lvalue_reference<To>::type)f; }
		};
		//From未指定版本
		template<class To>struct Convert<To,void> {
			template<class frm>static To from(frm f) { return Convert<To,frm>::from(f); }
			template<class frm>static To reference(frm const& f) { return Convert<To, frm>::reference(f); }
		};
		//To未指定则不转换
		template<class From>struct Convert<void,From> {
			static void from(From f) { }
			static void reference(From const& f) { }
		};

	}
}
#endif