#ifndef LK_BITS_GET_
#define LK_BITS_GET_
namespace LK {
	namespace Bits {
		/**
		@brief 获取某比特位上的内容
		@param [传入]参数pSource表示需要获取内存
		@param [传入]参数bitPosition表示获取位置比特偏移量
		@return 当指定位置的内容为1时返回true，否则返回false*/
		bool Get(const void* pSource, index_t bitPosition);
		/**
		@brief 在指定的比特位起获取size_t大小的内容
		@param [传入]参数pSource表示数据指针
		@param [传入]参数bitPosition表示起始位偏移
		@return 返回获取到的内容
		@note 该函数不做指针为NULL检查*/
		size_t GetX(const void* pSource, index_t bitPosition);

	}
}
#endif