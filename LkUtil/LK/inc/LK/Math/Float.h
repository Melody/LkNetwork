#pragma once
#ifdef LK_MATH_FLOAT256_//必须要Float256支持
#define LK_MATH_FLOAT_
namespace LK
{
	namespace Math
	{
		typedef ptr<Float> floatx;
		/**
		@brief 存储Float类的浮点数据（有效数字）*/
		struct FloatData : BigObject
		{
			/**
			@brief 分配数据指针*/
			u64 *source;
			/**
			@brief 数据起始指针*/
			u64 *data;
			/**@brief 有内容部分bit数（从最高位数起）*/
			u64 bitsCount;
			/**@brief 数组元素数*/
			size_t Length;
			//基类通用方法
			BMETHODS(ptr<FloatData>, FloatData)
			/**@brief 默认构造函数，构造空数据*/
			FloatData() : source(NULL), data(NULL), bitsCount(0), Length(0) {}
			/**
			@param[传入]参数bN表示有效bit数量
			@param[传入]参数len表示pd数组元素数量
			@param[传入]参数pd表示数据数组起始位置
			@param[传入,可选]参数ps表示数据分配其实位置
			@attention ps指向的空间必须用Malloc分配，而且必须有pd >= ps*/
			FloatData(u64 bN, size_t len, u64 *pd, u64 *ps = NULL)
				:source(ps == NULL ? pd : ps), data(pd), bitsCount(bN), Length(len) {
				if (ps > pd)Throw<std::runtime_error >("指针异常");
			}
			/**
			@brief 使用p所指内存区左移startPosition 个bit位置起的bitscount个bit的数据作为有效数据构造
			@param [传入]参数p表示内容起始地址
			@param [传入]参数startPosition表示起始数据对p的比特偏移量
			@param [传入]参数bitscount表示数据的比特数*/
			FloatData(const void *p, u64 startPosition, u64 bitscount)
				:source(NULL), data(NULL), bitsCount(0), Length(GetLengthByBits(bitscount))
			{
				size_t size = GetSizeByLength(Length);
				source = Malloc<u64>(size);
				data = source + size - Length;
				if (Length != 0)data[0] = 0;
				Bits::Copy(data, (index_t)(Length * 64ull - bitscount), p, (index_t)startPosition, (size_t)bitscount);
				if (Length)data[Length - 1] |= MinValues::Int64;
				bitsCount = CheckBitsCount();
			}
			/**
			@brief 复制构造函数*/
			FloatData(FloatData const& vf) :source(NULL), data(NULL), bitsCount(vf.bitsCount), Length(0)
			{
				if (bitsCount != 0)
				{
					Length = GetLengthByBits(bitsCount);
					try {
						size_t size = GetSizeByLength(Length);
						source = Malloc<u64>(size);
						data = source + size - Length;
						memcpy(data, vf.data + vf.Length - Length, sizeof(u64)*Length);
					}
					catch (std::bad_alloc  const&e) { Free(source); source = data = NULL; Length = 0; Throw<std::bad_alloc>(e); }
				}
			}
			/**
			@brief 返回一个浅表副本*/
			ptr<BigObject> Clone()const { return new FloatData(*this); }
			/**
			@brief 赋值*/
			FloatData& operator=(FloatData const& vf)
			{
				size_t len = GetLengthByBits(vf.bitsCount);
				if (len > Length)source =Realloc(source, Length = len);
				data = source + Length - len;
				for (size_t i = 0; i < len; ++i)data[i] = vf.data[i];
				bitsCount = vf.bitsCount;
				return *this;
			}
			/**
			@brief 复制构造并重新指定精度用以舍入
			@param [传入]参数vf表示一个FloatData数据
			@param [传入]参数precision表示精度大小
			@attention precision不受Float::MaxPrecision和Float::MinPrecision影响。设置precision小于bitscount时有可能导致全0进位*/
			FloatData(FloatData const& vf, u64 precision) :source(NULL), data(NULL), bitsCount(vf.bitsCount), Length(0)
			{
				size_t vflen = vf.Length;
				if (bitsCount <= precision)
				{
					Length = GetLengthByBits(bitsCount);
					try {
						size_t size = GetSizeByLength(Length);
						source = Malloc<u64>(size);
						data = source + size - Length;
					}
					catch (std::bad_alloc  const&e) { Free(source); Length = 0; source = data = NULL; Throw<std::bad_alloc>( e); }
					memcpy(data, vf.data + vflen - Length, Length*sizeof(u64));
				}
				else
				{
					size_t i = vf.Length - GetLengthByBits(precision);
					bool flag = ((precision & 63) != 0 && Bits::LeftZeroCount64(vf.data[i] ^ MaxValues::UInt64) > (index_t)(precision & 63))
						|| ((precision & 63) == 0 && i > 0 && vf.data[i - 1] > MaxValues::Int64);//是否需要进位
					if (flag)
					{
						while (++i < vf.Length)if (vf.data[i] != MaxValues::UInt64)break;
						if (i == vf.Length)//舍入导致全0,进位
						{
							try
							{
								size_t size = GetSizeByLength(1);
								source = Malloc<u64>(size);
								data = source + size - 1;
								Length = 1;
							}
							catch (std::bad_alloc  const&e) { Free(source); Length = 0; source = data = nullptr; Throw<std::bad_alloc>( e); }
							*data = 0;
							bitsCount = 0;
							goto End;
						}
					}
					try {
						Length = vf.Length - i;
						size_t size = GetSizeByLength(Length);
						source = Malloc<u64>(vf.Length - i);
						data = source + size - Length;
					}
					catch (std::bad_alloc  const&e) { Free(source); source = data = NULL; Length = 0;Throw<std::bad_alloc>( e); }
					memcpy(data, vf.data + i, Length*sizeof(u64));
					bitsCount = CheckBitsCount();
				}
			End:;
			}

			/**
			@brief 析构函数，释放内容*/
			~FloatData() { Free(source); }

			/**
			@brief 调整数据部分大小，并将新增部分置0
			@param [传入]参数len表示新的元素数量
			@note 当len小于bitsCount需求的Length时将导致bitsCount重算*/
			void Resize(size_t len)
			{
				if (len <= Length)
				{
					if (len < Length)
					{
						data += Length - len;
						Length = len;
						bitsCount = CheckBitsCount();
					}
					return;
				}
				if (len <= Length + (size_t)(data - source))
				{
					while (Length != len) { ++Length; *--data = 0; }
					return;
				}
				size_t size = GetSizeByLength(len);
				u64*p = Malloc<u64>(size);
				memcpy(p + size - Length, data, Length*sizeof(u64));
				Free(source);
				source = p;
				data = source + size - len;
				for (size_t i = 0; i < len - Length; ++i) data[i] = 0;
				Length = len;
			}
			//功能方法

			/**
			@brief 根据bits的大小获取至少需要的len大小
			@param 参数bits表示有数据位数
			@return 容纳这么多bits的数据至少需要的u64数组的元素数量*/
			static inline size_t GetLengthByBits(u64 bits) { return bits == 0 ? 0 : (size_t)(((bits - 1) >> 6) + 1); }
			/**
			@brief 根据指定元素数量获取实际适合分配的元素数量
			@param [传入]参数len表示存储数据至少需要的元素数量
			@return 返回实际适合分配的元素数量*/
			static inline size_t GetSizeByLength(size_t len) { return len == 0 ? 0 : Memory::GetResize(len*sizeof(u64)) / sizeof(u64); }
			/**
			@brief 重新计算当前有内容的bit数
			@param [传入]参数startIndex为开始进行计算的索引
			@return 返回计算结果，表示有内容的比特数*/
			inline u64 CheckBitsCount(size_t startIndex = 0) { if (data == NULL)return 0; for (size_t i = startIndex; i < Length; ++i) { if (data[i])return (Length - i) * 64ull - Bits::RightZeroCount64(data[i]); } return 0; }
			void operator<<=(s64 num)
			{
				if (num >= (s64)bitsCount) { data += Length; Length = 0; bitsCount = 0; }
				else
				{
					Bits::Move64(data, Length, num);
					bitsCount -= num;
					num = FloatData::GetLengthByBits(bitsCount);
					data += Length - num;
					Length = (size_t)num;
				}
			}
			/**
			@brief 设置尾数指定位(index~index+63)上的64bit内容，index<0时才产生效果，而且最高位保持1*/
			void SetDat64(s64 index, u64 dat)
			{
				if (index >= 0)return;
				index_t idx = (index_t)(Length + ((index - 64) / 64));
				index = 63 ^ ((-index) & 63);
				if (idx < 0) { Resize((index_t)Length - idx); idx = 0; }
				if (idx + 1 == (index_t)Length)
				{
					if (index == 0)data[idx] = dat | MinValues::Int64;
					else data[idx] = (data[idx] & (((u64)-1) >> (64 - index))) | MinValues::Int64 | (dat << index);
				}
				else
				{
					if (index == 0)data[idx] = dat;
					else
					{
						data[idx] = (data[idx] & (((u64)-1) >> (64 - index))) | (dat << index);
						data[idx + 1] = (data[idx + 1] & (((u64)-1) << index)) | (dat >> (64 - index));
					}
				}
				if (idx == 0)bitsCount = CheckBitsCount();
			}
			/**
			@brief 获取尾数指定位(index~index+63)上的64bit内容*/
			u64 GetDat64(s64 index)const
			{
				if (index >= 0)return index == 0 && Length != 0;
				index_t idx = (index_t)(Length + ((index - 64) / 64));
				index = 63 ^ ((-index) & 63);
				if (index == 0)return (idx > 0 && idx < (index_t)Length) ? data[idx] : 0;
				if (idx < 0)return (idx == -1 && Length != 0) ? (data[0] << (64 - index)) : 0;
				if (idx + 1 == (index_t)Length)return data[idx] >> index;
				return (data[idx] >> index) | (data[idx + 1] << (64 - index));
			}
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
				friend struct ptr<Math::FloatData>;
				friend struct Float;
		};
	}
	/**
	@brief 浮点数据代理,表示Float的“有效数字”*/
	template<>struct ptr<Math::FloatData> : ptr<BigObject>
	{
		PMETHODS(ptr, Math::FloatData);
		/**
		@brief 复制并构造指定精度的有效数据
		@param [传入]参数x表示被复制的有效数据
		@param [传入]参数prec表示复制的精度
		@note 这里的prec不受Float::MinPrecision和Float::MaxPrecision影响	*/
		ptr(const ptr& x, u64 prec) { if (x==nullptr)Throw<PointerIsNullException >(); *this = new Math::FloatData(*x.get(), prec); }
		/**
		@brief 使用指定的精度进行有效数字的相加运算，相当于*this+=x;
		@param [传入]参数x表示加数
		@param [传入]参数mov表示加数与被加数的指数差
		@param [传入]参数prec表示计算过程中的精度位数限制
		@return [传入]返回值为0时表示未产生进位；返回值为1时表示产生了进位；返回值为2时表示两数都是全1，在精度限制的作用下产生两次进位
		@note 合理设置prec参数可以加快计算，这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		int SelfPlus(ptr const&x, s64 mov, u64 prec);
		/**
		@brief 使用指定的精度进行有效数字的相加运算，相当于*this=x1+x2;
		@param [传入]参数x1表示加数1
		@param [传入]参数x2表示加数2
		@param [传入]参数mov表示2加数与被加数1的指数差
		@param [传入]参数prec表示计算过程中的精度位数限制
		@return 返回值为0时表示未产生进位；返回值为1时表示产生了进位；返回值为2时表示两数都是全1，在精度限制的作用下产生两次进位
		@note 相加得到的结果将按两个加数中的最高位来设置最高位
		@note 合理设置prec参数可以加快计算，这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		int FromPlus(ptr x1, ptr<Math::FloatData> x2, s64 mov, u64 prec);
		/**
		@brief 使用指定的精度进行-=运算
		@param [传入]参数x表示减数
		@param [传入]canshumov表示减数高位相对本数字高位的位移
		@param [传入]参数prec表示按本数最高位计算的计算精度
		@return 返回相减操作导致的最高位指数变化量
		@attention 要求mov<0，或者mov==0且*this>=x。返回结果<=0
		@attention 这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		s64 SelfMinus(ptr const&x, s64 mov, u64 prec);
		/**
		@brief this = x1-x2
		@param [传入]参数x1为被减数
		@param [传入]参数x2为减数
		@param [传入]参数mov为x2最高位相对于x1最高位的位移
		@param [传入]参数prec表示按x1最高位计算的计算精度
		@return 返回相减操作导致的相对原来x1的最高位指数变化量
		@attention 该操作自动进行数据位移保证最高位为1。要求mov<0，或者mov==0且x1>=x2
		@attention 这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		s64 FromMinus(ptr x1, ptr<Math::FloatData> x2, s64 mov, u64 prec);
		/**
		@brief 使用指定的精度进行*=运算
		@param [传入]参数x表示乘数
		@param [传入]参数prec表示按本数最高位计算的计算精度
		@return 返回相乘导致的指数偏移（可能是0、1、2）*/
		int SelfMultiply(ptr  const& x, u64 prec);
		/**
		@brief this = x1 * x2
		@param [传入]参数x1表示被乘数
		@param [传入]参数x2表示乘数
		@param [传入]参数prec表示按x1最高位计算的计算精度
		@return 返回相乘导致的指数偏移（可能是0、1、2）*/
		int FromMultiply(ptr x1, ptr<Math::FloatData> x2, u64 prec);
		/**
		@brief 使用指定的精度进行/=运算，返回值指示最高位指数是否发生变化
		@param [传入]参数x表示除数
		@param [传入]参数prec表示计算精度
		@return 返回值为0时表示最高位指数未发生变化，返回值为-1时表示最高位指数降1
		@attention 这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		int SelfDivide(ptr const& x, u64 prec);
		/**
		@brief this = x1 / x2
		@param[传入]参数x1表示被除数
		@param[传入]参数x2表示除数
		@param[传入]参数prec表示计算精度
		@return 返回值为0时表示最高位指数相对于x1未发生变化，返回值为 - 1时表示最高位指数降1
		@attention 这里的prec不受Float::MinPrecision和Float::MaxPrecision影响*/
		int FromDivide(ptr x1, ptr<Math::FloatData> x2, u64 prec);
		/**
		@brief 获取整数部分（截取）
		@param [传入]参数bits表示整数位数*/
		void SelfInteger(u64 bits);
		/**
		@brief 将x的整数部分赋值给this（截取）
		@param [传入]参数x表示操作数
		@param [传入]参数prec表示x的整数位数*/
		void FromInteger(ptr const& x, u64 bits);
		/**
		@brief 获取小数部分（截取）
		@param [传入]参数mov表示该数字d整数位数
		@return 返回实际导致的指数变化量*/
		s64 SelfDecimal(u64 mov);
		/**
		@breif 将x的小数部分赋值给this（截取）
		@param [传入]参数x表示操作数
		@param [传入]参数mov表示x的整数位数
		@return 返回实际导致的相对于x的指数的变化量*/
		s64 FromDecimal(ptr const& x, u64 mov);
		/**
		@breif 重新调整精度，返回是否发生进位
		@param [传入]参数bits表示该数精度位数
		@return 返回值为1时表示调整产生了进位*/
		bool SelfSetBits(u64 bits);
		/**
		@brief 设置尾数指定位(index~index+63)上的64bit内容，要求index<0，而且最高位保持1*/
		void SetDat64(s64 exp, u64 dat)
		{
			Conversion::Convert<ptr>::reference(chk()->CloneIfShared())->SetDat64(exp, dat);
		}
		/**
		@brief 获取尾数指定位(index~index+63)上的64bit内容*/
		u64 GetDat64(s64 index)const
		{
			return chk()->GetDat64(index);
		}
		/**
		@brief 比较
		@param [传入]参数xf为被比较数
		@param [传入]参数mov为xf最高位相对于该数最高位偏移量
		@return 返回值为0时表示两数相等，返回值大于0时表示该数>偏移mov的xf，否则该数<偏移mov的xf。返回值的绝对值表示差异位到最高位的位数*/
		s64 CompareWith(ptr const& xf, s64 mov)const;

		/**
		@brief 获取64bit数据
		@param [传入]参数mov表示获取的64bit最高位相对于最高位偏移量
		@return 返回获取的64bit数据*/
		inline u64 GetWindow64(s64 mov)const
		{
			Math::FloatData*p = get();
			return Bits::GetWindow64High64(p->data, p->Length, mov);
		}
		/**
		@brief 空有效部位*/
		static ptr const GetEmpty();

	};
	namespace Math {
		struct Float : BigObject
		{
		public:
			/*实体基类通用方法*/
			BMETHODS(ptr<Math::Float>, Float)
				/**
					@brief 析构函数*/
				~Float() {}
			/**
			@brief 设置当前Data所引用的对象的引用计数是否是原子增减的*/
			void SetEnableInterlock(bool tr)const
			{
				Data.chk()->UseCount.SetEnableThreadSafe(tr);
			}
			/**
			@brief 交换方法*/
			void SwapWith(Float & vf)
			{
				BigObject::Swap(Exp, vf.Exp); BigObject::Swap(Precision, vf.Precision); BigObject::Swap(Sign, vf.Sign);BigObject::Swap(Data, vf.Data);
			}
			/**
			@brief 赋值*/
			Float& operator=(Float const& vf)
			{
				Sign = vf.Sign;
				Exp = vf.Exp;
				Data = vf.Data;
				Precision = vf.Precision;
				return*this;
			}
#ifdef SUPPORT_RIGHT_VALUE_REFERENNE
			Float& operator=(Float && vf)
			{
				SwapWith(vf);
				return*this;
			}
#endif
			/**
			@brief 指数*/
			Float256 Exp;
			/**
			@brief 有效数字*/
			ptr<Math::FloatData> Data;
			/**
			@brief 当前数值精度,Precision在加减法中有可能增加或减少，在乘幂运算中不会变化*/
			u64 Precision;
			/**
			@brief 符号位和错误信息*/
			int Sign;
			/**
			@brief 最小数值精度，认为所有数字的Precision不会小于这个数值*/
			static u64 MinPrecision;
			/**
			@brief 最小数值精度，认为所有数字的Precision不会大于这个数值*/
			static u64 MaxPrecision;
			/**
			@brief 默认的初始化精度*/
			static u64 DefPrecision;
			/**
			@brief 预留精度系数（(当前精度-预留精度系数)*lg2得到十进制精度）*/
			static u64 OblPrecision;
			/**
			@brief 终止计算标记，设置为true时所有较花时间的计算都会立即返回，并且IsError为true*/
			static bool StopCalculating;
			/**
			@brief 与0比较，相等时返回true*/
			inline bool Equals0()const { return Sign == 0; }
			/**
			@brief 是否出错，出错时返回true*/
			inline bool IsError()const { return Sign < -1; }
			/**
			@brief 产生一个错误*/
			inline Float& MakeError(int code) { Sign = code; return*this; }
			/**
			@brief 复制构造*/
			Float(Float const&vf) :Exp(vf.Exp), Data(vf.Data), Precision(vf.Precision), Sign(vf.Sign) {}
			Float(Float const&vf, u64 prec) : Exp(vf.Exp), Data(vf.Data), Precision(prec > MaxPrecision ? MaxPrecision : prec < MinPrecision ? MinPrecision : prec), Sign(vf.Sign) { if (Data.SelfSetBits(Precision))++Exp; }
			/**
			@brief 使用正负号、指数、有效数字、精度进行初始化*/
			Float(int s, Float256 const& e, ptr<Math::FloatData> const& xf, s64 prec = DefPrecision) : Exp(e), Data(xf), Precision(prec), Sign(s) { if (Data.SelfSetBits(Precision))++Exp; }
			/**
			@breif 默认构造函数,构造0*/
			Float() : Exp(0), Data(ptr<Math::FloatData>::GetEmpty()), Precision(DefPrecision), Sign(0) {}
#ifdef LK_MATH_FLOATN_
			template<size_t N>Float(FloatN<N> const& num, u64 Precision = DefPrecision)
			{
				/*位数*/
				u64 bits = Precision + 1 > N * 64ll ? N * 64ll : Precision + 1;
				/*有效数字*/
				Data = new FloatData(num.Data.arr, N * 64ll - bits, bits);
				/*指数*/
				Exp = num.Exp;
				/*符号*/
				Sign = num.Sign;
				/*精度*/
				this->Precision = Precision;
				if (Data.SelfSetBits(Precision))++Exp;
			}
#endif
			/**
			@brief 从整形构造*/
			Float(u64 num, u64 prec = DefPrecision) :Precision(prec)
			{
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				//符号
				Sign = (num == 0) ? 0 : 1;

				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(unsigned long num, u64 prec = DefPrecision) :Precision(prec)
			{
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				//符号
				Sign = (num == 0) ? 0 : 1;
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(unsigned int num, u64 prec = DefPrecision) :Precision(prec)
			{
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				//符号
				Sign = (num == 0) ? 0 : 1;
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(unsigned short num, u64 prec = DefPrecision) :Precision(prec)
			{
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				//符号
				Sign = (num == 0) ? 0 : 1;
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(unsigned char num, u64 prec = DefPrecision) :Precision(prec)
			{
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				//符号
				Sign = (num == 0) ? 0 : 1;
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(bool num, u64 prec) : Exp(0), Data(), Precision(prec), Sign(0)
			{
				if (num)
				{
					//符号
					Sign = 1;
					//有效数字
					Data = new FloatData(&num, 0, 1);
				}
				else Data = ptr<Math::FloatData>::GetEmpty();
			}
			Float(s64 num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(long num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(int num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(short num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(signed char num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(wchar_t num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(char num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				if (num < 0) { Sign = -1; num = -num; }
				else Sign = (num == 0) ? 0 : 1;
				//有效数字
				Data = new FloatData(&num, 0, 64 - Bits::LeftZeroCount64(num));
				//指数
				Exp = (num == 0) ? 0 : (63 - Bits::LeftZeroCount64(num));
				if (Data.SelfSetBits(Precision))++Exp;
			}
#ifdef LK_MATH_INT_
			Float(ptr<Math::Int> const& x, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				Sign = x.chk()->Sign;
				//有效数字
				Data = new FloatData(x.chk()->pData, 0, x.chk()->BitsCount);
				//指数
				Exp = (x == 0) ? 0 : x.chk()->BitsCount - 1;
				if (Data.SelfSetBits(Precision))++Exp;
			}
#endif
			/**
			@brief 从浮点构造*/
			Float(double num, u64 prec = DefPrecision) :Precision(prec)
			{
				union
				{
					double t;
					u64 n;
				};
				t = num;
				if (num == 0)
				{
					Sign = 0; Data = ptr<Math::FloatData>::GetEmpty(); Exp = 0;
				}
				else {
					//符号
					Sign = num < 0 ? -1 : 1;
					//指数
					Exp = (s64)(((0x7ff0000000000000 & n) >> 52) - 1023);
					n |= 0x10000000000000;
					//有效数字
					Data = new FloatData(&n, 0, 53);
				}
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(float num, u64 prec = DefPrecision) :Precision(prec)
			{
				union
				{
					float t;
					u32 n;
				};
				t = num;
				if (num == 0)
				{
					Sign = 0; Data = ptr<Math::FloatData>::GetEmpty(); Exp = 0;
				}
				else {
					//符号
					Sign = num < 0 ? -1 : 1;
					//指数
					Exp = (long)(((0x7f800000 & n) >> 23) - 127);
					n |= 0x800000;
					//有效数字
					Data = new FloatData(&n, 0, 24);
				}
				if (Data.SelfSetBits(Precision))++Exp;
			}
			Float(Float256 const& num, u64 prec = DefPrecision) :Precision(prec)
			{
				//符号
				Sign = num.Sign;
				//指数
				Exp = num.Exp;
				//有效数字
				Data = new FloatData(num.Data.ull, 0, 256);
				if (Data.SelfSetBits(Precision))++Exp;
			}

			/**
			@brief 从字符串构造*/
			Float(std::string const& str, u64 prec = DefPrecision);

			/**
			@breif 转换为无符号整形(截取，非舍入)*/
			u64 ToUInt64()const
			{
				if (Sign < 0)return (u64)ToInt64();
				if (Sign == 0)return 0;
				if (Exp < 0)
					return 0;
				return Data.GetWindow64(63 - (s64)Exp);
			}
			/**
			@breif 转换为有符号整形(截取，非舍入)*/
			s64 ToInt64()const
			{
				if (Exp < 0)
					return 0;
				u64 d = Data.GetWindow64(63 - (s64)Exp);
				if (Sign < 0)
					return -(s64)d;
				return d;
			}
			/**
			@breif 转换为浮点（舍入）*/
			double ToDouble()const
			{
				if (Sign == 0)return 0;
				if (Exp > 1023)
				{
					if (Sign < 0)
						return MinValues::Double;
					return MaxValues::Double;
				}
				if (Exp < -1023)
					return 0;
				union
				{
					u64 d;
					double num;
				};
				d = Data.GetWindow64(-1);
				if ((d&(1ull << 11))) { d = (d >> 12) + 1; }
				else d >>= 12;
				d |= ((s64)Exp + 1023) << 52;
				if (Sign < 0)
					return -num;
				return num;
			}
			/**
			@brief 转换为Float256（截取）*/
			Float256 ToFloat256()const
			{
				Float256 f = Float256::Zero();
				if (Equals0())return f;
				if (Exp > Float256::ExpMaxValue)return f.MakeError(MathError::InputOverflow);
				f.Sign = Sign;
				f.Exp = Exp;
				size_t len = Data.chk()->Length;
				Bits::CopyShift64(Data.chk()->data, len, f.Data.ull, 4, 256 - (s64)(len * 64ll));
				if ((s64)Data.GetWindow64(-256) < 0)
				{
					if (++f.Data)
					{
						f.Data.ui[7] |= MinValues::Int32;
						f.Exp++;
					}
				}
				return f;
			}
#ifdef LK_MATH_FLOATN_
			/**
			@brief 转换为FloatN浮点数*/
			template<size_t N>FloatN<N> ToFloatN()const
			{
				FloatN<N> f = 0;
				if (Equals0() || Exp < MinValues::Int64)return f;
				if (Exp > MaxValues::Int64)return f.MakeError(MathError::InputOverflow);
				f.Sign = Sign;
				f.Exp = Exp;
				size_t len = Data.chk()->Length;
				Bits::Copy64(Data.chk()->data, len, f.Data.arr, N, (s64)N * 64ll - (s64)(len * 64ull));
				if ((s64)Data.GetWindow64((s64)N * -64ll) < 0)
				{
					if (++f.Data)
					{
						f.Data.arr[N - 1] |= MinValues::Int64;
						f.Exp++;
					}
				}
				return f;
			}
#endif
			/**
			@brief 返回一个浅表副本*/
			ptr<BigObject> Clone()const { return new Float(*this); }

			//＋号,返回副本
			Float operator+()const { return *this; }
			//+=自加
			Float& operator+=(Float const&vf);
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator+=(const T& t) { return operator+=(Float(t)); }
			//自增(前置)
			Float& operator++();
			//自增(后置)
			Float operator++(int);
			//相加
			Float operator+(Float const&vf)const;
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator+(const T& t)const { return operator+(Float(t)); }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator+(const T& t, const Float& vf) { return vf + t; }

			//-号，返回相反数副本
			Float operator-()const { return Float(*this).SelfNegative(); }
			/**
			@breif 自身取相反数*/
			Float&SelfNegative() { if (Sign >= -1)Sign = -Sign; return *this; }
			//-=自减
			Float& operator-=(Float const&vf);
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator-=(const T& t) { return operator-=(Float(t)); }
			//自减(前置)
			Float& operator--();
			//自减(后置)
			Float operator--(int);
			//相减
			Float operator-(Float const&vf)const;
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator-(const T& t)const { return operator-(Float(t)); }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator-(const T& t, const Float& vf) { return (vf - t).SelfNegative(); }

			//自乘
			Float& operator*=(Float const&vf);
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator*=(const T& t) { return operator*=(Float(t)); }
			//相乘
			Float operator*(Float const&vf)const;
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator*(const T& t)const { return operator*(Float(t)); }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator*(const T& t, const Float& vf) { return vf * t; }
			/**
			@brief 乘以2
			@return 返回该数字的两倍*/
			Float Multiply2()const { return Float(Sign, Exp + Float256::One(), Data, Precision); }
			/**
			@breif 自身乘以2的x次方,精度不变
			@param [传入,可选]参数x为次方数，默认为1
			@return 返回该数字本身*/
			Float&SelfMultiply2(Float256 const& x = Float256::One()) { Exp += x; return *this; }
			/**
			@brief 自身乘以10的x次方（x>=0）,精度不变
			@param [传入,可选]参数x表示次方数，默认为1
			@return 返回该数字本身*/
			Float&SelfMultiply10(u64 x = 1)
			{
				ptr<Math::FloatData> d;
				switch (x)
				{
				case 0:break;
				case 1:Exp += Float256::Three(); if (Data.FromPlus(Data, Data, -2, Precision)) { ++Exp; }break;
				case 2:d = Data; Exp += Float256::Six();
					if (Data.FromPlus(Data, Data, -1, Precision))
					{
						++Exp;
						if (Data.SelfPlus(d, -5, Precision))++Exp;
					}
					else if (Data.SelfPlus(d, -4, Precision))++Exp;
					break;
				case 3: d = Data; Exp += Float256::Ten();
					if (Data.FromMinus(Data, Data, -5, Precision))
					{
						--Exp;
						if (Data.SelfPlus(d, -6, Precision))++Exp;
					}
					else if (Data.SelfPlus(d, -7, Precision))++Exp;
					break;
				default:
				{
					Float vf(Ten(), Precision + Precision / 64);
					for (;;)
					{
						if (x & 1)*this *= vf;
						x >>= 1;
						if (x == 0)break;
						vf *= vf;
					}
				}
				}
				return *this;
			}

			/**
			@brief 使用乘法计算该数的倒数乘以num，默认num为1*/
			Float Reciprocal(Float const&num = One())const;
			//自除
			Float& operator/=(Float const&vf);
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator/=(const T& t) { return operator/=(Float(t)); }
			//相除
			Float operator/(Float const&vf2)const;
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator/(const T& t)const { return operator/(Float(t)); }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator/(const T& t, const Float& vf) { return Float(t) / vf; }
			/**
			@brief 取一半（精度不变）
			@return 返回该数字的一半*/
			Float Half()const { return Float(Sign, Exp - Float256::One(), Data, Precision); }
			/**
			@return 返回该数字*2^-f
			@warning f必须为整数，否则将导致不可预计的后果*/
			Float Half(Float256 const& f)const { return Float(Sign, Exp - f, Data, Precision); }
			/**
			@brief 自身取一半
			@return 返回该数字的一半*/
			Float& SelfHalf() { --Exp; return *this; }

			/**
			@brief 求余(>=0),商保存到quotient里，余数与num同号；num<0时向上取整，num>0时向下取整
			@note 要求num与quotient不能是同一个对象*/
			Float Mod(Float const& num, Float &quotient)const;
			/**
			@brief 求余*/
			Float Mod(Float const& num)const;
			//自求余
			Float& operator%=(Float const&vf);
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator%=(const T& t) { return operator%=(Float(t)); }
			//求余
			Float operator%(Float const&vf2)const;
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator%(const T& t)const { return operator%(Float(t)); }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), Float>::type operator%(const T& t, const Float& vf) { return Float(t) % vf; }

			/**
			@brief 判断是否整数*/
			bool IsInteger()const;
			/**
			@brief 自身取整数部分*/
			Float& SelfGetIntegerPart();
			/**
			@brief 取整数部分*/
			Float GetIntegerPart()const;
			/**
			@brief 自身取小数部分*/
			Float& SelfGetDecimalPart();
			/**
			@brief 取小数部分*/
			Float GetDecimalPart()const;
			/**
			@breif 取绝对值*/
			Float Abs()const { if (IsError() || Sign >= 0)return*this; return Float(1, Exp, Data, Precision); }
			/**
			@brief 自身取绝对值*/
			Float& SelfAbs() { if (!IsError() && Sign < 0)Sign = 1; return*this; }

			/**
			@brief 转换到字符串
			@param [传入,可选]参数len表示有效数字部分长度限制
			@param [传入,可选]参数explen表示指数的有效数字部分长度限制
			@return 返回一个表示该数字的字符串*/
			std::string ToString(size_t len , size_t explen = 6)const;
			/**
			@brief 按默认方式转换为字符串表示*/
			virtual std::string ToString()const { return ToString(0, 6); }

			/**
			@brief 获取两者叠加时，一般情况下结果相对于vf1最高位的精度
			@param [传入]参数vf1为参考数
			@param [传入]参数vf2为比较数
			@note 当计算结果精度超出long long表示范围时，将返回MinValues::Int64或者MaxValues::Int64*/
			static s64 GetPrec(Float256 const& e1, u64 prec1, Float256 const& e2, u64 prec2)
			{
				if (e1.Exp < 62 && e2.Exp < 62)
				{
					s64 s1 = e1.Data.ull[3] >> (63 - e1.Exp);
					s64 s2 = e2.Data.ull[3] >> (63 - e2.Exp);
					if (e1.Sign < 0)s1 = -s1;
					if (e2.Sign < 0)s2 = -s2;
					s64 s = s1;
					s1 = s1 - (s64)prec1;
					s2 = s2 - (s64)prec2;
					if (s1 > s2)return s - s1;
					return s - s2;
				}
				Float256 f1 = e1 - (s64)prec1;
				Float256 f2 = e2 - (s64)prec2;
				if (f1 > f2)f1 = e1 - f1;
				else f1 = e1 - f2;
				if (f1.Exp > 62)return f1.Sign>0 ? MaxValues::Int64 : MinValues::Int64;
				return (s64)f1;
			}
			/**
			@brief 快速计算f1-f2-num
			@param [写出]参数ps用以输出f1-f2的结果
			@note 计算过程认为f1和f2均为整数，结果越界时将取Int上下界*/
			static s64 PrecMinus(Float256 const&f1, Float256 const&f2, s64 num, s64* ps = NULL)
			{
				if (f1.Exp < 62 && f2.Exp < 62)
				{
					s64 s1 = f1.Data.ull[3] >> (63 - f1.Exp);
					s64 s2 = f2.Data.ull[3] >> (63 - f2.Exp);
					if (f1.Sign <= 0)s1 = f1.Equals0() ? 0 : -s1;
					if (f2.Sign <= 0)s2 = f2.Equals0() ? 0 : -s2;
					if (ps)*ps = s1 - s2;
					return s1 - s2 - num;
				}
				Float256 f = f1 - f2;
				if (f.Exp > 63) { if (ps)return *ps = (f.Sign > 0 ? MaxValues::Int64 : MinValues::Int64); return f.Sign > 0 ? MaxValues::Int64 : MinValues::Int64; }
				f.Data.ull[3] = f.Equals0() ? 0 : f.Data.ull[3] >> (f.Exp - 63);
				if (ps)
				{
					if (f.Sign > 0 && (s64)f.Data.ull[3] < 0)*ps = MaxValues::Int64;
					else *ps = f.Sign > 0 ? (s64)f.Data.ull[3] : -(s64)f.Data.ull[3];
				}
				if ((s64)f.Data.ull[3] < 0)
				{
					if (num > 0) { if ((num = f.Data.ull[3] - num) > 0)return MinValues::Int64; }
					else num = f.Data.ull[3] - num;
					return num;
				}
				return (s64)(f.Data.ull[3] - num);
			}
			//比较，约等于
			bool AppEquals(Float const& vf)const { index_t c = (index_t)Compare(*this, vf); return c > -4 && c < 4; }
			//比较，大于
			bool operator>(Float const&vf2)const
			{
				return Compare(*this, vf2) > 0;
			}
			//比较，小于
			bool operator<(Float const&vf2)const
			{
				return Compare(*this, vf2) < 0;
			}
			//比较，等于
			bool operator==(Float const&vf2)const
			{
				return Compare(*this, vf2) == 0;
			}
			//比较，大于等于
			bool inline operator>=(Float const&vf2)const { return !(*this < vf2); }
			//比较，小于等于
			bool inline operator<=(Float const&vf2)const { return !(*this > vf2); }
			//比较，非等于
			bool operator!=(Float const&vf2)const
			{
				return !(*this == vf2);
			}

			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator>(const T& t) const { return *this > Float(t); }
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator<(const T& t) const { return *this < Float(t); }
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator==(const T& t) const { return *this == Float(t); }
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator!=(const T& t) const { return *this != Float(t); }
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator>=(const T& t) const { return *this >= Float(t); }
			template<class T>inline typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator<=(const T& t) const { return *this <= Float(t); }

			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator>(const T& t, const Float& vi) { return vi < t; }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator<(const T& t, const Float& vi) { return vi > t; }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator==(const T& t, const Float& vi) { return vi == t; }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator!=(const T& t, const Float& vi) { return vi != t; }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator>=(const T& t, const Float& vi) { return vi <= t; }
			template<class T>inline friend typename std::enable_if<(std::is_same<T,s64>::value||std::is_same<T,s32>::value||std::is_same<T,i32>::value||std::is_same<T,s16>::value||std::is_same<T,s8>::value||std::is_same<T,char>::value||std::is_same<T,wchar_t>::value||std::is_same<T,u64>::value||std::is_same<T,u32>::value||std::is_same<T,ui32>::value||std::is_same<T,u16>::value||std::is_same<T,u8>::value||std::is_same<T,double>::value||std::is_same<T,float>::value), bool>::type operator<=(const T& t, const Float& vi) { return vi >= t; }

			/**
			@brief 设置该数字exp~exp+63指数位上的内容，该方法有可能使得数字精度增加*/
			void SetDat64(Float256 const& exp, u64 dat)
			{
				if (IsError())return;
				index_t bc = 64 - Bits::LeftZeroCount64(dat);
				Float256 expp = exp + bc;
				if (Equals0())
				{
					u64 prec = Float::GetPrec(--expp, bc, Exp, Precision);
					*this = Float(dat != 0, expp, new FloatData(&dat, 0, bc), prec < (u64)bc ? bc : prec); return;
				}
				if (exp > Exp)
				{
					if (dat == 0)return;
					*this += Float(Sign, --expp, new FloatData(&dat, 0, bc), MaxValues::Int64);
					return;
				}
				if (expp > Exp)
				{
					Exp = expp - Exp;
					SelfGetDecimalPart();
					*this += Float(Sign, bc - 1, new FloatData(&dat, 0, bc), MaxValues::Int64);;
					Exp = --expp;
					return;
				}
				if (Exp > expp + MaxPrecision)return;
				Data.SetDat64((s64)(exp - Exp), dat);
				if (Data.chk()->bitsCount > Precision)Precision = Data.chk()->bitsCount;
			}
			/**
			@brief 获取该数字exp~exp+63指数位上的内容*/
			u64 GetDat64(Float256 const& exp)const
			{
				if (IsError() || Equals0() || Exp < exp)return 0;
				if (Exp >= exp + MaxValues::Int64)return 0;
				return Data.GetDat64((s64)(exp - Exp));
			}

			static s64 Compare(Float const& vf1, Float const& vf2)
			{
				if (vf1.Sign != vf2.Sign)return vf1.Sign>vf2.Sign ? 1 : -1;
				if (vf1.Exp >= vf2.Exp)
				{
					if (vf1.Exp > vf2.Exp + MaxValues::Int32)
					{
						if (vf1.Sign > 0)
							return MaxValues::Int32;
						else return MinValues::Int32;
					}
					s64 mov = vf2.Exp - vf1.Exp;
					s64 Precision = (vf1.Precision < vf2.Precision - mov) ? vf1.Precision : vf2.Precision - mov;
					mov = vf1.Data.CompareWith(vf2.Data, mov);
					if (mov == 0 || mov > Precision || -mov > Precision)return 0;
					if (vf1.Sign > 0)
						return mov < 0 ? -Precision - mov - 1 : Precision - mov + 1;
					return mov < 0 ? Precision + mov + 1 : -Precision - mov - 1;
				}
				return -Compare(vf2, vf1);
			}

			//常量
			static const Float& Zero();
			static const Float& One();
			static const Float& Two();
			static const Float& Three();
			static const Float& Four();
			static const Float& Five();
			static const Float& Six();
			static const Float& Seven();
			static const Float& Eight();
			static const Float& Nine();
			static const Float& Ten();
		};
	}
	//表示一个大数浮点的代理
	template<>
	struct ptr<Math::Float> :ptr<BigObject>
	{
		/**
		@brief 获取当前数值的精度
		@return 返回当前数值的精度（实质是二进制有效位数）*/
		u64 GetPrecision()const { return chk()->Precision; }
	/**
	@brief 设置当前数值的精度
	@param [传入]参数prec表示要设置的精度（实质是二进制有效位数）
	@note 当设置的精度小于当前数值的精度时有可能致使有效位数减少*/
	void SetPrecision(u64 prec) { Math::Float* p = chk(); if (p->Precision == prec)return; if (p->UseCount == 1)p->Precision = prec; else *this = new Math::Float(*p, prec); }
	/**
	@brief 获取当前数值的符号
	@return 返回值为1时，表示该数值为正数；返回值为-1时，表示该数值为负数；返回值为0时，表示该数值为负数；其它值表示该数值的错误状态*/
	int GetSign()const { return chk()->Sign; }
	/**
	@brief 设置该数值的符号*/
	void SetSign(int sign) { Math::Float* p = chk(); if (p->Sign == sign)return; if (p->UseCount != 1)*this = p = new Math::Float(*p); p->Sign = sign; }
	/**
	@brief 获取该数值的指数值*/
	Math::Float256 const GetExp()const { return chk()->Exp; }
	/**
	@brief 设置该数值的指数值*/
	void SetExp(Math::Float256 const& exp) { Math::Float* p = chk(); if (p->Exp == exp)return; if (p->UseCount != 1)*this = p = new Math::Float(*p); p->Exp = exp; }
	/**
	@brief 设置当前Data所引用的对象的引用计数是否是原子增减的*/
	void SetEnableInterlock(bool tr)const
	{
		chk()->Data.chk()->UseCount.SetEnableThreadSafe(tr);
		chk()->UseCount.SetEnableThreadSafe(tr);
	}
	//通用基类方法和属性
	PMETHODS(ptr, Math::Float)

	/*构造方法*/

#ifdef LK_MATH_FLOATN_
	/**
	@brief 从 FloatN<N>构造*/
			template<size_t N>ptr(Math::FloatN<N> const& f, u64 Precision = Math::Float::DefPrecision)
				: ptr<BigObject>(new Math::Float(f, Precision)) {}
			/**
			@brief 转换到FloatN类型变量*/
			template<size_t N>Math::FloatN<N> ToFloatN()const { return chk()->ToFloatN<N>(); }
#endif
			/**
			@brief 从u64整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(u64 num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从s64整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(s64 num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从unsigned long整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(unsigned long num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从long整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(long num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从unsigned int整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(unsigned int num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从int整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(int num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从unsigned short整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(unsigned short num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从short整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(short num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从unsigned char整形变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(unsigned char num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从signed char变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(signed char num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从wchar_t变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(wchar_t num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从char变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(char num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
#ifdef LK_MATH_INT_
			/**
			@brief 从大数整形ptr<Math::Int>变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(ptr<Math::Int> const& num, u64 Precision = Math::Float::DefPrecision) : ptr<BigObject>(new Math::Float(num, Precision)) {}
#endif
			/**
			@brief 从double型浮点变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(double num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从float型浮点变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(float num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从Float256型浮点变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(Math::Float256 const& num, u64 Precision = Math::Float::DefPrecision) :ptr<BigObject>(new Math::Float(num, Precision)) {}
			/**
			@brief 从ptr型浮点变量num，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(ptr const& num, u64 Precision) :ptr<BigObject>(Precision == num.chk()->Precision ? num.chk() : new Math::Float(*num.chk(), Precision)) {}
			/**
			@brief 字符串构造，按指定的二进制位数精度Precision进行构造。
			@note 当Precision小于MinPrecision时，将会被改为MinPrecision；同样的，当Precision大于MaxPrecision时，将会被改为MaxPrecision*/
			ptr(std::string const& num, u64 Precision);
			ptr(std::string const& num);
			ptr(std::wstring const& num) { *this = ptr(Strings::ToString(num)); }
			ptr(std::wstring const& num, u64 Precision) { *this = ptr(Strings::ToString(num),Precision); }
			ptr(wchar_t const* str) { *this = ptr(Strings::ToString(std::wstring(str))); }
			ptr(char const* str) { *this = ptr(std::string(str)); }
			ptr(wchar_t const* str, u64 Precision) { *this = ptr(std::wstring(str), Precision); }
			ptr(char const* str, u64 Precision) { *this = ptr(std::string(str), Precision); }

			/*运算方法*/

			/*加法*/

			ptr operator+()const { return*this; }
			ptr operator++(int) { ptr tmp(*this); ++ *this; return tmp; }
			ptr& operator++() { Math::Float*p = chk(); if (p->UseCount == 1)++*p; else *this = *this + One(); return *this; }
			ptr& operator+=(ptr const& xf) { Math::Float*p = chk(); if (p->UseCount == 1)*p += *xf.chk(); else *this = *this + xf; return *this; }
			friend ptr operator+(ptr const&xf1, ptr const&xf2)
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(*xf1.chk() + *xf2.chk()));
				return tmp;
			}

			/*减法*/

			/**
			@brief 自身取相反数*/
			ptr& SelfNegative() { Math::Float*p = chk(); if (p->UseCount == 1)p->SelfNegative(); else *this = new Math::Float(-*p); return *this; }

			ptr operator-()const
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float>::reference(-*chk()));
				return tmp;
			}
			ptr operator--(int) { ptr tmp(*this); -- *this; return tmp; }
			ptr& operator--() { Math::Float*p = chk(); if (p->UseCount == 1)--*p; else *this = *this - One(); return *this; }
			ptr& operator-=(ptr const& xf) { Math::Float*p = chk(); if (p->UseCount == 1)*p -= *xf.chk(); else *this = *this - xf; return *this; }
			friend ptr operator-(ptr const&xf1, ptr const&xf2)
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(*xf1.chk() - *xf2.chk()));
				return tmp;
			}

			/*乘法*/

			ptr& operator*=(ptr const& xf) { Math::Float*p = chk();  if (p->UseCount == 1)*p *= *xf.chk(); else *this = *this * xf; return *this; }
			friend ptr operator*(ptr const&xf1, ptr const&xf2)
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(*xf1.chk() * *xf2.chk()));
				return tmp;
			}

			//自身快速乘以10
			ptr& SelfMultiply10() { Math::Float*p = chk(); if (IsError() || Equals0())return*this; if (p->UseCount != 1)*this = p = new Math::Float(*p); p->SelfMultiply10(); return *this; }

			/*除法*/

			ptr& operator/=(ptr const& xf) { Math::Float*p = chk(); if (p->UseCount == 1)*p /= *xf.chk(); else *this = *this / xf; return *this; }
			friend ptr operator/(ptr const&xf1, ptr const&xf2)
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(*xf1.chk() / *xf2.chk()));
				return tmp;
			}

			/**
			@brief 取一半*/
			ptr Half()const { ptr f = *this; f.SetExp(f.GetExp() - Math::Float256::One()); return f; }
			/**
			@brief 获取该数乘以2^-x*/
			ptr Half(Math::Float256 x)const { ptr f = *this; f.SetExp(f.GetExp() - x); return f; }
			/**
			@brief 自身取一半*/
			ptr& SelfHalf() { SetExp(GetExp() - 1); return*this; }

			/*求余*/

			ptr& operator%=(ptr const& xf) { Math::Float*p = chk(); if (p->UseCount == 1)*p *= *xf.chk(); else *this = *this % xf; return *this; }
			friend ptr operator%(ptr const&xf1, ptr const&xf2)
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float>::reference(*xf1.chk() % *xf2.chk()));
				return tmp;
			}
			/**
			@brief 计算该数字%num*/
			ptr Mod(ptr const& num)const
			{
				Math::Float* p = new Math::Float();
				p->SwapWith(Conversion::Convert<Math::Float>::reference(chk()->Mod(*num.chk())));
				return p;
			}
			/**
			@brief 计算该数字%num,对应的整数商保存到quotient中*/
			ptr Mod(ptr const& num, ptr & quotient)const
			{
				Math::Float* pq = quotient.chk();
				if (pq->UseCount != 1)quotient = pq = new Math::Float();
				Math::Float* p = new Math::Float();
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(chk()->Mod(*num.chk(), *pq)));
				return p;
			}
			/*比较*/

			friend bool operator>(ptr const&xf1, ptr const&xf2) { return !BigObject::ReferenceEqual(xf1, xf2) && *xf1.chk() > *xf2.chk(); }
			friend bool operator>=(ptr const&xf1, ptr const&xf2) { return BigObject::ReferenceEqual(xf1, xf2) || *xf1.chk() >= *xf2.chk(); }
			friend bool operator<(ptr const&xf1, ptr const&xf2) { return !BigObject::ReferenceEqual(xf1, xf2) && *xf1.chk() < *xf2.chk(); }
			friend bool operator<=(ptr const&xf1, ptr const&xf2) { return BigObject::ReferenceEqual(xf1, xf2) || *xf1.chk() <= *xf2.chk(); }
			friend bool operator==(ptr const&xf1, ptr const&xf2) { return BigObject::ReferenceEqual(xf1, xf2) || *xf1.chk() == *xf2.chk(); }
			friend bool operator!=(ptr const&xf1, ptr const&xf2) { return !BigObject::ReferenceEqual(xf1, xf2) && *xf1.chk() != *xf2.chk(); }
			static index_t Compare(ptr const&xf1, ptr const&xf2) { if (BigObject::ReferenceEqual(xf1, xf2))return 0; return (index_t)Math::Float::Compare(*xf1.chk(), *xf2.chk()); }

			/*特性函数*/

			/**
			@brief 约等于*/
			bool AppEquals(ptr const& x)const { index_t k = Compare(*this, x); return k<4 && k>-4; }
			/**
			@brief 是否为0*/
			bool Equals0()const { return chk()->Equals0(); }
			/**
			@brief 产生一个错误*/
			ptr& MakeError(int Sign) { SetSign(Sign); return *this; }
			/**
			@brief 是否错误*/
			bool IsError()const { return chk()->IsError(); }
			/**
			@brief 自身取绝对值*/
			ptr& SelfAbs() { Math::Float*p = chk(); if (p->UseCount == 1)p->SelfAbs(); else *this = Abs(); return*this; }
			/**
			@brief 取绝对值*/
			ptr Abs()const
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(chk()->Abs()));
				return tmp;
			}
			/**
			@brief 获取整数部分*/
			ptr GetIntegerPart()const
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(chk()->GetIntegerPart()));
				return tmp;
			}
			/**
			@brief 自身获取整数部分*/
			ptr& SelfGetIntegerPart()
			{
				Math::Float*pt = chk();
				if (pt->UseCount == 1) { pt->SelfGetIntegerPart(); return *this; }
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(pt->GetIntegerPart()));
				return *this = tmp;
			}
			/**
			@brief 获取小数部分*/
			ptr GetDecimalPart()const
			{
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(chk()->GetDecimalPart()));
				return tmp;
			}
			/**
			@brief 自身获取整数部分*/
			ptr& SelfGetDecimalPart()
			{
				Math::Float* pt = chk();
				if (pt->UseCount == 1) { pt->SelfGetDecimalPart(); return *this; }
				Math::Float *p = new Math::Float();
				ptr tmp = p;
				p->SwapWith(Conversion::Convert<Math::Float, Math::Float>::reference(pt->GetDecimalPart()));
				return *this = tmp;
			}
			/**
			@brief 判断是否为整数*/
			bool IsInteger()const { return chk()->IsInteger(); }

			/**
			@brief 设置该数字exp~exp+63指数位上的内容，该方法有可能使得数字精度增加*/
			void SetDat64(Math::Float256 const& exp, u64 dat)
			{
				if (IsError())return;
				index_t bc = 64 - Bits::LeftZeroCount64(dat);
				Math::Float256 expp = exp + bc;
				if (Equals0())
				{
					u64 prec = Math::Float::GetPrec(--expp, bc, GetExp(), GetPrecision());
					*this = new Math::Float(dat != 0, expp, new Math::FloatData(&dat, 0, bc), prec < (u64)bc ? bc : prec); return;
				}
				if (exp > GetExp())
				{
					if (dat == 0)return;
					*this += new Math::Float(get()->Sign, --expp, new Math::FloatData(&dat, 0, bc), MaxValues::Int64);
					return;
				}
				if (expp > GetExp())
				{
					SetExp(expp - GetExp());
					SelfGetDecimalPart();
					*this += new Math::Float(GetSign(), bc - 1, new Math::FloatData(&dat, 0, bc), MaxValues::Int64);;
					SetExp(--expp);
					return;
				}
				if (GetExp() > expp + MaxPrecision)return;
				Math::Float* pt = Conversion::Convert<ptr>::reference(get()->CloneIfShared()).get();
				pt->Data.SetDat64((s64)(exp - GetExp()), dat);
				if (pt->Data.chk()->bitsCount > GetPrecision())SetPrecision(pt->Data.chk()->bitsCount);
			}
			/**
			@brief 获取该数字exp~exp+63指数位上的内容*/
			u64 GetDat64(Math::Float256 const& exp)const
			{
				if (IsError() || Equals0() || GetExp() < exp)return 0;
				if (GetExp() >= exp + MaxValues::Int64)return 0;
				return get()->Data.GetDat64((s64)(exp - GetExp()));
			}

			/*数学函数*/

			/**
			@brief 计算以自然数e为底的对数*/
			ptr Ln()const;
			/**
			@brief 计算以2为底的对数*/
			ptr Log2()const;
			/**
			@brief 计算2的该数次幂*/
			ptr Exp2()const;
			/**
			@brief 计算该数的xf次幂*/
			ptr Pow(ptr const& xf)const;
			/**
			@brief 计算该数的开根号*/
			ptr Sqrt()const;
			/**
			@brief 计算f的开根号*/
			static ptr Sqrt(const ptr& f) { return f.Sqrt(); }

			/**
			@brief 计算以10为底的对数*/
			ptr Lg()const { return Ln() / CalculateLn10(GetPrecision()); }
			/*动态计算量*/

			//计算并返回指定二进制精度的π的代理
			static ptr CalculatePi(u64 Precision);
			//计算并返回指定二进制精度的e的代理
			static ptr CalculateE(u64 Precision);
			//计算并返回指定二进制精度的ln2的代理
			static ptr CalculateLn2(u64 Precision);
			/**
			@brief 使用指定的二进制精度计算2^(1/(2^(x+1)))
			@param [传入]参数x表示计算式的x值
			@param [传入]参数prec表示计算用的二进制精度
			@return 返回2^(1/(2^(x+1)))的计算结果*/
			static ptr Calculate2_(size_t x, u64 Precision);
			/**
			@brief 使用指定的二进制精度计算1/(2^(1/(2^(x+1))))
			@param [传入]参数x表示计算式的x值
			@param [传入]参数prec表示计算用的二进制精度
			@return 返回1/(2^(1/(2^(x+1))))的计算结果*/
			static ptr Calculate12_(size_t x, u64 Precision);
			/**
			@brief 以指定二进制精度计算ln10
			@param [传入]参数prec表示计算用的二进制精度
			@return 返回ln10的计算结果*/
			static ptr CalculateLn10(u64 prec);
			/**
			@brief 以指定二进制精度计算1/ln10
			@param [传入]参数prec表示计算用的二进制精度
			@return 返回1/ln10的计算结果*/
			static ptr Calculate1Ln10(u64 prec);
			/**
			@brief 计算 (n从0到+∞)∑(2^(x-a*n))*((-1)^n)/(b*n+c) 的小数部分（近似），其中b、c、x由外部传入,ife为true时正负交替（首项为正）
			@return 返回一个小于1大于0的小数
			@note 该函数用以辅助计算π，若要计算π请使用GetPiBBP32或者CalculatePi*/
			static double BBPn(s32 a, s32 b, s32 c, s32 x, bool ife);
			/**
			@brief 获取π在指数位为exp-1~exp-32上的内容，这里要求exp<=0*/
			static u32 GetPiBBP32(s32 exp);
			/**
			@计算根号2*/
			static ptr CalculateSqrt2(u64 prec);
			/**
			@brief 获取该数的正弦值(弧度制)*/
			ptr Sin()const;
			/**
			@brief 同时计算正弦和余弦值*/
			void SinAndCos(ptr & outsin, ptr & outcos)const;
			/**
			@brief 计算sin(f)*/
			static ptr Sin(const ptr& f) { return f.Sin(); }
			/**
			@brief 获取该数的余弦值(弧度制)*/
			ptr Cos()const;
			/**
			@brief 计算cos(f)*/
			static ptr Cos(const ptr& f) { return f.Cos(); }
			/**
			@brief 获取该数的正切值(弧度制)*/
			ptr Tan()const { ptr f1, f2; SinAndCos(f1, f2); if (f2.Equals0()) { Math::Float* p = new Math::Float(); p->MakeError(Math::MathError::Tan); return p; }return f1 / f2; }
			/**
			@brief 计算tan(f)*/
			static ptr Tan(const ptr& f) { return f.Tan(); }
			/**
			@brief 获取该数的余切值(弧度制)*/
			ptr Cot()const { ptr f1, f2; SinAndCos(f1, f2); if (f1.Equals0()) { Math::Float* p = new Math::Float(); p->MakeError(Math::MathError::Cot); return p; }return f2 / f1; }
			/**
			@brief 计算cot(f)*/
			static ptr Cot(const ptr& f) { return f.Cot(); }
			/**
			@brief 反正弦(弧度制)*/
			ptr Arcsin()const;
			/**
			@brief 反正弦(弧度制)*/
			static ptr Arcsin(ptr const&num) { return num.Arcsin(); }
			/**
			@brief 反余弦(弧度制)*/
			ptr Arccos()const;
			/**
			@brief 反余弦(弧度制)*/
			static ptr Arccos(ptr const&num) { return num.Arccos(); }
			/**
			@brief 反正切(弧度制)*/
			ptr Arctan()const;
			/**
			@brief 反正切(弧度制)*/
			static ptr Arctan(ptr const&num) { return num.Arctan(); }

			/*类型转换*/

			/**
			@brief 转换为字符串表示*/
			std::string ToString(size_t len = 0, size_t explen = 6)const { return chk()->ToString(len, explen); }
			/**
			@brief 转换为Float256*/
			Math::Float256 ToFloat256()const { return chk()->ToFloat256(); }
			/**
			@brief 转换为无符号64位整形*/
			u64 ToUInt64()const { return chk()->ToUInt64(); }
			/**
			@属性：最大精度(引用Float::MaxPrecision)*/
			static u64& MaxPrecision;
			/**
			@属性：最小精度(引用Float::MinPrecision)*/
			static u64& MinPrecision;
			/**
			@属性：默认精度(引用Float::DefPrecision)*/
			static u64& DefPrecision;
			/**
			@属性：终止计算(引用Float::StopCalculating)*/
			static bool& StopCalculating;
			/*固定常量*/

			static const ptr& Zero();
			static const ptr& One();
			static const ptr& Two();
			static const ptr& Three();
			static const ptr& Four();
			static const ptr& Five();
			static const ptr& Six();
			static const ptr& Seven();
			static const ptr& Eight();
			static const ptr& Nine();
			static const ptr& Ten();
	};
	template<>struct Conversion::Convert <std::string, ptr<Math::Float> >
	{
		static inline std::string from(ptr<Math::Float> const&l) { return l.ToString(DefParam[0], DefParam[1]); }
		static size_t DefParam[2];
	};
}
namespace std
{
	inline ::LK::ptr<LK::Math::Float> fmod(::LK::ptr<LK::Math::Float> const& f1, ::LK::ptr<LK::Math::Float> const& f2) { return f1%f2; }
	inline ::LK::ptr<LK::Math::Float> sin(::LK::ptr<LK::Math::Float> const& f) { return f.Sin(); }
	inline ::LK::ptr<LK::Math::Float> cos(::LK::ptr<LK::Math::Float> const& f) { return f.Cos(); }
	inline ::LK::ptr<LK::Math::Float> tan(::LK::ptr<LK::Math::Float> const& f) { return f.Tan(); }
	inline ::LK::ptr<LK::Math::Float> asin(::LK::ptr<LK::Math::Float> const& f) { return f.Arcsin(); }
	inline ::LK::ptr<LK::Math::Float> acos(::LK::ptr<LK::Math::Float> const& f) { return f.Arccos(); }
	inline ::LK::ptr<LK::Math::Float> atan(::LK::ptr<LK::Math::Float> const& f) { return f.Arctan(); }
	inline ::LK::ptr<LK::Math::Float> log10(::LK::ptr<LK::Math::Float> const& f) { return f.Lg(); }
	inline ::LK::ptr<LK::Math::Float> log(::LK::ptr<LK::Math::Float> const& f) { return f.Ln(); }
	inline ::LK::ptr<LK::Math::Float> pow(::LK::ptr<LK::Math::Float> const& f1, ::LK::ptr<LK::Math::Float> const& f2) { return f1.Pow(f2); }
	inline ::LK::ptr<LK::Math::Float> sqrt(::LK::ptr<LK::Math::Float> const& f) { return f.Sqrt(); }
	inline ::LK::ptr<LK::Math::Float> abs(::LK::ptr<LK::Math::Float> const& f) { return f.Abs(); }
	inline ::LK::ptr<LK::Math::Float> floor(::LK::ptr<LK::Math::Float> const& f) { ::LK::ptr<LK::Math::Float> ff = f.GetIntegerPart(); return f.GetSign() <= 0 ? ((ff == f) ? f : ff - ::LK::ptr<LK::Math::Float>::One()) : ff; }
	inline ::LK::ptr<LK::Math::Float> ceil(::LK::ptr<LK::Math::Float> const& f) { ::LK::ptr<LK::Math::Float> ff = f.GetIntegerPart(); return f.GetSign() <= 0 ? ff : ((ff == f) ? f : ff + ::LK::ptr<LK::Math::Float>::One()); }
}
#endif
