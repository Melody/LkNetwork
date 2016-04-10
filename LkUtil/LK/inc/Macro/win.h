
#ifdef _WIN32
#ifndef _WINDOWS
//在Windows环境中
#define _WINDOWS 1
#endif
#endif
#ifdef _WIN32_WINNT
#ifndef _WINDOWS
#define _WINDOWS 1
#endif
#endif
#ifdef WIN32
#ifndef _WINDOWS
#define _WINDOWS 1
#endif
#endif



#ifdef _WIN64
#ifndef _X64
//64位环境
#define _X64 1
#endif
#endif


#ifdef _WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif//这里的windows.h排除WinSock旧版本
#if _WIN32_WINNT<0x501
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x501//重定义Windows版本
#endif
#include <windows.h>
#endif