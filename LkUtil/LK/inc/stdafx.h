#include "Macro/stdafx.h"
#include "std/stdafx.h"
#include "LK/stdafx.h"
#include "TemplateDefine/stdafx.h"
//语法特性宏
#ifdef _HAS_DECLTYPE//支持decltype
#endif
#ifdef _HAS_EXCEPTION_POINTERS//支持std::exception_ptr
#endif
#ifdef _HAS_FUNCTION_DELETE	//支持定义成员函数删除
#endif
#ifdef _HAS_INITIALIZER_LISTS//支持初始化列表
#endif
#ifdef _HAS_NULLPTR_T	//支持std::nullptr_t
#endif
#ifdef _HAS_RVALUE_REFERENCES	//支持右值引用
#endif
#ifdef _HAS_SCOPED_ENUM//支持class 内部enum
#endif
#ifdef _HAS_TEMPLATE_ALIAS//支持模板转义：template <typename T>using MyAllocVec = std::vector<T, MyAllocator>;
#endif
#ifdef _HAS_TEMPLATE_TEMPLATE//支持模板作为模板参数
#endif
#ifdef _HAS_VARIADIC_TEMPLATES//支持可变参模板
#endif
