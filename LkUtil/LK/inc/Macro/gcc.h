/*
MS VC++ 10.0 _MSC_VER = 1600（VS2010）
MS VC++ 9.0 _MSC_VER = 1500
MS VC++ 8.0 _MSC_VER = 1400
MS VC++ 7.1 _MSC_VER = 1310
MS VC++ 7.0 _MSC_VER = 1300
MS VC++ 6.0 _MSC_VER = 1200
MS VC++ 5.0 _MSC_VER = 1100
*/
#ifdef __GNUC__
#define _GCC __GNUC__
#endif
#ifdef _GCC
#define _HAS_RVALUE_REFERENCES 1//右值引用
#define SUPPORT_VARIADIC_TEMPLATES 1//启用可变模版参数
#define UPPORT_DECLTYPE 1
#define _MRP_CNTM0 32//宏的重复次数
#define _MRP_CNTM1 31//宏的重复次数-1
#define _MRP_CNTM2 30//宏的重复次数-2
#define _MRP_CNTM3 29//宏的重复次数-3
#endif
