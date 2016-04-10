#pragma once
namespace LK
{
#ifdef _HAS_VARIADIC_TEMPLATES
	//抛出异常
	template<class ReturnType, class T, class ...Args>ReturnType RThrow(Args ...t) { throw T(t...); }
	template<class T, class ...Args>void Throw(Args ...t) { RThrow<void, T, Args...>(t...); }
#else
	//抛出异常
#define TMPF(x) template<MF4_RepeatAsc_(class T,,0,x)>void Throw(MF2_Repeat_b(MF1_Tt,x)){RThrow<void,MF4_RepeatAsc_(T,,0,x)>(MF4_RepeatAsc_(t,,1,x));}\
template<class RT,MF4_RepeatAsc_(class T,,0,x)>RT RThrow(MF2_Repeat_b(MF1_Tt,x)){throw T0(MF4_RepeatAsc_(t,,1,x));}
	MF3_RepeatAsc(TMPF, 0, _MRP_CNTM0);
#undef TMPF
#endif

	template<class _T, class _BT>struct __cast_tmp{
		static inline _T* _CastPointer(_BT const* p){
			if (!p)return NULL;
			_T*pt = dynamic_cast<_T*>((_BT*)p);
			if (!pt)Throw< std::runtime_error >("空指针异常");
			return pt;
		}
	};
	template<class _T>struct __cast_tmp<_T, _T> {
		static inline _T* _CastPointer(_T const* p) {
			return (_T*)p;
		}
	};
	//动态转换基类指针到派生类，不做检查
	template<class T, class BT>T* CastPointer(BT const* p){
		return __cast_tmp<T, BT>::_CastPointer(p);
	}
	//静态转换派生类指针到基类，不做检查
	template<class T>BigObject* StaticCastPointer(T const* p) { return (BigObject*)p; }
	//检查指针p，并在p为NULL时抛出空指针异常
	template<class T, class BT>T* CheckPointer(BT const* p){
		if (p)return CastPointer<T, BT>(p);
		return RThrow<T*, std::runtime_error >("空指针异常");
	}

}
