#pragma once
/**@author W意波*/
/**
@brief 表示无内容*/
#define M_NotThing
/**
@brief 表示一个逗号*/
#define M_Comma ,
/**
@brief 表示在before和after之间插入一个空格*/
#define MF2_InsertSpace(before,after) before  after
/**
@brief 插入一个逗号*/
#define MF2_InsertComma(before,after) before , after
/**
@brief 表示一个左括号*/
#define MF_LeftBracket() (
/**
@brief 表示一个右括号*/
#define MF_RightBracket() )

#define _MF2_Concat(a1 , a2) a1##a2
/**
@brief 表示串接两个内容宏展开后的内容*/
#define MF2_Concat(a1 , a2) _MF2_Concat(a1 , a2)

#define _MF3_Concat(a1 , a2, a3) a1##a2##a3
/**
@brief 表示串接3个内容宏展开后的内容*/
#define MF3_Concat(a1 , a2, a3) _MF3_Concat(a1 , a2,a3)

#define _MF4_Concat(a1 , a2, a3, a4) a1##a2##a3##a4
/**
@brief 表示串接4个内容宏展开后的内容*/
#define MF4_Concat(a1 , a2, a3, a4) _MF4_Concat(a1,a2,a3,a4)

#define _MF5_Concat(a1 , a2, a3, a4, a5) a1##a2##a3##a4##a5
/**
@brief 表示串接5个内容宏展开后的内容*/
#define MF5_Concat(a1 , a2, a3, a4, a5) _MF5_Concat(a1,a2,a3,a4,a5)

#define _MF6_Concat(a1 , a2, a3, a4, a5, a6) a1##a2##a3##a4##a5##a6
/**
@brief 表示串接6个内容宏展开后的内容*/
#define MF6_Concat(a1 , a2, a3, a4, a5, a6) _MF6_Concat(a1,a2,a3,a4,a5,a6)

#define _MF7_Concat(a1 , a2, a3, a4, a5, a6, a7) a1##a2##a3##a4##a5##a6##a7
/**
@brief 表示串接7个内容宏展开后的内容*/
#define MF7_Concat(a1 , a2, a3, a4, a5, a6, a7) _MF7_Concat(a1,a2,a3,a4,a5,a6,a7)

#define _MF8_Concat(a1 , a2, a3, a4, a5, a6, a7, a8) a1##a2##a3##a4##a5##a6##a7##a8
/**
@brief 表示串接8个内容宏展开后的内容*/
#define MF8_Concat(a1 , a2, a3, a4, a5, a6, a7, a8) _MF8_Concat(a1,a2,a3,a4,a5,a6,a7,a8)

#define _MF9_Concat(a1 , a2, a3, a4, a5, a6, a7, a8, a9) a1##a2##a3##a4##a5##a6##a7##a8##a9
/**
@brief 表示串接9个内容宏展开后的内容*/
#define MF9_Concat(a1 , a2, a3, a4, a5, a6, a7, a8, a9) _MF9_Concat(a1,a2,a3,a4,a5,a6,a7,a8,a9)

#define _MF1_ToString(str) #str
/**
@brief 宏展开后的内容转换为字符串表示*/
#define MF1_ToString(str) _MF1_ToString(str)

#ifdef _VC
#define _MF1_ToChar(ch) #@ch
/**
@brief 宏展开后的内容转换为字符*/
#define MF1_ToChar(ch) _MF1_ToChar(ch)
#endif

#define _MF1_Self(str) str
/**
@brief 表示内容宏展开后的内容*/
#define MF1_Self(str) _MF1_Self(str)

#define MF_Self(b)
#define MF0_Self(b)
#define _MF2_UseIf(a,b) MF##a##_Self(b)
/**
@brief 当a为1时表示内容b,0或者空时表示空*/
#define MF2_UseIf(a,b) _MF2_UseIf(a,b)

#define MF1__CT(x) ,class T##x
#define MF1_CT(x) class T##x
#define MF1__T(x) ,T##x
#define MF1_T(x) T##x
#define MF1__Tt(x) ,T##x t##x
#define MF1_Tt(x) T##x t##x
#define MF1__t(x) ,t##x
#define MF1_t(x) t##x

#define MF1__CTT(x) ,class TT##x
#define MF1_CTT(x) class TT##x
#define MF1__TT(x) ,TT##x
#define MF1_TT(x) TT##x
#define MF1__TTtt(x) ,TT##x tt##x
#define MF1_TTtt(x) TT##x tt##x
#define MF1__tt(x) ,tt##x
#define MF1_tt(x) tt##x
#define MF_0_() _
#define MF_1_() 0
#define MF_2_() 1
#define MF_3_() 2
#define MF_4_() 3
#define MF_5_() 4
#define MF_6_() 5
#define MF_7_() 6
#define MF_8_() 7
#define MF_9_() 8
#define MF_10_() 9
#define MF_11_() 10
#define MF_12_() 11
#define MF_13_() 12
#define MF_14_() 13
#define MF_15_() 14
#define MF_16_() 15
#define MF_17_() 16
#define MF_18_() 17
#define MF_19_() 18
#define MF_20_() 19
#define MF_21_() 20
#define MF_22_() 21
#define MF_23_() 22
#define MF_24_() 23
#define MF_25_() 24
#define MF_26_() 25
#define MF_27_() 26
#define MF_28_() 27
#define MF_29_() 28
#define MF_30_() 29
#define MF_31_() 30
#define MF_32_() 31
#define MF_33_() 32
#if _MRP_CNTM0>=32
#define MF_34_() 33
#define MF_35_() 34
#define MF_36_() 35
#define MF_37_() 36
#define MF_38_() 37
#define MF_39_() 38
#define MF_40_() 39
#define MF_41_() 40
#define MF_42_() 41
#define MF_43_() 42
#define MF_44_() 43
#define MF_45_() 44
#define MF_46_() 45
#define MF_47_() 46
#define MF_48_() 47
#define MF_49_() 48
#define MF_50_() 49
#define MF_51_() 50
#define MF_52_() 51
#define MF_53_() 52
#define MF_54_() 53
#define MF_55_() 54
#define MF_56_() 55
#define MF_57_() 56
#define MF_58_() 57
#define MF_59_() 58
#define MF_60_() 59
#define MF_61_() 60
#define MF_62_() 61
#define MF_63_() 62
#define MF_64_() 63
#define MF_65_() 64
#endif
//表示某个数字本身+1（0~65）
#define MF___1() 0
#define MF_0_1() 1
#define MF_1_1() 2
#define MF_2_1() 3
#define MF_3_1() 4
#define MF_4_1() 5
#define MF_5_1() 6
#define MF_6_1() 7
#define MF_7_1() 8
#define MF_8_1() 9
#define MF_9_1() 10
#define MF_10_1() 11
#define MF_11_1() 12
#define MF_12_1() 13
#define MF_13_1() 14
#define MF_14_1() 15
#define MF_15_1() 16
#define MF_16_1() 17
#define MF_17_1() 18
#define MF_18_1() 19
#define MF_19_1() 20
#define MF_20_1() 21
#define MF_21_1() 22
#define MF_22_1() 23
#define MF_23_1() 24
#define MF_24_1() 25
#define MF_25_1() 26
#define MF_26_1() 27
#define MF_27_1() 28
#define MF_28_1() 29
#define MF_29_1() 30
#define MF_30_1() 31
#define MF_31_1() 32
#if _MRP_CNTM0>=32
#define MF_32_1() 33
#define MF_33_1() 34
#define MF_34_1() 35
#define MF_35_1() 36
#define MF_36_1() 37
#define MF_37_1() 38
#define MF_38_1() 39
#define MF_39_1() 40
#define MF_40_1() 41
#define MF_41_1() 42
#define MF_42_1() 43
#define MF_43_1() 44
#define MF_44_1() 45
#define MF_45_1() 46
#define MF_46_1() 47
#define MF_47_1() 48
#define MF_48_1() 49
#define MF_49_1() 50
#define MF_50_1() 51
#define MF_51_1() 52
#define MF_52_1() 53
#define MF_53_1() 54
#define MF_54_1() 55
#define MF_55_1() 56
#define MF_56_1() 57
#define MF_57_1() 58
#define MF_58_1() 59
#define MF_59_1() 60
#define MF_60_1() 61
#define MF_61_1() 62
#define MF_62_1() 63
#define MF_63_1() 64
#define MF_64_1() 65
#endif

#define MF1_aMs_(a) _
#define MF1_aMs0(a) a
#define _MF1_aMs1(a) MF_##a##_()
#define MF1_aMs1(a) _MF1_aMs1(a)

#define MF1_aMs2(a) MF1_aMs1(MF1_aMs1(a))
#define MF1_aMs3(a) MF1_aMs2(MF1_aMs1(a))
#define MF1_aMs4(a) MF1_aMs2(MF1_aMs2(a))
#define MF1_aMs5(a) MF1_aMs4(MF1_aMs1(a))
#define MF1_aMs6(a) MF1_aMs4(MF1_aMs2(a))
#define MF1_aMs7(a) MF1_aMs4(MF1_aMs3(a))
#define MF1_aMs8(a) MF1_aMs4(MF1_aMs4(a))
#define MF1_aMs9(a) MF1_aMs8(MF1_aMs1(a))
#define MF1_aMs10(a) MF1_aMs8(MF1_aMs2(a))
#define MF1_aMs11(a) MF1_aMs8(MF1_aMs3(a))
#define MF1_aMs12(a) MF1_aMs8(MF1_aMs4(a))
#define MF1_aMs13(a) MF1_aMs8(MF1_aMs5(a))
#define MF1_aMs14(a) MF1_aMs8(MF1_aMs6(a))
#define MF1_aMs15(a) MF1_aMs8(MF1_aMs7(a))
#define MF1_aMs16(a) MF1_aMs8(MF1_aMs8(a))
#define MF1_aMs17(a) MF1_aMs16(MF1_aMs1(a))
#define MF1_aMs18(a) MF1_aMs16(MF1_aMs2(a))
#define MF1_aMs19(a) MF1_aMs16(MF1_aMs3(a))
#define MF1_aMs20(a) MF1_aMs16(MF1_aMs4(a))
#define MF1_aMs21(a) MF1_aMs16(MF1_aMs5(a))
#define MF1_aMs22(a) MF1_aMs16(MF1_aMs6(a))
#define MF1_aMs23(a) MF1_aMs16(MF1_aMs7(a))
#define MF1_aMs24(a) MF1_aMs16(MF1_aMs8(a))
#define MF1_aMs25(a) MF1_aMs16(MF1_aMs9(a))
#define MF1_aMs26(a) MF1_aMs16(MF1_aMs10(a))
#define MF1_aMs27(a) MF1_aMs16(MF1_aMs11(a))
#define MF1_aMs28(a) MF1_aMs16(MF1_aMs12(a))
#define MF1_aMs29(a) MF1_aMs16(MF1_aMs13(a))
#define MF1_aMs30(a) MF1_aMs16(MF1_aMs14(a))
#define MF1_aMs31(a) MF1_aMs16(MF1_aMs15(a))
#if _MRP_CNTM0>=32
#define MF1_aMs32(a) MF1_aMs16(MF1_aMs16(a))
#define MF1_aMs33(a) MF1_aMs32(MF1_aMs1(a))
#define MF1_aMs34(a) MF1_aMs32(MF1_aMs2(a))
#define MF1_aMs35(a) MF1_aMs32(MF1_aMs3(a))
#define MF1_aMs36(a) MF1_aMs32(MF1_aMs4(a))
#define MF1_aMs37(a) MF1_aMs32(MF1_aMs5(a))
#define MF1_aMs38(a) MF1_aMs32(MF1_aMs6(a))
#define MF1_aMs39(a) MF1_aMs32(MF1_aMs7(a))
#define MF1_aMs40(a) MF1_aMs32(MF1_aMs8(a))
#define MF1_aMs41(a) MF1_aMs32(MF1_aMs9(a))
#define MF1_aMs42(a) MF1_aMs32(MF1_aMs10(a))
#define MF1_aMs43(a) MF1_aMs32(MF1_aMs11(a))
#define MF1_aMs44(a) MF1_aMs32(MF1_aMs12(a))
#define MF1_aMs45(a) MF1_aMs32(MF1_aMs13(a))
#define MF1_aMs46(a) MF1_aMs32(MF1_aMs14(a))
#define MF1_aMs47(a) MF1_aMs32(MF1_aMs15(a))
#define MF1_aMs48(a) MF1_aMs32(MF1_aMs16(a))
#define MF1_aMs49(a) MF1_aMs32(MF1_aMs17(a))
#define MF1_aMs50(a) MF1_aMs32(MF1_aMs18(a))
#define MF1_aMs51(a) MF1_aMs32(MF1_aMs19(a))
#define MF1_aMs52(a) MF1_aMs32(MF1_aMs20(a))
#define MF1_aMs53(a) MF1_aMs32(MF1_aMs21(a))
#define MF1_aMs54(a) MF1_aMs32(MF1_aMs22(a))
#define MF1_aMs55(a) MF1_aMs32(MF1_aMs23(a))
#define MF1_aMs56(a) MF1_aMs32(MF1_aMs24(a))
#define MF1_aMs57(a) MF1_aMs32(MF1_aMs25(a))
#define MF1_aMs58(a) MF1_aMs32(MF1_aMs26(a))
#define MF1_aMs59(a) MF1_aMs32(MF1_aMs27(a))
#define MF1_aMs60(a) MF1_aMs32(MF1_aMs28(a))
#define MF1_aMs61(a) MF1_aMs32(MF1_aMs29(a))
#define MF1_aMs62(a) MF1_aMs32(MF1_aMs30(a))
#define MF1_aMs63(a) MF1_aMs32(MF1_aMs31(a))
#define MF1_aMs64(a) MF1_aMs32(MF1_aMs32(a))
#endif

#define MF1_aPs_(a) _
#define MF1_aPs0(a) a
#define _MF1_aPs1(a) MF_##a##_1()
#define MF1_aPs1(a) _MF1_aPs1(a)

#define MF1_aPs2(a) MF1_aPs1(MF1_aPs1(a))
#define MF1_aPs3(a) MF1_aPs2(MF1_aPs1(a))
#define MF1_aPs4(a) MF1_aPs2(MF1_aPs2(a))
#define MF1_aPs5(a) MF1_aPs4(MF1_aPs1(a))
#define MF1_aPs6(a) MF1_aPs4(MF1_aPs2(a))
#define MF1_aPs7(a) MF1_aPs4(MF1_aPs3(a))
#define MF1_aPs8(a) MF1_aPs4(MF1_aPs4(a))
#define MF1_aPs9(a) MF1_aPs8(MF1_aPs1(a))
#define MF1_aPs10(a) MF1_aPs8(MF1_aPs2(a))
#define MF1_aPs11(a) MF1_aPs8(MF1_aPs3(a))
#define MF1_aPs12(a) MF1_aPs8(MF1_aPs4(a))
#define MF1_aPs13(a) MF1_aPs8(MF1_aPs5(a))
#define MF1_aPs14(a) MF1_aPs8(MF1_aPs6(a))
#define MF1_aPs15(a) MF1_aPs8(MF1_aPs7(a))
#define MF1_aPs16(a) MF1_aPs8(MF1_aPs8(a))
#define MF1_aPs17(a) MF1_aPs16(MF1_aPs1(a))
#define MF1_aPs18(a) MF1_aPs16(MF1_aPs2(a))
#define MF1_aPs19(a) MF1_aPs16(MF1_aPs3(a))
#define MF1_aPs20(a) MF1_aPs16(MF1_aPs4(a))
#define MF1_aPs21(a) MF1_aPs16(MF1_aPs5(a))
#define MF1_aPs22(a) MF1_aPs16(MF1_aPs6(a))
#define MF1_aPs23(a) MF1_aPs16(MF1_aPs7(a))
#define MF1_aPs24(a) MF1_aPs16(MF1_aPs8(a))
#define MF1_aPs25(a) MF1_aPs16(MF1_aPs9(a))
#define MF1_aPs26(a) MF1_aPs16(MF1_aPs10(a))
#define MF1_aPs27(a) MF1_aPs16(MF1_aPs11(a))
#define MF1_aPs28(a) MF1_aPs16(MF1_aPs12(a))
#define MF1_aPs29(a) MF1_aPs16(MF1_aPs13(a))
#define MF1_aPs30(a) MF1_aPs16(MF1_aPs14(a))
#define MF1_aPs31(a) MF1_aPs16(MF1_aPs15(a))
#define MF1_aPs32(a) MF1_aPs16(MF1_aPs16(a))
#if _MRP_CNTM0>=32
#define MF1_aPs33(a) MF1_aPs32(MF1_aPs1(a))
#define MF1_aPs34(a) MF1_aPs32(MF1_aPs2(a))
#define MF1_aPs35(a) MF1_aPs32(MF1_aPs3(a))
#define MF1_aPs36(a) MF1_aPs32(MF1_aPs4(a))
#define MF1_aPs37(a) MF1_aPs32(MF1_aPs5(a))
#define MF1_aPs38(a) MF1_aPs32(MF1_aPs6(a))
#define MF1_aPs39(a) MF1_aPs32(MF1_aPs7(a))
#define MF1_aPs40(a) MF1_aPs32(MF1_aPs8(a))
#define MF1_aPs41(a) MF1_aPs32(MF1_aPs9(a))
#define MF1_aPs42(a) MF1_aPs32(MF1_aPs10(a))
#define MF1_aPs43(a) MF1_aPs32(MF1_aPs11(a))
#define MF1_aPs44(a) MF1_aPs32(MF1_aPs12(a))
#define MF1_aPs45(a) MF1_aPs32(MF1_aPs13(a))
#define MF1_aPs46(a) MF1_aPs32(MF1_aPs14(a))
#define MF1_aPs47(a) MF1_aPs32(MF1_aPs15(a))
#define MF1_aPs48(a) MF1_aPs32(MF1_aPs16(a))
#define MF1_aPs49(a) MF1_aPs32(MF1_aPs17(a))
#define MF1_aPs50(a) MF1_aPs32(MF1_aPs18(a))
#define MF1_aPs51(a) MF1_aPs32(MF1_aPs19(a))
#define MF1_aPs52(a) MF1_aPs32(MF1_aPs20(a))
#define MF1_aPs53(a) MF1_aPs32(MF1_aPs21(a))
#define MF1_aPs54(a) MF1_aPs32(MF1_aPs22(a))
#define MF1_aPs55(a) MF1_aPs32(MF1_aPs23(a))
#define MF1_aPs56(a) MF1_aPs32(MF1_aPs24(a))
#define MF1_aPs57(a) MF1_aPs32(MF1_aPs25(a))
#define MF1_aPs58(a) MF1_aPs32(MF1_aPs26(a))
#define MF1_aPs59(a) MF1_aPs32(MF1_aPs27(a))
#define MF1_aPs60(a) MF1_aPs32(MF1_aPs28(a))
#define MF1_aPs61(a) MF1_aPs32(MF1_aPs29(a))
#define MF1_aPs62(a) MF1_aPs32(MF1_aPs30(a))
#define MF1_aPs63(a) MF1_aPs32(MF1_aPs31(a))
#define MF1_aPs64(a) MF1_aPs32(MF1_aPs32(a))
#endif

/**
@brief 表示num1-num2所得数值文本，num1、num2、结果 三者都要在0~64范围内*/
#define MF2_Minus(num1 , num2) MF1_aMs##num2 (num1)

/**
@brief 表示num1+num2所得数值文本，num1、num2、结果 三者都要在0~64范围内*/
#define MF2_Plus(num1 , num2) MF1_aPs##num2 (num1)

#define _MR0(a)
#define _MR1(a) a
#define _MR2(a) _MR1(a)##a
#define _MR3(a) _MR2(a)##a
#define _MR4(a) _MR3(a)##a
#define _MR5(a) _MR4(a)##a
#define _MR6(a) _MR5(a)##a
#define _MR7(a) _MR6(a)##a
#define _MR8(a) _MR7(a)##a
#define _MR9(a) _MR8(a)##a
#define _MR10(a) _MR9(a)##a
#define _MR11(a) _MR10(a)##a
#define _MR12(a) _MR11(a)##a
#define _MR13(a) _MR12(a)##a
#define _MR14(a) _MR13(a)##a
#define _MR15(a) _MR14(a)##a
#define _MR16(a) _MR15(a)##a
#define _MR17(a) _MR16(a)##a
#define _MR18(a) _MR17(a)##a
#define _MR19(a) _MR18(a)##a
#define _MR20(a) _MR19(a)##a
#define _MR21(a) _MR20(a)##a
#define _MR22(a) _MR21(a)##a
#define _MR23(a) _MR22(a)##a
#define _MR24(a) _MR23(a)##a
#define _MR25(a) _MR24(a)##a
#define _MR26(a) _MR25(a)##a
#define _MR27(a) _MR26(a)##a
#define _MR28(a) _MR27(a)##a
#define _MR29(a) _MR28(a)##a
#define _MR30(a) _MR29(a)##a
#define _MR31(a) _MR30(a)##a
#define _MR32(a) _MR31(a)##a
#if _MRP_CNTM0>=32
#define _MR33(a) _MR32(a)##a
#define _MR34(a) _MR33(a)##a
#define _MR35(a) _MR34(a)##a
#define _MR36(a) _MR35(a)##a
#define _MR37(a) _MR36(a)##a
#define _MR38(a) _MR37(a)##a
#define _MR39(a) _MR38(a)##a
#define _MR40(a) _MR39(a)##a
#define _MR41(a) _MR40(a)##a
#define _MR42(a) _MR41(a)##a
#define _MR43(a) _MR42(a)##a
#define _MR44(a) _MR43(a)##a
#define _MR45(a) _MR44(a)##a
#define _MR46(a) _MR45(a)##a
#define _MR47(a) _MR46(a)##a
#define _MR48(a) _MR47(a)##a
#define _MR49(a) _MR48(a)##a
#define _MR50(a) _MR49(a)##a
#define _MR51(a) _MR50(a)##a
#define _MR52(a) _MR51(a)##a
#define _MR53(a) _MR52(a)##a
#define _MR54(a) _MR53(a)##a
#define _MR55(a) _MR54(a)##a
#define _MR56(a) _MR55(a)##a
#define _MR57(a) _MR56(a)##a
#define _MR58(a) _MR57(a)##a
#define _MR59(a) _MR58(a)##a
#define _MR60(a) _MR59(a)##a
#define _MR61(a) _MR60(a)##a
#define _MR62(a) _MR61(a)##a
#define _MR63(a) _MR62(a)##a
#define _MR64(a) _MR63(a)##a
#define _MR65(a) _MR64(a)##a
#endif

#define _MR0_(a)
#define _MR1_(a) a
#define _MR2_(a) _MR1_(a),a
#define _MR3_(a) _MR2_(a),a
#define _MR4_(a) _MR3_(a),a
#define _MR5_(a) _MR4_(a),a
#define _MR6_(a) _MR5_(a),a
#define _MR7_(a) _MR6_(a),a
#define _MR8_(a) _MR7_(a),a
#define _MR9_(a) _MR8_(a),a
#define _MR10_(a) _MR9_(a),a
#define _MR11_(a) _MR10_(a),a
#define _MR12_(a) _MR11_(a),a
#define _MR13_(a) _MR12_(a),a
#define _MR14_(a) _MR13_(a),a
#define _MR15_(a) _MR14_(a),a
#define _MR16_(a) _MR15_(a),a
#define _MR17_(a) _MR16_(a),a
#define _MR18_(a) _MR17_(a),a
#define _MR19_(a) _MR18_(a),a
#define _MR20_(a) _MR19_(a),a
#define _MR21_(a) _MR20_(a),a
#define _MR22_(a) _MR21_(a),a
#define _MR23_(a) _MR22_(a),a
#define _MR24_(a) _MR23_(a),a
#define _MR25_(a) _MR24_(a),a
#define _MR26_(a) _MR25_(a),a
#define _MR27_(a) _MR26_(a),a
#define _MR28_(a) _MR27_(a),a
#define _MR29_(a) _MR28_(a),a
#define _MR30_(a) _MR29_(a),a
#define _MR31_(a) _MR30_(a),a
#define _MR32_(a) _MR31_(a),a
#if _MRP_CNTM0>=32
#define _MR33_(a) _MR32_(a),a
#define _MR34_(a) _MR33_(a),a
#define _MR35_(a) _MR34_(a),a
#define _MR36_(a) _MR35_(a),a
#define _MR37_(a) _MR36_(a),a
#define _MR38_(a) _MR37_(a),a
#define _MR39_(a) _MR38_(a),a
#define _MR40_(a) _MR39_(a),a
#define _MR41_(a) _MR40_(a),a
#define _MR42_(a) _MR41_(a),a
#define _MR43_(a) _MR42_(a),a
#define _MR44_(a) _MR43_(a),a
#define _MR45_(a) _MR44_(a),a
#define _MR46_(a) _MR45_(a),a
#define _MR47_(a) _MR46_(a),a
#define _MR48_(a) _MR47_(a),a
#define _MR49_(a) _MR48_(a),a
#define _MR50_(a) _MR49_(a),a
#define _MR51_(a) _MR50_(a),a
#define _MR52_(a) _MR51_(a),a
#define _MR53_(a) _MR52_(a),a
#define _MR54_(a) _MR53_(a),a
#define _MR55_(a) _MR54_(a),a
#define _MR56_(a) _MR55_(a),a
#define _MR57_(a) _MR56_(a),a
#define _MR58_(a) _MR57_(a),a
#define _MR59_(a) _MR58_(a),a
#define _MR60_(a) _MR59_(a),a
#define _MR61_(a) _MR60_(a),a
#define _MR62_(a) _MR61_(a),a
#define _MR63_(a) _MR62_(a),a
#define _MR64_(a) _MR63_(a),a
#define _MR65_(a) _MR64_(a),a
#endif

#define _MR0_b(a)
#define _MR1_b(a) a(1)
#define _MR2_b(a) _MR1_b(a),a(2)
#define _MR3_b(a) _MR2_b(a),a(3)
#define _MR4_b(a) _MR3_b(a),a(4)
#define _MR5_b(a) _MR4_b(a),a(5)
#define _MR6_b(a) _MR5_b(a),a(6)
#define _MR7_b(a) _MR6_b(a),a(7)
#define _MR8_b(a) _MR7_b(a),a(8)
#define _MR9_b(a) _MR8_b(a),a(9)
#define _MR10_b(a) _MR9_b(a),a(10)
#define _MR11_b(a) _MR10_b(a),a(11)
#define _MR12_b(a) _MR11_b(a),a(12)
#define _MR13_b(a) _MR12_b(a),a(13)
#define _MR14_b(a) _MR13_b(a),a(14)
#define _MR15_b(a) _MR14_b(a),a(15)
#define _MR16_b(a) _MR15_b(a),a(16)
#define _MR17_b(a) _MR16_b(a),a(17)
#define _MR18_b(a) _MR17_b(a),a(18)
#define _MR19_b(a) _MR18_b(a),a(19)
#define _MR20_b(a) _MR19_b(a),a(20)
#define _MR21_b(a) _MR20_b(a),a(21)
#define _MR22_b(a) _MR21_b(a),a(22)
#define _MR23_b(a) _MR22_b(a),a(23)
#define _MR24_b(a) _MR23_b(a),a(24)
#define _MR25_b(a) _MR24_b(a),a(25)
#define _MR26_b(a) _MR25_b(a),a(26)
#define _MR27_b(a) _MR26_b(a),a(27)
#define _MR28_b(a) _MR27_b(a),a(28)
#define _MR29_b(a) _MR28_b(a),a(29)
#define _MR30_b(a) _MR29_b(a),a(30)
#define _MR31_b(a) _MR30_b(a),a(31)
#define _MR32_b(a) _MR31_b(a),a(32)
#if _MRP_CNTM0>=32
#define _MR33_b(a) _MR32_b(a),a(33)
#define _MR34_b(a) _MR33_b(a),a(34)
#define _MR35_b(a) _MR34_b(a),a(35)
#define _MR36_b(a) _MR35_b(a),a(36)
#define _MR37_b(a) _MR36_b(a),a(37)
#define _MR38_b(a) _MR37_b(a),a(38)
#define _MR39_b(a) _MR38_b(a),a(39)
#define _MR40_b(a) _MR39_b(a),a(40)
#define _MR41_b(a) _MR40_b(a),a(41)
#define _MR42_b(a) _MR41_b(a),a(42)
#define _MR43_b(a) _MR42_b(a),a(43)
#define _MR44_b(a) _MR43_b(a),a(44)
#define _MR45_b(a) _MR44_b(a),a(45)
#define _MR46_b(a) _MR45_b(a),a(46)
#define _MR47_b(a) _MR46_b(a),a(47)
#define _MR48_b(a) _MR47_b(a),a(48)
#define _MR49_b(a) _MR48_b(a),a(49)
#define _MR50_b(a) _MR49_b(a),a(50)
#define _MR51_b(a) _MR50_b(a),a(51)
#define _MR52_b(a) _MR51_b(a),a(52)
#define _MR53_b(a) _MR52_b(a),a(53)
#define _MR54_b(a) _MR53_b(a),a(54)
#define _MR55_b(a) _MR54_b(a),a(55)
#define _MR56_b(a) _MR55_b(a),a(56)
#define _MR57_b(a) _MR56_b(a),a(57)
#define _MR58_b(a) _MR57_b(a),a(58)
#define _MR59_b(a) _MR58_b(a),a(59)
#define _MR60_b(a) _MR59_b(a),a(60)
#define _MR61_b(a) _MR60_b(a),a(61)
#define _MR62_b(a) _MR61_b(a),a(62)
#define _MR63_b(a) _MR62_b(a),a(63)
#define _MR64_b(a) _MR63_b(a),a(64)
#define _MR65_b(a) _MR64_b(a),a(65)
#endif

#define _MR0b(a)
#define _MR1b(a) a(1)
#define _MR2b(a) _MR1b(a)a(2)
#define _MR3b(a) _MR2b(a)a(3)
#define _MR4b(a) _MR3b(a)a(4)
#define _MR5b(a) _MR4b(a)a(5)
#define _MR6b(a) _MR5b(a)a(6)
#define _MR7b(a) _MR6b(a)a(7)
#define _MR8b(a) _MR7b(a)a(8)
#define _MR9b(a) _MR8b(a)a(9)
#define _MR10b(a) _MR9b(a)a(10)
#define _MR11b(a) _MR10b(a)a(11)
#define _MR12b(a) _MR11b(a)a(12)
#define _MR13b(a) _MR12b(a)a(13)
#define _MR14b(a) _MR13b(a)a(14)
#define _MR15b(a) _MR14b(a)a(15)
#define _MR16b(a) _MR15b(a)a(16)
#define _MR17b(a) _MR16b(a)a(17)
#define _MR18b(a) _MR17b(a)a(18)
#define _MR19b(a) _MR18b(a)a(19)
#define _MR20b(a) _MR19b(a)a(20)
#define _MR21b(a) _MR20b(a)a(21)
#define _MR22b(a) _MR21b(a)a(22)
#define _MR23b(a) _MR22b(a)a(23)
#define _MR24b(a) _MR23b(a)a(24)
#define _MR25b(a) _MR24b(a)a(25)
#define _MR26b(a) _MR25b(a)a(26)
#define _MR27b(a) _MR26b(a)a(27)
#define _MR28b(a) _MR27b(a)a(28)
#define _MR29b(a) _MR28b(a)a(29)
#define _MR30b(a) _MR29b(a)a(30)
#define _MR31b(a) _MR30b(a)a(31)
#define _MR32b(a) _MR31b(a)a(32)
#if _MRP_CNTM0>=32
#define _MR33b(a) _MR32b(a)a(33)
#define _MR34b(a) _MR33b(a)a(34)
#define _MR35b(a) _MR34b(a)a(35)
#define _MR36b(a) _MR35b(a)a(36)
#define _MR37b(a) _MR36b(a)a(37)
#define _MR38b(a) _MR37b(a)a(38)
#define _MR39b(a) _MR38b(a)a(39)
#define _MR40b(a) _MR39b(a)a(40)
#define _MR41b(a) _MR40b(a)a(41)
#define _MR42b(a) _MR41b(a)a(42)
#define _MR43b(a) _MR42b(a)a(43)
#define _MR44b(a) _MR43b(a)a(44)
#define _MR45b(a) _MR44b(a)a(45)
#define _MR46b(a) _MR45b(a)a(46)
#define _MR47b(a) _MR46b(a)a(47)
#define _MR48b(a) _MR47b(a)a(48)
#define _MR49b(a) _MR48b(a)a(49)
#define _MR50b(a) _MR49b(a)a(50)
#define _MR51b(a) _MR50b(a)a(51)
#define _MR52b(a) _MR51b(a)a(52)
#define _MR53b(a) _MR52b(a)a(53)
#define _MR54b(a) _MR53b(a)a(54)
#define _MR55b(a) _MR54b(a)a(55)
#define _MR56b(a) _MR55b(a)a(56)
#define _MR57b(a) _MR56b(a)a(57)
#define _MR58b(a) _MR57b(a)a(58)
#define _MR59b(a) _MR58b(a)a(59)
#define _MR60b(a) _MR59b(a)a(60)
#define _MR61b(a) _MR60b(a)a(61)
#define _MR62b(a) _MR61b(a)a(62)
#define _MR63b(a) _MR62b(a)a(63)
#define _MR64b(a) _MR63b(a)a(64)
#define _MR65b(a) _MR64b(a)a(65)
#endif

#define _MF2_Repeat(text , num) _MR##num (text)
/**
@brief 表示text内容重复连接num次。num最大不能超过65*/
#define MF2_Repeat(text , num) _MF2_Repeat(text , num)
#define _MF2_Repeat_(text , num) _MR##num##_(text)
/**
@brief 表示text内容重复num次，项目之间用,间隔。num最大不能超过65*/
#define MF2_Repeat_(text , num) _MF2_Repeat_(text , num)
#define _MF2_Repeatb(text , num) _MR ## num ## b(text)
/**
@brief 表示text内容重复num次,而且每项后面附加(n)，n为1~num。num最大不能超过65*/
#define MF2_Repeatb(text , num) _MF2_Repeatb(text , num)
#define _MF2_Repeat_b(text , num) _MR ## num ## _b(text)
/**
@brief 表示text内容重复num次,而且每项后面附加(n)，n为1~num，项目之间用,间隔。num最大不能超过65*/
#define MF2_Repeat_b(text , num) _MF2_Repeat_b(text , num)


//升序系列
#define _MA_(b,a,s)
#define _MA0(b,a,s) MF3_Concat(b, s,a)
#define _MA1(b,a,s) MF4_Concat(_MA0(b,a,s),b, MF2_Plus(s,1),a)
#define _MA2(b,a,s) MF4_Concat(_MA1(b,a,s),b, MF2_Plus(s,2),a)
#define _MA3(b,a,s) MF4_Concat(_MA2(b,a,s),b, MF2_Plus(s,3),a)
#define _MA4(b,a,s) MF4_Concat(_MA3(b,a,s),b, MF2_Plus(s,4),a)
#define _MA5(b,a,s) MF4_Concat(_MA4(b,a,s),b, MF2_Plus(s,5),a)
#define _MA6(b,a,s) MF4_Concat(_MA5(b,a,s),b, MF2_Plus(s,6),a)
#define _MA7(b,a,s) MF4_Concat(_MA6(b,a,s),b, MF2_Plus(s,7),a)
#define _MA8(b,a,s) MF4_Concat(_MA7(b,a,s),b, MF2_Plus(s,8),a)
#define _MA9(b,a,s) MF4_Concat(_MA8(b,a,s),b, MF2_Plus(s,9),a)
#define _MA10(b,a,s) MF4_Concat(_MA9(b,a,s),b, MF2_Plus(s,10),a)
#define _MA11(b,a,s) MF4_Concat(_MA10(b,a,s),b, MF2_Plus(s,11),a)
#define _MA12(b,a,s) MF4_Concat(_MA11(b,a,s),b, MF2_Plus(s,12),a)
#define _MA13(b,a,s) MF4_Concat(_MA12(b,a,s),b, MF2_Plus(s,13),a)
#define _MA14(b,a,s) MF4_Concat(_MA13(b,a,s),b, MF2_Plus(s,14),a)
#define _MA15(b,a,s) MF4_Concat(_MA14(b,a,s),b, MF2_Plus(s,15),a)
#define _MA16(b,a,s) MF4_Concat(_MA15(b,a,s),b, MF2_Plus(s,16),a)
#define _MA17(b,a,s) MF4_Concat(_MA16(b,a,s),b, MF2_Plus(s,17),a)
#define _MA18(b,a,s) MF4_Concat(_MA17(b,a,s),b, MF2_Plus(s,18),a)
#define _MA19(b,a,s) MF4_Concat(_MA18(b,a,s),b, MF2_Plus(s,19),a)
#define _MA20(b,a,s) MF4_Concat(_MA19(b,a,s),b, MF2_Plus(s,20),a)
#define _MA21(b,a,s) MF4_Concat(_MA20(b,a,s),b, MF2_Plus(s,21),a)
#define _MA22(b,a,s) MF4_Concat(_MA21(b,a,s),b, MF2_Plus(s,22),a)
#define _MA23(b,a,s) MF4_Concat(_MA22(b,a,s),b, MF2_Plus(s,23),a)
#define _MA24(b,a,s) MF4_Concat(_MA23(b,a,s),b, MF2_Plus(s,24),a)
#define _MA25(b,a,s) MF4_Concat(_MA24(b,a,s),b, MF2_Plus(s,25),a)
#define _MA26(b,a,s) MF4_Concat(_MA25(b,a,s),b, MF2_Plus(s,26),a)
#define _MA27(b,a,s) MF4_Concat(_MA26(b,a,s),b, MF2_Plus(s,27),a)
#define _MA28(b,a,s) MF4_Concat(_MA27(b,a,s),b, MF2_Plus(s,28),a)
#define _MA29(b,a,s) MF4_Concat(_MA28(b,a,s),b, MF2_Plus(s,29),a)
#define _MA30(b,a,s) MF4_Concat(_MA29(b,a,s),b, MF2_Plus(s,30),a)
#define _MA31(b,a,s) MF4_Concat(_MA30(b,a,s),b, MF2_Plus(s,31),a)
#define _MA32(b,a,s) MF4_Concat(_MA31(b,a,s),b, MF2_Plus(s,32),a)
#if _MRP_CNTM0>=32
#define _MA33(b,a,s) MF4_Concat(_MA32(b,a,s),b, MF2_Plus(s,33),a)
#define _MA34(b,a,s) MF4_Concat(_MA33(b,a,s),b, MF2_Plus(s,34),a)
#define _MA35(b,a,s) MF4_Concat(_MA34(b,a,s),b, MF2_Plus(s,35),a)
#define _MA36(b,a,s) MF4_Concat(_MA35(b,a,s),b, MF2_Plus(s,36),a)
#define _MA37(b,a,s) MF4_Concat(_MA36(b,a,s),b, MF2_Plus(s,37),a)
#define _MA38(b,a,s) MF4_Concat(_MA37(b,a,s),b, MF2_Plus(s,38),a)
#define _MA39(b,a,s) MF4_Concat(_MA38(b,a,s),b, MF2_Plus(s,39),a)
#define _MA40(b,a,s) MF4_Concat(_MA39(b,a,s),b, MF2_Plus(s,40),a)
#define _MA41(b,a,s) MF4_Concat(_MA40(b,a,s),b, MF2_Plus(s,41),a)
#define _MA42(b,a,s) MF4_Concat(_MA41(b,a,s),b, MF2_Plus(s,42),a)
#define _MA43(b,a,s) MF4_Concat(_MA42(b,a,s),b, MF2_Plus(s,43),a)
#define _MA44(b,a,s) MF4_Concat(_MA43(b,a,s),b, MF2_Plus(s,44),a)
#define _MA45(b,a,s) MF4_Concat(_MA44(b,a,s),b, MF2_Plus(s,45),a)
#define _MA46(b,a,s) MF4_Concat(_MA45(b,a,s),b, MF2_Plus(s,46),a)
#define _MA47(b,a,s) MF4_Concat(_MA46(b,a,s),b, MF2_Plus(s,47),a)
#define _MA48(b,a,s) MF4_Concat(_MA47(b,a,s),b, MF2_Plus(s,48),a)
#define _MA49(b,a,s) MF4_Concat(_MA48(b,a,s),b, MF2_Plus(s,49),a)
#define _MA50(b,a,s) MF4_Concat(_MA49(b,a,s),b, MF2_Plus(s,50),a)
#define _MA51(b,a,s) MF4_Concat(_MA50(b,a,s),b, MF2_Plus(s,51),a)
#define _MA52(b,a,s) MF4_Concat(_MA51(b,a,s),b, MF2_Plus(s,52),a)
#define _MA53(b,a,s) MF4_Concat(_MA52(b,a,s),b, MF2_Plus(s,53),a)
#define _MA54(b,a,s) MF4_Concat(_MA53(b,a,s),b, MF2_Plus(s,54),a)
#define _MA55(b,a,s) MF4_Concat(_MA54(b,a,s),b, MF2_Plus(s,55),a)
#define _MA56(b,a,s) MF4_Concat(_MA55(b,a,s),b, MF2_Plus(s,56),a)
#define _MA57(b,a,s) MF4_Concat(_MA56(b,a,s),b, MF2_Plus(s,57),a)
#define _MA58(b,a,s) MF4_Concat(_MA57(b,a,s),b, MF2_Plus(s,58),a)
#define _MA59(b,a,s) MF4_Concat(_MA58(b,a,s),b, MF2_Plus(s,59),a)
#define _MA60(b,a,s) MF4_Concat(_MA59(b,a,s),b, MF2_Plus(s,60),a)
#define _MA61(b,a,s) MF4_Concat(_MA60(b,a,s),b, MF2_Plus(s,61),a)
#define _MA62(b,a,s) MF4_Concat(_MA61(b,a,s),b, MF2_Plus(s,62),a)
#define _MA63(b,a,s) MF4_Concat(_MA62(b,a,s),b, MF2_Plus(s,63),a)
#define _MA64(b,a,s) MF4_Concat(_MA63(b,a,s),b, MF2_Plus(s,64),a)
#endif

#define _MA__(b,a,s)
#define _MA0_(b,a,s) MF3_Concat(b, s,a)
#define _MA1_(b,a,s) _MA0_(b,a,s),MF3_Concat(b, MF2_Plus(s,1),a)
#define _MA2_(b,a,s) _MA1_(b,a,s),MF3_Concat(b, MF2_Plus(s,2),a)
#define _MA3_(b,a,s) _MA2_(b,a,s),MF3_Concat(b, MF2_Plus(s,3),a)
#define _MA4_(b,a,s) _MA3_(b,a,s),MF3_Concat(b, MF2_Plus(s,4),a)
#define _MA5_(b,a,s) _MA4_(b,a,s),MF3_Concat(b, MF2_Plus(s,5),a)
#define _MA6_(b,a,s) _MA5_(b,a,s),MF3_Concat(b, MF2_Plus(s,6),a)
#define _MA7_(b,a,s) _MA6_(b,a,s),MF3_Concat(b, MF2_Plus(s,7),a)
#define _MA8_(b,a,s) _MA7_(b,a,s),MF3_Concat(b, MF2_Plus(s,8),a)
#define _MA9_(b,a,s) _MA8_(b,a,s),MF3_Concat(b, MF2_Plus(s,9),a)
#define _MA10_(b,a,s) _MA9_(b,a,s),MF3_Concat(b, MF2_Plus(s,10),a)
#define _MA11_(b,a,s) _MA10_(b,a,s),MF3_Concat(b, MF2_Plus(s,11),a)
#define _MA12_(b,a,s) _MA11_(b,a,s),MF3_Concat(b, MF2_Plus(s,12),a)
#define _MA13_(b,a,s) _MA12_(b,a,s),MF3_Concat(b, MF2_Plus(s,13),a)
#define _MA14_(b,a,s) _MA13_(b,a,s),MF3_Concat(b, MF2_Plus(s,14),a)
#define _MA15_(b,a,s) _MA14_(b,a,s),MF3_Concat(b, MF2_Plus(s,15),a)
#define _MA16_(b,a,s) _MA15_(b,a,s),MF3_Concat(b, MF2_Plus(s,16),a)
#define _MA17_(b,a,s) _MA16_(b,a,s),MF3_Concat(b, MF2_Plus(s,17),a)
#define _MA18_(b,a,s) _MA17_(b,a,s),MF3_Concat(b, MF2_Plus(s,18),a)
#define _MA19_(b,a,s) _MA18_(b,a,s),MF3_Concat(b, MF2_Plus(s,19),a)
#define _MA20_(b,a,s) _MA19_(b,a,s),MF3_Concat(b, MF2_Plus(s,20),a)
#define _MA21_(b,a,s) _MA20_(b,a,s),MF3_Concat(b, MF2_Plus(s,21),a)
#define _MA22_(b,a,s) _MA21_(b,a,s),MF3_Concat(b, MF2_Plus(s,22),a)
#define _MA23_(b,a,s) _MA22_(b,a,s),MF3_Concat(b, MF2_Plus(s,23),a)
#define _MA24_(b,a,s) _MA23_(b,a,s),MF3_Concat(b, MF2_Plus(s,24),a)
#define _MA25_(b,a,s) _MA24_(b,a,s),MF3_Concat(b, MF2_Plus(s,25),a)
#define _MA26_(b,a,s) _MA25_(b,a,s),MF3_Concat(b, MF2_Plus(s,26),a)
#define _MA27_(b,a,s) _MA26_(b,a,s),MF3_Concat(b, MF2_Plus(s,27),a)
#define _MA28_(b,a,s) _MA27_(b,a,s),MF3_Concat(b, MF2_Plus(s,28),a)
#define _MA29_(b,a,s) _MA28_(b,a,s),MF3_Concat(b, MF2_Plus(s,29),a)
#define _MA30_(b,a,s) _MA29_(b,a,s),MF3_Concat(b, MF2_Plus(s,30),a)
#define _MA31_(b,a,s) _MA30_(b,a,s),MF3_Concat(b, MF2_Plus(s,31),a)
#define _MA32_(b,a,s) _MA31_(b,a,s),MF3_Concat(b, MF2_Plus(s,32),a)
#if _MRP_CNTM0>=32
#define _MA33_(b,a,s) _MA32_(b,a,s),MF3_Concat(b, MF2_Plus(s,33),a)
#define _MA34_(b,a,s) _MA33_(b,a,s),MF3_Concat(b, MF2_Plus(s,34),a)
#define _MA35_(b,a,s) _MA34_(b,a,s),MF3_Concat(b, MF2_Plus(s,35),a)
#define _MA36_(b,a,s) _MA35_(b,a,s),MF3_Concat(b, MF2_Plus(s,36),a)
#define _MA37_(b,a,s) _MA36_(b,a,s),MF3_Concat(b, MF2_Plus(s,37),a)
#define _MA38_(b,a,s) _MA37_(b,a,s),MF3_Concat(b, MF2_Plus(s,38),a)
#define _MA39_(b,a,s) _MA38_(b,a,s),MF3_Concat(b, MF2_Plus(s,39),a)
#define _MA40_(b,a,s) _MA39_(b,a,s),MF3_Concat(b, MF2_Plus(s,40),a)
#define _MA41_(b,a,s) _MA40_(b,a,s),MF3_Concat(b, MF2_Plus(s,41),a)
#define _MA42_(b,a,s) _MA41_(b,a,s),MF3_Concat(b, MF2_Plus(s,42),a)
#define _MA43_(b,a,s) _MA42_(b,a,s),MF3_Concat(b, MF2_Plus(s,43),a)
#define _MA44_(b,a,s) _MA43_(b,a,s),MF3_Concat(b, MF2_Plus(s,44),a)
#define _MA45_(b,a,s) _MA44_(b,a,s),MF3_Concat(b, MF2_Plus(s,45),a)
#define _MA46_(b,a,s) _MA45_(b,a,s),MF3_Concat(b, MF2_Plus(s,46),a)
#define _MA47_(b,a,s) _MA46_(b,a,s),MF3_Concat(b, MF2_Plus(s,47),a)
#define _MA48_(b,a,s) _MA47_(b,a,s),MF3_Concat(b, MF2_Plus(s,48),a)
#define _MA49_(b,a,s) _MA48_(b,a,s),MF3_Concat(b, MF2_Plus(s,49),a)
#define _MA50_(b,a,s) _MA49_(b,a,s),MF3_Concat(b, MF2_Plus(s,50),a)
#define _MA51_(b,a,s) _MA50_(b,a,s),MF3_Concat(b, MF2_Plus(s,51),a)
#define _MA52_(b,a,s) _MA51_(b,a,s),MF3_Concat(b, MF2_Plus(s,52),a)
#define _MA53_(b,a,s) _MA52_(b,a,s),MF3_Concat(b, MF2_Plus(s,53),a)
#define _MA54_(b,a,s) _MA53_(b,a,s),MF3_Concat(b, MF2_Plus(s,54),a)
#define _MA55_(b,a,s) _MA54_(b,a,s),MF3_Concat(b, MF2_Plus(s,55),a)
#define _MA56_(b,a,s) _MA55_(b,a,s),MF3_Concat(b, MF2_Plus(s,56),a)
#define _MA57_(b,a,s) _MA56_(b,a,s),MF3_Concat(b, MF2_Plus(s,57),a)
#define _MA58_(b,a,s) _MA57_(b,a,s),MF3_Concat(b, MF2_Plus(s,58),a)
#define _MA59_(b,a,s) _MA58_(b,a,s),MF3_Concat(b, MF2_Plus(s,59),a)
#define _MA60_(b,a,s) _MA59_(b,a,s),MF3_Concat(b, MF2_Plus(s,60),a)
#define _MA61_(b,a,s) _MA60_(b,a,s),MF3_Concat(b, MF2_Plus(s,61),a)
#define _MA62_(b,a,s) _MA61_(b,a,s),MF3_Concat(b, MF2_Plus(s,62),a)
#define _MA63_(b,a,s) _MA62_(b,a,s),MF3_Concat(b, MF2_Plus(s,63),a)
#define _MA64_(b,a,s) _MA63_(b,a,s),MF3_Concat(b, MF2_Plus(s,64),a)
#endif

#define _Ma_b(b,s)
#define _Ma0b(b,s) b (s)
#define _Ma1b(b,s) _Ma0b(b,s) _Ma0b(b,MF2_Plus(s,1))
#define _Ma2b(b,s) _Ma1b(b,s) _Ma0b(b,MF2_Plus(s,2))
#define _Ma3b(b,s) _Ma2b(b,s) _Ma0b(b,MF2_Plus(s,3))
#define _Ma4b(b,s) _Ma3b(b,s) _Ma0b(b,MF2_Plus(s,4))
#define _Ma5b(b,s) _Ma4b(b,s) _Ma0b(b,MF2_Plus(s,5))
#define _Ma6b(b,s) _Ma5b(b,s) _Ma0b(b,MF2_Plus(s,6))
#define _Ma7b(b,s) _Ma6b(b,s) _Ma0b(b,MF2_Plus(s,7))
#define _Ma8b(b,s) _Ma7b(b,s) _Ma0b(b,MF2_Plus(s,8))
#define _Ma9b(b,s) _Ma8b(b,s) _Ma0b(b,MF2_Plus(s,9))
#define _Ma10b(b,s) _Ma9b(b,s) _Ma0b(b,MF2_Plus(s,10))
#define _Ma11b(b,s) _Ma10b(b,s) _Ma0b(b,MF2_Plus(s,11))
#define _Ma12b(b,s) _Ma11b(b,s) _Ma0b(b,MF2_Plus(s,12))
#define _Ma13b(b,s) _Ma12b(b,s) _Ma0b(b,MF2_Plus(s,13))
#define _Ma14b(b,s) _Ma13b(b,s) _Ma0b(b,MF2_Plus(s,14))
#define _Ma15b(b,s) _Ma14b(b,s) _Ma0b(b,MF2_Plus(s,15))
#define _Ma16b(b,s) _Ma15b(b,s) _Ma0b(b,MF2_Plus(s,16))
#define _Ma17b(b,s) _Ma16b(b,s) _Ma0b(b,MF2_Plus(s,17))
#define _Ma18b(b,s) _Ma17b(b,s) _Ma0b(b,MF2_Plus(s,18))
#define _Ma19b(b,s) _Ma18b(b,s) _Ma0b(b,MF2_Plus(s,19))
#define _Ma20b(b,s) _Ma19b(b,s) _Ma0b(b,MF2_Plus(s,20))
#define _Ma21b(b,s) _Ma20b(b,s) _Ma0b(b,MF2_Plus(s,21))
#define _Ma22b(b,s) _Ma21b(b,s) _Ma0b(b,MF2_Plus(s,22))
#define _Ma23b(b,s) _Ma22b(b,s) _Ma0b(b,MF2_Plus(s,23))
#define _Ma24b(b,s) _Ma23b(b,s) _Ma0b(b,MF2_Plus(s,24))
#define _Ma25b(b,s) _Ma24b(b,s) _Ma0b(b,MF2_Plus(s,25))
#define _Ma26b(b,s) _Ma25b(b,s) _Ma0b(b,MF2_Plus(s,26))
#define _Ma27b(b,s) _Ma26b(b,s) _Ma0b(b,MF2_Plus(s,27))
#define _Ma28b(b,s) _Ma27b(b,s) _Ma0b(b,MF2_Plus(s,28))
#define _Ma29b(b,s) _Ma28b(b,s) _Ma0b(b,MF2_Plus(s,29))
#define _Ma30b(b,s) _Ma29b(b,s) _Ma0b(b,MF2_Plus(s,30))
#define _Ma31b(b,s) _Ma30b(b,s) _Ma0b(b,MF2_Plus(s,31))
#define _Ma32b(b,s) _Ma31b(b,s) _Ma0b(b,MF2_Plus(s,32))
#if _MRP_CNTM0>=32
#define _Ma33b(b,s) _Ma32b(b,s) _Ma0b(b,MF2_Plus(s,33))
#define _Ma34b(b,s) _Ma33b(b,s) _Ma0b(b,MF2_Plus(s,34))
#define _Ma35b(b,s) _Ma34b(b,s) _Ma0b(b,MF2_Plus(s,35))
#define _Ma36b(b,s) _Ma35b(b,s) _Ma0b(b,MF2_Plus(s,36))
#define _Ma37b(b,s) _Ma36b(b,s) _Ma0b(b,MF2_Plus(s,37))
#define _Ma38b(b,s) _Ma37b(b,s) _Ma0b(b,MF2_Plus(s,38))
#define _Ma39b(b,s) _Ma38b(b,s) _Ma0b(b,MF2_Plus(s,39))
#define _Ma40b(b,s) _Ma39b(b,s) _Ma0b(b,MF2_Plus(s,40))
#define _Ma41b(b,s) _Ma40b(b,s) _Ma0b(b,MF2_Plus(s,41))
#define _Ma42b(b,s) _Ma41b(b,s) _Ma0b(b,MF2_Plus(s,42))
#define _Ma43b(b,s) _Ma42b(b,s) _Ma0b(b,MF2_Plus(s,43))
#define _Ma44b(b,s) _Ma43b(b,s) _Ma0b(b,MF2_Plus(s,44))
#define _Ma45b(b,s) _Ma44b(b,s) _Ma0b(b,MF2_Plus(s,45))
#define _Ma46b(b,s) _Ma45b(b,s) _Ma0b(b,MF2_Plus(s,46))
#define _Ma47b(b,s) _Ma46b(b,s) _Ma0b(b,MF2_Plus(s,47))
#define _Ma48b(b,s) _Ma47b(b,s) _Ma0b(b,MF2_Plus(s,48))
#define _Ma49b(b,s) _Ma48b(b,s) _Ma0b(b,MF2_Plus(s,49))
#define _Ma50b(b,s) _Ma49b(b,s) _Ma0b(b,MF2_Plus(s,50))
#define _Ma51b(b,s) _Ma50b(b,s) _Ma0b(b,MF2_Plus(s,51))
#define _Ma52b(b,s) _Ma51b(b,s) _Ma0b(b,MF2_Plus(s,52))
#define _Ma53b(b,s) _Ma52b(b,s) _Ma0b(b,MF2_Plus(s,53))
#define _Ma54b(b,s) _Ma53b(b,s) _Ma0b(b,MF2_Plus(s,54))
#define _Ma55b(b,s) _Ma54b(b,s) _Ma0b(b,MF2_Plus(s,55))
#define _Ma56b(b,s) _Ma55b(b,s) _Ma0b(b,MF2_Plus(s,56))
#define _Ma57b(b,s) _Ma56b(b,s) _Ma0b(b,MF2_Plus(s,57))
#define _Ma58b(b,s) _Ma57b(b,s) _Ma0b(b,MF2_Plus(s,58))
#define _Ma59b(b,s) _Ma58b(b,s) _Ma0b(b,MF2_Plus(s,59))
#define _Ma60b(b,s) _Ma59b(b,s) _Ma0b(b,MF2_Plus(s,60))
#define _Ma61b(b,s) _Ma60b(b,s) _Ma0b(b,MF2_Plus(s,61))
#define _Ma62b(b,s) _Ma61b(b,s) _Ma0b(b,MF2_Plus(s,62))
#define _Ma63b(b,s) _Ma62b(b,s) _Ma0b(b,MF2_Plus(s,63))
#define _Ma64b(b,s) _Ma63b(b,s) _Ma0b(b,MF2_Plus(s,64))
#endif

#define _Ma__b(b,s)
#define _Ma0_b(b,s) b(s)
#define _Ma1_b(b,s) _Ma0_b(b,s),_Ma0b(b,MF2_Plus(s,1))
#define _Ma2_b(b,s) _Ma1_b(b,s),_Ma0b(b,MF2_Plus(s,2))
#define _Ma3_b(b,s) _Ma2_b(b,s),_Ma0b(b,MF2_Plus(s,3))
#define _Ma4_b(b,s) _Ma3_b(b,s),_Ma0b(b,MF2_Plus(s,4))
#define _Ma5_b(b,s) _Ma4_b(b,s),_Ma0b(b,MF2_Plus(s,5))
#define _Ma6_b(b,s) _Ma5_b(b,s),_Ma0b(b,MF2_Plus(s,6))
#define _Ma7_b(b,s) _Ma6_b(b,s),_Ma0b(b,MF2_Plus(s,7))
#define _Ma8_b(b,s) _Ma7_b(b,s),_Ma0b(b,MF2_Plus(s,8))
#define _Ma9_b(b,s) _Ma8_b(b,s),_Ma0b(b,MF2_Plus(s,9))
#define _Ma10_b(b,s) _Ma9_b(b,s),_Ma0b(b,MF2_Plus(s,10))
#define _Ma11_b(b,s) _Ma10_b(b,s),_Ma0b(b,MF2_Plus(s,11))
#define _Ma12_b(b,s) _Ma11_b(b,s),_Ma0b(b,MF2_Plus(s,12))
#define _Ma13_b(b,s) _Ma12_b(b,s),_Ma0b(b,MF2_Plus(s,13))
#define _Ma14_b(b,s) _Ma13_b(b,s),_Ma0b(b,MF2_Plus(s,14))
#define _Ma15_b(b,s) _Ma14_b(b,s),_Ma0b(b,MF2_Plus(s,15))
#define _Ma16_b(b,s) _Ma15_b(b,s),_Ma0b(b,MF2_Plus(s,16))
#define _Ma17_b(b,s) _Ma16_b(b,s),_Ma0b(b,MF2_Plus(s,17))
#define _Ma18_b(b,s) _Ma17_b(b,s),_Ma0b(b,MF2_Plus(s,18))
#define _Ma19_b(b,s) _Ma18_b(b,s),_Ma0b(b,MF2_Plus(s,19))
#define _Ma20_b(b,s) _Ma19_b(b,s),_Ma0b(b,MF2_Plus(s,20))
#define _Ma21_b(b,s) _Ma20_b(b,s),_Ma0b(b,MF2_Plus(s,21))
#define _Ma22_b(b,s) _Ma21_b(b,s),_Ma0b(b,MF2_Plus(s,22))
#define _Ma23_b(b,s) _Ma22_b(b,s),_Ma0b(b,MF2_Plus(s,23))
#define _Ma24_b(b,s) _Ma23_b(b,s),_Ma0b(b,MF2_Plus(s,24))
#define _Ma25_b(b,s) _Ma24_b(b,s),_Ma0b(b,MF2_Plus(s,25))
#define _Ma26_b(b,s) _Ma25_b(b,s),_Ma0b(b,MF2_Plus(s,26))
#define _Ma27_b(b,s) _Ma26_b(b,s),_Ma0b(b,MF2_Plus(s,27))
#define _Ma28_b(b,s) _Ma27_b(b,s),_Ma0b(b,MF2_Plus(s,28))
#define _Ma29_b(b,s) _Ma28_b(b,s),_Ma0b(b,MF2_Plus(s,29))
#define _Ma30_b(b,s) _Ma29_b(b,s),_Ma0b(b,MF2_Plus(s,30))
#define _Ma31_b(b,s) _Ma30_b(b,s),_Ma0b(b,MF2_Plus(s,31))
#define _Ma32_b(b,s) _Ma31_b(b,s),_Ma0b(b,MF2_Plus(s,32))
#if _MRP_CNTM0>=32
#define _Ma33_b(b,s) _Ma32_b(b,s),_Ma0b(b,MF2_Plus(s,33))
#define _Ma34_b(b,s) _Ma33_b(b,s),_Ma0b(b,MF2_Plus(s,34))
#define _Ma35_b(b,s) _Ma34_b(b,s),_Ma0b(b,MF2_Plus(s,35))
#define _Ma36_b(b,s) _Ma35_b(b,s),_Ma0b(b,MF2_Plus(s,36))
#define _Ma37_b(b,s) _Ma36_b(b,s),_Ma0b(b,MF2_Plus(s,37))
#define _Ma38_b(b,s) _Ma37_b(b,s),_Ma0b(b,MF2_Plus(s,38))
#define _Ma39_b(b,s) _Ma38_b(b,s),_Ma0b(b,MF2_Plus(s,39))
#define _Ma40_b(b,s) _Ma39_b(b,s),_Ma0b(b,MF2_Plus(s,40))
#define _Ma41_b(b,s) _Ma40_b(b,s),_Ma0b(b,MF2_Plus(s,41))
#define _Ma42_b(b,s) _Ma41_b(b,s),_Ma0b(b,MF2_Plus(s,42))
#define _Ma43_b(b,s) _Ma42_b(b,s),_Ma0b(b,MF2_Plus(s,43))
#define _Ma44_b(b,s) _Ma43_b(b,s),_Ma0b(b,MF2_Plus(s,44))
#define _Ma45_b(b,s) _Ma44_b(b,s),_Ma0b(b,MF2_Plus(s,45))
#define _Ma46_b(b,s) _Ma45_b(b,s),_Ma0b(b,MF2_Plus(s,46))
#define _Ma47_b(b,s) _Ma46_b(b,s),_Ma0b(b,MF2_Plus(s,47))
#define _Ma48_b(b,s) _Ma47_b(b,s),_Ma0b(b,MF2_Plus(s,48))
#define _Ma49_b(b,s) _Ma48_b(b,s),_Ma0b(b,MF2_Plus(s,49))
#define _Ma50_b(b,s) _Ma49_b(b,s),_Ma0b(b,MF2_Plus(s,50))
#define _Ma51_b(b,s) _Ma50_b(b,s),_Ma0b(b,MF2_Plus(s,51))
#define _Ma52_b(b,s) _Ma51_b(b,s),_Ma0b(b,MF2_Plus(s,52))
#define _Ma53_b(b,s) _Ma52_b(b,s),_Ma0b(b,MF2_Plus(s,53))
#define _Ma54_b(b,s) _Ma53_b(b,s),_Ma0b(b,MF2_Plus(s,54))
#define _Ma55_b(b,s) _Ma54_b(b,s),_Ma0b(b,MF2_Plus(s,55))
#define _Ma56_b(b,s) _Ma55_b(b,s),_Ma0b(b,MF2_Plus(s,56))
#define _Ma57_b(b,s) _Ma56_b(b,s),_Ma0b(b,MF2_Plus(s,57))
#define _Ma58_b(b,s) _Ma57_b(b,s),_Ma0b(b,MF2_Plus(s,58))
#define _Ma59_b(b,s) _Ma58_b(b,s),_Ma0b(b,MF2_Plus(s,59))
#define _Ma60_b(b,s) _Ma59_b(b,s),_Ma0b(b,MF2_Plus(s,60))
#define _Ma61_b(b,s) _Ma60_b(b,s),_Ma0b(b,MF2_Plus(s,61))
#define _Ma62_b(b,s) _Ma61_b(b,s),_Ma0b(b,MF2_Plus(s,62))
#define _Ma63_b(b,s) _Ma62_b(b,s),_Ma0b(b,MF2_Plus(s,63))
#define _Ma64_b(b,s) _Ma63_b(b,s),_Ma0b(b,MF2_Plus(s,64))
#endif

//降序系列
#define _MD_(b,a,s)
#define _MD0(b,a,s) MF3_Concat(b,s,a)
#define _MD1(b,a,s) MF4_Concat(_MD0(b,a,s),b, MF2_Minus(s,1),a)
#define _MD2(b,a,s) MF4_Concat(_MD1(b,a,s),b, MF2_Minus(s,2),a)
#define _MD3(b,a,s) MF4_Concat(_MD2(b,a,s),b, MF2_Minus(s,3),a)
#define _MD4(b,a,s) MF4_Concat(_MD3(b,a,s),b, MF2_Minus(s,4),a)
#define _MD5(b,a,s) MF4_Concat(_MD4(b,a,s),b, MF2_Minus(s,5),a)
#define _MD6(b,a,s) MF4_Concat(_MD5(b,a,s),b, MF2_Minus(s,6),a)
#define _MD7(b,a,s) MF4_Concat(_MD6(b,a,s),b, MF2_Minus(s,7),a)
#define _MD8(b,a,s) MF4_Concat(_MD7(b,a,s),b, MF2_Minus(s,8),a)
#define _MD9(b,a,s) MF4_Concat(_MD8(b,a,s),b, MF2_Minus(s,9),a)
#define _MD10(b,a,s) MF4_Concat(_MD9(b,a,s),b, MF2_Minus(s,10),a)
#define _MD11(b,a,s) MF4_Concat(_MD10(b,a,s),b, MF2_Minus(s,11),a)
#define _MD12(b,a,s) MF4_Concat(_MD11(b,a,s),b, MF2_Minus(s,12),a)
#define _MD13(b,a,s) MF4_Concat(_MD12(b,a,s),b, MF2_Minus(s,13),a)
#define _MD14(b,a,s) MF4_Concat(_MD13(b,a,s),b, MF2_Minus(s,14),a)
#define _MD15(b,a,s) MF4_Concat(_MD14(b,a,s),b, MF2_Minus(s,15),a)
#define _MD16(b,a,s) MF4_Concat(_MD15(b,a,s),b, MF2_Minus(s,16),a)
#define _MD17(b,a,s) MF4_Concat(_MD16(b,a,s),b, MF2_Minus(s,17),a)
#define _MD18(b,a,s) MF4_Concat(_MD17(b,a,s),b, MF2_Minus(s,18),a)
#define _MD19(b,a,s) MF4_Concat(_MD18(b,a,s),b, MF2_Minus(s,19),a)
#define _MD20(b,a,s) MF4_Concat(_MD19(b,a,s),b, MF2_Minus(s,20),a)
#define _MD21(b,a,s) MF4_Concat(_MD20(b,a,s),b, MF2_Minus(s,21),a)
#define _MD22(b,a,s) MF4_Concat(_MD21(b,a,s),b, MF2_Minus(s,22),a)
#define _MD23(b,a,s) MF4_Concat(_MD22(b,a,s),b, MF2_Minus(s,23),a)
#define _MD24(b,a,s) MF4_Concat(_MD23(b,a,s),b, MF2_Minus(s,24),a)
#define _MD25(b,a,s) MF4_Concat(_MD24(b,a,s),b, MF2_Minus(s,25),a)
#define _MD26(b,a,s) MF4_Concat(_MD25(b,a,s),b, MF2_Minus(s,26),a)
#define _MD27(b,a,s) MF4_Concat(_MD26(b,a,s),b, MF2_Minus(s,27),a)
#define _MD28(b,a,s) MF4_Concat(_MD27(b,a,s),b, MF2_Minus(s,28),a)
#define _MD29(b,a,s) MF4_Concat(_MD28(b,a,s),b, MF2_Minus(s,29),a)
#define _MD30(b,a,s) MF4_Concat(_MD29(b,a,s),b, MF2_Minus(s,30),a)
#define _MD31(b,a,s) MF4_Concat(_MD30(b,a,s),b, MF2_Minus(s,31),a)
#define _MD32(b,a,s) MF4_Concat(_MD31(b,a,s),b, MF2_Minus(s,32),a)
#if _MRP_CNTM0>=32
#define _MD33(b,a,s) MF4_Concat(_MD32(b,a,s),b, MF2_Minus(s,33),a)
#define _MD34(b,a,s) MF4_Concat(_MD33(b,a,s),b, MF2_Minus(s,34),a)
#define _MD35(b,a,s) MF4_Concat(_MD34(b,a,s),b, MF2_Minus(s,35),a)
#define _MD36(b,a,s) MF4_Concat(_MD35(b,a,s),b, MF2_Minus(s,36),a)
#define _MD37(b,a,s) MF4_Concat(_MD36(b,a,s),b, MF2_Minus(s,37),a)
#define _MD38(b,a,s) MF4_Concat(_MD37(b,a,s),b, MF2_Minus(s,38),a)
#define _MD39(b,a,s) MF4_Concat(_MD38(b,a,s),b, MF2_Minus(s,39),a)
#define _MD40(b,a,s) MF4_Concat(_MD39(b,a,s),b, MF2_Minus(s,40),a)
#define _MD41(b,a,s) MF4_Concat(_MD40(b,a,s),b, MF2_Minus(s,41),a)
#define _MD42(b,a,s) MF4_Concat(_MD41(b,a,s),b, MF2_Minus(s,42),a)
#define _MD43(b,a,s) MF4_Concat(_MD42(b,a,s),b, MF2_Minus(s,43),a)
#define _MD44(b,a,s) MF4_Concat(_MD43(b,a,s),b, MF2_Minus(s,44),a)
#define _MD45(b,a,s) MF4_Concat(_MD44(b,a,s),b, MF2_Minus(s,45),a)
#define _MD46(b,a,s) MF4_Concat(_MD45(b,a,s),b, MF2_Minus(s,46),a)
#define _MD47(b,a,s) MF4_Concat(_MD46(b,a,s),b, MF2_Minus(s,47),a)
#define _MD48(b,a,s) MF4_Concat(_MD47(b,a,s),b, MF2_Minus(s,48),a)
#define _MD49(b,a,s) MF4_Concat(_MD48(b,a,s),b, MF2_Minus(s,49),a)
#define _MD50(b,a,s) MF4_Concat(_MD49(b,a,s),b, MF2_Minus(s,50),a)
#define _MD51(b,a,s) MF4_Concat(_MD50(b,a,s),b, MF2_Minus(s,51),a)
#define _MD52(b,a,s) MF4_Concat(_MD51(b,a,s),b, MF2_Minus(s,52),a)
#define _MD53(b,a,s) MF4_Concat(_MD52(b,a,s),b, MF2_Minus(s,53),a)
#define _MD54(b,a,s) MF4_Concat(_MD53(b,a,s),b, MF2_Minus(s,54),a)
#define _MD55(b,a,s) MF4_Concat(_MD54(b,a,s),b, MF2_Minus(s,55),a)
#define _MD56(b,a,s) MF4_Concat(_MD55(b,a,s),b, MF2_Minus(s,56),a)
#define _MD57(b,a,s) MF4_Concat(_MD56(b,a,s),b, MF2_Minus(s,57),a)
#define _MD58(b,a,s) MF4_Concat(_MD57(b,a,s),b, MF2_Minus(s,58),a)
#define _MD59(b,a,s) MF4_Concat(_MD58(b,a,s),b, MF2_Minus(s,59),a)
#define _MD60(b,a,s) MF4_Concat(_MD59(b,a,s),b, MF2_Minus(s,60),a)
#define _MD61(b,a,s) MF4_Concat(_MD60(b,a,s),b, MF2_Minus(s,61),a)
#define _MD62(b,a,s) MF4_Concat(_MD61(b,a,s),b, MF2_Minus(s,62),a)
#define _MD63(b,a,s) MF4_Concat(_MD62(b,a,s),b, MF2_Minus(s,63),a)
#define _MD64(b,a,s) MF4_Concat(_MD63(b,a,s),b, MF2_Minus(s,64),a)
#endif

#define _MD__(b,a,s)
#define _MD0_(b,a,s) MF3_Concat(b,s,a)
#define _MD1_(b,a,s) _MD0_(b,a,s),MF3_Concat(b, MF2_Minus(s,1),a)
#define _MD2_(b,a,s) _MD1_(b,a,s),MF3_Concat(b, MF2_Minus(s,2),a)
#define _MD3_(b,a,s) _MD2_(b,a,s),MF3_Concat(b, MF2_Minus(s,3),a)
#define _MD4_(b,a,s) _MD3_(b,a,s),MF3_Concat(b, MF2_Minus(s,4),a)
#define _MD5_(b,a,s) _MD4_(b,a,s),MF3_Concat(b, MF2_Minus(s,5),a)
#define _MD6_(b,a,s) _MD5_(b,a,s),MF3_Concat(b, MF2_Minus(s,6),a)
#define _MD7_(b,a,s) _MD6_(b,a,s),MF3_Concat(b, MF2_Minus(s,7),a)
#define _MD8_(b,a,s) _MD7_(b,a,s),MF3_Concat(b, MF2_Minus(s,8),a)
#define _MD9_(b,a,s) _MD8_(b,a,s),MF3_Concat(b, MF2_Minus(s,9),a)
#define _MD10_(b,a,s) _MD9_(b,a,s),MF3_Concat(b, MF2_Minus(s,10),a)
#define _MD11_(b,a,s) _MD10_(b,a,s),MF3_Concat(b, MF2_Minus(s,11),a)
#define _MD12_(b,a,s) _MD11_(b,a,s),MF3_Concat(b, MF2_Minus(s,12),a)
#define _MD13_(b,a,s) _MD12_(b,a,s),MF3_Concat(b, MF2_Minus(s,13),a)
#define _MD14_(b,a,s) _MD13_(b,a,s),MF3_Concat(b, MF2_Minus(s,14),a)
#define _MD15_(b,a,s) _MD14_(b,a,s),MF3_Concat(b, MF2_Minus(s,15),a)
#define _MD16_(b,a,s) _MD15_(b,a,s),MF3_Concat(b, MF2_Minus(s,16),a)
#define _MD17_(b,a,s) _MD16_(b,a,s),MF3_Concat(b, MF2_Minus(s,17),a)
#define _MD18_(b,a,s) _MD17_(b,a,s),MF3_Concat(b, MF2_Minus(s,18),a)
#define _MD19_(b,a,s) _MD18_(b,a,s),MF3_Concat(b, MF2_Minus(s,19),a)
#define _MD20_(b,a,s) _MD19_(b,a,s),MF3_Concat(b, MF2_Minus(s,20),a)
#define _MD21_(b,a,s) _MD20_(b,a,s),MF3_Concat(b, MF2_Minus(s,21),a)
#define _MD22_(b,a,s) _MD21_(b,a,s),MF3_Concat(b, MF2_Minus(s,22),a)
#define _MD23_(b,a,s) _MD22_(b,a,s),MF3_Concat(b, MF2_Minus(s,23),a)
#define _MD24_(b,a,s) _MD23_(b,a,s),MF3_Concat(b, MF2_Minus(s,24),a)
#define _MD25_(b,a,s) _MD24_(b,a,s),MF3_Concat(b, MF2_Minus(s,25),a)
#define _MD26_(b,a,s) _MD25_(b,a,s),MF3_Concat(b, MF2_Minus(s,26),a)
#define _MD27_(b,a,s) _MD26_(b,a,s),MF3_Concat(b, MF2_Minus(s,27),a)
#define _MD28_(b,a,s) _MD27_(b,a,s),MF3_Concat(b, MF2_Minus(s,28),a)
#define _MD29_(b,a,s) _MD28_(b,a,s),MF3_Concat(b, MF2_Minus(s,29),a)
#define _MD30_(b,a,s) _MD29_(b,a,s),MF3_Concat(b, MF2_Minus(s,30),a)
#define _MD31_(b,a,s) _MD30_(b,a,s),MF3_Concat(b, MF2_Minus(s,31),a)
#if _MRP_CNTM0>=32
#define _MD32_(b,a,s) _MD31_(b,a,s),MF3_Concat(b, MF2_Minus(s,32),a)
#define _MD33_(b,a,s) _MD32_(b,a,s),MF3_Concat(b, MF2_Minus(s,33),a)
#define _MD34_(b,a,s) _MD33_(b,a,s),MF3_Concat(b, MF2_Minus(s,34),a)
#define _MD35_(b,a,s) _MD34_(b,a,s),MF3_Concat(b, MF2_Minus(s,35),a)
#define _MD36_(b,a,s) _MD35_(b,a,s),MF3_Concat(b, MF2_Minus(s,36),a)
#define _MD37_(b,a,s) _MD36_(b,a,s),MF3_Concat(b, MF2_Minus(s,37),a)
#define _MD38_(b,a,s) _MD37_(b,a,s),MF3_Concat(b, MF2_Minus(s,38),a)
#define _MD39_(b,a,s) _MD38_(b,a,s),MF3_Concat(b, MF2_Minus(s,39),a)
#define _MD40_(b,a,s) _MD39_(b,a,s),MF3_Concat(b, MF2_Minus(s,40),a)
#define _MD41_(b,a,s) _MD40_(b,a,s),MF3_Concat(b, MF2_Minus(s,41),a)
#define _MD42_(b,a,s) _MD41_(b,a,s),MF3_Concat(b, MF2_Minus(s,42),a)
#define _MD43_(b,a,s) _MD42_(b,a,s),MF3_Concat(b, MF2_Minus(s,43),a)
#define _MD44_(b,a,s) _MD43_(b,a,s),MF3_Concat(b, MF2_Minus(s,44),a)
#define _MD45_(b,a,s) _MD44_(b,a,s),MF3_Concat(b, MF2_Minus(s,45),a)
#define _MD46_(b,a,s) _MD45_(b,a,s),MF3_Concat(b, MF2_Minus(s,46),a)
#define _MD47_(b,a,s) _MD46_(b,a,s),MF3_Concat(b, MF2_Minus(s,47),a)
#define _MD48_(b,a,s) _MD47_(b,a,s),MF3_Concat(b, MF2_Minus(s,48),a)
#define _MD49_(b,a,s) _MD48_(b,a,s),MF3_Concat(b, MF2_Minus(s,49),a)
#define _MD50_(b,a,s) _MD49_(b,a,s),MF3_Concat(b, MF2_Minus(s,50),a)
#define _MD51_(b,a,s) _MD50_(b,a,s),MF3_Concat(b, MF2_Minus(s,51),a)
#define _MD52_(b,a,s) _MD51_(b,a,s),MF3_Concat(b, MF2_Minus(s,52),a)
#define _MD53_(b,a,s) _MD52_(b,a,s),MF3_Concat(b, MF2_Minus(s,53),a)
#define _MD54_(b,a,s) _MD53_(b,a,s),MF3_Concat(b, MF2_Minus(s,54),a)
#define _MD55_(b,a,s) _MD54_(b,a,s),MF3_Concat(b, MF2_Minus(s,55),a)
#define _MD56_(b,a,s) _MD55_(b,a,s),MF3_Concat(b, MF2_Minus(s,56),a)
#define _MD57_(b,a,s) _MD56_(b,a,s),MF3_Concat(b, MF2_Minus(s,57),a)
#define _MD58_(b,a,s) _MD57_(b,a,s),MF3_Concat(b, MF2_Minus(s,58),a)
#define _MD59_(b,a,s) _MD58_(b,a,s),MF3_Concat(b, MF2_Minus(s,59),a)
#define _MD60_(b,a,s) _MD59_(b,a,s),MF3_Concat(b, MF2_Minus(s,60),a)
#define _MD61_(b,a,s) _MD60_(b,a,s),MF3_Concat(b, MF2_Minus(s,61),a)
#define _MD62_(b,a,s) _MD61_(b,a,s),MF3_Concat(b, MF2_Minus(s,62),a)
#define _MD63_(b,a,s) _MD62_(b,a,s),MF3_Concat(b, MF2_Minus(s,63),a)
#define _MD64_(b,a,s) _MD63_(b,a,s),MF3_Concat(b, MF2_Minus(s,64),a)
#endif

#define _Md_b(b,s)
#define _Md0b(b,s) b(s)
#define _Md1b(b,s) _Md0b(b,s) _Ma0b(b,MF2_Minus(s,1))
#define _Md2b(b,s) _Md1b(b,s) _Ma0b(b,MF2_Minus(s,2))
#define _Md3b(b,s) _Md2b(b,s) _Ma0b(b,MF2_Minus(s,3))
#define _Md4b(b,s) _Md3b(b,s) _Ma0b(b,MF2_Minus(s,4))
#define _Md5b(b,s) _Md4b(b,s) _Ma0b(b,MF2_Minus(s,5))
#define _Md6b(b,s) _Md5b(b,s) _Ma0b(b,MF2_Minus(s,6))
#define _Md7b(b,s) _Md6b(b,s) _Ma0b(b,MF2_Minus(s,7))
#define _Md8b(b,s) _Md7b(b,s) _Ma0b(b,MF2_Minus(s,8))
#define _Md9b(b,s) _Md8b(b,s) _Ma0b(b,MF2_Minus(s,9))
#define _Md10b(b,s) _Md9b(b,s) _Ma0b(b,MF2_Minus(s,10))
#define _Md11b(b,s) _Md10b(b,s) _Ma0b(b,MF2_Minus(s,11))
#define _Md12b(b,s) _Md11b(b,s) _Ma0b(b,MF2_Minus(s,12))
#define _Md13b(b,s) _Md12b(b,s) _Ma0b(b,MF2_Minus(s,13))
#define _Md14b(b,s) _Md13b(b,s) _Ma0b(b,MF2_Minus(s,14))
#define _Md15b(b,s) _Md14b(b,s) _Ma0b(b,MF2_Minus(s,15))
#define _Md16b(b,s) _Md15b(b,s) _Ma0b(b,MF2_Minus(s,16))
#define _Md17b(b,s) _Md16b(b,s) _Ma0b(b,MF2_Minus(s,17))
#define _Md18b(b,s) _Md17b(b,s) _Ma0b(b,MF2_Minus(s,18))
#define _Md19b(b,s) _Md18b(b,s) _Ma0b(b,MF2_Minus(s,19))
#define _Md20b(b,s) _Md19b(b,s) _Ma0b(b,MF2_Minus(s,20))
#define _Md21b(b,s) _Md20b(b,s) _Ma0b(b,MF2_Minus(s,21))
#define _Md22b(b,s) _Md21b(b,s) _Ma0b(b,MF2_Minus(s,22))
#define _Md23b(b,s) _Md22b(b,s) _Ma0b(b,MF2_Minus(s,23))
#define _Md24b(b,s) _Md23b(b,s) _Ma0b(b,MF2_Minus(s,24))
#define _Md25b(b,s) _Md24b(b,s) _Ma0b(b,MF2_Minus(s,25))
#define _Md26b(b,s) _Md25b(b,s) _Ma0b(b,MF2_Minus(s,26))
#define _Md27b(b,s) _Md26b(b,s) _Ma0b(b,MF2_Minus(s,27))
#define _Md28b(b,s) _Md27b(b,s) _Ma0b(b,MF2_Minus(s,28))
#define _Md29b(b,s) _Md28b(b,s) _Ma0b(b,MF2_Minus(s,29))
#define _Md30b(b,s) _Md29b(b,s) _Ma0b(b,MF2_Minus(s,30))
#define _Md31b(b,s) _Md30b(b,s) _Ma0b(b,MF2_Minus(s,31))
#if _MRP_CNTM0>=32
#define _Md32b(b,s) _Md31b(b,s) _Ma0b(b,MF2_Minus(s,32))
#define _Md33b(b,s) _Md32b(b,s) _Ma0b(b,MF2_Minus(s,33))
#define _Md34b(b,s) _Md33b(b,s) _Ma0b(b,MF2_Minus(s,34))
#define _Md35b(b,s) _Md34b(b,s) _Ma0b(b,MF2_Minus(s,35))
#define _Md36b(b,s) _Md35b(b,s) _Ma0b(b,MF2_Minus(s,36))
#define _Md37b(b,s) _Md36b(b,s) _Ma0b(b,MF2_Minus(s,37))
#define _Md38b(b,s) _Md37b(b,s) _Ma0b(b,MF2_Minus(s,38))
#define _Md39b(b,s) _Md38b(b,s) _Ma0b(b,MF2_Minus(s,39))
#define _Md40b(b,s) _Md39b(b,s) _Ma0b(b,MF2_Minus(s,40))
#define _Md41b(b,s) _Md40b(b,s) _Ma0b(b,MF2_Minus(s,41))
#define _Md42b(b,s) _Md41b(b,s) _Ma0b(b,MF2_Minus(s,42))
#define _Md43b(b,s) _Md42b(b,s) _Ma0b(b,MF2_Minus(s,43))
#define _Md44b(b,s) _Md43b(b,s) _Ma0b(b,MF2_Minus(s,44))
#define _Md45b(b,s) _Md44b(b,s) _Ma0b(b,MF2_Minus(s,45))
#define _Md46b(b,s) _Md45b(b,s) _Ma0b(b,MF2_Minus(s,46))
#define _Md47b(b,s) _Md46b(b,s) _Ma0b(b,MF2_Minus(s,47))
#define _Md48b(b,s) _Md47b(b,s) _Ma0b(b,MF2_Minus(s,48))
#define _Md49b(b,s) _Md48b(b,s) _Ma0b(b,MF2_Minus(s,49))
#define _Md50b(b,s) _Md49b(b,s) _Ma0b(b,MF2_Minus(s,50))
#define _Md51b(b,s) _Md50b(b,s) _Ma0b(b,MF2_Minus(s,51))
#define _Md52b(b,s) _Md51b(b,s) _Ma0b(b,MF2_Minus(s,52))
#define _Md53b(b,s) _Md52b(b,s) _Ma0b(b,MF2_Minus(s,53))
#define _Md54b(b,s) _Md53b(b,s) _Ma0b(b,MF2_Minus(s,54))
#define _Md55b(b,s) _Md54b(b,s) _Ma0b(b,MF2_Minus(s,55))
#define _Md56b(b,s) _Md55b(b,s) _Ma0b(b,MF2_Minus(s,56))
#define _Md57b(b,s) _Md56b(b,s) _Ma0b(b,MF2_Minus(s,57))
#define _Md58b(b,s) _Md57b(b,s) _Ma0b(b,MF2_Minus(s,58))
#define _Md59b(b,s) _Md58b(b,s) _Ma0b(b,MF2_Minus(s,59))
#define _Md60b(b,s) _Md59b(b,s) _Ma0b(b,MF2_Minus(s,60))
#define _Md61b(b,s) _Md60b(b,s) _Ma0b(b,MF2_Minus(s,61))
#define _Md62b(b,s) _Md61b(b,s) _Ma0b(b,MF2_Minus(s,62))
#define _Md63b(b,s) _Md62b(b,s) _Ma0b(b,MF2_Minus(s,63))
#define _Md64b(b,s) _Md63b(b,s) _Ma0b(b,MF2_Minus(s,64))
#endif

#define _Md__b(b,s)
#define _Md0_b(b,s) b(s)
#define _Md1_b(b,s) _Md0_b(b,s),_Ma0b(b,MF2_Minus(s,1))
#define _Md2_b(b,s) _Md1_b(b,s),_Ma0b(b,MF2_Minus(s,2))
#define _Md3_b(b,s) _Md2_b(b,s),_Ma0b(b,MF2_Minus(s,3))
#define _Md4_b(b,s) _Md3_b(b,s),_Ma0b(b,MF2_Minus(s,4))
#define _Md5_b(b,s) _Md4_b(b,s),_Ma0b(b,MF2_Minus(s,5))
#define _Md6_b(b,s) _Md5_b(b,s),_Ma0b(b,MF2_Minus(s,6))
#define _Md7_b(b,s) _Md6_b(b,s),_Ma0b(b,MF2_Minus(s,7))
#define _Md8_b(b,s) _Md7_b(b,s),_Ma0b(b,MF2_Minus(s,8))
#define _Md9_b(b,s) _Md8_b(b,s),_Ma0b(b,MF2_Minus(s,9))
#define _Md10_b(b,s) _Md9_b(b,s),_Ma0b(b,MF2_Minus(s,10))
#define _Md11_b(b,s) _Md10_b(b,s),_Ma0b(b,MF2_Minus(s,11))
#define _Md12_b(b,s) _Md11_b(b,s),_Ma0b(b,MF2_Minus(s,12))
#define _Md13_b(b,s) _Md12_b(b,s),_Ma0b(b,MF2_Minus(s,13))
#define _Md14_b(b,s) _Md13_b(b,s),_Ma0b(b,MF2_Minus(s,14))
#define _Md15_b(b,s) _Md14_b(b,s),_Ma0b(b,MF2_Minus(s,15))
#define _Md16_b(b,s) _Md15_b(b,s),_Ma0b(b,MF2_Minus(s,16))
#define _Md17_b(b,s) _Md16_b(b,s),_Ma0b(b,MF2_Minus(s,17))
#define _Md18_b(b,s) _Md17_b(b,s),_Ma0b(b,MF2_Minus(s,18))
#define _Md19_b(b,s) _Md18_b(b,s),_Ma0b(b,MF2_Minus(s,19))
#define _Md20_b(b,s) _Md19_b(b,s),_Ma0b(b,MF2_Minus(s,20))
#define _Md21_b(b,s) _Md20_b(b,s),_Ma0b(b,MF2_Minus(s,21))
#define _Md22_b(b,s) _Md21_b(b,s),_Ma0b(b,MF2_Minus(s,22))
#define _Md23_b(b,s) _Md22_b(b,s),_Ma0b(b,MF2_Minus(s,23))
#define _Md24_b(b,s) _Md23_b(b,s),_Ma0b(b,MF2_Minus(s,24))
#define _Md25_b(b,s) _Md24_b(b,s),_Ma0b(b,MF2_Minus(s,25))
#define _Md26_b(b,s) _Md25_b(b,s),_Ma0b(b,MF2_Minus(s,26))
#define _Md27_b(b,s) _Md26_b(b,s),_Ma0b(b,MF2_Minus(s,27))
#define _Md28_b(b,s) _Md27_b(b,s),_Ma0b(b,MF2_Minus(s,28))
#define _Md29_b(b,s) _Md28_b(b,s),_Ma0b(b,MF2_Minus(s,29))
#define _Md30_b(b,s) _Md29_b(b,s),_Ma0b(b,MF2_Minus(s,30))
#define _Md31_b(b,s) _Md30_b(b,s),_Ma0b(b,MF2_Minus(s,31))
#define _Md32_b(b,s) _Md31_b(b,s),_Ma0b(b,MF2_Minus(s,32))
#if _MRP_CNTM0>=32
#define _Md33_b(b,s) _Md32_b(b,s),_Ma0b(b,MF2_Minus(s,33))
#define _Md34_b(b,s) _Md33_b(b,s),_Ma0b(b,MF2_Minus(s,34))
#define _Md35_b(b,s) _Md34_b(b,s),_Ma0b(b,MF2_Minus(s,35))
#define _Md36_b(b,s) _Md35_b(b,s),_Ma0b(b,MF2_Minus(s,36))
#define _Md37_b(b,s) _Md36_b(b,s),_Ma0b(b,MF2_Minus(s,37))
#define _Md38_b(b,s) _Md37_b(b,s),_Ma0b(b,MF2_Minus(s,38))
#define _Md39_b(b,s) _Md38_b(b,s),_Ma0b(b,MF2_Minus(s,39))
#define _Md40_b(b,s) _Md39_b(b,s),_Ma0b(b,MF2_Minus(s,40))
#define _Md41_b(b,s) _Md40_b(b,s),_Ma0b(b,MF2_Minus(s,41))
#define _Md42_b(b,s) _Md41_b(b,s),_Ma0b(b,MF2_Minus(s,42))
#define _Md43_b(b,s) _Md42_b(b,s),_Ma0b(b,MF2_Minus(s,43))
#define _Md44_b(b,s) _Md43_b(b,s),_Ma0b(b,MF2_Minus(s,44))
#define _Md45_b(b,s) _Md44_b(b,s),_Ma0b(b,MF2_Minus(s,45))
#define _Md46_b(b,s) _Md45_b(b,s),_Ma0b(b,MF2_Minus(s,46))
#define _Md47_b(b,s) _Md46_b(b,s),_Ma0b(b,MF2_Minus(s,47))
#define _Md48_b(b,s) _Md47_b(b,s),_Ma0b(b,MF2_Minus(s,48))
#define _Md49_b(b,s) _Md48_b(b,s),_Ma0b(b,MF2_Minus(s,49))
#define _Md50_b(b,s) _Md49_b(b,s),_Ma0b(b,MF2_Minus(s,50))
#define _Md51_b(b,s) _Md50_b(b,s),_Ma0b(b,MF2_Minus(s,51))
#define _Md52_b(b,s) _Md51_b(b,s),_Ma0b(b,MF2_Minus(s,52))
#define _Md53_b(b,s) _Md52_b(b,s),_Ma0b(b,MF2_Minus(s,53))
#define _Md54_b(b,s) _Md53_b(b,s),_Ma0b(b,MF2_Minus(s,54))
#define _Md55_b(b,s) _Md54_b(b,s),_Ma0b(b,MF2_Minus(s,55))
#define _Md56_b(b,s) _Md55_b(b,s),_Ma0b(b,MF2_Minus(s,56))
#define _Md57_b(b,s) _Md56_b(b,s),_Ma0b(b,MF2_Minus(s,57))
#define _Md58_b(b,s) _Md57_b(b,s),_Ma0b(b,MF2_Minus(s,58))
#define _Md59_b(b,s) _Md58_b(b,s),_Ma0b(b,MF2_Minus(s,59))
#define _Md60_b(b,s) _Md59_b(b,s),_Ma0b(b,MF2_Minus(s,60))
#define _Md61_b(b,s) _Md60_b(b,s),_Ma0b(b,MF2_Minus(s,61))
#define _Md62_b(b,s) _Md61_b(b,s),_Ma0b(b,MF2_Minus(s,62))
#define _Md63_b(b,s) _Md62_b(b,s),_Ma0b(b,MF2_Minus(s,63))
#define _Md64_b(b,s) _Md63_b(b,s),_Ma0b(b,MF2_Minus(s,64))
#endif

#define __MF4_RepeatAsc(before,after,start,ms) _MA##ms##(before,after,start)
#define _MF4_RepeatAsc(before,after,start,len) __MF4_RepeatAsc(before,after,start,len)
/**
@brief 数字按升序重复，允许在每个重复的数字前或者数字后串接内容，start不得小于0，end不得大于64，且start<=end*/
#define MF4_RepeatAsc(before,after,start,end) _MF4_RepeatAsc(before,after,start,MF2_Minus(end,start))
#define __MF4_RepeatAsc_(before,after,start,ms) _MA##ms##_(before,after,start)
#define _MF4_RepeatAsc_(before,after,start,len) __MF4_RepeatAsc_(before,after,start,len)
/**
@brief 数字按升序重复，允许在每个重复的数字前或者数字后串接内容,重复内容之间用逗号间隔，start不得小于0，end不得大于64，且start<=end*/
#define MF4_RepeatAsc_(before,after,start,end) _MF4_RepeatAsc_(before,after,start,MF2_Minus(end,start))
#define __MF3_RepeatAsc(before,start,ms) _Ma##ms##b(before,start)
#define _MF3_RepeatAsc(before,start,len) __MF3_RepeatAsc(before,start,len)
/**
@brief 数字按升序重复，允许在每个重复的数字前添加内容,数字用括号括起来，start不得小于0，end不得大于64，且start<=end*/
#define MF3_RepeatAsc(before,start,end) _MF3_RepeatAsc(before,start,MF2_Minus(end,start))
#define __MF3_RepeatAsc_(before,start,ms) _Ma##ms##_b(before,start)
#define _MF3_RepeatAsc_(before,start,len) __MF3_RepeatAsc_(before,start,len)
/**
@brief 数字按升序重复，允许在每个重复的数字前添加内容,重复内容之间用逗号间隔,数字用括号括起来，start不得小于0，end不得大于64，且start<=end*/
#define MF3_RepeatAsc_(before,start,end) _MF3_RepeatAsc_(before,start,MF2_Minus(end,start))

#define __MF4_RepeatDec(before,after,start,ms) _MD##ms##(before,after,start)
#define _MF4_RepeatDec(before,after,start,len) __MF4_RepeatDec(before,after,start,len)
/**
@brief 数字按降序重复，允许在每个重复的数字前或者数字后串接内容，start不得大于64，end不得小于0，且start>=end*/
#define MF4_RepeatDec(before,after,start,end) _MF4_RepeatDec(before,after,start,MF2_Minus(start,end))
#define __MF4_RepeatDec_(before,after,start,ms) _MD##ms##_(before,after,start)
#define _MF4_RepeatDec_(before,after,start,len) __MF4_RepeatDec_(before,after,start,len)
/**
@brief 数字按降序重复，允许在每个重复的数字前或者数字后串接内容,重复内容之间用逗号间隔，start不得大于64，end不得小于0，且start>=end*/
#define MF4_RepeatDec_(before,after,start,end) _MF4_RepeatDec_(before,after,start,MF2_Minus(start,end))
#define __MF3_RepeatDec(before,start,ms) _Md##ms##b(before,start)
#define _MF3_RepeatDec(before,start,len) __MF3_RepeatDec(before,start,len)
/**
@brief 数字按降序重复，允许在每个重复的数字前添加内容,数字用括号括起来，start不得大于64，end不得小于0，且start>=end*/
#define MF3_RepeatDec(before,start,end) _MF3_RepeatDec(before,start,MF2_Minus(start,end))
#define __MF3_RepeatDec_(before,start,ms) _Md##ms##_b(before,start)
#define _MF3_RepeatDec_(before,start,len) __MF3_RepeatDec_(before,start,len)
/**
@brief 数字按降序重复，允许在每个重复的数字前添加内容,重复内容之间用逗号间隔,数字用括号括起来，start不得大于64，end不得小于0，且start>=end*/
#define MF3_RepeatDec_(before,start,end) _MF3_RepeatDec_(before,start,MF2_Minus(start,end))
