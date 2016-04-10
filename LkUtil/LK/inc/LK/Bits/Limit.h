#ifndef LK_BITS_LIMIT_
#define LK_BITS_LIMIT_
namespace LK {
	namespace Bits {
		/**
		@brief 将p和len所代表数组指定位置及其后的内容置0并舍入
		@param [传入,写出]参数p代表要进行操作的unsigned long long数组
		@param [传入]参数len代表数组p的元素数
		@param [传入]参数mov表示指定位置与最高位的偏移量，当mov>0时全内容置0，当mov==0时对最高位进行舍入操作
		@return 返回进位*/
		bool SelfLimitBitsRoundHigh64(u64* p, u64 len, s64 mov);

	}
}
#endif