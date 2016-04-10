#pragma once
#include<new>
namespace LK
{
	template<class T>inline T* placementnew() { return new T; }
#if _HAS_VARIADIC_TEMPLATES
	/**
	@brief 在指定的内存中构造对象并返回该对象指针
	@param [传入]参数t指向一块用户已分配好的内存
	@param [传入,...]参数args表示构造该对象所需参数，默认按复制传入
	@return 返回t的值*/
	template<class T, class ...Args>inline T* placementnew(T* t, Args...args) { return new(t)T(args...); }
#else
#define TMPF(x) template<MF3_RepeatAsc_(MF1_CT,1,x)>inline T1* placementnew(T1* t MF3_RepeatAsc(MF1__Tt,2,x)){return new(t)T1(MF3_RepeatAsc_(MF1_t,2,x));}
	MF2_Repeatb(TMPF, _MRP_CNTM0)
#undef TMPF
#endif
}