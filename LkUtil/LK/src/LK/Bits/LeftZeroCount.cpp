#include"stdafx.h"
#ifdef LK_BITS_LEFTZEROCOUNT_
namespace LK{
	namespace Bits {
		//测试函数
		index_t LeftZeroCount32(u32 u){
			static index_t(*leftZeroCount32)(u32);
			if (leftZeroCount32)return leftZeroCount32(u);
			u32 test = 1;
			int i = 0;
			for (; i < 32; ++i, test <<= 1)if (_1LeftZeroCount32(test) != _LeftZeroCount32(test))break;
			if (i == 32) { return (leftZeroCount32 = _1LeftZeroCount32)(u);}
			test = 1; i = 0;
			for (; i < 32; ++i, test <<= 1)if (_2LeftZeroCount32(test) != _LeftZeroCount32(test))break;
			if (i == 32) { return (leftZeroCount32 = _2LeftZeroCount32)(u); }
			return (leftZeroCount32 = _LeftZeroCount32)(u);
		}
		index_t LeftZeroCount64(u64 u){
			static index_t(*leftZeroCount64)(u64);
			if (leftZeroCount64)return leftZeroCount64(u);
			//测试高速算法是否准确
			u64 test = 1;
			int i = 0;
			for (; i < 64; ++i, test <<= 1)if (_1LeftZeroCount64(test) != _LeftZeroCount64(test))break;
			if (i == 64) { return (leftZeroCount64 = _1LeftZeroCount64)(u); }
			test = 1; i = 0;
			for (; i < 64; ++i, test <<= 1)if (_2LeftZeroCount64(test) != _LeftZeroCount64(test))break;
			if (i == 64) { return (leftZeroCount64 = _2LeftZeroCount64)(u); }
			return (leftZeroCount64 = _LeftZeroCount64)(u);
		}
		index_t _LeftZeroCount32(u32 i){
			if (i < 0x10000){
				if (i < 0x100)return 24 + LeftZeroCount8((u8)i); return 16 + LeftZeroCount8((u8)(i >> 8));
			}if (i < 0x1000000)return 8 + LeftZeroCount8((u8)(i >> 16)); return LeftZeroCount8((u8)(i >> 24));
		}
		index_t _LeftZeroCount64(u64 i){
			if (i < 0x100000000){
				if (i < 0x10000){
					if (i < 0x100)return 56 + LeftZeroCount8((u8)i); return 48 + LeftZeroCount8((u8)(i >> 8));
				}if (i < 0x1000000)return 40 + LeftZeroCount8((u8)(i >> 16)); return 32 + LeftZeroCount8((u8)(i >> 24));
			}if (i < 0x1000000000000){
				if (i < 0x10000000000)return 24 + LeftZeroCount8((u8)(i >> 32));	return 16 + LeftZeroCount8((u8)(i >> 40));
			}if (i < 0x100000000000000)return 8 + LeftZeroCount8((u8)(i >> 48)); return LeftZeroCount8((u8)(i >> 56));
		}
	}
}
#endif