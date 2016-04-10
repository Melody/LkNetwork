#ifndef LK_BITS_DIVIDE_
#define LK_BITS_DIVIDE_
namespace LK {
	namespace Bits {
		/**
		@brief 将high64和low64表示的128位无符号整数除以一个64位无符号整数num，返回所得结果的低64位数据，余数保存到p所指向变量中
		@param [传入]参数high64表示被除数的高64bit
		@param [传入]参数low64表示被除数的低64bit
		@param [传入]参数num表示64位无符号除数
		@param [写出,可选]参数p用以输出余数
		@return 返回商
		@note 如果num为0将导致错误*/
		u64 Divide(u64 high64, u64 low64, u64 num, u64*p = NULL);
		/**
		@brief 将p[len]数组除以一个32位无符号整数num，（按低位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u32 SelfDivideInt32Low32(u32 *p, size_t len, u32 num);
		/**
		@brief 将p[len]数组除以一个32位无符号整数num，（按低位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u32 SelfDivideInt32Low64(u64*p, size_t len, u32 num);
		/**
		@brief 将p[len]数组除以一个64位无符号整数num，（按低位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u64 SelfDivideInt64Low32(u32 *p, size_t len, u64 num);
		/**
		@brief 将p[len]数组除以一个64位无符号整数num，（按低位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u64 SelfDivideInt64Low64(u64*p, size_t len, u64 num);
		/**
		@brief 将p[len]数组除以一个32位无符号整数num，（按高位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个32位无符号除数
		@param [传入]参数mov为true时，认为最高位权重为0；mov为false时，认为最高位权重为-1
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u32 SelfDivideInt32High32(u32 *p, size_t len, u32 num, bool mov = false);
		/**
		@brief 将p[len]数组除以一个64位无符号整数num，（按高位对齐），返回余数
		@param [传入,写出]参数p代表被除数数组
		@param [传入]参数len表示数组p的元素数
		@param [传入]参数num表示一个64位无符号除数
		@param [传入]参数mov为true时，认为最高位权重为0；mov为false时，认为最高位权重为-1
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u64 SelfDivideInt64High64(u64 *p, size_t len, u64 num, bool mov = false);
		/**
		@brief 将pSource[lenSource]数组除以一个32位无符号整数num，并将所得结果存储到数组pDest[lenDest]中，（按低位对齐），返回余数
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pSource代表被除数
		@param [传入]参数lenSource表示数组pSource的元素数
		@param [传入]参数num表示一个32位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u32 DivideInt32Low32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num);
		/**
		@brief 将pSource[lenSource]数组除以一个64位无符号整数num，并将所得结果存储到数组pDest[lenDest]中，（按低位对齐），返回余数
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pSource代表被除数
		@param [传入]参数lenSource表示数组pSource的元素数
		@param [传入]参数num表示一个64位无符号除数
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u64 DivideInt64Low64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num);
		/**
		@brief 将pSource[lenSource]数组除以一个32位无符号整数num，并将所得结果存储到数组pDest[lenDest]中，（按高位对齐），返回余数
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pSource代表被除数
		@param [传入]参数lenSource表示数组pSource的元素数
		@param [传入]参数num表示一个32位无符号除数
		@param [传入]参数mov为true时，表示商最高位权重为-1；false表示商最高位权重为0
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u32 DivideInt32High32(u32 *pDest, size_t lenDest, cu32*pSource, size_t lenSource, u32 num, bool mov = false);
		/**
		@brief 将pSource[lenSource]数组除以一个64位无符号整数num，并将所得结果存储到数组pDest[lenDest]中，（按高位对齐），返回余数
		@param [写出]参数pDest代表用以输出结果的数组
		@param [传入]参数lenDest表示pDest数组的元素数
		@param [传入]参数pSource代表被除数
		@param [传入]参数lenSource表示数组pSource的元素数
		@param [传入]参数num表示一个64位无符号除数
		@param [传入]参数mov为true时，表示商最高位权重为-1；false表示商最高位权重为0
		@return 当除数和被除数都不为0时返回余数，否则返回0
		@note 除数num为0时不会抛出异常*/
		u64 DivideInt64High64(u64 *pDest, size_t lenDest, cu64*pSource, size_t lenSource, u64 num, bool mov = false);
		/**
		@brief 将p1[len1]除以p2[len2]的结果存储到p[len]数组中（按低位对齐）
		@param [写出]参数p代表结果数组
		@param [传入]参数len表示p数组的元素数
		@param [传入]参数p1代表被除数
		@param [传入]参数bitsCount1表示p1数组有数据比特数
		@param [传入]参数p2代表被除数
		@param [传入]参数bitsCount表示p2数组有数据比特数
		@param [写出,可选]参数pd代表余数数组
		@param [传入,可选]参数lend表示pd数组元素数
		@param [传入,可选]参数ptmp可用以提供一块计算专用临时内存，要求元素数不小于(len2 + 1) * 2*/
		void DivideLow64(u64*p, size_t len, cu64*p1, u64 bitsCount1, cu64*p2, u64 bitsCount2, u64 *pd = NULL, size_t lend = 0, u64*ptmp = NULL);
		/**
		@brief 将p1[len1]除以p2[len2]的结果存储到p[len]数组中（按高位对齐）
		@param [写出]参数p代表结果数组
		@param [传入]参数len表示p数组的元素数
		@param [传入]参数p1代表被除数
		@param [传入]参数len1表示p1数组元素数
		@param [传入]参数p2代表被除数
		@param [传入]参数len2表示p2数组元素数
		@param [传入,可选]参数mov为true时表示商最高位权重为0，否则为-1
		@param [写出,可选]参数pd代表余数数组（高位对齐）
		@param [传入,可选]参数lend表示pd数组元素数
		@param [传入,可选]参数ptmp可用以提供一块计算专用临时内存，要求元素数不小于(len2 + 1) * 2
		@note 商的长度参数len2必须要匹配数组的有内容数据，要求最高位不为0*/
		void DivideHigh64(u64*p, size_t len, cu64*p1, u64 len1, cu64*p2, u64 len2, bool mov = false, u64 *pd = NULL, size_t lend = 0, u64*ptmp = NULL);
		/**
		@brief 将p1[len1]除以p2[len2]的结果存储到p[len]数组中（按高位对齐）
		@param [写出]参数p代表结果数组
		@param [传入]参数len表示p数组的元素数
		@param [传入]参数bits表示生商位数
		@param [传入]参数p1代表被除数
		@param [传入]参数len1表示p1数组元素数
		@param [传入]参数p2代表被除数
		@param [传入]参数len2表示p2数组元素数
		@param [传入,可选]参数mov为true时表示商最高位权重为0，否则为-1
		@param [写出,可选]参数pd代表余数数组（高位对齐）
		@param [传入,可选]参数lend表示pd数组元素数
		@param [传入,可选]参数ptmp可用以提供一块计算专用临时内存，要求元素数不小于(len2 + 1) * 2
		@note 商的长度参数len2必须要匹配数组的有内容数据，要求最高位不为0
		@note 通常使用该函数指定产生整数的生商位数来求余
		@note 余数的最高位指数*/
		void ModHigh64(u64*p, size_t len, u64 bits, cu64*p1, u64 len1, cu64*p2, u64 len2, bool mov = false, u64 *pd = NULL, size_t lend = 0, u64*ptmp = NULL);
	}
}
#endif