#ifndef LK_BITS_LEFTZEROCOUNT_
#define LK_BITS_LEFTZEROCOUNT_
namespace LK {
	namespace Bits {
		inline size_t LeftZeroCount8(u8 i) {
			static u8 const arr[256] =
			{ 8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
				2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
			return arr[i];
		}
		/**
		@brief 获取i前导0的比特数（32位）
		@param [传入]参数n表示一个32位的被测数字
		@return 返回该数字高位为0的bit数量，比如LeftZeroCount32(0)得到32，LeftZeroCount32(9)得到28*/
		index_t LeftZeroCount32(u32 n);
		
		/**
		@brief 获取i前导0的比特数（64位）
		@param [传入]参数n表示一个64位的被测数字
		@return 返回该数字高位为0的bit数量，比如LeftZeroCount64(0)得到64，LeftZeroCount64(0x11)得到59*/
		index_t LeftZeroCount64(u64 n);
		/*获取i前导0的比特数（32位保守算法）*/
		index_t _LeftZeroCount32(u32 i);
		/*获取i前导0的比特数（64位保守算法）*/
		index_t _LeftZeroCount64(u64 i);
		/**
		@brief 获取i前导0的比特数（重载以自动匹配位数）
		@param [传入]参数i为被测数字
		@return 返回被测数字高位0的位数
		@note 该函数为重载函数的一个版本，函数会根传入参数类型进行重载：
		@note 当传入参数为unsigned long或者unsigned int时将调用LeftZeroCount32函数，
		@note 当传入参数类型为unsigned long long时将调用LeftZeroCount64函数*/
		inline index_t LeftZeroCount(u64 i){return LeftZeroCount64(i);}
		inline index_t LeftZeroCount(u32 i){return LeftZeroCount32(i);}
		inline index_t LeftZeroCount(ui32 i) { return LeftZeroCount((u32)i); }
		//32位高速算法1
		inline index_t _1LeftZeroCount32(u32 i){
#ifdef __INTRIN_H_
#ifdef _M_IX86
			return 1 == 0 ? 32 : (31 ^ __lzcnt(i));
#else
#ifdef _M_X64
			return i == 0 ? 32 : (31 ^ __lzcnt(i));
#endif
#endif
#endif
			return 0;
		}
		//64位高速算法1
		inline index_t _1LeftZeroCount64(u64 i){
#ifdef __INTRIN_H_
#ifdef _M_IX86
			if (i < 0x100000000)
				return i == 0 ? 64 : (63 ^ __lzcnt((u32)i));
			return 31 ^ __lzcnt((unsigned int)(i >> 32));
#else
#ifdef _M_X64
			return i == 0 ? 64 : (63 ^ __lzcnt64(i));
#endif
#endif
#endif
			return 0;
		}
		//32位高速算法2
		inline index_t _2LeftZeroCount32(u32 i){
#ifdef __INTRIN_H_
#ifdef _M_IX86
			return i == 0 ? 32 : __lzcnt(i);
#else
#ifdef _M_X64
			return i == 0 ? 32 : __lzcnt(i);
#endif
#endif
#endif
			return 0;
		}
		//64位高速算法2
		inline index_t _2LeftZeroCount64(u64 i){
#ifdef __INTRIN_H_
#ifdef _M_IX86
			if (i < 0x100000000)
				return i == 0 ? 64 : __lzcnt((u32)i);
			return __lzcnt((unsigned int)(i >> 32));
#else
#ifdef _M_X64
			return i == 0 ? 64 : __lzcnt64(i);
#endif
#endif
#endif
			return 0;
		}

	}
}
#endif