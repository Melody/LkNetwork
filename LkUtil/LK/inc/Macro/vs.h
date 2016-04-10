//编译器相关
//VC
/*vc++ 5.0 　　　　 VS 5.0               1100
vc++ 6.0              VS 6.0              1200
vc++7.0               VS 2003            1310
vc++ 8.0()           VS 2005           1400
vc++9.0()            VS 2008           1500
VC++10.0			VS 2010				1600
VC++11.0			VS 2012				1700
VC++12.0			VS 2013				1800 */

#ifdef _MSC_VER
#define _VC _MSC_VER
#endif

#if _VC!=0
#if _VC<=1700
#define _MRP_CNTM0 16
#define _MRP_CNTM1 15
#define _MRP_CNTM2 14
#define _MRP_CNTM3 13
#else
#define _MRP_CNTM0 32
#define _MRP_CNTM1 31
#define _MRP_CNTM2 30
#define _MRP_CNTM3 29
#endif
#endif
