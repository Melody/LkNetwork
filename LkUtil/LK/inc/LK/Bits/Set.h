#ifndef LK_BITS_SET_
#define LK_BITS_SET_
namespace LK {
	namespace Bits {
		/**
		@brief 将p所指内存从startBitPosition位置起的bitsCount比特全部设置为bitValue的值。
		@param [传入,写出]参数p指向目标内存区以写入内容
		@param [传入]参数startBitPositon表示起始比特偏移
		@param [传入]参数bitsCount表示需要设置的比特数量
		@param [传入,可选]参数bitValue表示目标改变值，默认为false*/
		void Set(void* p, index_t startBitPosition, size_t bitsCount, bool b);


	}
}
#endif