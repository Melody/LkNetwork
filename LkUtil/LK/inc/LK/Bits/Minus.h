#ifndef LK_BITS_MINUS_
#define LK_BITS_MINUS_
namespace LK {
	namespace Bits {
		/**
		@brief 在p和len所代表的数组的指定位上减1，返回借位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表数组p的元素数
		@param [传入]参数mov代表指定位偏移（0代表最低位，1代表左移1位，-1代表右移1位），超出范围时将不起作用
		@return 当相减产生借位时返回true，否则返回false*/
		bool SelfMinusOne64(u64*p, u64 len, s64 mov);
		/**
		@brief 在p和len所代表的数组的指定位上减1，返回借位（下限舍入）
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表数组p的元素数
		@param [传入]参数mov代表指定位偏移（0代表最低位，1代表左移1位，-1代表最低位右移1位），超出数组上限时将不起作用，低于数组下限1位时对结果进行舍入操作
		@return 当相减产生借位时返回true，否则返回false
		@note 由于低于下限1位时舍入后又进位，因此该函数跟SelfMinusOne64完全一样*/
		inline bool SelfMinusOneRound64(u64*p, u64 len, s64 mov) { return SelfMinusOne64(p, len, mov); }
		/**
		@brief 将p和len所代表的数组减去一个按指定位移量偏移的数字，返回借位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表数组p的元素数
		@param [传入]参数num代表减数
		@param [传入]参数mov代表num位移量（0代表num最低位与数组最低位对齐，1代表左移1位，-1代表最低位右移1位），位移后超出数组范围的部分将不起作用
		@return 当相减产生借位时返回true，否则返回false*/
		bool SelfMinusInt64(u64*p, u64 len, u64 num, s64 mov);
		/**
		@brief 将p和len所代表的数组减去一个按指定位移量偏移的数字num，返回借位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表数组p的元素数
		@param [传入]参数num代表减数
		@param [传入]参数mov代表num位移量（0代表num最低位与数组最低位对齐，1代表左移1位，-1代表最低位右移1位），位移后超出数组上限的部分将不起作用，超出下限的部分进行舍入
		@return 当相减产生借位时返回true，否则返回false*/
		bool SelfMinusIntRound64(u64*p, u64 len, u64 num, s64 mov);
		/**
		@brief 在pDest和lenDest所代表的数组的指定位上减去pSource和lenSource所指定的内容，按低位对齐，返回借位
		@param [传入,写出]参数pDest代表需要进行操作的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行操作的pDest数组元素数
		@param [传入]参数pSource代表减数unsigned long long数组
		@param [传入]参数lenSource代表减数pSource数组元素数
		@param [传入]参数mov代表减数pSource数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。pSource按此偏移后超出len限制范围上下限的部分将被忽略
		@return 当相减产生借位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=()或者operator-=()运算*/
		bool SelfMinus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov);
		/**
		@brief 在pDest和lenDest所代表的数组的指定位上减去pSource和lenSource所指定的内容，按低位对齐，返回借位
		@param [传入,写出]参数pDest代表需要进行操作的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行操作的pDest数组元素数
		@param [传入]参数pSource代表减数unsigned long long数组
		@param [传入]参数lenSource代表减数pSource数组元素数
		@return 当相减产生借位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=()或者operator-=()运算*/
		bool SelfMinus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource);
		/**
		@brief 在pDest和lenDest所代表的数组的指定位上减去pSource和lenSource所指定的内容（越下限部分舍入），返回借位
		@param [传入,写出]参数pDest代表需要进行操作的unsigned long long[]数组（被减数）
		@param [传入]参数lenDest代表需要进行操作的pDest数组元素数
		@param [传入]参数pSource代表减数unsigned long long数组
		@param [传入]参数lenSource代表减数pSource数组元素数
		@param [传入]参数mov代表减数pSource数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。pSource按此偏移后超出len限制范围上限的部分将被忽略，超出下限的部分将进行舍入操作
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=()或者operator-=()运算*/
		bool SelfMinusRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的1相减，然后将结果保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0表示两者最低位对齐）
		@param [传入]参数mov代表指定位相对pDest代表数组最低位的偏移（0代表最低位），超出范围时将不起作用
		@return 当相减产生借位时返回true，否则返回false*/
		bool MinusOne64(u64 *pDest, u64 lenDset, cu64*pSource, u64 lenSource, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的1相减，然后将结果保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0表示两者最低位对齐），超出上限时将不起作用，低于下限部分对结果进行舍入
		@param [传入]参数mov代表要进行-1操作的位相对pDest代表数组最低位的偏移（0代表最低位），超出上限时将不起作用，低于下限部分对结果进行舍入
		@return 当相减舍入产生进位时返回1，产生借位时返回-1，否则返回0*/
		index_t MinusOneRound64(u64 *pDest, u64 lenDset, cu64*pSource, u64 lenSource, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的64位数字num相减，然后将结果保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数，两者共同表示数组在内存中的区域
		@param [传入]参数pSource表示被减数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数num表示减数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位），超出写出数组上下限部分将被忽略
		@param [传入]参数mov代表num相对pDest代表数组最低位的偏移（0代表最低位），位移后超出写出数组上下限部分将被忽略
		@return 当相减产生借位时返回true，否则返回false*/
		bool MinusInt64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的64位数字num相减，然后将结果舍入后保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数，两者共同表示数组在内存中的区域
		@param [传入]参数pSource表示被减数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数num表示减数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位），超出写出数组上限部分将被忽略,低于下限部分计算结果进行舍入
		@param [传入]参数mov代表num相对pDest代表数组最低位的偏移（0代表最低位），位移后超出写出数组上限部分将被忽略，低于下限部分计算结果进行舍入
		@return 当舍入产生进位时返回1，产生借位时返回-1，其它返回0*/
		index_t MinusIntRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov);
		/**
		@brief 在将p1和len1所代表的数组与p2和len2所代表的数组分别按指定偏移量偏移后进行相减，并将结果保存到pDest中，超出pDest和lenDest所表示范围的内容将被忽略
		@param [传入,写出]参数pDest代表输出的目标unsigned long long[]数组
		@param [传入]参数lenDest代表输出的目标pDest数组元素数
		@param [传入]参数p1代表被减数的unsigned long long[]数组
		@param [传入]参数len1代表被减数数组元素数
		@param [传入]参数p2代表减数的unsigned long long[]数组
		@param [传入]参数len2代表减数数组元素数
		@param [传入]参数mov1代表被减数p1数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p1按此偏移后超出lenDest限制范围上下限的部分将被忽略
		@param [传入]参数mov2代表减数p2数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p2按此偏移后超出lenDest限制范围上下限的部分将被忽略
		@return 当相减产生借位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+或者operator-运算*/
		bool Minus64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2);
		/**
		@brief 在将p1和len1所代表的数组与p2和len2所代表的数组分别按指定偏移量偏移后进行相减，并将结果按舍入保存到pDest中
		@param [传入,写出]参数pDest代表输出的目标unsigned long long[]数组
		@param [传入]参数lenDest代表输出的目标pDest数组元素数
		@param [传入]参数p1代表被减数的unsigned long long[]数组
		@param [传入]参数len1代表被减数数组元素数
		@param [传入]参数p2代表减数的unsigned long long[]数组
		@param [传入]参数len2代表减数数组元素数
		@param [传入]参数mov1代表被减数p1数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p1按此偏移后超出lenDest限制范围上限的部分将被忽略，超出下限的部分按结果舍入
		@param [传入]参数mov2代表减数p2数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p2按此偏移后超出lenDest限制范围上限的部分将被忽略，超出下限的部分按结果舍入
		@return 当舍入产生进位时返回1，相减产生借位时返回-1，其它返回0
		@note 该函数可以用于二进制数字的operator+或者operator-运算*/
		index_t MinusRound64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2);

	}
}
#endif