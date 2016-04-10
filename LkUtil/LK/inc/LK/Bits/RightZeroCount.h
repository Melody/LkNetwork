#ifndef LK_BITS_RIGHTZEROCOUNT_
#define LK_BITS_RIGHTZEROCOUNT_
namespace LK {
	namespace Bits {
		inline size_t RightZeroCount8(u8 i) {
			static u8 const arr[256] =
			{ 8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,
				0,5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0 };
			return arr[i];
		}
		/**
		@brief 获取i后导0的比特数（64位）
		@param [传入]参数i表示一个64位的被测数字
		@return 返回该数字低位为0的bit数量，比如RightZeroCount64(0)得到64，RightZeroCount64(0x10)得到4*/
		index_t RightZeroCount64(u64 i);
		/**
		@brief 获取i后导0的比特数（32位）
		@param [传入]参数i表示一个32位的被测数字
		@return 返回该数字低位为0的bit数量，比如RightZeroCount32(0)得到32，RightZeroCount32(0x20)得到5*/
		index_t RightZeroCount32(u32 i);
		/**
		@brief 获取i后导0的比特数（32位）
		@param [传入]参数i表示一个32位的被测数字
		@return 返回该数字低位为0的bit数量，比如RightZeroCount32(0)得到32，RightZeroCount32(0x30)得到4
		@note 该函数为重载函数的一个版本，函数会根传入参数类型进行重载：
		@note 当传入参数为unsigned long或者unsigned int时将调用RightZeroCount32函数，
		@note 当传入参数类型为unsigned long long时将调用RightZeroCount64函数*/
		inline index_t RightZeroCount(unsigned int i) { return RightZeroCount32((u32)i); }
		inline index_t RightZeroCount(u32 i) { return RightZeroCount32(i); }
		inline index_t RightZeroCount(u64 i) { return RightZeroCount64(i); }
	}
}
#endif