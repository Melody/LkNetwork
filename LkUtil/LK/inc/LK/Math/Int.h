#ifndef LK_MATH_INT_//由W意波编写
#define LK_MATH_INT_
#define HAS_SIGNED_TYPES_EXCEPT64 (std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value)
#define HAS_UNSIGNED_TYPES_EXCEPT64 (std::is_same<T, u32>::value|| std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value)
#define HAS_ALL_TYPES_EXCEPT_FLOAT (std::is_same<T, u32>::value || std::is_same<T, u64>::value || std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value || std::is_same<T, s64>::value || std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value  || std::is_same<T, char>::value)
#define HAS_ALL_TYPES (std::is_same<T, u32>::value || std::is_same<T, u64>::value || std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value || std::is_same<T, s64>::value || std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value  || std::is_same<T, char>::value || std::is_same<T, double>::value || std::is_same<T, float>::value)
#define HAS_ALL_TYPES_EXCEPT_FLOAT_INT64 (std::is_same<T, u32>::value || std::is_same<T, u64>::value || std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value || std::is_same<T, s32>::value || std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value  || std::is_same<T, char>::value)
namespace LK
{
	namespace Math
	{

		Int operator|(Int const &v1, Int const &v2);
		Int operator&(const Int&t, const Int&vi);
		Int operator^(const Int&t, const Int&vi);
		/**
		@brief 大数整形实体*/
		struct Int :BigObject
		{
			/**
			@brief 位数*/
			u64 BitsCount;
			/**
			@brief 具体数据，原码记录*/
			u64*pData;
			/**
			@brief 以64为单位的bit长度,即pData的元素数（实际上len64不能超过MaxValues::Of(size_t())）*/
			size_t len64;
			/**
			@brief 当前内存可用数量*/
			size_t maxSize;
			/**
			@brief 正负号，正为1，负为-1，0为0，其他为错误信息记录*/
			int Sign;

			/**
			@brief 计算两者的绝对值之和*/
			static Int AbsPlus(const Int& vi1, const Int& vi2);
			/**
			@brief 计算两者差值的绝对值*/
			static Int AbsMinus(const Int& vi1, const Int& vi2);

			/**
			@brief 最大比特数限制*/
			static u64 MaxBitsCount;
			/**
			@brief‘ 最大元素数限制*/
			static u64 MaxLen64;

			/**
			@brief 返回一个浅表副本*/
			ptr<BigObject> Clone()const { return new Int(*this); }
			/**
			@brief 在该实例mov比特位起取32位数据*/
			u32 GetBits32(u64 mov)const;
			/**
			@brief 重置为0，并移除数据*/
			void ResetTo0() { Free<u64>(pData); pData = nullptr; maxSize = 0; BitsCount = 0; len64 = 0; Sign = 0; }
			/**
			@brief 在该实例数据mov位起取bitscount位放到pDes中*/
			void GetBits(u64*pDes, u64 mov, u64 bitscount)const { Bits::Copy(pDes, 0, pData, (index_t)mov, (size_t)bitscount); }
			/**
			@brief 往该实例中mov位起写入一个num（32位bit数据）*/
			void SetBits32(ui32 num, u64 mov);
			/**
			@brief 检测该实例第mov位是否有数据*/
			bool HasBits(u64 mov)const;
		private:
			template<class T>static inline T*Realloc(T* p, size_t size) {
				return (T*)::realloc(p, size*sizeof(T));
			}
			template<class T>static inline T*Realloc(T* p, size_t size, size_t&maxSize) {
				maxSize = Memory::GetResize(size);
				return Realloc<T>(p, maxSize);
			}
			template<class T>static inline T*Malloc(size_t size, size_t& maxSize) {
				maxSize = Memory::GetResize(size);
				return Malloc<T>(maxSize);
			}
			template<class T>static inline T*Malloc(size_t size) {
				return (T*)::malloc(size*sizeof(T));
			}
			template<class T>static inline void Free(T* p) {
				::free(p);
			}
			friend struct ptr<Int>;
			/**
			@brief 将psource指向的内容叠加到pdes上*/
			static bool BitsPlus(u64 *pdes, u64 * psource, u64 deslen, u64 sourcelen);
			/**
			@brief 把pdes的内容减去psource的部分*/
			static bool BitsMinus(u64 *pdes, u64 * psource, u64 deslen, u64 sourcelen);
			/**
			@brief 改变pData长度并保留数据，len每增加1变化64bits*/
			void Relen64(size_t len)
			{
				if (len <= maxSize) { for (size_t i = len64; i < len; ++i)pData[i] = 0; len64 = len; }
				if (len > MaxLen64) { Throw<std::bad_alloc >(); }
				pData = Realloc<u64>(pData, len, maxSize);
				for (size_t i = len64; i < len; ++i)pData[i] = 0;
				len64 = len;
			}
			/**
			@brief 改变pData长度但不保留数据*/
			void _Relen64(size_t len)
			{
				if (len > MaxLen64) { Throw<std::bad_alloc >(); }
				if (maxSize >= len) { len64 = len; return; }
				Free<u64>(pData);
				pData = Malloc<u64>(len, maxSize);
				len64 = len;
			}
			/**
			@brief 查找有数据比特数 ,l64为涉及的元素数*/
			u64 FindBitsCount(size_t l64);
			/**
			@brief 在pDat指向的数据mov位起取64位*/
			static u64 GetBits64(u64* pDat, u64 mov);
			/**
			@brief 将该实例的数据与num的积放到pDes中*/
			void BitsMultiply(u64 *pDes, u64 num)const;
			/**
			@brief 把pDat指向的数据减去pNum指向数据，numl64指示pNum的元素数*/
			static void BitsMinus(u64* pDat, u64* pNum, u64 numl64);
			/**
			@brief 对pDat指向的数据进行左移32位，l64指示pDat的元素数*/
			static void BitsMovLeft32(u64* pDat, u64 l64);
			/**
			@brief 比较pDat1指向数据与pDat2指向数据的大小，l64指示比较的元素数*/
			static bool BitsNotLessThan(u64* pDat1, u64*pDat2, u64 l64);
		public:
			/**
			@brief 比较绝对值大小*/
			static int AbsCompare(const Int& vi1, const Int& vi2);

			Int& operator=(const Int& li);
#ifdef _HAS_RVALUE_REFERENCES
			Int& operator=(const Int&& li)
			{
				SwapWith(const_cast<Int&>(li));
				return *this;
			}
#endif


			/**
			@brief 析构*/
			~Int() { Free(pData); }
			/**
			@brief 默认构造函数，构造一个0*/
			Int() :BitsCount(0), pData(NULL), len64(0), maxSize(0), Sign(0) {}
			/**
			@brief 通过传入各数据参数构造(慎用)*/
			Int(u64 *pdat, size_t l64, int s)
				:pData(pdat), len64(l64), maxSize(l64), Sign(s) {
				BitsCount = (s == 1 || s == -1) ? FindBitsCount(l64) : 0;
			}
			Int(u64 *pdat, size_t l64, size_t maxsize, u64 bc, int s) :BitsCount(bc), pData(pdat), len64(l64), maxSize(maxsize), Sign(s)
			{
				if (bc == 0 && !IsError())Sign = 0;
			}
			/**
			@brief 复制构造函数*/
			Int(const Int&);
#ifdef _HAS_RVALUE_REFERENCES
			/**
			@brief 右值复制*/
			Int(Int&& v) :BitsCount(v.BitsCount), pData(v.pData), len64(v.len64), maxSize(v.maxSize), Sign(v.Sign)
			{
				v.pData = NULL;
				v.BitsCount = 0;
				v.maxSize = 0;
				v.len64 = 0;
				v.Sign = 0;
			}
#endif
			Int(u64 num);
			Int(u32 num);
			Int(ui32 num);
			Int(u16 num);
			Int(u8 num);
			Int(s64 num);
			Int(s32 num);
			Int(i32 num);
			Int(s16 num);
			Int(s8 num);
			Int(char num);
			Int(double num);
			Int(float num);
			Int(const std::string& str) :BitsCount(0), pData(NULL), len64(0), Sign(0) { Int tmp(str.c_str()); SwapWith(tmp); }
			Int(const std::wstring& str) :BitsCount(0), pData(NULL), len64(0), Sign(0) { Int tmp(Strings::ToString(str)); SwapWith(tmp); }
			Int(wchar_t const* str) :BitsCount(0), pData(NULL), len64(0), Sign(0) { std::wstring ws(str); Int tmp(ws); SwapWith(tmp); }
			Int(char const* str);
			std::string ToString()const;

			void SwapWith(Int& vi) { Swap(pData, vi.pData); Swap(maxSize, vi.maxSize); Swap(BitsCount, vi.BitsCount); Swap(len64, vi.len64); Swap(Sign, vi.Sign); }
			/**
			@brief 产生一个错误*/
			Int& MakeError(int err) { Sign = err; return *this; }
			/**
			@brief 检测是否有错*/
			bool IsError()const { return Sign < -1; }
			/**
			@brief 是否为0*/
			bool Equals0()const { return Sign == 0; }
			/**
			@brief 自身取绝对值*/
			Int& SelfAbs() { if (!IsError() && Sign < 0)Sign = -Sign; return *this; }
			/**
			@brief 取绝对值*/
			static Int Abs(Int vi) { if (!vi.IsError() && vi.Sign < 0)vi.Sign = -vi.Sign; return vi; }
			/**
			@brief 自身取相反数*/
			Int& SelfNegative() { if (!IsError())Sign = -Sign; return *this; }
			/**
			@brief 获取该数的绝对值*/
			Int Abs()const { return Int(*this).SelfAbs(); }
			//=
			Int& operator=(s64 num);
			Int& operator=(u64 num);
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int&>::type operator=(T const& num)
			{
				return operator=((u64)num);
			}//无符号整形转为调用operator+=(u64)
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Int&>::type operator=(T const& num)
			{
				return operator=((s64)num);
			}//其它转为调用operator+=(Int)
			template<class T>inline typename std::enable_if<!HAS_ALL_TYPES_EXCEPT_FLOAT, Int&>::type operator=(T const& num)
			{
				return operator=(Int(num));
			}//有符号整形转为调用operator+=(s64)
			 //+=
			Int& operator+=(const Int& vi);
			Int& operator+=(u64 num);
			Int& operator+=(s64 num);
			Int& operator+=(double num);
			Int& operator+=(float num) { return *this += (double)num; }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int&>::type operator+=(T const& num)
			{
				return operator+=((u64)num);
			}//无符号整形转为调用operator+=(u64)
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Int&>::type operator+=(T const& num)
			{
				return operator+=((s64)num);
			}//有符号整形转为调用operator+=(s64)
			 //+
			Int operator+()const { return Int(*this); }
			Int operator+(const Int& vi)const;
			Int operator+(u64 num)const;
			Int operator+(s64 num)const;
			double operator+(double num)const;
			float operator+(float num)const { return float(*this + (double)num); }
			friend double operator+(double num, Int const& vi) { return vi + num; }
			friend float operator+(float num, Int const& vi) { return vi + num; }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Int>::type operator+(T const& t)const { return *this + (s64)t; }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int>::type operator+(T const& t)const { return *this + (u64)t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, Int>::type operator+(T const& t, Int const& f) { return f + t; }
			//++
			Int operator++(int);
			Int& operator++();
			//-=
			Int& operator-=(const Int& vi);
			Int& operator-=(u64 num);
			Int& operator-=(s64 num);
			Int& operator-=(double num);
			Int& operator-=(float num) { return *this -= (double)num; }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int&>::type operator-=(T const& num)
			{
				return operator-=((u64)num);
			}//无符号整形转为调用operator-=(u64)
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Int&>::type operator-=(T const& num)
			{
				return operator-=((s64)num);
			}//有符号整形转为调用operator-=(s64)
			 //-
			Int operator-()const { return Int(*this).SelfNegative(); }
			Int operator-(const Int& vi)const;
			Int operator-(u64 num)const;
			Int operator-(s64 num)const;
			double operator-(double num)const;
			float operator-(float num)const { return float(*this - (double)num); }
			friend double operator-(double num, const Int& vi) { return -(vi - num); }
			friend float operator-(float num, const Int& vi) { return -(vi - num); }
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, Int>::type operator-(T const& t)const { return *this - (s64)t; }
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int>::type operator-(T const& t)const { return *this - (u64)t; }
			template<class T>friend inline typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, Int>::type operator-(T const& t, Int const& f) { return (f - t).SelfNegative(); }
			//--
			Int operator--(int);
			Int& operator--();
			//乘法
			Int& operator*=(const Int& vi);
			Int& operator*=(u64 i);
			Int& operator*=(u32 i);
			Int& operator*=(s64 i);
			Int& operator*=(s32 i);
			Int& operator*=(double i);
			Int& operator*=(float i) { return operator*=(double(i)); }
			template<class T>inline typename std::enable_if< std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value , Int&>::type operator*=(T const& num)
			{
				return operator*=((u32)num);
			}//无符号整形转为调用operator-=(u32)
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int&>::type operator*=(T const& num)
			{
				return operator*=((s32)num);
			}//有符号整形转为调用operator-=(s32)

			Int operator*(const Int& vi)const;
			Int operator*(u64 num)const;
			Int operator*(s64 num)const;
			Int operator*(u32 num)const;
			Int operator*(s32 num)const;
			double operator*(double num)const;
			float operator*(float num)const { return float(*this * (double)num); }
			friend double operator*(double num, const Int& vi) { return vi * num; }
			friend float operator*(float num, const Int& vi) { return float(vi * (double)num); }
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int>::type operator*(T const& t)const { return *this  * (s32)t; }
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, Int>::type operator*(T const& t)const { return *this * (u32)t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, Int>::type operator*(T const& t, Int const& f) { return f*t; }

			//除法
			Int& operator/=(const Int& vi);
			Int& operator/=(u64 i);
			Int& operator/=(s64 i) { if (i < 0)return (*this /= ((u64)-i)).SelfNegative(); return *this /= ((u64)i); }
			Int& operator/=(u32 i);
			Int& operator/=(s32 i) { if (i < 0)return (*this /= ((u32)-i)).SelfNegative(); return *this /= ((u32)i); }
			Int& operator/=(double i);
			Int& operator/=(float i) { return operator/=((double)i); }
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, Int&>::type operator/=(T const& num)
			{
				return operator/=((u32)num);
			}//无符号整形转为调用operator-=(u32)
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int&>::type operator/=(T const& num)
			{
				return operator/=((s32)num);
			}//有符号整形转为调用operator-=(s32)

			Int operator/(const Int& vi)const;
			Int operator/(u64 num)const;
			Int operator/(s64 num)const;
			Int operator/(u32 num)const;
			Int operator/(s32 num)const;
			double operator/(double num)const { return ((double)*this) / num; }
			float operator/(float num)const { return float(*this / (double)num); }
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int>::type operator/(T const& t) const { return *this / (s32)t; }
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, Int>::type operator/(T const& t)const { return *this / (u32)t; }

			friend double operator/(double num, const Int& vi) { return num / (double)vi; }
			friend float operator/(float num, const Int& vi) { return float((double)num / vi); }
			friend Int operator/(u64 num, const Int& vi)
			{
				if (vi.IsError())return vi;
				if (vi.Equals0())return Int().MakeError(MathError::Divide0);
				if (vi.BitsCount > 64)return 0;
				return vi.Sign < 0 ? Int(num / vi.pData[0]).SelfNegative() : Int(num / vi.pData[0]);
			}
			friend Int operator/(s64 num, const Int& vi) { if (num < 0)return (((u64)-num) / vi).SelfNegative(); return ((u64)num) / vi; }
			friend Int operator/(u32 num, const Int& vi)
			{
				if (vi.IsError())return vi;
				if (vi.Equals0())return Int().MakeError(MathError::Divide0);
				if (vi.BitsCount > 32)return 0;
				return vi.Sign < 0 ? Int(num / vi.pData[0]).SelfNegative() : Int(num / vi.pData[0]);
			}
			friend Int operator/(s32 num, const Int& vi) { if (num < 0)return (((u32)-num) / vi).SelfNegative(); return ((u32)num) / vi; }
			template<class T>inline friend typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int>::type operator/(T const& t, Int const& f) { return ((s32)t) / f; }
			template<class T>inline friend typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, Int>::type operator/(T const& t, Int const& f) { return ((u32)t) / f; }

			//求余
			Int& operator%=(const Int& vi);
			Int& operator%=(u64 vi);
			Int& operator%=(s64 vi) { return vi > 0 ? operator%=((u64)vi) : (operator%=((u64)-vi)).SelfNegative(); }
			Int& operator%=(u32 vi);
			Int& operator%=(s32 vi) { return vi > 0 ? operator%=((u32)vi) : (operator%=((u32)-vi)).SelfNegative(); }
			Int& operator%=(ui32 num) { return *this = SelfDivide(num); }
			Int& operator%=(ci32 num) { return num > 0 ? operator%=((ui32)num) : (operator%=((ui32)-num)).SelfNegative(); }
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, Int&>::type operator%=(T const& num)
			{
				return operator%=((u32)num);
			}//无符号整形转为调用operator%=(u32)
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, Int&>::type operator%=(T const& num)
			{
				return operator%=((s32)num);
			}//有符号整形转为调用operator%=(s32)

			Int operator%(Int const & vi)const;
			//对u64类型求余将忽略正负
			u64 operator%(u64 vi)const;
			s64 operator%(s64 num)const { return num < 0 ? -(s64)operator%((u64)-num) : operator%((u64)num); }
			u32 operator%(u32 num)const;
			s32 operator%(s32 num)const { return num < 0 ? -(s32)operator%((u32)-num) : operator%((u32)num); }
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, T>::type operator%(T const& num)
			{
				return operator%((u32)num);
			}//无符号整形转为调用operator%(u32)
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, T>::type operator%(T const& num)
			{
				return operator%((s32)num);
			}//有符号整形转为调用operator%(s32)

			friend u64 operator%(u64 num, Int const& vi) { return vi > num ? num : num % (u64)vi; }
			friend s64 operator%(s64 num, Int const& vi) { return num < 0 ? -(s64)((u64)num%vi) : (s64)((u64)num%vi); }
			template<class T>friend inline typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, T>::type operator%(T const&t, Int const& num)
			{
				return (Int(t)) % num;
			}//其它转为调用 (Int(t))%num
			template<class T>friend inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, T>::type operator%(T const&t, T const& num)
			{
				return ((u64)t) % num;
			}//其它转为调用 (Int(t))%num
			 //自身除以num，并返回余数
			ui32 SelfDivide(ui32 num);

			Int operator>>(s64 num);
			Int operator<<(s64 num);
			Int& operator>>=(s64 num);
			Int& operator<<=(s64 num);
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, Int&>::type operator<<=(T num) { return operator<<=((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, Int>::type operator<<(T num) { return operator<<((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, Int&>::type operator>>=(T num) { return operator>>=((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, Int>::type operator>>(T num) { return operator>>((s64)num); }

			bool operator==(const Int& vi)const { if (pData == vi.pData)return true; if (BitsCount != vi.BitsCount || Sign != vi.Sign)return false; for (u64 i = 0; i < (BitsCount + 63) / 64; ++i)if (pData[i] != vi.pData[i])return false; return true; }
			bool operator==(u64 num)const { if (Sign == 0)return num == 0; return BitsCount <= 64 && Sign > 0 && pData[0] == num; }
			bool operator==(s64 num)const { if (Sign == 0)return num == 0; return BitsCount <= 64 && (pData[0] == (u64)num) && Sign == (num < 0 ? -1 : 1); }
			bool operator==(double  num)const { return (double)*this == num; }
			bool operator==(float  num)const { return operator==((double)num); }
			template<class T>inline typename std::enable_if<!HAS_ALL_TYPES_EXCEPT_FLOAT, bool>::type operator==(T const& num)const
			{
				return operator==(Int(num));
			}
			template<class T>inline typename std::enable_if<HAS_UNSIGNED_TYPES_EXCEPT64, bool>::type operator==(T num)const
			{
				return operator==((u64)num);
			}
			template<class T>inline typename std::enable_if<HAS_SIGNED_TYPES_EXCEPT64, bool>::type operator==(T num)const
			{
				return operator==((s64)num);
			}

			bool operator>(const Int& vi)const { return Sign > vi.Sign || (Sign > 0 && AbsCompare(*this, vi) > 0) || (Sign < 0 && AbsCompare(*this, vi) < 0); }
			bool operator>(u64 num)const { return Sign > 0 && (BitsCount > 64 || pData[0] > num); }
			bool operator>(s64 num)const { return (num > 0 ? *this > (u64)num:num == 0 ? Sign > 0:(Sign >= 0 || (BitsCount <= 64 && pData[0] < (u64)(-num)))); }
			bool operator>(u32 num)const { return Sign > 0 && (BitsCount > 32 || pData[0] > num); }
			bool operator>(s32 num)const { return (num > 0 ? *this > (u64)num:num == 0 ? Sign > 0:(Sign >= 0 || (BitsCount <= 64 && pData[0] < (u64)(-num)))); }
			bool operator>(i32 num)const { return (num > 0 ? *this > (u64)num:num == 0 ? Sign > 0:(Sign >= 0 || (BitsCount <= 64 && pData[0] < (u64)(-num)))); }
			bool operator>(double  num)const;
			bool operator>(float  num)const { return operator>((double)num); }
			template<class T>inline typename std::enable_if<!HAS_ALL_TYPES_EXCEPT_FLOAT, bool>::type operator>(T const& num)const
			{
				return operator>(Int(num));
			}
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, bool>::type operator>(T const& num)const
			{
				return operator>((u32)num);
			}
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, bool>::type operator>(T const& num)const
			{
				return operator>((s32)num);
			}

			inline bool operator<(const Int& vi)const { return vi>*this; }
			bool operator<(u64 num)const { return Sign < 0 || (Sign == 0 && num != 0) || (BitsCount <= 64 && pData[0] < num); }
			bool operator<(s64 num)const { return (num < 0 ? *this >(u64) - num:num == 0 ? Sign < 0 : (Sign <= 0 || (BitsCount <= 64 && pData[0] < (u64)(num)))); }
			bool operator<(u32 num)const { return Sign < 0 || (Sign == 0 && num != 0) || (BitsCount <= 32 && pData[0] < num); }
			bool operator<(s32 num)const { return (num < 0 ? *this >(u64) - num:num == 0 ? Sign < 0 : (Sign <= 0 || (BitsCount <= 64 && pData[0] < (u64)(num)))); }
			bool operator<(i32 num)const { return (num < 0 ? *this >(u64) - num:num == 0 ? Sign < 0 : (Sign <= 0 || (BitsCount <= 64 && pData[0] < (u64)(num)))); }
			bool operator<(double  num)const;
			bool operator<(float  num)const { return operator<((double)num); }
			template<class T>inline typename std::enable_if<!HAS_ALL_TYPES_EXCEPT_FLOAT, bool>::type operator<(T const& num)const
			{
				return operator<(Int(num));
			}
			template<class T>inline typename std::enable_if<std::is_same<T, ui32>::value || std::is_same<T, u16>::value || std::is_same<T, u8>::value, bool>::type operator<(T const& num)const
			{
				return operator<((u32)num);
			}
			template<class T>inline typename std::enable_if<std::is_same<T, i32>::value || std::is_same<T, s16>::value || std::is_same<T, s8>::value || std::is_same<T, wchar_t>::value || std::is_same<T, char>::value, bool>::type operator<(T const& num)const
			{
				return operator<((s32)num);
			}

			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(const T& t, const Int& vi) { return vi == t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(const T& t, const Int& vi) { return vi < t; }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(const T& t, const Int& vi) { return vi > t; }

			bool operator!=(const Int& vi)const { return !operator==(vi); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T&t, const Int& vi) { return !(t == vi); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(const T&t)const { return !(*this == t); }

			bool operator>=(const Int& vi)const { return !operator<(vi); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T&t, const Int& vi) { return !(t < vi); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(const T&t)const { return !(*this < t); }

			bool operator<=(const Int& vi)const { return !operator>(vi); }
			template<class T>inline friend typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T&t, const Int& vi) { return !(t > vi); }
			template<class T>inline typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(const T&t)const { return !(*this > t); }

			Int& operator|=(const Int&);
			Int& operator&=(const Int&);
			Int& operator^=(const Int&);

			friend Int operator|(Int const &t, Int const&vi);
			friend Int operator&(const Int&t, const Int&vi);
			friend Int operator^(const Int&t, const Int&vi);

			operator u64()const { return (Equals0() || IsError()) ? 0 : pData[0]; }
			operator s64()const { return (Equals0() || IsError()) ? 0 : (Sign > 0 ? pData[0] : -(s64)pData[0]); }
			operator s32()const { return (s32)(operator s64()); }
			operator i32()const { return (i32)(operator s64()); }
			operator u32()const { return (u32)(operator u64()); }
			operator ui32()const { return (ui32)(operator u64()); }
			operator s16()const { return (s16)(operator s64()); }
			operator u16()const { return (u16)(operator u64()); }
			operator s8()const { return (s8)(operator s64()); }
			operator char()const { return (char)(operator s64()); }
			operator u8()const { return (u8)(operator u64()); }
			operator double()const;
			operator bool()const { return Sign != 0; }
			operator float()const { return (float)(operator double()); }
#ifdef LK_MATH_FLOAT256_
			//从Float256构造
			Int(const Float256&f);
			Float256 ToFloat256()const;
			operator Float256()const { return ToFloat256(); }
			Int& operator+=(const Float256& f) { return *this = Int(ToFloat256() + f); }
			Int& operator-=(const Float256& f) { return *this = Int(ToFloat256() - f); }
			Int& operator*=(const Float256& f) { return *this = Int(ToFloat256() * f); }
			Int& operator/=(const Float256& f) { return *this = Int(ToFloat256() / f); }
			Float256 operator+(const Float256& f)const { return f + ToFloat256(); }
			friend Float256 operator+(const Float256& f, const Int& vi) { return f + vi.ToFloat256(); }
			Float256 operator-(const Float256& f)const { return f - ToFloat256(); }
			friend Float256 operator-(const Float256& f, const Int& vi) { return f - vi.ToFloat256(); }
			Float256 operator*(const Float256& f)const { return f * ToFloat256(); }
			friend Float256 operator*(const Float256& f, const Int& vi) { return f * vi.ToFloat256(); }
			Float256 operator/(const Float256& f)const { return f / ToFloat256(); }
			friend Float256 operator/(const Float256& f, const Int& vi) { return f / vi.ToFloat256(); }
			bool operator==(const Float256& f)const { return (s64)BitsCount == f.Exp + 1 && operator==(Int(f)); }
			friend bool operator==(const Float256& f, Int const& vi) { return vi == f; }
			bool operator!=(const Float256& f)const { return !operator==(f); }
			friend bool operator!=(const Float256& f, Int const& vi) { return !(vi == f); }
			bool operator>(const Float256& f)const { return ToFloat256() > f; }
			friend bool operator>(const Float256& f, Int const& vi) { return f > vi.ToFloat256(); }
			bool operator<=(const Float256& f)const { return !(ToFloat256() > f); }
			friend bool operator<=(const Float256& f, Int const& vi) { return !(f > vi.ToFloat256()); }
			bool operator<(const Float256& f)const { return ToFloat256() < f; }
			friend bool operator<(const Float256& f, Int const& vi) { return f < vi.ToFloat256(); }
			bool operator>=(const Float256& f)const { return ToFloat256() >= f; }
			friend bool operator>=(const Float256& f, Int const& vi) { return f >= vi.ToFloat256(); }
#endif
		};
	}

		/**
		@brief 表示一个整数（表示范围可以很大）*/
		template<>struct ptr<Math::Int> : ptr<BigObject>
		{
			PMETHODS(ptr, Math::Int)
			u64 GetBitsCount()const { return chk()->BitsCount; }
			int GetSign()const { return chk()->Sign; }
			/*引用类通用方法*/
			/*构造*/
			ptr(u64 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(u32 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(s64 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(s32 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(ui32 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(int t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(u16 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(s16 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(u8 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(s8 t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(char t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(double t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(float t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(std::string const& t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(std::wstring const& t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(wchar_t const* t) :ptr<BigObject>(new Math::Int(t)) {}
			ptr(char const* t) :ptr<BigObject>(new Math::Int(t)) {}
			/*转换*/
			operator u64()const { return *chk();; }
			operator s64()const { return *chk();; }
			operator s32()const { return *chk();; }
			operator u32()const { return *chk();; }
			operator int()const { return *chk();; }
			operator ui32()const { return *chk();; }
			operator s16()const { return *chk();; }
			operator u16()const { return *chk();; }
			operator s8()const { return *chk();; }
			operator char()const { return *chk();; }
			operator u8()const { return *chk();; }
			operator double()const { return *chk();; }
			operator bool()const { return *chk();; }
			operator float()const { return *chk();; }
			/**
			@brief 绝对值加*/
			inline static ptr AbsPlus(ptr const& xi1, ptr const xi2) { Math::Int*p = new Math::Int; p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(Math::Int::AbsPlus(*CheckPointer<Math::Int>(xi1.get()),* CheckPointer<Math::Int>(xi2.get())))); return p; }
			/**
			@brief 差的绝对值*/
			inline static ptr AbsMinus(ptr const& xi1, ptr const xi2) { Math::Int*p = new Math::Int; p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(Math::Int::AbsMinus(*CheckPointer<Math::Int>(xi1.get()), *CheckPointer<Math::Int>(xi2.get())))); return p; }
			/**
			@brief 获取mov位起的32bit数据（mov从0开始）*/
			u32 GetBits32(u64 mov)const { return chk()->GetBits32(mov); }
			/**
			@brief 往该实例中mov位起写入一个num（32位bit数据）*/
			void SetBits32(ui32 num, u64 mov) { if (chk()->UseCount != 1) *this = new Math::Int(*chk()); return get()->SetBits32(num, mov); }
			/**
			@brief 检测该实例第mov位是否有数据*/
			bool HasBits(u64 mov)const { return chk()->HasBits(mov); }
			/**
			@brief 检测是否有错*/
			bool IsError()const { return chk()->IsError(); }
			/**
			@brief 比较大小*/
			static int AbsCompare(const ptr& vi1, const ptr& vi2) { return Math::Int::AbsCompare(*vi1.chk(), *vi2.chk()); }
			/**
			@brief 自身取绝对值*/
			ptr& SelfAbs() { if (chk()->UseCount != 1) *this = new Math::Int(*get()); get()->SelfAbs(); return *this; }
			/**
			@brief 取绝对值*/
			ptr Abs()const { Math::Int* p = new Math::Int(*chk()); p->SelfAbs(); return p; }
			/**
			@brief 自身取相反数*/
			ptr& SelfNegative() { if (chk()->UseCount != 1) *this = new Math::Int(*get()); get()->SelfNegative(); return *this; }
			/**
			@brief 开平方*/
			ptr Sqrt()const
			{
				if (chk()->Sign == -1) { ptr t = *this; (Conversion::Convert<ptr>::reference(t->CloneIfShared()))->MakeError(Math::MathError::SqrtLessThan0); return t; }
				if (get()->IsError() || GetBitsCount() <= 1)return *this;
				u64* p = get()->pData;
				u64 bitsCount = get()->BitsCount;
				size_t len = (size_t)((bitsCount + 128) / 128);
				u8* arr=new u8[len * 16];
				u64* p1 = (u64*)arr;
				u64* p2 = p1 + len;
				p1[len - 1] = 0;
				u64 sbc = bitsCount - bitsCount / 2;
				Bits::Copy64(p1, 0, p, bitsCount / 2, sbc);
				//牛顿迭代
				for (;;) {
					//将商保存到p2中
					Bits::DivideLow64(p2, len, p, bitsCount, p1, sbc);
					//判断是否满足差距
					u64 flag = p1[0] - p2[0];
					if ((s64)flag <= 1 && (s64)flag >= -1) {
						if (flag == 0) {
							size_t i;
							for (i = 1; i < len; ++i)if (p1[i] != p2[i])break;
							if (i == len)break;
						}
						else if (flag < 0) {
							size_t i = 1;
							if (p1[0] == (u64)-1ll) {
								for (i = 1; i < len; ++i)if (p1[i] + 1 != p2[i])break;
							}
							for (; i < len; ++i)if (p1[i] != p2[i])break;
							if (i == len)break;
						}
						else
						{
							size_t i = 1;
							if (p2[0] == (u64)-1ll) {
								for (i = 1; i < len; ++i)if (p2[i] + 1 != p1[i])break;
							}
							for (; i < len; ++i)if (p1[i] != p2[i])break;
							if (i == len) { p1 = p2; break; }
						}
					}
					//求和
					flag = Bits::SelfPlus64(p1, len, p2, len, 0);
					//求平均数
					for (size_t i = 0; i < len - 1; ++i)
						p1[i] = Bits::ShiftRight(p1[i + 1], p1[i], 1);
					p1[len - 1] = Bits::ShiftRight(flag, p1[len - 1], 1);
					//调整比特数
					if (p1[len - 1] == 0)
						sbc = 64 * len - 64 - Bits::LeftZeroCount64(p1[len - 2]);
					else sbc = 64 * len - Bits::LeftZeroCount64(p1[len - 1]);
				}
				size_t maxs;
				p2 = Math::Int::Malloc<u64>(len, maxs);
				for (size_t i = 0; i < len; ++i)p2[i] = p1[i];
				Math::Int*presult = new Math::Int(p2, len, maxs, sbc, sbc != 0);
				delete[]arr;
				return presult;
			}
			//加法
			ptr& operator+=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator+=(*t.get());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp + *t.chk()));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type operator+=(T const& t)
			{
				Math::Int* tp = chk();
				if (tp->UseCount == 1)
				{
					tp->operator+=(t);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp + t));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type operator=(T const& t)
			{
				if (IsNull())*this = new Math::Int(t);
				else *OnePtr() = t;
				return *this;
			}

			template<class T>friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator+(T const& num, const ptr& x) { return x + num; }
			friend double operator+(double num, const ptr& x) { return x + num; }
			friend float operator+(float num, const ptr& x) { return x + num; }

			ptr operator+(const ptr& x2)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int>::reference(*chk() + *x2.chk()));
				return p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator+(T const& num)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int>::reference(*chk(); + num));
				return p;
			}
			inline double operator+(double num)const { return *chk() + num; }
			inline float operator+(float num)const { return *chk() + num; }

			ptr& operator++()
			{
				Math::Int* p = chk();;
				if (p->UseCount == 1)
				{
					p->operator++();
					return *this;
				}
				return operator+=(1);
			}
			ptr operator++(int) { ptr x = *this; operator+=(1); return x; }
			ptr operator+()const { return *this; }
			//减法
			ptr& operator-=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator-=(*t.chk());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp - *t.chk()));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type operator-=(T const&t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator-=(t);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp - t));
				return *this = p;
			}

			ptr operator-(const ptr& x2)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() - *x2.chk()));
				return p;
			}
			friend double operator-(double t, const ptr&x) { return t - *x.chk(); }
			friend float operator-(float t, const ptr&x) { return t - *x.chk(); }
			template<class T>friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator-(T const&t, const ptr& x)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(t - *x.chk()));
				return p;
			}

			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator-(T const&t)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk(); - t));
				return p;
			}
			inline double operator-(double num)const { return *chk() - num; }
			inline float operator-(float num)const { return *chk() - num; }

			ptr& operator--()
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1) { tp->operator--(); return*this; } return *this -= 1;
			}
			ptr operator--(int) { ptr x = *this; *this -= 1; return x; }
			ptr operator-()const { Math::Int*p = new Math::Int(*chk()); p->SelfNegative(); return p; }
			//乘法
			ptr& operator*=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator*=(*t.chk());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp * *t.chk()));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type 
				operator*=(T const&t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator*=(t);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp * t));
				return *this = p;
			}

			ptr operator*(const ptr& x2)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() * *x2.chk()));
				return p;
			}
			friend double operator*(double t, const ptr&x) { return t * *x.chk(); }
			friend float operator*(float t, const ptr&x) { return t * *x.chk(); }
			template<class T>friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator*(T const&t, const ptr& x)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(t * *x.chk()));
				return p;
			}

			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator*(T const&t)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk(); * t));
				return p;
			}
			inline double operator*(double num)const { return*chk()*num; }
			inline float operator*(float num)const { return*chk()*num; }
			//除法
			ptr& operator/=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator/=(*t.chk());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp / *t.chk()));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type operator/=(T const&t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator/=(t);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp / t));
				return *this = p;
			}

			ptr operator/(const ptr& x2)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() / *x2.chk()));
				return p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator/(T const&t)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() / t));
				return p;
			}
			inline double operator/(double num)const { return*chk() / num; }
			inline float operator/(float num)const { return*chk() / num; }
			template<class T>friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, ptr>::type operator/(T const&t, const ptr& x)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(t / *x.chk()));
				return p;
			}
			friend double operator/(double num, const ptr& x) { return num / *x.chk(); }
			friend float operator/(float num, const ptr& x) { return num / *x.chk(); }

			//求余
			ptr& operator%=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator%=(*t.get());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp % *t.chk()));
				return *this = p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES, ptr&>::type operator%=(T const&t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator%=(t);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp % t));
				return *this = p;
			}
			ptr operator%(const ptr& x2)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() % *x2.chk()));
				return p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, T>::type operator%(T const&t)const { return *chk(); % t; }
			template<class T>friend typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT, T>::type operator%(T const&t, const ptr& x) { return t% *x.chk(); }
			//或
			ptr& operator|=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					*tp|=(*t.get());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp | *t.chk()));
				return *this = p;
			}
			friend ptr operator|(const ptr& x1, const ptr& x2)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*x1.chk() | *x2.chk()));
				return p;
			}
			//与
			ptr& operator&=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator&=(*t.get());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp & *t.chk()));
				return *this = p;
			}
			friend ptr operator&(const ptr& x1, const ptr& x2)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*x1.chk() & *x2.chk()));
				return p;
			}
			//亦或
			ptr& operator^=(const ptr& t)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator^=(*t.get());
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp ^ *t.chk()));
				return *this = p;
			}
			friend ptr operator^(const ptr& x1, const ptr& x2)
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*x1.chk() ^ *x2.chk()));
				return p;
			}
			//左移
			ptr& operator<<=(s64 num)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator<<=(num);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp << num));
				return *this = p;
			}
			ptr operator<<(s64 num)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() << num));
				return p;
			}
			//右移
			ptr& operator>>=(s64 num)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator>>=(num);
					return *this;
				}
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*tp >> num));
				return *this = p;
			}
			ptr operator>>(s64 num)const
			{
				Math::Int *p = new Math::Int();
				p->SwapWith(Conversion::Convert<Math::Int, Math::Int>::reference(*chk() >> num));
				return p;
			}
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, ptr&>::type operator<<=(T num) { return operator<<=((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, ptr>::type operator<<(T num)const { return operator<<((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, ptr&>::type operator>>=(T num) { return operator>>=((s64)num); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES_EXCEPT_FLOAT_INT64, ptr>::type operator>>(T num)const { return operator>>((s64)num); }

			//比较
			bool operator>(const ptr& x2)const { return *chk() > *x2.chk(); }
			bool operator<(const ptr& x2)const { return *chk() < *x2.chk(); }
			bool operator>=(const ptr& x2)const { return *chk() >= *x2.chk(); }
			bool operator<=(const ptr& x2)const { return *chk()<= *x2.chk(); }
			bool operator==(const ptr& x2)const { return *chk()== *x2.chk(); }
			bool operator!=(const ptr& x2)const { return *chk() != *x2.chk(); }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>(T const&t)const { return *chk() > t; }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<(T const&t) const { return *chk() < t; }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator>=(T const&t)const { return *chk() >= t; }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator<=(T const&t)const { return *chk() <= t; }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator==(T const&t)const { return *chk() == t; }
			template<class T>typename std::enable_if<HAS_ALL_TYPES, bool>::type operator!=(T const&t) const { return *chk() != t; }

			//逻辑
			bool Equals0()const { return chk()->Equals0(); }
			bool operator!()const { return chk()->Equals0(); }
#ifdef LK_MATH_FLOAT256_
			//从Math::Float256构造
			ptr(Math::Float256 const& t) :ptr<BigObject>(new Math::Int(t)) {}
			operator Math::Float256()const { return chk()->ToFloat256(); }
			Math::Float256 ToFloat256()const { return chk()->ToFloat256(); }
			ptr& operator+=(const Math::Float256& f)
			{
				Math::Int* tp = chk();
				if (tp->UseCount == 1)
				{
					tp->operator+=(f);
					return *this;
				}
				return *this = *this + f;
			}
			Math::Float256 operator+(const Math::Float256& f)const { return f + ToFloat256(); }
			friend Math::Float256 operator+(const Math::Float256& f, const ptr& vi) { return f + vi.ToFloat256(); }
			ptr& operator-=(const Math::Float256& f)
			{
				if (chk()->UseCount == 1)
				{
					get()->operator-=(f);
					return *this;
				}
				return *this = *this - f;
			}
			Math::Float256 operator-(const Math::Float256& f)const { return f - ToFloat256(); }
			friend Math::Float256 operator-(const Math::Float256& f, const ptr& vi) { return f - vi.ToFloat256(); }
			ptr& operator*=(const Math::Float256& f)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator*=(f);
					return *this;
				}
				return *this = *this * f;
			}
			Math::Float256 operator*(const Math::Float256& f)const { return f * ToFloat256(); }
			friend Math::Float256 operator*(const Math::Float256& f, const ptr& vi) { return f * vi.ToFloat256(); }
			ptr& operator/=(const Math::Float256& f)
			{
				Math::Int* tp = chk();;
				if (tp->UseCount == 1)
				{
					tp->operator/=(f);
					return *this;
				}
				return *this = *this / f;
			}
			Math::Float256 operator/(const Math::Float256& f)const { return f / ToFloat256(); }
			friend Math::Float256 operator/(const Math::Float256& f, const ptr& vi) { return f / vi.ToFloat256(); }
			bool operator==(const Math::Float256& f)const { return chk()->operator==(f); }
			friend bool operator==(const Math::Float256& f, ptr const& vi) { return vi == f; }
			bool operator!=(const Math::Float256& f)const { return !operator==(f); }
			friend bool operator!=(const Math::Float256& f, ptr const& vi) { return !(vi == f); }
			bool operator>(const Math::Float256& f)const { return ToFloat256() > f; }
			friend bool operator>(const Math::Float256& f, ptr const& vi) { return f > vi.ToFloat256(); }
			bool operator<=(const Math::Float256& f)const { return !(ToFloat256() > f); }
			friend bool operator<=(const Math::Float256& f, ptr const& vi) { return !(f > vi.ToFloat256()); }
			bool operator<(const Math::Float256& f)const { return ToFloat256() < f; }
			friend bool operator<(const Math::Float256& f, ptr const& vi) { return f < vi.ToFloat256(); }
			bool operator>=(const Math::Float256& f)const { return ToFloat256() >= f; }
			friend bool operator>=(const Math::Float256& f, ptr const& vi) { return f >= vi.ToFloat256(); }
#endif
		};

	template<>struct Conversion::Convert <std::string, ptr<Math::Int> >
	{
		static inline std::string from(ptr<Math::Int> const&l) { return l->ToString(); }
	};
	template<>struct Conversion::Convert <std::string, Math::Int>
	{
		static inline std::string from(Math::Int const&l) { return l.ToString(); }
	};
}
namespace std//定义数学运算
{
	/*template<>struct is_integral<LK::ptr<LK::Math::Int> >
	{
		static const bool V = true;
	};
	template<>struct is_integral<LK::ptr<LK::Math::Int>>
	{
		static const bool V = true;
	};*/
	inline ::LK::ptr<LK::Math::Int> sin(::LK::ptr<LK::Math::Int> const& x) { if (x.IsError())return x; return 0; }
	inline ::LK::ptr<LK::Math::Int> cos(::LK::ptr<LK::Math::Int> const& x) { if (x.IsError())return x; return x.Equals0() ? 1 : 0; }
	inline ::LK::ptr<LK::Math::Int> fmod(::LK::ptr<LK::Math::Int> const& f1, ::LK::ptr<LK::Math::Int> const& f2) { return f1%f2; }
	inline ::LK::ptr<LK::Math::Int> tan(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Tan(); }
	inline ::LK::ptr<LK::Math::Int> log(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Ln(); }
	inline ::LK::ptr<LK::Math::Int> log10(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Lg(); }
	::LK::ptr<LK::Math::Int> pow(::LK::ptr<LK::Math::Int> const& f1, ::LK::ptr<LK::Math::Int> const& f2);
	inline ::LK::ptr<LK::Math::Int> asin(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Arcsin(); }
	inline ::LK::ptr<LK::Math::Int> acos(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Arccos(); }
	inline ::LK::ptr<LK::Math::Int> atan(::LK::ptr<LK::Math::Int> const& f) { return f.ToFloat256().Arctan(); }
	inline ::LK::ptr<LK::Math::Int> sqrt(::LK::ptr<LK::Math::Int> const& f) { return f.Sqrt(); }
	inline ::LK::ptr<LK::Math::Int> abs(::LK::ptr<LK::Math::Int> const& f) { return f.Abs(); }
	inline ::LK::ptr<LK::Math::Int> floor(::LK::ptr<LK::Math::Int> const& f) { return f; }
	inline ::LK::ptr<LK::Math::Int> ceil(::LK::ptr<LK::Math::Int> const& f) { return f; }
}
#undef HAS_SIGNED_TYPES_EXCEPT64
#undef HAS_UNSIGNED_TYPES_EXCEPT64
#undef HAS_ALL_TYPES_EXCEPT_FLOAT
#undef HAS_ALL_TYPES
#undef HAS_ALL_TYPES_EXCEPT_FLOAT_INT64
#endif
