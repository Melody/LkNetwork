#ifndef LK_BITS_BITPTR_
#define LK_BITS_BITPTR_
namespace LK
{
	namespace Bits {
		/**
		@brief 表示一个以位为偏移单位的指针*/
		struct BitPtr{
			/**
			@brief 用以获取ptrValue*/
			static const size_t PtrValueMask = ((size_t)-1) ^ (sizeof(size_t) - 1);
			/**
			@brief 用以截取字节指针偏移*/
			static const size_t PtrMoveMask = sizeof(size_t) - 1;
			/**
			@brief 内部位偏移量最大值*/
			static const size_t BitMoveMax = ((sizeof(size_t) - 1) << 3) | 7;
			/**
			@brief 从指针进行构造*/
			BitPtr(const void* p) :ptrValue((size_t)p&PtrValueMask), bitMove(((size_t)p&PtrMoveMask) << 3) {}
			/**
			@brief 从指针和位偏移量进行构造*/
			BitPtr(const void* p, index_t bitShift){
				if (bitShift < 0) { (u8*&)p += (bitShift + 1) / 8 - 1; }
				else if (bitShift >= 8) { (u8*&)p += bitShift >> 3; }
				ptrValue = (size_t)p&PtrValueMask;
				bitMove = (((size_t)p&PtrMoveMask) << 3) | (bitShift & 7);
			}
			/**
			@brief 直接传入成员数值进行构造*/
			BitPtr(size_t ptrVal, size_t bitMov) :ptrValue(ptrVal), bitMove(bitMov) {}
			/**
			@brief 获取size_t*指针*/
			size_t* GetPtr() { return (size_t*)ptrValue; }
			size_t const* GetPtr()const { return (size_t const*)ptrValue; }
			/**
			@brief 获取该指针所指定比特内容
			@return 如果指定位有内容则返回true
			@note 该操作不做指针为NULL检查*/
			bool Get()const { return (((*(size_t*)ptrValue) >> bitMove) & 1) != 0; }
			/**
			@brief 在该位指针所指向的位置起往高位获取n bit(n为1~64之间)内容
			@param[传入，可省]参数n表示需要获取的位数，默认为64
			@return 返回获取的内容*/
			u64 Get64(size_t n = 64)const {
				if (bitMove == 0)return *(u64*)ptrValue&(0xffffffffffffffff >> (64 - n));
				if (bitMove + n>64)
					return ((*(u64*)ptrValue >> bitMove) | (*((u64*)ptrValue + 1) << (64 - bitMove)))&(0xffffffffffffffff >> (64 - n));
				return (*(u64*)ptrValue >> bitMove) &(0xffffffffffffffff >> (64 - n));
			}
			/**
			@brief 将该位指针指定位置起往高64bit的内容设置为ull
			@param [传入]参数ull表示设定内容
			@note 该操作不做指针为NULL检查*/
			void Set64(u64 ull){
				if (bitMove == 0)*(u64*)ptrValue = ull;
				else{
					*(u64*)ptrValue = (*(u64*)ptrValue&(((u64) - 1) >> (64 - bitMove))) | (ull << bitMove);
					*((u64*)ptrValue + 1) = (*((u64*)ptrValue + 1)&(((u64) - 1) << bitMove)) | (ull >> (64 - bitMove));
				}
			}
			/**
			@brief 将该位指针指定位设置为boolean所指定的值
			@param [传入]参数boolean表示指定位设定内容
			@note 该操作不做指针为NULL检查*/
			void Set(bool boolean) { if (boolean)(*(size_t*)ptrValue) |= ((size_t)1 << bitMove); else (*(size_t*)ptrValue) = (*(size_t*)ptrValue)&((*(size_t*)ptrValue) ^ ((size_t)1 << bitMove)); }
			/**
			@brief 将该位指针指定位状态翻转
			@note 该操作不做指针为NULL检查*/
			void Flip() { (*(size_t*)ptrValue) ^= ((size_t)1 << bitMove); }
			/**
			@brief 获取位偏移量*/
			size_t GetBitMove()const { return bitMove; }
			/**
			@自增*/
			BitPtr& operator++() { if (bitMove == BitMoveMax) { bitMove = 0; ptrValue += sizeof(size_t); } else ++bitMove; return*this; }
			BitPtr operator++(int) { BitPtr tmp(*this); ++*this; return tmp; }
			/**
			@brief 相加*/
			BitPtr operator+(size_t num)const { return BitPtr((void*)ptrValue, bitMove + num); }
			/**
			@brief 相加*/
			BitPtr& operator+=(size_t num){
				bitMove += num;
				if ((index_t)bitMove < 0) { ptrValue += ((index_t)bitMove + 1) / 8 - 1; }
				else if (bitMove >= 8) { ptrValue += bitMove >> 3; }
				bitMove = ((ptrValue&PtrMoveMask) << 3) | (bitMove & 7);
				ptrValue &= PtrValueMask;
				return *this;
			}
			/**
			@自减*/
			BitPtr& operator--() { if (bitMove == 0) { bitMove = BitMoveMax; ptrValue -= sizeof(size_t); } else --bitMove; return*this; }
			BitPtr operator--(int) { BitPtr tmp(*this); --*this; return tmp; }
			/**
			@brief 相减*/
			size_t operator-(BitPtr const& bp)const { return ((ptrValue << 3) | bitMove) - ((bp.ptrValue << 3) | bp.bitMove); }
			/**
			@brief 相减*/
			BitPtr operator-(size_t num)const { return BitPtr((void*)ptrValue, bitMove - num); }
			/**
			@brief 相减*/
			BitPtr& operator-=(size_t num){
				bitMove -= num;
				if ((index_t)bitMove < 0) { ptrValue += ((index_t)bitMove + 1) / 8 - 1; }
				else if (bitMove >= 8) { ptrValue += bitMove >> 3; }
				bitMove = ((ptrValue&PtrMoveMask) << 3) | (bitMove & 7);
				ptrValue &= PtrValueMask;
				return *this;
			}
			/**
			@brief 比较*/
			bool operator==(BitPtr const& bp)const { return ptrValue == bp.ptrValue&&bitMove == bp.bitMove; }
			bool operator!=(BitPtr const& bp)const { return ptrValue != bp.ptrValue || bitMove != bp.bitMove; }
			bool operator>=(BitPtr const& bp)const { if (ptrValue != bp.ptrValue)return ptrValue > bp.ptrValue; return bitMove >= bp.bitMove; }
			bool operator<=(BitPtr const& bp)const { if (ptrValue != bp.ptrValue)return ptrValue < bp.ptrValue; return bitMove <= bp.bitMove; }
			bool operator>(BitPtr const& bp)const { if (ptrValue != bp.ptrValue)return ptrValue > bp.ptrValue; return bitMove > bp.bitMove; }
			bool operator<(BitPtr const& bp)const { if (ptrValue != bp.ptrValue)return ptrValue < bp.ptrValue; return bitMove < bp.bitMove; }
			index_t CompareWith(BitPtr const& bp)const { if (ptrValue != bp.ptrValue)return ptrValue > bp.ptrValue ? 1 : -1; return bitMove > bp.bitMove ? 1 : bitMove < bp.bitMove ? -1 : 0; }
			/**
			@brief 输出指向数据的字符串表示
			@param [传入]参数t表示读取并输出的位的数量，如果t<0，将进行逆向探查并输出字符串，如果t等于0，将输出该指针数据内容*/
			std::string ToString(index_t t = 1);
			/**
			@brief 输出指向数据的字符串表示(16进制表示)
			@param [传入]参数t表示读取并输出的位的数量，如果t<0，将逆向逆序输出字符串，如果t等于0，将输出该指针数据内容*/
			std::string ToString16(index_t t = 1);
			/**
			@brief operator[]只读重载，如果指定位为1则返回true，否则返回false*/
			bool operator[](index_t id)const{
				id += bitMove;
				if (id < 0)return (*((size_t*)ptrValue + (id - (index_t)(sizeof(size_t) * 8 - 1)) / (sizeof(size_t) * 8)) >> (id&((sizeof(size_t) * 8) - 1)) & 1);
				return (*((size_t*)ptrValue + id / (sizeof(size_t) * 8)) >> (id&((sizeof(size_t) * 8) - 1)) & 1);
			}
			/**
			@brief operator*只读解引用*/
			bool operator*()const { return Get(); }
		private:
			/**@brief 实际的地址*/
			size_t ptrValue;
			/**@brief 位偏移量*/
			size_t bitMove;
		};

	}
}
#endif