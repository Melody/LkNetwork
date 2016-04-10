#ifndef LK_BITS_SHIFT_
#define LK_BITS_SHIFT_
namespace LK {
	namespace Bits {
		/**
		@brief 进行128位的左移操作并返回高位左移结果
		@param [传入]参数high64表示高64bit数据
		@param [传入]参数low64表示低64bit数据
		@param [传入]参数mov表示左移位数
		@return 返回位移所得高64位部分
		@note 参数mov必须小于64，否则结果将是不确定的*/
		inline u64 ShiftLeft(u64 high64, u64 low64, u8 mov){
			return (high64 << mov) | (low64 >> (64 - mov));
		}
		/**
		@brief 进行128位的右移操作并返回低位右移结果
		@param [传入]参数high64表示高64bit数据
		@param [传入]参数low64表示低64bit数据
		@param [传入]参数mov表示右移位数
		@return 返回位移所得低64位部分
		@note 参数mov必须小于64，否则结果将是不确定的*/
		inline u64 ShiftRight(u64 high64, u64 low64, u8 mov){
			return (high64 << (64 - mov)) | (low64 >> mov);
		}
		/**
		@brief 将pSource所指向的sLen个内存块（每个内存块8字节）复制到pDest所指向的dLen个内存块中，并左移mov个比特位
		@param [传入]参数pSouece表示用以进行复制的源数据（unsigned long long数组）
		@param [传入]参数sLen表示源数据的元素数
		@param [传入,写出]参数pDest表示复制的目标内存（unsigned long long数组）
		@param [传入]参数dLen表示目标数组的元素数
		@param [传入]参数mov表示复制后继续进行的偏移量，mov>0时表示向左（高位）移动，mov<0表示向右（低位）移动。
		@note 超出范围的部分将被截断。被移空的位置用0补充，若目标左端（高位端）有空间剩余，则不做处理。复制源和目标位置可以重叠。*/
		void CopyShift64(cu64* pSource, size_t sLen, u64* pDest, size_t dLen, s64 mov);
	}
}
#endif