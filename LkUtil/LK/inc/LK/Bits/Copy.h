#ifndef LK_BITS_COPY_
#define LK_BITS_COPY_
namespace LK {
	namespace Bits {
		/**
		@brief 将指定unsigned long long数组中的一系列比特数据复制到另一处数组的指定内存中，两处内存区域不可重叠
		@param [传入,写出]参数pDest表示目标数组（unsigned long long[]）
		@param [传入]参数destStartBitPositon表示起始写入位偏移，可以小于0
		@param [传入]参数pSource表示源数据数组（unsigned long long[]）
		@param [传入]参数sourceStartBitPositon表示源数据起始复制位偏移，可以小于0
		@param [传入]参数bitsCount表示要复制的比特数
		@warning 源数据区域和目标数据区域不可重叠。
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		 void Copy64(u64* pDest, s64 destStartBitPositon, cu64* pSource, s64 sourceStartBitPositon, u64 bitsCount);
		/**
		@brief 将指定unsigned long数组中的一系列比特数据复制到另一处数组的指定内存中，两处内存区域不可重叠
		@param [传入,写出]参数pDest表示目标数组（unsigned long[]）
		@param [传入]参数destStartBitPositon表示起始写入位偏移，可以小于0
		@param [传入]参数pSource表示源数据数组（unsigned long[]）
		@param [传入]参数sourceStartBitPositon表示源数据起始复制位偏移，可以小于0
		@param [传入]参数bitsCount表示要复制的比特数
		@warning 源数据区域和目标数据区域不可重叠。
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		 void Copy32(u32* pDest, s32 destStartBitPositon, cu32* pSource, s32 sourceStartBitPositon, u32 bitsCount);
		/**
		@brief 从pSource指向内存的startBitPositon位起复制bitsCount比特到pDestination指向内存的destinationBitPosition位处。
		@param [传入,写出]参数pDest指向目标内存区以复制内容
		@param [传入]参数destStartBitPositon表示目标写出起始位偏移（可以小于0）
		@param [传入]参数pSource指向源数据以供读取复制
		@param [传入]参数sourceStartBitPositon表示起始位偏移（可以小于0）
		@param [传入]参数bitsCount表示需要复制的比特数量
		@warning 源内存跟目标内存不能重叠，否则将出现意外
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		 void Copy(void* pDest, index_t destStartBitPositon, const void* pSource, index_t sourceStartBitPositon, size_t bitsCount);
	}
}
#endif