#ifndef LK_BITS_PLUS_
#define LK_BITS_PLUS_
namespace LK {
	namespace Bits {
		/**
		@brief 在p和len所代表的数组的指定位上加1，返回进位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表需要进行操作的数组p的元素数
		@param [传入]参数mov代表指定位偏移（0代表最低位），超出范围时将不起作用
		@return 当相加产生进位时返回true，否则返回false*/
		bool SelfPlusOne64(u64*p, u64 len, s64 mov);
		/**
		@brief 在p和len所代表的数组的指定位上加1（越下限时舍入），返回进位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表需要进行操作的数组元素数
		@param [传入]参数mov代表指定位偏移（0代表最低位），超出范围上限时将不起作用，低于下限1位时舍入，
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator++或者operator--运算*/
		bool SelfPlusOneRound64(u64*p, u64 len, s64 mov);
		/**
		@brief 在p和len所代表的数组的指定位上加上一个64位无符号数字num，返回进位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表需要进行操作的数组元素数
		@param [传入]参数num代表加数num
		@param [传入]参数mov代表num的位偏移（0代表最低位）。num按此偏移后超出len限制范围上下限的部分将被忽略
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=(int)或者operator-=(int)运算*/
		bool SelfPlusInt64(u64*p, u64 len, u64 num, s64 mov);
		/**
		@brief 在p和len所代表的数组的指定位上加上一个64位无符号数字num（越下限时舍入），返回进位
		@param [传入,写出]参数p代表需要进行操作的unsigned long long[]数组
		@param [传入]参数len代表需要进行操作的数组元素数
		@param [传入]参数num代表加数num
		@param [传入]参数mov代表num的位偏移（0代表最低位）。num按此偏移后超出len限制范围上限的部分将被忽略，低于下限的部分将进行自动舍入
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=(int)或者operator-=(int)运算*/
		bool SelfPlusIntRound64(u64*p, u64 len, u64 num, s64 mov);
		/**
		@brief 在pDest和lenDest所代表的数组的指定位上加上pSource和lenSource所指定的内容，返回进位
		@param [传入,写出]参数pDest代表需要进行操作的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行操作的pDest数组元素数
		@param [传入]参数pSource代表加数unsigned long long数组
		@param [传入]参数lenSource代表加数pSource数组元素数
		@param [传入]参数mov代表加数pSource数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。pSource按此偏移后超出len限制范围上下限的部分将被忽略
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=()或者operator-=()运算*/
		bool SelfPlus64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov);
		/**
		@brief 在pDest和lenDest所代表的数组的指定位上加上pSource和lenSource所指定的内容（越下限部分舍入），返回进位
		@param [传入,写出]参数pDest代表需要进行操作的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行操作的pDest数组元素数
		@param [传入]参数pSource代表加数unsigned long long数组
		@param [传入]参数lenSource代表加数pSource数组元素数
		@param [传入]参数mov代表加数pSource数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。pSource按此偏移后超出len限制范围上限的部分将被忽略，超出下限的部分将进行舍入操作
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+=()或者operator-=()运算*/
		bool SelfPlusRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的1叠加，然后将结果保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位）
		@param [传入]参数mov代表指定位相对pDest代表数组最低位的偏移（0代表最低位），超出范围时将不起作用
		@return 当相加产生进位时返回true，否则返回false*/
		bool PlusOne64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的1叠加，然后将结果保存到pDest和lenDest所代表的数组中（越下限部分舍入）
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位）
		@param [传入]参数mov代表指定位相对pDest代表数组最低位的偏移（0代表最低位），超出上限时将被忽略，低于下限1位时进行舍入
		@return 当相加产生进位时返回true，否则返回false*/
		bool PlusOneRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的64位数字num叠加，然后将结果保存到pDest和lenDest所代表的数组中
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位），超出上下限部分将被忽略
		@param [传入]参数mov代表指定位相对pDest代表数组最低位的偏移（0代表最低位），超出上下限部分将被忽略
		@return 当相加产生进位时返回true，否则返回false*/
		bool PlusInt64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov);
		/**
		@brief 将pSource和lenSource所代表的数组按指定位偏移，与按指定位偏移的64位数字num叠加，然后将结果保存到pDest和lenDest所代表的数组中（越下限部分舍入）
		@param [传入,写出]参数pDest代表需要进行写入的目标unsigned long long[]数组
		@param [传入]参数lenDest表示pDest数组元素数
		@param [传入]参数pSource表示加数数组
		@param [传入]参数lenSource表示pSource数组元素数
		@param [传入]参数num表示另一个加数
		@param [传入]参数movSource代表pSource代表数组最低位相对pDest代表数组最低位的偏移（0代表最低位），超出上下限部分将被忽略
		@param [传入]参数mov表示num相对pDest代表数组最低位的偏移（0代表最低位），超出上下限部分将被忽略
		@return 返回进位（有可能返回进位2）
		@note 该函数可以用于二进制数字的operator + 或者operator - 运算。该函数有可能返回2：当两个加数都是全1且都越下限时,即lenDest为0*/
		index_t PlusIntRound64(u64*pDest, u64 lenDest, cu64*pSource, u64 lenSource, u64 num, s64 movSource, s64 mov);
		/**
		@brief 在将p1和len1所代表的数组与p2和len2所代表的数组按指定指定偏移进行叠加，并将结果保存到pDest中，超出pDest和lenDest所表示范围的内容将被忽略
		@param [传入,写出]参数pDest代表需要进行输出的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行输出的pDest数组元素数
		@param [传入]参数p1代表加数1的unsigned long long[]数组
		@param [传入]参数len1代表加数1数组元素数
		@param [传入]参数p2代表加数2的unsigned long long[]数组
		@param [传入]参数len2代表加数2数组元素数
		@param [传入]参数mov1代表加数p1数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p1按此偏移后超出lenDest限制范围上下限的部分将被忽略
		@param [传入]参数mov2代表加数p2数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p2按此偏移后超出lenDest限制范围上下限的部分将被忽略
		@return 当相加产生进位时返回true，否则返回false
		@note 该函数可以用于二进制数字的operator+或者operator-运算*/
		bool Plus64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2);
		/**
		@brief 在将p1和len1所代表的数组与p2和len2所代表的数组按指定指定偏移进行叠加，并将结果保存到pDest中，超出pDest和lenDest所表示范围上限的内容将被忽略，超出下限的内容将进行舍入
		@param [传入,写出]参数pDest代表需要进行输出的unsigned long long[]数组
		@param [传入]参数lenDest代表需要进行输出的pDest数组元素数
		@param [传入]参数p1代表加数1的unsigned long long[]数组
		@param [传入]参数len1代表加数1数组元素数
		@param [传入]参数p2代表加数2的unsigned long long[]数组
		@param [传入]参数len2代表加数2数组元素数
		@param [传入]参数mov1代表加数p1数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p1按此偏移后超出lenDest限制范围上限的部分将被忽略，超出下限的内容将进行舍入
		@param [传入]参数mov2代表加数p2数组低位相对pDest数组低位的位偏移（0代表两者最低位对齐）。p2按此偏移后超出lenDest限制范围上限的部分将被忽略，超出下限的内容将进行舍入
		@return 返回进位
		@note 该函数可以用于二进制数字的operator+或者operator-运算。该函数有可能返回2：当两个加数都是全1且都越下限时*/
		index_t PlusRound64(u64*pDest, u64 lenDest, cu64*p1, u64 len1, cu64*p2, u64 len2, s64 mov1, s64 mov2);
		/**
		@brief 将pSource指向内存数据叠加到pDes指向内存中
		@param [传入,写出]参数pDest表示用以进行叠加和写出叠加结果的目标数组
		@param [传入]参数pSource表示用以进行叠加的源数组
		@param [传入]参数len表示元素数，要求pDes与pSource的元素数一样
		@return 如果相加结果溢出，那么将返回true，否则返回false
		@note 该函数可以用于二进制数字的operator++或者operator--运算*/
		bool Plus64(u64 *pDes, cu64* pSource, size_t len);
	}
}
#endif