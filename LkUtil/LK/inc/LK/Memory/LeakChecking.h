#ifndef LK_LEAKCHECKING_//由W意波(LuoJun)编写
#define LK_LEAKCHECKING_
//是否使用Windows自带的内存泄露检测机制，注释掉这句define表示不使用
#ifdef _VC
#define USEMFCLEAKCHECK  1
#endif
#include<map>
namespace LK
{
	/**
	@brief 表示一个内存泄漏检测对象（节点）*/
	class LeakCheckingObject
	{
		static std::map<void*, LeakCheckingObject*>& map();
		static std::recursive_mutex& mutex();
	public:
		/**
		@brief 分配内存*/
		static inline void* malloc(size_t s) { return malloc(s); }
		/**
		@brief 释放内存*/
		static inline void free(void* p) { free(p); }
		/**
		@brief 记录文件名*/
		char const* fileName;
		/**
		@brief 记录所在行数*/
		size_t line;
		/**
		@brief 记录分配内存大小*/
		size_t size;
		/**
		@brief 记录是否已经开始进入记录过程，以减少循环次数*/
		static bool& record() { Default(); static bool rec = true; return rec; }
		/**
		@brief 移除记录里的某个指针*/
		static void Remove(void*p);
		/**
		@brief 全局节点，用来进行记录的操作*/
		inline static LeakCheckingObject& Default();
		/**
		@brief 构造一个记录节点*/
		LeakCheckingObject(void*p, char const*file, size_t line, size_t size);
		/**
		@brief 默认构造函数*/
		LeakCheckingObject();
		/**
		@brief 使用线程互斥对象和映射来构造全局信息节点*/
		LeakCheckingObject(std::recursive_mutex* pm, std::map<void*, LeakCheckingObject*>*pp);
		/**
		@brief 析构函数*/
		~LeakCheckingObject();
	};
	inline std::map<void*, LeakCheckingObject*>& LeakCheckingObject::map()
	{
		return *(std::map<void*, LeakCheckingObject*>*)Default().line;;
	}
	inline std::recursive_mutex& LeakCheckingObject::mutex()
	{
		return *(std::recursive_mutex*)Default().fileName;
	}
	inline LeakCheckingObject::LeakCheckingObject(void*p, char const*file, size_t line, size_t size)
		:fileName(file), line(line), size(size)
	{
		mutex().lock();
		record() = false;
		map()[p] = this;
		record() = true;
		mutex().unlock();
	}
	inline LeakCheckingObject& LeakCheckingObject::Default()
	{
		static bool hasinitialized = false;
		static LeakCheckingObject obj;
		if (hasinitialized == false)
		{
			hasinitialized = true;
			record() = false;
			std::recursive_mutex* pm = (std::recursive_mutex*)malloc(sizeof(std::recursive_mutex));
			std::map<void*, LeakCheckingObject*>* pp = (std::map<void*, LeakCheckingObject*>*)malloc(sizeof(std::map<void*, LeakCheckingObject*>));
			placementnew(pm);
			placementnew(pp);
			obj = LeakCheckingObject(pm, pp);
			record() = true;
		}
		return obj;
	}
	inline LeakCheckingObject::LeakCheckingObject() :fileName(NULL), line(0) {}
	inline LeakCheckingObject::LeakCheckingObject(std::recursive_mutex* pm, std::map<void*, LeakCheckingObject*>*pp)
		: fileName((char const*)pm), line((size_t)pp), size(0) {}

}
#ifdef _DEBUG
#if USEMFCLEAKCHECK==0
inline void* operator new(size_t size, char const* file, int line)
{
	void* p = LK::LeakCheckingObject::malloc(size);
if (LK::LeakCheckingObject::record())
{
	LK::LeakCheckingObject* pl = (LK::LeakCheckingObject*)LK::LeakCheckingObject::malloc(sizeof(LK::LeakCheckingObject));
	placementnew(pl, p, file, line, size);
}
return p;
}
inline void* operator new[](size_t size, char const* file, int line)
{
	void* p = LK::LeakCheckingObject::malloc(size);
	if (LK::LeakCheckingObject::record())
	{
		LK::LeakCheckingObject* pl = (LK::LeakCheckingObject*)LK::LeakCheckingObject::malloc(sizeof(LK::LeakCheckingObject));
		placementnew(pl, p, file, line, size);
	}
	return p;
}
inline void* operator new(size_t s)
{
	if (s == 0)return NULL;
void* p = LK::LeakCheckingObject::malloc(s);
/*不做记录
if (LK::LeakCheckingObject::record())
{
LK::LeakCheckingObject* pl = (LK::LeakCheckingObject*)LK::LeakCheckingObject::malloc(sizeof(LK::LeakCheckingObject));
placementnew(pl, p, "未知", 0, 0);
}*/
return p;
}
inline void* operator new[](size_t s)
{
	if (s == 0)return NULL;
	void* p = LK::LeakCheckingObject::malloc(s);
	/*不做记录
	if (LK::LeakCheckingObject::record())
	{
	LK::LeakCheckingObject* pl = (LK::LeakCheckingObject*)LK::LeakCheckingObject::malloc(sizeof(LK::LeakCheckingObject));
	placementnew(pl, p, "未知", 0, 0);
	}*/
	return p;
}
inline void operator delete(void * p, char const*, int)
{
	if (p == NULL)return;
	LK::LeakCheckingObject::Remove(p);
	LK::LeakCheckingObject::free(p);
}
inline void operator delete[](void * p, char const*, int)
{
	if (p == NULL)return;
	LK::LeakCheckingObject::Remove(p);
	LK::LeakCheckingObject::free(p);
}
inline void operator delete(void * p)
{
	if (p == NULL)return;
	LK::LeakCheckingObject::Remove(p);
	LK::LeakCheckingObject::free(p);
}
inline void operator delete[](void * p)
{
	if (p == NULL)return;
	LK::LeakCheckingObject::Remove(p);
	LK::LeakCheckingObject::free(p);
}
#define new new(__FILE__,__LINE__)
static int* __USELEAKCHECKING_ = (delete new int(), 0);//哎，还是要依赖于初始化顺序
#endif
#endif


//这一段是MFC Debug模式下的内存泄露检测机制，很有用的哈，如果有内存泄露，程序结束时将弹窗
#if USEMFCLEAKCHECK==1
#include <crtdbg.h>
#include <assert.h>
//延时0.5s检测
static void _LEAK_CHECK(void*) { Sleep(500); int i = _CrtDumpMemoryLeaks(); assert(i == 0); }
//注意 要把“代码生成”中的运行时库设置为/MDd或/MTd，否则将出现链接错误
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_CLIENTBLOCK
#if (_M_CEE == 1) ||  (1==_MANAGED) //托管支持
static int __clrcall _Exit_L_()
{
	DWORD dwThreadId;
	HANDLE h = ::CreateThread(
		NULL,//NULL,使用默认安全性
		NULL,//NULL,使用与调用该函数的线程相同的栈空间大小
			 //Thread1,//指定新线程的入口函数名(DWORD WINAPI 声明方法)
		(LPTHREAD_START_ROUTINE)_LEAK_CHECK,//指定新线程的入口函数名(void 声明方法)
		NULL,//向新线程传递的参数，不需要传递时设为NULL
		0,//表示新线程立即开始执行
		&dwThreadId);//保存新线程的ID
					 //int i = _CrtDumpMemoryLeaks();//内存泄露检测结果将在输出的调试页，双击即可找到泄露位置
					 //assert( i == 0);//对于全局的静态对象，可能在该语句后才会被释放，所以提示的内存泄露可能有误
	return 0;
}
static _onexit_m_t _d_e_b_ug = (
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF),//启用内存泄露检测报告
	_onexit_m(&_Exit_L_));//程序结束时检查
#else
static inline void _Exit_L_()//跟下面的atexit();配合使用
{
	DWORD dwThreadId;
	::CreateThread(
		NULL,//NULL,使用默认安全性
		NULL,//NULL,使用与调用该函数的线程相同的栈空间大小
			 //Thread1,//指定新线程的入口函数名(DWORD WINAPI 声明方法)
		(LPTHREAD_START_ROUTINE)_LEAK_CHECK,//指定新线程的入口函数名(void 声明方法)
		0,//向新线程传递的参数，不需要传递时设为NULL
		0,//表示新线程立即开始执行
		&dwThreadId);//保存新线程的ID
					 //int i = _CrtDumpMemoryLeaks();//内存泄露检测结果将在输出的调试页，双击即可找到泄露位置
					 //assert( i == 0);//对于全局的静态对象，可能在该语句后才会被释放，所以提示的内存泄露可能有误
}
static int _d_e_b_ug = (
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF),//启用内存泄露检测报告
																				//_CrtSetBreakAlloc(15),//设置自动断点位置
	atexit(_Exit_L_));//程序结束时检查
#endif
#endif
#endif
