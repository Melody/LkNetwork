#ifndef LK_BITS_MOVE_
#define LK_BITS_MOVE_
namespace LK {
	namespace Bits {
		/**
		@brief 将指定unsigned long long数组中的一系列比特数据复制到另一处数组的指定内存中，两处内存区域可以重叠
		@param [传入,写出]参数pDest表示目标数组（unsigned long long[]）
		@param [传入]参数destStartBitPositon表示起始写入位偏移，可以小于0
		@param [传入]参数pSource表示源数据数组（unsigned long long[]）
		@param [传入]参数sourceStartBitPositon表示源数据起始复制位偏移，可以小于0
		@param [传入]参数bitsCount表示要复制的比特数
		@note 源数据区域和目标数据区域可以重叠。
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		void Move64(u64* pDest, s64 destStartBitPositon, cu64* pSource, s64 sourceStartBitPositon, u64 bitsCount);
		/**
		@brief 将指定unsigned long数组中的一系列比特数据复制到另一数组的指定内存中，两处内存区域可以重叠
		@param [传入,写出]参数pDest表示目标数组（unsigned long[]）
		@param [传入]参数destStartBitPositon表示起始写入位偏移，可以小于0
		@param [传入]参数pSource表示源数据数组（unsigned long[]）
		@param [传入]参数sourceStartBitPositon表示源数据起始复制位偏移，可以小于0
		@param [传入]参数bitsCount表示要复制的bit数量
		@note 源数据区域和目标数据区域可以重叠。
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		void Move32(u32* pDest, s32 destStartBitPositon, cu32* pSource, s32 sourceStartBitPositon, u32 bitsCount);
		/**
		@brief 将指定内存中的一系列比特数据复制到另一数组的指定内存中，两处内存区域可以重叠
		@param [传入,写出]参数pDest表示目标内存
		@param [传入]参数destStartBitPositon表示起始写入位偏移，可以小于0
		@param [传入]参数pSource表示源数据内存
		@param [传入]参数sourceStartBitPositon表示源数据起始复制位偏移，可以小于0
		@param [传入]参数bitsCount表示要复制的比特数
		@note 源数据区域和目标数据区域可以重叠。
		@note 该函数能精确地进行位写入，而不影响写入比特位之外的内容*/
		void Move(void* pDest, index_t destStartBitPositon, void const* pSource, index_t sourceStartBitPositon, size_t bitsCount);
		/**
		@brief 将p指向的内存的startBitPosition比特位起bitsCount比特的数据进行mov比特位移
		@param [传入,写出]参数p指向目标内存
		@param [传入]参数startBitPosition表示起始比特偏移
		@param [传入]参数bitsCount表示操作的总比特位数
		@param [传入]参数mov表示移动的比特数当mov大于0时表示进行左移（往高位移动），当mov小于0时表示右移（往低位移动）*/
		void Move(void* p, u64 startBitPosition, u64 bitsCount, s64 mov);
		/**
		@brief 将p指向的len个无符号内存块进行左移mov个比特。
		@param [传入,写出]参数p表示操作目标数组
		@param [传入]参数len表示p所表示数组的元素数
		@param [传入]参数mov表示移动偏移量。若mov<0表示右移（往低位），若mov>0表示左移（往高位）
		@return 返回移出的64bit内容
		@note 超出范围部分将被截断，被移空的位置用0补充*/
		u64 Move64(u64* p, size_t len, s64 mov);
	}
}
#endif