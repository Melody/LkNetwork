#ifndef LK_BITS_GETWINDOW_
#define LK_BITS_GETWINDOW_
namespace LK {
	namespace Bits {
		/**
		@brief 以pData[0]的最低位的权重为0，进行64位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最低位的权重为0，截取并返回权重为bitsmov~bitsmov+63的64位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回pData[0]
		@note 当bitsmov为-1，len非0时，将返回pData[0]<<1
		@note 如果bitsmov为3，那么len为1时，将返回pData[0]>>3；len大于1时，将返回(pData[0]>>3)|(pData[1]<<61);*/
		u64 GetWindow64Low64(cu64*pData, size_t len, s64 bitsmov);
		/**
		@brief 以pData[len-1]的最高位的权重为0，进行64位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最高位的权重为0，截取并返回权重为bitsmov-63~bitsmov的64位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回pData[len-1]
		@note 当bitsmov为1，len非0时，将返回pData[len-1]>>1
		@note 如果bitsmov为-3，那么len为1时，将返回pData[len-1]<<3；len大于1时，将返回(pData[len-1]<<3)|(pData[len-2]>>61);*/
		u64 GetWindow64High64(cu64*pData, size_t len, s64 bitsmov);
		/**
		@brief 以pData[0]的最低位的权重为0，进行64位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最低位的权重为0，截取并返回权重为bitsmov~bitsmov+63的64位数据，超出数组范围的数据置0比如：
		@note 如果bitsmov为0，那么len为1时，将返回pData[0]；len大于1时，将返回pData[0] | (unsigned long long)pData[1]<<32
		@note 如果bitsmov为-1，那么len为1时，将返回pData[0]<<1；len大于1时，将返回pData[0]<<1 | (unsigned long long)pData[1]<<33
		@note 如果bitsmov为3，那么len为1时，将返回pData[0]>>3；len为2时，将返回(pData[0]>>3) | (unsigned long long)pData[1]<<29；len大于2时，将返回(pData[0]>>3) | (unsigned long long)pData[1]<<29| (unsigned long long)pData[2]<<61*/
		u64 GetWindow64Low32(cu32*pData, size_t len, long bitsmov);
		/**
		@brief 以pData[len-1]的最高位的权重为0，进行64位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long *型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最高位的权重为0，截取并返回权重为bitsmov-63~bitsmov的64位数据，超出数组范围的数据置0比如：
		@note 如果bitsmov为0，那么len为1时，将返回(unsigned long long)pData[len-1]<<32；len大于1时，将返回((unsigned long long)pData[len-1]<<32) | pData[len-2]
		@note 如果bitsmov为1，那么len为1时，将返回(unsigned long long)pData[len-1]<<31；len大于1时，将返回((unsigned long long)pData[len-1]<<31) | (pData[len-2]>>1)
		@note 如果bitsmov为-3，那么len为1时，将返回(unsigned long long)pData[len-1]<<35；len为2时，将返回((unsigned long long)pData[len-1]<<35) | ((unsigned long long)pData[len-2]<<3)；len大于2时，将返回((unsigned long long)pData[len-1]<<35) | ((unsigned long long)pData[len-2]<<3)| (pData[len-3]>>29)*/
		u64 GetWindow64High32(cu32*pData, size_t len, long bitsmov);
		/**
		@brief 以pData[0]的最低位的权重为0，进行32位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最低位的权重为0，截取并返回权重为bitsmov~bitsmov+31的32位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回*(unsigned long*)pData
		@note 当bitsmov为-1，len非0时，将返回*(unsigned long*)pData<<1
		@note 如果bitsmov为33，那么len为1时，将返回pData[0]>>33；len大于1时，将返回(pData[0]>>33)|(pData[1]<<29);*/
		u32 GetWindow32Low64(cu64*pData, size_t len, s64 bitsmov);
		/**
		@brief 以pData[len-1]的最高位的权重为0，进行32位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最高位的权重为0，截取并返回权重为bitsmov-31~bitsmov的32位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回(unsigned long)(pData[len-1]>>32)
		@note 当bitsmov为1，len非0时，将返回(unsigned long)(pData[len-1]>>33)*/
		u32 GetWindow32High64(cu64*pData, size_t len, s64 bitsmov);
		/**
		@brief 以pData[0]的最低位的权重为0，进行32位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最低位的权重为0，截取并返回权重为bitsmov~bitsmov+31的32位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回*pData
		@note 当bitsmov为-1，len非0时，将返回*pData<<1*/
		u32 GetWindow32Low32(cu32*pData, size_t len, long bitsmov);
		/**
		@brief 以pData[len-1]的最高位的权重为0，进行32位数据窗口截取
		@param [传入]参数pData表示需要进行截取的数组(unsigned long*型)
		@param [传入]参数len表示pData数组的元素数
		@param [传入]参数bitsmov表示截取窗向左（高位）位移量，当bitsmov小于0时表示向右位移
		@return 返回截取到的内容
		@note 当len为0时将直接返回0。
		@note 该函数认为pData数组最高位的权重为0，截取并返回权重为bitsmov-31~bitsmov的32位数据，超出数组范围的数据置0比如：
		@note 当bitsmov为0，len非0时，将返回pData[len-1]
		@note 当bitsmov为1，len非0时，将返回pData[len-1]>>1*/
		u32 GetWindow32High32(cu32*pData, size_t len, long bitsmov);

	}
}
#endif