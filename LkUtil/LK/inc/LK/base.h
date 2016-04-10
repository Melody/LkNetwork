#ifndef LK_BASE_
#define LK_BASE_
#include<string>
namespace LK{
#ifdef _VC
	inline void memcpy(void* dest, const void* source, size_t len) {
		memcpy_s(dest, len, source, len);
	}
	inline void memmove(void *dest, const void* source, size_t len) {
		memmove_s(dest, len, source, len);
	}
#endif
	//定义index_t
#if _X64 == 1
	typedef long long index_t;
#else
	typedef int index_t;
#endif
	//一个大小为size的结构体，size>0
	template<const int size>struct StructOfSize { unsigned char tmp[size]; };

#ifdef _HAS_VARIADIC_TEMPLATES
	//抛出异常的提前声明
	template<class T, class ...Args>inline void Throw(Args ...t);
	template<class ReturnType, class T, class ...Args>inline ReturnType RThrow(Args ...t);
#else
	//抛出异常的提前声明
#define TMPF(x) template<MF4_RepeatAsc_(class T,,0,x)>inline void Throw(MF4_RepeatAsc_(T, , 1, x));\
	template<class RT,MF4_RepeatAsc_(class T,,0,x)>inline RT RThrow(MF4_RepeatAsc_(T, , 1, x));
	MF3_RepeatAsc(TMPF, 0, 16);
#undef TMPF
#endif
	/**
	@brief 操作未支持异常*/
	struct NotSupportException :std::runtime_error {
		//使用一个错误描述字符串来生成异常
		NotSupportException(std::string const& str = "该操作未支持") :std::runtime_error(str) {}
	};
	/**
	@brief 空指针异常*/
	struct PointerIsNullException :std::runtime_error {
		//使用一个错误描述字符串来生成异常
		PointerIsNullException(std::string const& str = "空指针异常") :std::runtime_error(str) {}
	};

	//动态转换基类指针到派生类，不做检查
	template<class T, class BT>inline T* CastPointer(BT const* p);
	//静态转换派生类指针到基类，不做检查
	template<class T>inline BigObject* StaticCastPointer(T const* p);
	//检查指针，如果指针为NULL，则抛出异常
	template<class T, class BT>inline T* CheckPointer(BT const* p);
	//默认互斥锁
	std::recursive_mutex& GetDefaultMutexRef();
	//默认随机数
	unsigned long long GetDefaultRand();

	//类型重命名
	typedef unsigned long long u64;
	typedef unsigned long u32;
	typedef unsigned int ui32;
	typedef unsigned short u16;
	typedef unsigned char u8;
	typedef long long s64;
	typedef long s32;
	typedef int i32;
	typedef short s16;
	typedef signed char s8;

	typedef const unsigned long long cu64;
	typedef const unsigned long cu32;
	typedef const unsigned int cui32;
	typedef const unsigned short cu16;
	typedef const unsigned char  cu8;
	typedef const long long cs64;
	typedef const int ci32;
	typedef const long cs32;
	typedef const short cs16;
	typedef const signed char cs8;
	
}
#endif