#pragma once
#define LK_MATH_UIntN_
/**@author W意波编写*/
#include <math.h>
namespace LK
{
	namespace Math
	{
		/**
		@brief 表示一个(N*64)位无符号整数*/
		template<size_t N>struct UIntN
		{
			//数据
			u64 arr[N];
			//默认构造，数据状态未知
			UIntN() {}
#ifdef LK_MATH_FLOAT256_
			//从Float256构造
			explicit UIntN(Float256 const& d)
			{
				if (d.Exp < 0)Clear();
				else{
					size_t i = 0;
					for (; i != N&&i != 4; ++i)arr[N-1-i] = d.Data.ull[3-i];
					while (i != N) { arr[N-1 - i] = 0; ++i; }
					*this >>= N*64-d.Exp-1;
					if (d.Sign < 0)*this = -*this;
				}
			}
#endif
#ifdef LK_MATH_INT_
			//从大数整形转换
			explicit UIntN(intx const& d)
			{
				size_t i = 0;
				size_t m = (size_t)(d.GetBitsCount() / 64)+1;
				u64 const*p = d.Ptr()->pData;
				for (; i != N&&i != m; ++i)arr[i] = p[i];
				while (i != N)arr[i++] = 0;
				if (d.GetSign() < 0)*this = -*this;
			}
#endif
			//从double构造
			explicit UIntN(double d)
			{
				if (d<1 && d>-1)Clear();
				else {
					arr[0] = (reinterpret_cast<u64&>(d)&0xfffffffffffff)|0x10000000000000;
					for (size_t i = 1; i != N; ++i)arr[i] = 0;
					*this <<= ((reinterpret_cast<u64&>(d) >> 52) & 0xfff)-1023-52;
					if (d < 0)*this = -*this;
				}
			}
			//从float构造
			explicit UIntN(float d)
			{
				if (d<1 && d>-1)Clear();
				else {
					arr[0] = (reinterpret_cast<u32&>(d)&0xffffff) | 0x1000000;
					for (size_t i = 1; i != N; ++i)arr[i] = 0;
					*this <<= (s32)((reinterpret_cast<u32&>(d) >> 24) & 0x7f)-63-24;
					if (d < 0)*this = -*this;
				}
			}
			template<class T>UIntN(T u,typename std::enable_if<(std::is_same<T,u8>::value||std::is_same<T,u16>::value||std::is_same<T,u32>::value||std::is_same<T,u64>::value||std::is_same<T,ui32>::value)>::type*p=nullptr)
			{
				arr[0] = u;
				for (size_t i = 1; i != N; ++i)arr[i] = 0;
			}
			template<class T>UIntN(T s,typename std::enable_if<(std::is_same<T,s8>::value || std::is_same<T,s16>::value || std::is_same<T,s32>::value || std::is_same<T,s64>::value || std::is_same<T,i32>::value)>::type*p= nullptr)
			{
				arr[0] = s;
				for (size_t i = 1; i != N; ++i)arr[i] = s < 0 ? -1 : 0;
			}
			/**
			@brief 比较绝对值大小*/
			static index_t AbsCompare(UIntN const& n1, UIntN const&n2)
			{
				size_t i = N;
				while (i-- != 0)if (n1.arr[i] != n2.arr[i])return n1.arr[i] > n2.arr[i] ? 1 : -1;
				return 0;
			}
			/**
			@brief 获取指定索引开始计算的bits*/
			static inline size_t GetBitsCount64(cu64* arr, size_t idx) {
				if (idx >= N)idx = N-1;
				while (idx != 0 && arr[idx] == 0)idx--;
				return idx * 64 + 64 - Bits::LeftZeroCount64(arr[idx]);
			}
			/**
			@brief 全部数据清零*/
			void Clear() { for (size_t i = 0; i != N; ++i)arr[i] = 0; }
			/**
			@brief 判断该数值是否为0*/
			bool Equals0()const { for (size_t i = 0; i != N; ++i)if (arr[i])return false; return true; }
			//加
			UIntN operator+()const { return *this; }
			UIntN operator++(int) { UIntN tmp(*this); ++*this; return tmp; }
			UIntN& operator++() {
				size_t i = 0;
				while ( i != N&&++arr[i] == 0)++i;
				return *this; }
			size_t GetBitsCount()const { size_t i = N - 1; for (; i != 0; --i)if (arr[i] != 0)break; return i * 64 + 64 - Bits::LeftZeroCount64(arr[i]); }
			friend UIntN operator+(UIntN const& n1, UIntN const&n2) {
				UIntN n; bool flag = false;
					index_t i = 0;
					while (i < N) {
						n.arr[i] = n1.arr[i] + n2.arr[i] + flag;
						if (flag)
							flag = n1.arr[i] >= (~n2.arr[i]);
						else flag = n1.arr[i] >(~n2.arr[i]);
						++i;
					}
				return n;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value || std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value ||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator+(UIntN const& n1, T n2) {
				UIntN n = n1;
				return n+=n2;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator+(T n1, UIntN const& n2) { return n2 + n1; }
			UIntN&operator+=(UIntN const&n){
				bool flag = false;
				for (index_t i = 0; i < N; ++i) {
					if (flag){
						flag = arr[i] >= (~n.arr[i]);
						arr[i] += n.arr[i] + 1;
					}
					else {
						flag = arr[i] > (~n.arr[i]);
						arr[i] += n.arr[i];
					}
				}
				return *this;
			}
			UIntN&operator+=(u64 n){
				if(arr[0]>~n)
				for (size_t i = 1; i != N; ++i)if (++arr[i] != 0)break;
				arr[0] += n;
				return *this;
			}
			template<class T>typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), UIntN&>::type operator+=(T n) { return operator+=((u64)n); }
			template<class T>typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN&>::type operator+=(T n) { if(n>=0)return operator+=((u64)n); return operator-=((u64)-n);}
			//减
			UIntN operator-()const { UIntN n; bool flag = false; for (index_t i = 0; i < N; ++i)if (flag)n.arr[i] = ~arr[i]; else { flag = arr[i] != 0; n.arr[i] = -(long long)arr[i]; } return n; }
			UIntN operator--(int) { UIntN tmp(*this); --*this; return tmp; }
			UIntN& operator--() { for (index_t i = 0; i < N; ++i)if (arr[i]-- != 0)break; return *this; }
			friend UIntN operator-(UIntN const& n1, UIntN const&n2){
				UIntN n; bool flag = false;
				for (index_t i = 0; i < N; ++i) {
					n.arr[i] = n1.arr[i] - n2.arr[i] - flag;
					if (flag)
						flag = n1.arr[i] <= n2.arr[i];
					else flag = n1.arr[i] < n2.arr[i];
				}
				return n;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value || std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value ||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator-(UIntN const& n1, T n2) {
				UIntN n = n1;
				return n-=n2;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator-(T n1, UIntN const& n2){
				UIntN n = -n2; return n + n1;
			}
			UIntN&operator-=(UIntN const&n){
				bool flag = false;
				for (index_t i = 0; i < N; ++i) {
					if (flag){
						flag = arr[i] <= n.arr[i];
						arr[i] -= n.arr[i] + 1;
					}
					else {
						flag = arr[i] < n.arr[i];
						arr[i] -= n.arr[i];
					}
				}
				return *this;
			}
			UIntN&operator-=(u64 n){
				if(arr[0]<n)
				for (size_t i = 1; i != N; ++i)if (arr[i]-- != 0)break;
				arr[0] -= n;
				return *this;
			}
			template<class T>typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), UIntN&>::type operator-=(T n) { return operator-=((u64)n); }
			template<class T>typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN&>::type operator-=(T n) { if (n >= 0)return operator-=((u64)n); return operator+=((u64)-n) ;}
			//乘
			friend UIntN operator*(UIntN const& n1, UIntN const&n2){
				UIntN n;
				Bits::MultiplyLow64(n.arr, N, n1.arr, N, n2.arr, N);
				return n;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator*(UIntN const& n1, T n2) {
				UIntN n = n1;
				return n *= n2;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator*(T n1,UIntN const& n2 ) {
				UIntN n = n2;
				return n *= n1;
			}
			UIntN& operator*=(UIntN const&n) { Bits::MultiplyLow64(arr, N, arr, N, n.arr, N); return *this; }
			UIntN& operator*=(u64 n) { Bits::SelfMultiplyInt64Low64(arr, N, n); return*this; }
			UIntN& operator*=(s64 n) { Bits::SelfMultiplyInt64Low64(arr, N, -n); return*this = -*this; }
			UIntN& operator*=(u32 n) { Bits::SelfMultiplyInt32Low64(arr, N, n); return*this; }
			UIntN& operator*=(i32 n) { Bits::SelfMultiplyInt32Low64(arr, N, -n); return*this = -*this; }
			template<class T>typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), UIntN&>::type operator*=(T n) { return operator*=((u32)n); }
			template<class T>typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN&>::type operator*=(T n) { return operator*=((i32)n); }
			//除
			friend UIntN operator/(UIntN const& n1, UIntN const&n2){
				UIntN n;
				Bits::DivideLow64(n.arr, N, n1.arr, n1.GetBitsCount(), n2.arr, n2.GetBitsCount());
				return n;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator/(UIntN const& n1, T n2) {
				UIntN n = n1;
				return n /= n2;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), UIntN>::type operator/(T n1, UIntN const& n2) {
				for (index_t i = 1; i < N; ++i)if (n2.arr[i] != 0)return 0;
				return n1 / n2.arr[0];
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN>::type operator/(T n1, UIntN const& n2) {
				for (index_t i = 1; i < N; ++i)if (n2.arr[i] != 0)return 0;
				if (n1 < 0)return -UIntN((-n1) / n2.arr[0]);
				return n1 / n2.arr[0];
			}
			UIntN& operator/=(UIntN const&n) { Bits::DivideLow64(arr, N, arr, GetBitsCount(), n.arr, n.GetBitsCount()); return *this; }
			UIntN& operator/=(u64 n) { Bits::SelfDivideInt64Low64(arr,N,n) ;return*this; }
			UIntN& operator/=(u32 n) { Bits::SelfDivideInt32Low64(arr,N,n); return*this; }
			UIntN& operator/=(s64 n) { Bits::SelfDivideInt64Low64(arr, N, -n); return*this = -*this; }
			UIntN& operator/=(s32 n) { Bits::SelfDivideInt32Low64(arr, N, -n); return*this = -*this; }
			template<class T>typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), UIntN&>::type operator/=(T n) { return operator/=((u32)n); }
			template<class T>typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN&>::type operator/=(T n) { return operator/=((i32)n); }
			//余
			friend UIntN operator%(UIntN const& n1, UIntN const&n2){
				UIntN n;
				Bits::DivideLow64(NULL, 0, n1.arr, n1.GetBitsCount(), n2.arr, n2.GetBitsCount(),n.arr,N);
				return n;
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), T>::type operator%(UIntN const& n1, T n2) {
				return (T)Bits::DivideInt64Low64(NULL, 0, n1.arr, N, n2);
			}
			template<class T>friend typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), T>::type operator%(UIntN const& n1, T n2) {
				if(n2<0)return (T)Bits::DivideInt64Low64(NULL, 0, n1.arr, N, -n2);
				return (T)Bits::DivideInt64Low64(NULL, 0, n1.arr, N, n2);
			}
			UIntN& operator%=(UIntN const&n) { Bits::DivideLow64(NULL, 0, arr, N * 64, n.arr, N * 64, arr, N); return *this; }
			template<class T>typename std::enable_if<(std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,i32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), UIntN&>::type operator%=(T n) { return *this=*this%n; }
			UIntN Mod(UIntN const& n2, UIntN*p = NULL)const{
				UIntN n;
				if (p){
					Bits::DivideLow64(n.arr, N, arr, N * 64, n2.arr, N * 64,p->arr,N);
				}
				else Bits::DivideLow64(n.arr, N,arr, N * 64, n2.arr, N * 64);
				return n;
			}
			//自除并求余
			u64 SelfDevide64(u64 num)
			{
				return Bits::SelfDivideInt64Low64(arr, N, num);
			}
			//按位取反
			UIntN operator~()const { UIntN n; for (index_t i = 0; i < N; ++i)n.arr[i] = ~arr[i]; return n; }
			//按位或
			friend UIntN operator|(UIntN const&n1,UIntN const&n2) { UIntN n; for (index_t i = 0; i < N; ++i)n.arr[i] = n1.arr[i]|n2.arr[i]; return n; }
			UIntN&operator|=(UIntN const&n) { for (index_t i = 0; i < N; ++i)arr[i] |= n.arr[i]; return *this; }
			//按位与
			UIntN&operator&=(UIntN const&n) { for (index_t i = 0; i < N; ++i)arr[i] &= n.arr[i]; return *this; }
			friend UIntN operator&(UIntN const&n1,UIntN const&n2) { UIntN n; for (index_t i = 0; i < N; ++i)n.arr[i] = n1.arr[i]&n2.arr[i]; return n; }
			//按位异或
			friend UIntN operator^(UIntN const&n1,UIntN const&n2) { UIntN n; for (index_t i = 0; i < N; ++i)n.arr[i] = n1.arr[i]^n2.arr[i]; return n; }
			UIntN&operator^=(UIntN const&n) { for (index_t i = 0; i < N; ++i)arr[i] ^= n.arr[i]; return *this; }
			//左移
			UIntN operator<<(index_t i) { if (i > 0)return *this >> (-i); UIntN n; n.Clear(); Bits::CopyShift64(arr, N, n.arr, N, -i); return n; }
			UIntN& operator<<=(index_t i) { if (i > 0)return *this >>= (-i); Bits::Shift(arr, 0, N * 64, i); return*this; }
			//右移
			UIntN operator>>(index_t i) { if (i > 0)return *this << (-i); UIntN n; n.Clear(); Bits::CopyShift64(arr, N, n.arr, N, i); return n; }
			UIntN& operator>>=(index_t i) { if (i > 0)return *this <<= (-i); Bits::Shift(arr, 0, N * 64, -i); return*this; }
			//转换到bool值
			operator bool()const { for (index_t i = 0; i < N; ++i)if(arr[i] != 0)return true; return false; }
			bool operator!()const { for (index_t i = 0; i < N; ++i)if(arr[i] != 0)return false; return true; }
			//比较
			friend bool operator==(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)==0; }
			friend bool operator==(UIntN const& n1, u64 n2) { for (size_t i = 1; i < N; ++i)if (n1.arr[i] != 0)return false; return n1.arr[0] == n2; }
			friend bool operator==(UIntN const& n1, s64 n2) { if (n2 >= 0)return n1 == (u64)n2; for (size_t i = 1; i < N; ++i)if (n1.arr[i] != ~0)return false; return n1.arr[0] == n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), bool>::type operator==(UIntN const& n1, T n2) {return n1 == (u64)n2;}
			template<class T>friend typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,ui32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator==(UIntN const& n1, T n2) { return n1 == (s64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator==(T n2,UIntN const& n1 ) { return n1 ==n2; }
			friend bool operator!=(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)!=0; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator!=( UIntN const& n1,T n2) { return !(n1 == n2); }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator!=(T n2, UIntN const& n1) { return !(n1 == n2); }
			friend bool operator>(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)>0; }
			friend bool operator>(UIntN const& n1, u64 n2) { for (size_t i = 1; i < N; ++i)if (n1.arr[i] != 0)return true; return n1.arr[0] > n2; }
			friend bool operator>(UIntN const& n1, s64 n2) { if (n2 >= 0)return n1 > (u64)n2; for (size_t i = 1; i < N; ++i)if (n1.arr[i] != ~0)return false; return n1.arr[0] > (u64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), bool>::type operator>(UIntN const& n1, T n2) { return n1 > (u64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,ui32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator>(UIntN const& n1, T n2) { return n1 > (s64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator>(T n1, UIntN const& n2) { return n2<n1; }
			friend bool operator<(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)<0; }
			friend bool operator<(UIntN const& n1, u64 n2) { for (size_t i = 1; i < N; ++i)if (n1.arr[i] != 0)return false; return n1.arr[0] < n2; }
			friend bool operator<(UIntN const& n1, s64 n2) { if (n2 >= 0)return n1 < (u64)n2; for (size_t i = 1; i < N; ++i)if (n1.arr[i] != ~0)return true; return n1.arr[0] < (u64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value), bool>::type operator<(UIntN const& n1, T n2) { return n1 <(u64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,ui32>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator<(UIntN const& n1, T n2) { return n1 < (s64)n2; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator<(T n1, UIntN const& n2) { return n2>n1; }
			friend bool operator>=(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)>=0; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator>=(UIntN const& n1, T n2) { return !(n1<n2); }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator>=(T n1, UIntN const& n2) { return !(n1<n2); }
			friend bool operator<=(UIntN const& n1, UIntN const& n2) { return AbsCompare(n1,n2)<=0; }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator<=(UIntN const& n1, T n2) { return !(n1>n2); }
			template<class T>friend typename std::enable_if<(std::is_same<T,u32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,ui32>::value||std::is_same<T,s32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value), bool>::type operator<=(T n1, UIntN const& n2) { return !(n1>n2); }
			//转换到字符串
			std::string ToString()const
			{
				//获取比特数
				index_t bc = GetBitsCount64(arr, N - 1);
				//计算十进制位数
				index_t exp = (index_t)(bc*0.3010299957);
				//分配字符串空间
				std::string str;
				str.resize(exp + 2);
				//字符串结尾为0
				str[exp + 1] = str[exp] = 0;
				//每次写入19位数值
				index_t i = exp;
				//创建一个副本来计算
				UIntN num(*this);
				//开始写入
				while (i >= 19){
					i -= 19;
					Strings::WriteUNum(num.SelfDevide64(10000000000000000000ull), &str[i], 19);
				}
				//创建临时数组tstr来实测剩下内容长度
				char tstr[19];
				int len = Strings::WriteUNum(num.SelfDevide64(10000000000000000000ull), tstr,0);
				//如果i跟len不等，则移位
				if (i != len) { for (index_t k = ++exp; k != i; k--)str[k] = str[k - 1]; }
				//复制tstr的内容到str
				for (i = 0; i < len; ++i)str[i] = tstr[i];
				//处理0的情况
				if (exp == 0)str[exp++] = '0';
				//返回转换结果
				str.resize(exp);
				return str;
			}
			//转换到int值
			operator int()const { return (int)arr[0]; }
			operator long()const { return (long)arr[0]; }
			operator long long()const { return (long long)arr[0]; }
			operator unsigned long()const { return (unsigned long)arr[0]; }
			operator unsigned int()const { return (unsigned int)arr[0]; }
			operator unsigned long long()const { return (unsigned long long)arr[0]; }
			operator unsigned short()const { return (unsigned short)arr[0]; }
			operator unsigned char()const { return (unsigned char)arr[0]; }
			operator short()const { return (short)arr[0]; }
			operator signed char()const { return (signed char)arr[0]; }
			operator double()const{//不考虑进位的转换
				for (index_t i = N - 1; i >= 0;--i)
					if (arr[i] != 0) {
						if (i == 0)return (double)arr[0];
						index_t exp = i * 64 + 10 - Bits::LeftZeroCount64(arr[i]);
						if (exp > 1024-51)return MaxValues::Double;
						double d=0;
						Bits::Copy(&d, 0, arr, exp, 53);
						if ((reinterpret_cast<u64&>(d) & 1) != 0 && ++reinterpret_cast<u64&>(d) == 0x20000000000000){
							if(++exp> 1024 - 51)return MaxValues::Double;
						}
						reinterpret_cast<u64&>(d) >>= 1;
						exp += 1023+53;
						Bits::Copy(&d, 52, &exp, 0, 12);
						return d;
					}
				return 0;
			}
			operator float()const {
				return (float)(double)*this;
			}
#ifdef LK_MATH_FLOAT256_
			//转换到Float256
			operator Float256()const{
				Float256 f(0);
				f.Exp = GetBitsCount64(arr, N - 1);
				f.Sign = f.Exp != 0;
				int cnt = (int)(f.Exp > 256 ? 256 : f.Exp);
				Bits::Copy(f.Data.ull, 256 - cnt, arr, f.Exp - cnt, cnt);
				if (f.Exp != 0)f.Exp--;
				return f;
			}
#endif
#ifdef LK_MATH_INT_
			//转换到大数整形
			operator intx()const
			{
				Int* p = new Int();
				p ->_Relen64(N);
				for (size_t i = 0; i != N; ++i)
					p->pData[i] = arr[i];
				p->BitsCount = p->FindBitsCount(N);
				p->Sign = p->BitsCount != 0;
				return p;
			}
#endif
			//最大值
			static const UIntN& MaxValue;
			//private
			static const UIntN& GetMaxValueRef() { static UIntN n; if (n.arr[0] == 0) { for (size_t i = 0; i != N; ++i)n.arr[i] = ~0; } return n; }
		};
		template<size_t N> const UIntN<N>& UIntN<N>::MaxValue = UIntN<N>::GetMaxValueRef();

	}
}
