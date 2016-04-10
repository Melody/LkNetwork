#include"stdafx.h"
#ifdef LK_BITS_COMPARE_
namespace LK{
	namespace Bits {
		index_t Compare(void const* pa, index_t startBitPosition1, void const* pb, index_t startBitPosition2, size_t bitsCount){
			if (bitsCount == 0)return 0;
			startBitPosition1 += ((size_t)pa&(sizeof(size_t) - 1)) * 8;
			size_t* p1 = (size_t*)(((size_t)pa)&(~(sizeof(size_t) - 1)));
			startBitPosition2 += ((size_t)pb&(sizeof(size_t) - 1)) * 8;
			size_t* p2 = (size_t*)(((size_t)pb)&(~(sizeof(size_t) - 1)));
			if (startBitPosition1 < 0) { p1 += (startBitPosition1 - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); startBitPosition1 &= (sizeof(size_t) * 8) - 1; }
			else if (startBitPosition1 >= (index_t)(sizeof(size_t) * 8)) { p1 += startBitPosition1 / ((index_t)sizeof(size_t) * 8); startBitPosition1 &= (sizeof(size_t) * 8) - 1; }
			if (startBitPosition2 < 0) { p2 += (startBitPosition2 - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); startBitPosition2 &= (sizeof(size_t) * 8) - 1; }
			else if (startBitPosition2 >= (index_t)(sizeof(size_t) * 8)) { p2 += startBitPosition2 / ((index_t)sizeof(size_t) * 8); startBitPosition2 &= (sizeof(size_t) * 8) - 1; }

			size_t bmv1 = bitsCount + startBitPosition1;
			size_t bmv2 = bitsCount + startBitPosition2;

			if (bitsCount <= sizeof(size_t) * 8){
				size_t num1 = *(size_t*)p1 >> startBitPosition1;
				size_t num2 = *(size_t*)p2 >> startBitPosition2;
				if (startBitPosition1 + bitsCount > sizeof(size_t) * 8)num1 |= *((size_t*)p1 + 1) << ((sizeof(size_t) * 8) - startBitPosition1);
				if (startBitPosition2 + bitsCount > sizeof(size_t) * 8)num2 |= *((size_t*)p2 + 1) << ((sizeof(size_t) * 8) - startBitPosition2);
				if (bitsCount != (sizeof(size_t) * 8)){
					num1 &= ((size_t)-1) >> ((sizeof(size_t) * 8) - bitsCount);
					num2 &= ((size_t)-1) >> ((sizeof(size_t) * 8) - bitsCount);
				}
				return num1 > num2 ? 1 : num1 < num2 ? -1 : 0;
			}

			size_t bc1 = bmv1 & ((sizeof(size_t) * 8) - 1);
			size_t bc2 = bmv2 & ((sizeof(size_t) * 8) - 1);

			bmv1 /= sizeof(size_t) * 8;
			bmv2 /= sizeof(size_t) * 8;

			size_t* sp1 = (size_t*)p1 + bmv1;
			size_t* sp2 = (size_t*)p2 + bmv2;

			if (bc1 != 0){
				size_t num1 = *sp1 << ((sizeof(size_t) * 8) - bc1);
				size_t num2 = bc2 == 0 ? 0 : *sp2 << ((sizeof(size_t) * 8) - bc2);
				if (bc1 >= bc2)num2 |= (*(sp2 - 1)) >> bc2;
				num2 &= ((size_t)-1) << ((sizeof(size_t) * 8) - bc1);
				if (num1 != num2)return num1 > num2 ? 1 : -1;
			}
			if (sp1 != p1) {
				if (bc1 > bc2){
					size_t bc = bc1 - bc2;
					while (--sp1 != p1){
						--sp2;
						size_t num1 = *sp1;
						size_t num2 = (*sp2 << bc) | (*(sp2 - 1) >> ((sizeof(size_t) * 8) - bc));
						if (num1 != num2)return num1 > num2 ? 1 : -1;
					}
				}
				else if (bc1 == bc2){
					while (--sp1 != p1){
						size_t num1 = *sp1;
						size_t num2 = *--sp2;
						if (num1 != num2)return num1 > num2 ? 1 : -1;
					}
				}
				else{
					size_t bc = bc2 - bc1;
					while (--sp1 != p1){
						size_t num1 = *sp1;
						size_t num2 = (*sp2 << ((sizeof(size_t) * 8) - bc)) | (*(sp2 - 1) >> bc);
						if (num1 != num2)return num1 > num2 ? 1 : -1;
						--sp2;
					}
				}
			}
			size_t num1 = *(size_t*)p1 >> startBitPosition1;
			size_t num2 = *(size_t*)p2 >> startBitPosition2;
			if (startBitPosition2 != 0 && (startBitPosition1 == 0 || startBitPosition1 < startBitPosition2))num2 |= *((size_t*)p2 + 1) << ((sizeof(size_t) * 8) - startBitPosition2);
			num2 &= ((size_t)-1) >> startBitPosition1;
			return num1 > num2 ? 1 : num1 < num2 ? -1 : 0;
		}
		index_t DifferenceIndexHigh(void const *pa, index_t startBitPosition1, size_t bitCount1, void const*pb, index_t startBitPosition2, size_t bitCount2, index_t mov){
			startBitPosition1 += ((size_t)pa&(sizeof(size_t) - 1)) * 8;
			size_t* p1 = (size_t*)((size_t)pa&(~(sizeof(size_t) - 1)));
			startBitPosition2 += ((size_t)pb&(sizeof(size_t) - 1)) * 8;
			size_t* p2 = (size_t*)((size_t)pb&(~(sizeof(size_t) - 1)));

			if (startBitPosition1 < 0) { (size_t*&)p1 += (startBitPosition1 - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); startBitPosition1 &= (sizeof(size_t) * 8) - 1; }
			else if (startBitPosition1 >= (index_t)(sizeof(size_t) * 8)) { (size_t*&)p1 += startBitPosition1 / ((index_t)sizeof(size_t) * 8); startBitPosition1 &= (sizeof(size_t) * 8) - 1; }
			if (startBitPosition2 < 0) { (size_t*&)p2 += (startBitPosition2 - (index_t)(sizeof(size_t) * 8 - 1)) / ((index_t)sizeof(size_t) * 8); startBitPosition2 &= (sizeof(size_t) * 8) - 1; }
			else if (startBitPosition2 >= (index_t)(sizeof(size_t) * 8)) { (size_t*&)p2 += startBitPosition2 / ((index_t)sizeof(size_t) * 8); startBitPosition2 &= (sizeof(size_t) * 8) - 1; }


			size_t abc1 = startBitPosition1 + bitCount1;
			size_t abc2 = startBitPosition2 + bitCount2;

			size_t* ps1 = (size_t*)p1 + (abc1 == 0 ? 0 : (abc1 - 1) / (sizeof(size_t) * 8));//指针
			size_t* ps2 = (size_t*)p2 + (abc2 == 0 ? 0 : (abc2 - 1) / (sizeof(size_t) * 8));

			size_t tbc1 = 0;//已读取比特数
			size_t tbc2 = 0;
			index_t state = 0;

			abc1 &= sizeof(size_t) * 8 - 1;//高位比特偏移
			abc2 &= sizeof(size_t) * 8 - 1;

			for (;;){
				size_t num1;
				if (tbc1 >= bitCount1) { if (tbc2 >= bitCount2)break; num1 = 0; }
				else{
					if (mov >= ((index_t)sizeof(size_t) * 8)) { num1 = 0; }
					else{
						if (mov <= 0)
							num1 = abc1 == 0 ? *ps1 : ((*ps1 << (sizeof(size_t) * 8 - abc1)) | (*(ps1 - 1) >> abc1));
						else{
							if (abc1 == 0)
								num1 = tbc1 == 0 ? (*ps1 >> mov) : (*(ps1 + 1) << (sizeof(size_t) * 8 - mov)) | (*ps1 >> mov);
							else{
								if (tbc1 == 0)
									num1 = ((*ps1 << (sizeof(size_t) * 8 - abc1)) | (*(ps1 - 1) >> abc1)) >> mov;
								else{
									if (mov + abc1 > sizeof(size_t) * 8)num1 = (*(ps1 + 1) << (sizeof(size_t) * 16 - (abc1 + mov))) | (*ps1 >> (mov + abc1 - (sizeof(size_t) * 8)));
									else if (mov + abc1 == sizeof(size_t) * 8)num1 = *ps1;
									else num1 = (*ps1 << ((sizeof(size_t) * 8) - mov - abc1)) | (*(ps1 - 1) >> (abc1 + mov));
								}
							}
						}
						if (tbc1 == 0 && mov > 0){
							if (tbc1 + sizeof(size_t) * 8 - mov > bitCount1)
								num1 &= ((size_t)-1) << (tbc1 - mov - bitCount1 + sizeof(size_t) * 8);
						}
						else if (tbc1 + sizeof(size_t) * 8 > bitCount1)
							num1 &= ((size_t)-1) << (tbc1 - bitCount1 + sizeof(size_t) * 8);
						--ps1;
					}
				}
				size_t num2;
				if (tbc2 >= bitCount2) { num2 = 0; }
				else{
					if (mov <= (-(index_t)sizeof(size_t) * 8)) { num2 = 0; }
					else{
						if (mov >= 0)
							num2 = abc2 == 0 ? *ps2 : ((*ps2 << (sizeof(size_t) * 8 - abc2)) | (*(ps2 - 1) >> abc2));
						else{
							if (abc2 == 0)
								num2 = tbc2 == 0 ? (*ps2 >> -mov) : (*(ps2 + 1) << (sizeof(size_t) * 8 + mov)) | (*ps2 >> -mov);
							else{
								if (tbc2 == 0)
									num2 = ((*ps2 << (sizeof(size_t) * 8 - abc2)) | (*(ps2 - 1) >> abc2)) >> -mov;
								else{
									if (abc2 - mov > sizeof(size_t) * 8)num2 = (*(ps2 + 1) << (sizeof(size_t) * 16 - (abc2 - mov))) | (*ps2 >> (abc2 - mov - (sizeof(size_t) * 8)));
									else if (abc2 - mov == sizeof(size_t) * 8)num2 = *ps2;
									else num2 = (*ps2 << ((sizeof(size_t) * 8) + mov - abc2)) | (*(ps2 - 1) >> (abc2 - mov));
								}
							}
						}
						if (tbc2 == 0 && mov<0){
							if (tbc2 + sizeof(size_t) * 8 + mov> bitCount2)
								num2 &= ((size_t)-1) << (tbc2 + mov - bitCount2 + sizeof(size_t) * 8);
						}
						else if (tbc2 + sizeof(size_t) * 8 > bitCount2)
							num2 &= ((size_t)-1) << (tbc2 - bitCount2 + sizeof(size_t) * 8);
						--ps2;
					}
				}
				if (state == 0){
					if (num1 != num2){
						if (num1 > num2){
							if (num1 != num2 + 1)
								return (tbc1 > tbc2 ? tbc1 : tbc2) + LeftZeroCount(num1 - num2) + 1;
							state = 1;
						}
						else{
							if (num2 != num1 + 1)
								return -(index_t)(tbc1 > tbc2 ? tbc1 : tbc2) - LeftZeroCount(num2 - num1) - 1;
							state = -1;
						}
					}
				}
				else if (state == 1){
					if (num1 != 0 || num2 != (size_t)-1){
						if (tbc1 < tbc2) tbc1 = tbc2;
						if (num1 < num2)return tbc1 + LeftZeroCount(num1 - num2) + 1;
						return tbc1;
					}
				}
				else  if (num2 != 0 || num1 != (size_t)-1){
					if (tbc1 > tbc2) tbc2 = tbc1;
					if (num2 < num1)return -(index_t)tbc2 - LeftZeroCount(num2 - num1) - 1;
					return -(index_t)tbc2;
				}

				if (mov >= (index_t)sizeof(size_t) * 8){
					mov -= sizeof(size_t) * 8;
					tbc2 += sizeof(size_t) * 8;
				}
				else if (mov <= -(index_t)sizeof(size_t) * 8){
					mov += sizeof(size_t) * 8;
					tbc1 += sizeof(size_t) * 8;
				}
				else{
					if (mov > 0){
						if (tbc1 == 0)tbc1 = -mov;
					}
					else if (mov < 0){
						if (tbc2 == 0)tbc2 = mov;
					}
					tbc1 += sizeof(size_t) * 8;
					tbc2 += sizeof(size_t) * 8;
				}
			}
			if (state == 0)
				return 0;
			if (state == 1)return (tbc1>tbc2 ? tbc1 : tbc2);
			return -(index_t)(tbc1 > tbc2 ? tbc1 : tbc2);
		}
		int CompareHigh64(cu64*p1, cu64*p2, size_t len1, size_t len2){
			while (len1 != 0 && len2 != 0){
				--len1; --len2;
				if (p1[len1] != p2[len2])return p1[len1] > p2[len2] ? 1 : -1;
			}
			if (len1 == 0) { for (size_t i = 0; i < len2; ++i)if (p2[i] != 0)return -1; return 0; }
			for (size_t i = 0; i < len1; ++i)if (p1[i] != 0)return 1;
			return 0;
		}
		int Compare(void const* pSource, u64 startBitPositon, u64 bitsCount, void const* pDestination, u64 destinationBitPosition){//比较两串二进制序列大小，从高位往低位比较
			u64 num1, num2;
			while (bitsCount){
				if (bitsCount >= 64){
					Copy(&num1, 0, pSource, (index_t)(startBitPositon + bitsCount - 64), 64); Copy(&num2, 0, pDestination, (index_t)(destinationBitPosition + bitsCount - 64), 64); bitsCount -= 64;
				}
				else { num1 = num2 = 0; Copy(&num1, 0, pSource, (index_t)startBitPositon, (size_t)bitsCount); Copy(&num2, 0, pDestination, (index_t)destinationBitPosition, (size_t)bitsCount); bitsCount = 0; }
				if (num1 > num2)return 1;
				if (num1 < num2)return -1;
			}
			return 0;
		}
	}
}
#endif