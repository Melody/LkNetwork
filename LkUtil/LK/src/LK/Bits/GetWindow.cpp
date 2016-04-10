#include"stdafx.h"
#ifdef LK_BITS_GETWINDOW_
namespace LK{
	namespace Bits {
		u64 GetWindow64Low64(cu64*pData, size_t len, s64 bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov <= -64)return 0;
			if (bitsmov <= 0)return *pData << (-bitsmov);
			register size_t index = (size_t)(bitsmov >> 6);
			if (index >= len)return 0;
			if ((bitsmov &= 63) == 0)return pData[index];
			return index == len - 1 ? (pData[index] >> bitsmov) : ShiftRight(pData[index + 1], pData[index], (u8)bitsmov);
		}
		u64 GetWindow64High64(cu64*pData, size_t len, s64 bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov >= 64)return 0;
			if (bitsmov >= 0)return *(pData + len - 1) >> (bitsmov);
			size_t index = (size_t)(len - 1 + bitsmov / 64);
			if (index >= len)return 0;
			if ((bitsmov = (-bitsmov) & 63) == 0)return pData[index];
			return index == 0 ? (pData[0] << bitsmov) : ShiftLeft(pData[index], pData[index - 1], (u8)bitsmov);
		}
		u64 GetWindow64Low32(cu32*pData, size_t len, long bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov <= -64)return 0;
			if (bitsmov <= 0)
			{
				if (len > 1)return *(u64*)pData << (-bitsmov);
				return (u64)*pData << (-bitsmov);
			}
			register size_t index = bitsmov >> 5;
			if (index >= len)return 0;
			if ((bitsmov &= 31) == 0)return index == len - 1 ? pData[index] : *(u64 *)(pData + index);
			return index == len - 1 ? (pData[index] >> bitsmov) :
				index == len - 2 ? (*(u64 *)(pData + index) >> bitsmov) :
				ShiftRight(pData[index + 2], *(u64 *)(pData + index), (u8)bitsmov);
		}
		u64 GetWindow64High32(cu32*pData, size_t len, long bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov >= 64)return 0;
			if (bitsmov >= 0) { if (len == 1)return ((u64)*(pData + len - 1) << 32) >> bitsmov; return *(u64*)(pData + len - 2) >> bitsmov; }
			register size_t index = len - 1 + bitsmov / 32;
			if (index >= len)return 0;
			if ((bitsmov = (-bitsmov) & 31) == 0) { if (index == 0)return (u64)pData[index] << 32; return *(u64*)(pData + index - 1); }
			return index == 0 ? ((u64)pData[0] << (32 + bitsmov)) :
				index == 1 ? (*(u64*)pData << bitsmov) :
				(*(u64*)(pData + index - 1) << bitsmov) | (*(u64*)(pData + index - 2) >> (32 - bitsmov));
		}
		u32 GetWindow32Low64(cu64*pData, size_t len, s64 bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov <= -32)return 0;
			if (bitsmov <= 0)return (u32)(*pData << (-bitsmov));
			register size_t index = (size_t)(bitsmov >> 6);
			if (index >= len)return 0;
			if ((bitsmov &= 63) == 0)return (u32)pData[index];
			return (u32)(index == len - 1 ? (pData[index] >> bitsmov) :
				ShiftRight(pData[index + 1], pData[index], (u8)bitsmov));
		}
		u32 GetWindow32High64(cu64*pData, size_t len, s64 bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov >= 32)return 0;
			if (bitsmov >= 0)return (u32)(*(pData + len - 1) >> (bitsmov));
			size_t index = (size_t)(len - 1 + bitsmov / 64);
			if (index >= len)return 0;
			if ((bitsmov = (-bitsmov) & 63) == 0)return (u32)pData[index];
			return (index == 0 ? (pData[0] << bitsmov) : ((pData[index] << bitsmov) | ((pData[index - 1] >> (64 - bitsmov))))) >> 32;
		}
		u32 GetWindow32Low32(cu32*pData, size_t len, long bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov <= -32)return 0;
			if (bitsmov <= 0)return (u32)(*pData << (-bitsmov));
			register size_t index = (size_t)(bitsmov >> 5);
			if (index >= len)return 0;
			if ((bitsmov &= 31) == 0)return (u32)pData[index];
			return index == len - 1 ? (pData[index] >> bitsmov) : ((pData[index] >> bitsmov) | ((pData[index + 1] << (32 - bitsmov))));
		}
		u32 GetWindow32High32(cu32*pData, size_t len, long bitsmov)
		{
			if (len == 0)return 0;
			if (bitsmov >= 32)return 0;
			if (bitsmov >= 0)return *(pData + len - 1) >> (bitsmov);
			size_t index = (size_t)(len - 1 + bitsmov / 32);
			if (index >= len)return 0;
			if ((bitsmov = (-bitsmov) & 31) == 0)return pData[index];
			return index == 0 ? (pData[0] << bitsmov) : ((pData[index] << bitsmov) | ((pData[index - 1] >> (32 - bitsmov))));
		}
	}
}
#endif