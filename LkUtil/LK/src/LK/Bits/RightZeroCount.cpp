#include"stdafx.h"
#ifdef LK_BITS_RIGHTZEROCOUNT_
namespace LK{
	namespace Bits {
		index_t Bits::RightZeroCount64(u64 i){
			if ((u32)i != 0){
				if ((u16)i != 0){
					if ((u8)i != 0)return RightZeroCount8((u8)i); return 8 + RightZeroCount8((u8)(i >> 8));
				}if ((i & 0xffffff) != 0)return 16 + RightZeroCount8((u8)(i >> 16)); return 24 + RightZeroCount8((u8)(i >> 24));
			}if ((i & 0xffffffffffff) != 0){
				if ((i & 0xffffffffff) != 0)return 32 + RightZeroCount8((u8)(i >> 32)); return 40 + RightZeroCount8((u8)(i >> 40));
			}if ((i & 0xffffffffffffff) != 0)return 48 + RightZeroCount8((u8)(i >> 48)); return 56 + RightZeroCount8((u8)(i >> 56));
		}
		index_t Bits::RightZeroCount32(u32 i){
			if ((u16)i != 0){
				if ((u8)i != 0)return RightZeroCount8((u8)i); return 8 + RightZeroCount8((u8)(i >> 8));
			}if ((i & 0xffffff) != 0)return 16 + RightZeroCount8((u8)(i >> 16)); return 24 + RightZeroCount8((u8)(i >> 24));
		}
	}
}
#endif