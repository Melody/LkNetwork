#ifndef LK_BITS_MULTIPLY_
#define LK_BITS_MULTIPLY_
namespace LK {
	namespace Bits {
		/**
		@brief 将无符号整数m1和m2乘积的结果的高64bit保存到p所指向的内存中，并返回低64位内容
		@param [传入]参数m1表示64位无符号整数（乘数1）
		@param [传入]参数m2表示64位无符号整数（乘数2）
		@param [写出]参数p表示用以输出高64bit相乘结果
		@return 返回低64bit结果*/
		inline u64 Multiply(u64 m1, u64 m2, u64 *p){
#ifdef __INTRIN_H_//优先采用内部函数
#ifdef _M_X64
			return _umul128(m1, m2, p);
#endif
#endif
			u64 tmp1 = (((u64)(u32)m1)*((u64)((u32)m2))) >> 32;
			u64 tmp2 = ((u32)m1)*(m2 >> 32);
			u64 tmp3 = ((u32)m2)*(m1 >> 32);
			u64 tmp4 = tmp1 + (u32)tmp2 + (u32)tmp3;
			*p = (m1 >> 32)*(m2 >> 32) + (tmp2 >> 32) + (tmp3 >> 32) + (tmp4 >> 32);
			return m1*m2;
		}
		/**
		@brief 将p和len所代表的数组乘上一个32位整数num，按低位对齐，并返回溢出内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位乘数
		@return 返回相乘后溢出的内容，即相乘的结果由返回值和数组p共同表示*/
		u32 SelfMultiplyInt32Low32(u32*p, size_t len, u32 num);
		/**
		@brief 将p和len所代表的数组乘上一个32位整数num，按低位对齐，并返回溢出内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位乘数
		@return 返回相乘后溢出的内容，即相乘的结果由返回值和数组p共同表示*/
		inline u32 SelfMultiplyInt32Low64(u64*p, size_t len, u32 num) { return SelfMultiplyInt32Low32((u32*)p, len << 1, num); }
		/**
		@brief 将p和len所代表的数组乘上一个64位整数num，按低位对齐，并返回溢出内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位乘数
		@return 返回相乘后溢出的内容，即相乘的结果由返回值和数组p共同表示*/
		u64 SelfMultiplyInt64Low32(u32*p, size_t len, u64 num);
		/**
		@brief 将p和len所代表的数组乘上一个64位整数num，按低位对齐，并返回溢出内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位乘数
		@return 返回相乘后溢出的内容，即相乘的结果由返回值和数组p共同表示*/
		u64 SelfMultiplyInt64Low64(u64*p, size_t len, u64 num);
		/**
		@brief 将p和len所代表的数组乘上一个32位整数num，按高位对齐，并返回截去内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位乘数
		@return 返回相乘后低于下限截去的内容，即相乘的结果由数组p和返回值共同表示*/
		u32 SelfMultiplyInt32High32(u32*p, size_t len, u32 num);
		/**
		@brief 将p和len所代表的数组乘上一个32位整数num，按高位对齐，并返回截去内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位乘数
		@return 返回相乘后低于下限截去的内容，即相乘的结果由数组p和返回值共同表示*/
		inline u32 SelfMultiplyInt32High64(u64*p, size_t len, u32 num) { return SelfMultiplyInt32High32((u32*)p, len << 1, num); }
		/**
		@brief 将p和len所代表的数组乘上一个64位整数num，按高位对齐，并返回下限截去内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位乘数
		@return 返回相乘后下限截去的内容，即相乘的结果由数组p和返回值共同表示*/
		u64 SelfMultiplyInt64High32(u32*p, size_t len, u64 num);
		/**
		@brief 将p和len所代表的数组乘上一个64位整数num，按高位对齐，并返回下限截去内容
		@param [传入,写出]参数p表示准备进行乘法操作的数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位乘数
		@return 返回相乘后下限截去的内容，即相乘的结果由数组p和返回值共同表示*/
		u64 SelfMultiplyInt64High64(u64*p, size_t len, u64 num);
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个32位整数num，按低位对齐，并返回pDest的溢出内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个32位整数（乘数2）
		@return 返回相乘后溢出的32bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按低位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		u32 MultiplyInt32Low32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num);
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个32位整数num，按低位对齐，并返回pDest的溢出内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个32位整数（乘数2）
		@return 返回相乘后溢出的32bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按低位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		inline u32 MultiplyInt32Low64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u32 num){
			return MultiplyInt32Low32((u32*)pDest, lenDest << 1, (u32*)pSource, lenSource << 1, num);
		}
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个64位整数num，按低位对齐，并返回pDest的溢出内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个64位整数（乘数2）
		@return 返回相乘后溢出的64bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按低位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		u64 MultiplyInt64Low32(u32*pDest, size_t lenDest, cu32*pSource, size_t lenSource, u64 num);
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个64位整数num，按低位对齐，并返回pDest的溢出内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个64位整数（乘数2）
		@return 返回相乘后溢出的64bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按低位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		u64 MultiplyInt64Low64(u64*pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num);
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个32位整数num，按高位对齐，并返回pDest下限截去内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个32位整数（乘数2）
		@return 返回相乘后pDest下限截去的32bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按高位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		u32 MultiplyInt32High32(u32*pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num);
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个32位整数num，按高位对齐，并返回pDest下限截去内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示乘数1
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个32位整数（乘数2）
		@return 返回相乘后pDest下限截去的32bit内容，即相乘的结果由返回值和数组pDest共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按高位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		inline u32 MultiplyInt32High64(u64*pDest, size_t lenDest, cu64*pSource, size_t lenSource, u32 num) { return MultiplyInt32High32((u32*)pDest, lenDest << 1, (u32*)pSource, lenSource << 1, num); }
		/**
		@brief 将pSource和lenSource所代表的数组乘上一个64位整数num，按高位对齐，并返回pDest下限截去内容
		@param [写出]参数pDest表示准备进行结果写出的数组
		@param [传入]参数lenDest表示数组pDest的元素数
		@param [传入]参数pSource表示一个unsigned long long数组（乘数1）
		@param [传入]参数len表示数组pSource的元素数
		@param [传入]参数num表示一个64位整数（乘数2）
		@return 返回相乘后pDest下限截去的64bit内容，即相乘的结果由数组pDest和返回值共同表示
		@note 如果pDest数组大小不足以容纳输出结果，那么将按高位对齐输出尽量多的结果
		@note 源数据与目标数据不可重叠*/
		u64 MultiplyInt64High64(u64*pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按低位对齐，并返回溢出的32bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned long数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned long数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@return 当pDest数组无法容纳乘积结果时，返回溢出的32位数值，否则返回0
		@note 源数据与目标数据不可重叠*/
		u32 MultiplyLow32(u32*pDest, size_t lenDest, cu32* pm1, size_t len1, cu32*pm2, size_t len2);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按低位对齐，并返回溢出的64bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned long long数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned long long数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@return 当pDest数组无法容纳乘积结果时，返回溢出的32位数值，否则返回0
		@note 源数据与目标数据不可重叠*/
		u64 MultiplyLow64(u64*pDest, size_t lenDest, cu64* pm1, size_t len1, cu64*pm2, size_t len2);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按低位对齐，并返回溢出的8bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned char数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned char数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@param [传入,可选]参数ptmp指向计算使用的临时内存，要求其大小不小于sizeof(Math::DoubleComplex)*len * 2，其中len为len1与len2中较大者对其到2的n次方的值的2倍
		@return 当pDest数组无法容纳乘积结果时，返回溢出的8位数值，否则返回0
		@note 源数据与目标数据可以重叠
		@note 使用该函数必须包含LK::Equation::DoubleComplex类型
		@note 由于FFT计算使用的是double型数据，受其精度限制，要求输入的数组总字节数都不得超过2^32*/
		u8 MultiplyFFTLow8(u8*pDest, size_t lenDest, cu8* pm1, size_t len1, cu8*pm2, size_t len2, u8*ptmp = NULL);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按高对齐，并返回低位截取掉的8bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned short数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned short数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@param [传入,可选]参数ptmp指向计算使用的临时内存，要求其大小不小于sizeof(Math::DoubleComplex)*len * 2，其中len为len1与len2中较大者对其到2的n次方的值的2倍
		@return 当pDest数组无法容纳乘积结果时，返回低位截取掉的8位数值，否则返回0
		@note 源数据与目标数据可以重叠
		@note 使用该函数必须包含LK::Equation::DoubleComplex类型
		@note 由于FFT计算使用的是double型数据，受其精度限制，要求输入的数组总字节数不得超过2^32,否则请考虑MultiplyFFTHigh32*/
		u8 MultiplyFFTHigh8(u8*pDest, size_t lenDest, cu8* pm1, size_t len1, cu8*pm2, size_t len2, u8*ptmp = NULL);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按低位对齐，并返回溢出的16bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned short数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned short数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@param [传入,可选]参数ptmp指向计算使用的临时内存，要求其大小不小于sizeof(Math::DoubleComplex)*len * 2，其中len为len1与len2中较大者对其到2的n次方的值的2倍
		@return 当pDest数组无法容纳乘积结果时，返回溢出的16位数值，否则返回0
		@note 源数据与目标数据可以重叠
		@note 使用该函数必须包含LK::Equation::DoubleComplex类型
		@note 由于FFT计算使用的是double型数据，受其精度限制，要求输入的数组元素数都不得超过2^20*/
		u16 MultiplyFFTLow(u16*pDest, size_t lenDest, cu16* pm1, size_t len1, cu16*pm2, size_t len2, u8*ptmp = NULL);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按高对齐，并返回低位截取掉的16bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned short数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned short数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@param [传入,可选]参数ptmp指向计算使用的临时内存，要求其大小不小于sizeof(Math::DoubleComplex)*len * 2，其中len为len1与len2中较大者对其到2的n次方的值的2倍
		@return 当pDest数组无法容纳乘积结果时，返回低位截取掉的16位数值，否则返回0
		@note 源数据与目标数据可以重叠
		@note 使用该函数必须包含LK::Equation::DoubleComplex类型
		@note 由于FFT计算使用的是double型数据，受其精度限制，要求输入的数组总比特数不得超过2^16,否则请考虑MultiplyFFTHigh32*/
		u16 MultiplyFFTHigh(u16*pDest, size_t lenDest, cu16* pm1, size_t len1, cu16*pm2, size_t len2, u8*ptmp = NULL);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按高对齐，并返回低位截取掉的32bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned long long数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned long long数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@param [传入,可选]参数ptmp指向计算使用的临时内存，要求其大小不小于sizeof(Math::Complex128)*len * 2，其中len为len1与len2中较大者对其到2的n次方的值的2倍
		@return 当pDest数组无法容纳乘积结果时，返回低位截取掉的64位数值，否则返回0
		@note 源数据与目标数据可以重叠
		@note 使用该函数必须包含LK::Equation::Complex128类型*/
		u32 MultiplyFFTHigh32(u32*pDest, size_t lenDest, cu32* pm1, size_t len1, cu32*pm2, size_t len2, u8*ptmp = NULL);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按高位对齐，并返回下限截取掉的32bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned long数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned long数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@return 当pDest数组无法容纳乘积结果时，返回下限截取掉的数值的高32位，否则返回0
		@note 源数据与目标数据不可重叠*/
		u32 MultiplyHigh32(u32*pDest, size_t lenDest, cu32* pm1, size_t len1, cu32*pm2, size_t len2);
		/**
		@brief 将数组pm1[len1]与数组pm2[len2]相乘的结果保存到数组pDest[lenDest]中，三者按高位对齐，并返回下限截取掉的64bit内容
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pm1表示一个unsigned long long数组（乘数1）
		@param [传入]参数len1表示pm1数组的元素数
		@param [传入]参数pm2表示一个unsigned long long数组（乘数2）
		@param [传入]参数len2表示pm2数组的元素数
		@return 当pDest数组无法容纳乘积结果时，返回下限截取掉的数值的高64位，否则返回0
		@note 源数据与目标数据不可重叠
		@note 认为乘数、商的最高位指数均为-1*/
		u64 MultiplyHigh64(u64*pDest, size_t lenDest, cu64* pm1, size_t len1, cu64*pm2, size_t len2);
	}
}
#endif