#ifndef LK_BITS_COMPARE_
#define LK_BITS_COMPARE_
namespace LK {
	namespace Bits{
		/**
		@brief 比较两块指定位区域的大小，从高位往低位比较
		@param [传入]参数p1表示用以进行比较的第一个数据指针
		@param [传入]参数startBitPositon1表示第一块区域的起始位偏移
		@param [传入]参数p2表示用以进行比较的第二个数据指针
		@param [传入]参数startBitPositon2表示第二块区域的起始位偏移
		@param [传入]参数bitsCount表示进行比较的位数量，当此参数为0时将直接返回0
		@return 返回值为1时，说明p1表示的内容大于p2代表的内容，返回值为-1时则小于，返回值为0时说明两个区域的内容完全一样
		@note p1和p2所表示的内存区域可以重叠*/
		index_t Compare(void const* p1, index_t startBitPositon1, void const* p2, index_t startBitPositon2, size_t bitsCount);
		/**
		@brief 按高位对齐（允许相对位移）比较两处指定的内容并返回偏差结果
		@param [传入]参数p1表示内容1的指针
		@param [传入]参数startBitPosition1表示内容1的起始位偏移（可以小于0）
		@param [传入]参数bitsCount1表示内容1的位数
		@param [传入]参数p2表示内容2的指针
		@param [传入]参数startBitPosition2表示内容2的起始位偏移（可以小于0）
		@param [传入]参数bitsCount2表示内容2的位数
		@param [传入,可选]参数mov表示内容2的最高位相对内容1的最高位的偏移量。当mov>0时，表示内容2的最高位比内容1的最高位的权重要大；当mov<0时则反之
		@return 当两者完全相等时返回0。如果内容1大于内容2则返回数字大于0，如果内容1小于内容2则返回数字小于0，而且返回数字的绝对值表示比较最高位到偏差位的位数（>=1）
		@note 比较时，两者最高位按mov指定的位移对齐。指定区域外的内容按0看待。*/
		index_t DifferenceIndexHigh(void const *p1, index_t startBitPosition1, size_t bitsCount1, void const*p2, index_t startBitPosition2, size_t bitsCount2, index_t mov = 0);
		/**
		@brief 比较两块内存区的大小，从高位往低位比较
		@param [传入]参数pSource表示用以进行比较的源数据
		@param [传入]参数startBitPositon表示源数据其实比特偏移
		@param [传入]参数bitsCount表示进行比较的比特数量
		@param [传入]参数pDestination表示用以进行比较的目标数据
		@param [传入,可选]参数destinationBitPosition表示目标其起始位置比特偏移量
		@return 返回值为1时，说明pSource表示的内容大于pDestination代表的内容，返回值为-1时则小于，返回值为0时说明两个区域的内容完全一样
		@note pSource和pDestination所表示的内存区域可以重叠，被移空的位置用0补充*/
		int Compare(void const* pSource, u64 startBitPositon, u64 bitsCount, void const* pDestination, u64 destinationBitPosition = 0);
		/**
		@brief 比较两块内存区的大小，从高位往低位比较
		@param [传入]参数pSource表示用以进行比较的源数据
		@param [传入]参数bitsCount表示进行比较的比特数量
		@param [传入]参数pDestination表示用以进行比较的目标数据
		@param [传入,可选]参数destinationBitPosition表示目标其起始位置比特偏移量
		@return 返回值为1时，说明pSource表示的内容大于pDestination代表的内容，返回值为-1时则小于，返回值为0时说明两个区域的内容完全一样
		@note pSource和pDestination所表示的内存区域可以重叠*/
		inline int  Compare(void* pSource, u64 bitsCount, void* pDestination, u64 destinationBitPosition = 0) { return Bits::Compare(pSource, 0, bitsCount, pDestination, destinationBitPosition); }
		/**
		@brief 比较两块内存区的大小，按高位对齐，从高位往低位比较
		@param [传入]参数p1表示用以进行比较的源数据（unsigned long long型数组）
		@param [传入]参数p2表示用以进行比较的目标数据（unsigned long long型数组）
		@param [传入]参数len1表示p1所代表数组的元素数
		@param [传入]参数len2表示p2所代表数组的元素数
		@return 返回值为1时，说明p1表示的内容大于p2代表的内容，返回值为-1时则小于，返回值为0时说明两个区域的内容完全一样
		@note p1和p2所表示的内存区域可以重叠.当len1和len2不等时，p1与p2各自代表的内容按高位对齐进行比较，末端用0补齐*/
		int CompareHigh64(cu64*p1, cu64*p2, size_t len1, size_t len2);

	}
}
#endif