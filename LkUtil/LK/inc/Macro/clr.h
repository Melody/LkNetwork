#if (_MANAGED == 1) || (_M_CEE == 1) //托管支持
//表示有公共语言运行时支持
#define _CLR 1
#include "gcroot.h"
#endif