#include "stdafx.h"
#ifdef LK_BASE_
namespace LK {
	std::recursive_mutex& GetDefaultMutexRef() {
		static std::recursive_mutex mt;
		return mt;
	}
	unsigned long long GetDefaultRand() {
		static u64 r1 = 0xd8f641b512498c41, r2 = 0x8173446f1163a324, r3 = 0x419427b1709d61fa;
		r1 = ((r1 << 11) | (r1 >> 53)) + r3 + 1;
		r2 = ((r2 << 17) | (r2 >> 47)) + r1 + 1;
		r3 = ((r3 << 43) | (r3 >> 21)) + r2 + 1;
		return r1^r2^r3;
	}
	/*
#ifndef _WINDOWS
	static void* thr_fn(void* p)
	{
		typedef void(*FUN)(void*);
		FUN f = *(FUN*)p;
		void* param = *(void**)(((char*)p) + sizeof(FUN));
		((unsigned char*)p)[sizeof(FUN) + sizeof(void*)] = 0;
		f(param);
		return NULL;
	}
#endif
	/**
	@brief 启动线程的基础API/
	index_t APIStartThread(void(*fun)(void*), void* param)
	{
#ifdef _WINDOWS
		DWORD dwThreadId;
		::CreateThread(
			0,//NULL,使用默认安全性
			0,//NULL,使用与调用该函数的线程相同的栈空间大小
			  //Thread1,//指定新线程的入口函数名(DWORD WINAPI 声明方法)
			(LPTHREAD_START_ROUTINE)fun,//指定新线程的入口函数名(void 声明方法)
			param,//向新线程传递的参数，不需要传递时设为NULL
			0,//表示新线程立即开始执行
			&dwThreadId);//保存新线程的ID
		return dwThreadId;
#else
		typedef void(*FUN)(void*);
		int err;
		pthread_t ntid;
		unsigned char tmp[sizeof(FUN) + sizeof(void*) + sizeof(unsigned char)];
		tmp[sizeof(FUN) + sizeof(void*)] = 1;
		void *p = tmp;
		*(FUN*)p = fun;
		*(void**)(((unsigned char*)p) + sizeof(FUN)) = param;
		err = pthread_create(&ntid, NULL, thr_fn, p);
		if (err)Throw<std::runtime_error >("创建线程失败！");
		while (tmp[sizeof(FUN) + sizeof(void*)] != 0)
			Sleep(0);
		return ntid;
#endif
	}
	index_t APIGetThreadID()
	{
#ifdef _WINDOWS
		return (index_t)GetCurrentThreadId();
#else
		return (index_t)pthread_self();
#endif
	}*/
}
#endif