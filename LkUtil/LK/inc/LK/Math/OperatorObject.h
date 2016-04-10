#ifndef LK_MATH_OPERATOROBJECT_
#define LK_MATH_OPERATOROBJECT_
namespace LK
{
	namespace Math
	{
		/**
		@brief 表示一个算式函数（操作符）接口*/
		class IOperatorObjects :public ::LK::BigObject
		{
		public:
			IMETHODS(POperatorObjects,IOperatorObjects);
			//使用名称、优先级、参数数量和算式属性来构造一个新的操作符
			IOperatorObjects(ptr<String> n, unsigned short lv,unsigned short parms,unsigned short p = DefaultProperty) :name(n), prop(p),level(lv) ,count(parms){}
			/**
			@brief 运算符名称*/
			ptr<String> name;
			/**
			@brief 运算符属性*/
			unsigned short prop;
			/**
			@brief 运算符优先级*/
			unsigned short level;
			/**
			@brief 运算符参数数量，参数数量为65535时*/
			unsigned short count;
			/**
			@brief 转换为字符串表示*/
			ptr<String> ToString()const {return name;}
			/**
			@brief 清理操作*/
			virtual void Clear() {}
			/**
			@brief 默认的运算符属性*/
			static unsigned short DefaultProperty;
			/**
			@brief 终止运算标记*/
			static bool StopCalculating;
			/**
			@brief 计算一次便得出最终值*/
			static const unsigned short CalculateOnceProperty = 0x1;
			/**
			@brief 相同输入得到相同结果*/
			static const unsigned short CalculatePassProperty = 0x2;
			/**
			@brief 支持单独算子*/
			static const unsigned short SupportAloneProperty = 0x4;
			/**
			@brief 支持单目运算算子*/
			static const unsigned short SupportSingleProperty = 0x8;
			/**
			@brief 支持双目运算算子*/
			static const unsigned short SupportDoubleProperty = 0x10;
			/**
			@brief 支持函数运算算子*/
			static const unsigned short SupportFunctionProperty = 0x20;
			/**
			@brief 是否后置运算符*/
			static const unsigned short CalculateBackProperty = 0x40;
			/**
			@brief 获取比较数字，按运算符优先级、单目、函数、可变参函数、双目、非后置、单独算子排序*/
			unsigned long long GetComparingNumber()const
			{
				return ((unsigned long long)(~level) << 48)//优先级高者数字比较小，排在前面
					| (IsSingleSupport() ? 0x800000000000 : 0)
					| (IsFunctionSupport() ? 0x80000000000 : 0)
					| ((unsigned long long)count << 24)
					| (IsDoubleSupport() ? 0x800000 : 0)
					| (!IsCalculateBack() ? 0x800000 : 0)
					| (!IsAloneSupport() ? 0x80000 : 0);
			}
			/**
			@brief 获取运算符优先级*/
			unsigned short GetOperatingLevel()const { return level; }
			/**
			@brief 设置运算优先级*/
			void SetOperatingLevel(unsigned short lv) { level =lv; }
			/**
			@brief 获取运算符参数数量*/
			unsigned short GetParametersCount()const { return count; }
			/**
			@brief 设置运算参数数量*/
			void SetParametersCount(short c) { count = c; }
			/**
			@brief 获取一个bool值，指示该运算符是否只计算一次便得到最终结果（如果运算关系包含随机分量时，该值应设置为false）*/
			bool IsCalculateOnce()const { return (prop&CalculateOnceProperty)!=0; }
			/**
			@brief 设置该运算符是否只计算一次便得到最终结果（如果运算关系包含随机分量时，该值应设置为false）*/
			void SetIsCalculateOnce(bool b) { if (b)prop = prop | CalculateOnceProperty; else prop = prop&(~CalculateOnceProperty); }
			/**
			@brief 获取一个bool值，指示该运算符如果参数不变，输出是否不变*/
			bool IsCalculatePass()const { return (prop&CalculatePassProperty) != 0; }
			/**
			@brief 设置该运算符如果参数不变，输出是否不变*/
			void SetIsCalculatePass(bool b) { if (b)prop = prop | CalculatePassProperty; else prop = prop&(~CalculatePassProperty); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持单独算子形式*/
			bool IsAloneSupport()const { return (prop&SupportAloneProperty) != 0; }
			/**
			@brief 设置该运算符是否支持单独算子形式*/
			void SetIsAloneSupport(bool b) { if (b)prop = prop | SupportAloneProperty; else prop = prop&(~SupportAloneProperty); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持单目算子形式*/
			bool IsSingleSupport()const { return (prop&SupportSingleProperty) != 0; }
			/**
			@brief 设置该运算符是否支持单目算子形式*/
			void SetIsSingleSupport(bool b) { if (b)prop = prop | SupportSingleProperty; else prop = prop&(~SupportSingleProperty); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持双目算子形式*/
			bool IsDoubleSupport()const { return (prop&SupportDoubleProperty) != 0; }
			/**
			@brief 设置该运算符是否支持双目算子形式*/
			void SetIsDoubleSupport(bool b) { if (b)prop = prop | SupportDoubleProperty; else prop = prop&(~SupportDoubleProperty); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持函数算子形式*/
			bool IsFunctionSupport()const { return (prop&SupportFunctionProperty) != 0; }
			/**
			@brief 设置该运算符是否支持函数算子形式*/
			void SetIsFunctionSupport(bool b) { if (b)prop = prop | SupportFunctionProperty; else prop = prop&(~SupportFunctionProperty); }
			/**
			@brief 获取一个bool值，指示该运算符是否后置运算符*/
			bool IsCalculateBack()const { return (prop&CalculateBackProperty) != 0; }
			/**
			@brief 设置该运算符是否后置运算符*/
			void SetIsCalculateBack(bool b) { if (b)prop = prop | SupportFunctionProperty; else prop = prop&(~SupportFunctionProperty); }
			virtual ~IOperatorObjects() {}
		};
		/**
		@表示IOperatorObject的代理*/
		struct POperatorObjects : ptr<BigObject>
		{
			PIMETHODS(POperatorObjects, IOperatorObjects);
			/**
			@brief 获取运算符名称*/
			ptr<String> GetName()const { return Ptr()->name; }
			/**
			@brief 默认的运算符属性(引用IOperatorObjects::DefaultProperty)*/
			static unsigned short& DefaultProperty;
			/**
			@brief 终止运算标记(引用IOperatorObjects::StopCalculating)*/
			static bool& StopCalculating;
			/**
			@brief 计算一次便得出最终值*/
			static const unsigned short CalculateOnceProperty = IOperatorObjects::CalculateOnceProperty;
			/**
			@brief 相同输入得到相同结果*/
			static const unsigned short CalculatePassProperty = IOperatorObjects::CalculatePassProperty;
			/**
			@brief 支持单独算子*/
			static const unsigned short SupportAloneProperty = IOperatorObjects::SupportAloneProperty;
			/**
			@brief 支持单目运算算子*/
			static const unsigned short SupportSingleProperty = IOperatorObjects::SupportSingleProperty;
			/**
			@brief 支持双目运算算子*/
			static const unsigned short SupportDoubleProperty = IOperatorObjects::SupportDoubleProperty;
			/**
			@brief 支持函数运算算子*/
			static const unsigned short SupportFunctionProperty = IOperatorObjects::SupportFunctionProperty;
			/**
			@brief 是否后置运算符*/
			static const unsigned short CalculateBackProperty = IOperatorObjects::CalculateBackProperty;
			/**
			@brief 获取运算符优先级*/
			unsigned short GetOperatingLevel()const { return Ptr()->GetOperatingLevel(); }
			/**
			@brief 获取比较数字，按运算符优先级、单目、函数、可变参函数、双目、非后置、单独算子排序*/
			unsigned long long GetComparingNumber()const{ return Ptr()->GetComparingNumber(); }
			/**
			@brief 设置运算优先级*/
			void SetOperatingLevel(unsigned short lv) { Ptr()->SetOperatingLevel(lv); }
			/**
			@brief 获取运算符参数数量*/
			unsigned short GetParametersCount()const { return Ptr()->GetParametersCount(); }
			/**
			@brief 设置运算参数数量*/
			void SetParametersCount(short c) { Ptr()->SetParametersCount(c); }
			/**
			@brief 获取一个bool值，指示该运算符是否只计算一次便得到最终结果（如果运算关系包含随机分量时，该值应设置为false）*/
			bool IsCalculateOnce()const { return Ptr()->IsCalculateOnce(); }
			/**
			@brief 设置该运算符是否只计算一次便得到最终结果（如果运算关系包含随机分量时，该值应设置为false）*/
			void SetIsCalculateOnce(bool b) { Ptr()->SetIsCalculateOnce(b); }
			/**
			@brief 获取一个bool值，指示该运算符如果参数不变，输出是否不变*/
			bool IsCalculatePass()const { return Ptr()->IsCalculatePass(); }
			/**
			@brief 设置该运算符如果参数不变，输出是否不变*/
			void SetIsCalculatePass(bool b) { Ptr()->SetIsCalculatePass(b); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持单独算子形式*/
			bool IsAloneSupport()const { return Ptr()->IsAloneSupport(); }
			/**
			@brief 设置该运算符是否支持单独算子形式*/
			void SetIsAloneSupport(bool b) { Ptr()->SetIsAloneSupport(b); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持单目算子形式*/
			bool IsSingleSupport()const { return Ptr()->IsSingleSupport(); }
			/**
			@brief 设置该运算符是否支持单目算子形式*/
			void SetIsSingleSupport(bool b) { Ptr()->SetIsSingleSupport(b); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持双目算子形式*/
			bool IsDoubleSupport()const { return Ptr()->IsDoubleSupport(); }
			/**
			@brief 设置该运算符是否支持双目算子形式*/
			void SetIsDoubleSupport(bool b) { Ptr()->SetIsDoubleSupport(b); }
			/**
			@brief 获取一个bool值，指示该运算符是否支持函数算子形式*/
			bool IsFunctionSupport()const { return Ptr()->IsFunctionSupport(); }
			/**
			@brief 设置该运算符是否支持函数算子形式*/
			void SetIsFunctionSupport(bool b) { Ptr()->SetIsFunctionSupport(b); }
			/**
			@brief 获取一个bool值，指示该运算符是否后置运算符*/
			bool IsCalculateBack()const { return Ptr()->IsCalculateBack(); }
			/**
			@brief 设置该运算符是否后置运算符*/
			void SetIsCalculateBack(bool b) { Ptr()->SetIsCalculateBack(b); }
			/**
			@brief 清理操作*/
			void Clear()const { Ptr()->Clear(); }
		};

		//存放类型为T的参数
		template<class T>struct OperatorArgs
		{
			T vals[8];//缓存8个参数，不够存放时使用arr
			Collection::PArray<T>  arr;
			void* args[8];//缓存8个内部参数，不够时使用varr
			Collection::PArray<void*> varr;
			bool bl[8];//是否计算过，不够时使用barr
			Collection::PArray<bool> barr;
			index_t len;//参数总数
			T&(*fun)(void*);//获取参数的函数
		public:
			//默认的公共参数，一般为空
			static OperatorArgs Default;
			//使用回调函数、回调参数、参数数量来构造
			OperatorArgs(T&(*f)(void*)=NULL,void**pargs=NULL,index_t length=0) :len(length),fun(f)
			{
				if (length != 0)
				{
					if (length > 8)
					{
						arr = Collection::PArray<T> (length-8);
						varr = Collection::PArray<void*>(length - 8);
						barr = Collection::PArray<bool>(length - 8);
					}
					for (index_t i = 0; i < length; ++i)
					{
						if (i < 8)
						{
							args[i] = pargs[i];
							bl[i] = false;
						}
						else
						{
							varr[i - 8] = pargs[i];
							barr[i] = false;
						}
					}
				}
			}
			//获取指定索引的参数（每个参数第一个获取时都会调用fun进行获取）
			T& operator[](index_t i)
			{
				if (i < 8)
				{
					if (fun&&!bl[i])
					{
						vals[i] = fun(args[i]);
						bl[i] = true;
					}
					return vals[i];
				}
				i -= 8;
				if (fun&&!barr[i])
				{
					arr[i] = fun(varr[i]);
					barr[i] = true;
				}
				return arr[i];
			}
			//获取指定索引的参数是否计算过
			bool HasCalculated(index_t index)const { return index < 8 ? bl[index] : barr[index - 8]; }
			//获取保存的参数数量
			index_t GetLength()const { return len; }
			//设置获取参数的函数
			void SetFunction(T&(*f)(void*)) { fun = f; }
			//添加1个参数
			void AddArg(void* p)
			{
				if (len < 8)
				{
					args[len] = p;
					bl[len] = false;
					len++;
				}
				else
				{
					if (arr.IsNull())
					{
						arr = Collection::PArray<T> (len-7);
						varr = Collection::PArray<void*>(len - 7);
						barr = Collection::PArray<bool>(len - 7);
					}
					else
					{
						arr.Resize(len - 7);
						varr.Resize(len - 7);
						barr.Resize(len - 7);
					}
					varr[len - 7] = p;
					barr[len - 7] = true;
					++len;
				}
			}
			//清空所有参数
			void Clear()
			{
				len = 0;
				arr.SetToNull();
				varr.SetToNull();
				barr.SetToNull();
			}
			//获取第index个参数的指针
			T* GetPointer(index_t index) { (*this)[index]; return index < 8 ? vals + index : ((T*)arr + index - 8); }
			//添加一个参数值，并将其设置为已计算
			void AddValue(T const& t)
			{
				if (len < 8)
				{
					vals[len] = t;
					bl[len] = true;
					len++;
				}
				else
				{
					if (arr.IsNull())
					{
						arr = Collection::PArray<T> (len - 7);
						barr = Collection::PArray<bool>(len - 7);
					}
					else
					{
						arr.Resize(len - 7);
						barr.Resize(len - 7);
					}
					arr[len - 7] = t;
					barr[len-7] = true;
					++len;
				}
			}
		};
		template<class T>OperatorArgs<T> OperatorArgs<T>::Default;
		//存放类型为T的参数(详细)
		template<class T>struct OperatorDebugArgs
		{
			Collection::PArray<T>  arr;//数值参数
			Collection::PArray<void*> varr;//回调指针参数
			Collection::PArray<bool > barr;//记录每个参数是否已解析
			Collection::PArray<Collection::PArray<ptr<String>> > strs;//记录每个参数解析的字符串数组，对于表达式来说，strs[][0]表示值的字符串形式；对于运算符来说，strs[][0]则表示每个实参值与运算符的直接组合
			T&(*fun)(void*, Collection::PArray<ptr<String>>&);//获取参数的函数
		public:
			index_t deep;//最大运算符穿透次数
			index_t level;//最深解析层级
			//使用回调函数、最大运算符穿透次数、最深解析层级来构造
			OperatorDebugArgs(T&(*f)(void*,Collection::PArray<ptr<String>>&), index_t d,index_t l) :fun(f),deep(d),level(l){}
			//获取指定索引的参数（每个参数第一个获取时都会调用fun进行获取）
			T& operator[](index_t i)
			{
				if (fun&& !barr[i])
				{
					arr[i] = fun(varr[i],strs[i]);
					barr[i] = true;
				}
				return arr[i];
			}
			//获取指定索引变量对应的字符串数组
			Collection::PArray<ptr<String>> operator()(index_t i)
			{
				return strs[i];
			}
			//获取指定索引的参数指定解析层级时的字符串（每个参数第一个获取时都会调用sfun进行获取）
			ptr<String> operator()(index_t i,index_t l)
			{
				if (!barr[i])return L"(...)";
				if (-l >= strs[i].GetLength())return strs[i].LastElement();
				return strs[i][-l];
			}
			//获取指定索引的参数是否计算过
			bool HasCalculated(index_t index)const { return barr[index]; }
			//获取保存的参数数量
			index_t GetLength()const { return arr.IsNull()?0:arr.GetLength(); }
			//设置获取参数的函数
			void SetFunction(T&(*f)(void*, Collection::PArray<ptr<String>>&)) { fun = f; }
			//添加1个参数
			void AddArg(void* p)
			{
				if (arr.IsNull())
				{
					arr = Collection::PArray<T> (1);
					varr = Collection::PArray<void*>(1);
					barr = Collection::PArray<bool>(1);
					strs = Collection::PArray<Collection::PArray<ptr<String>>>(1);
				}
				else
				{
					arr.Resize(arr.GetLength()+1);
					varr.Resize(arr.GetLength());
					barr.Resize(arr.GetLength());
					strs.Resize(arr.GetLength());
				}
				varr.LastElement() = p;
				barr.LastElement() = false;
			}
			//清空所有参数
			void Clear()
			{
				arr.SetToNull();
				varr.SetToNull();
				barr.SetToNull();
				strs.SetToNull();
			}
			//获取第index个参数的指针
			T* GetPointer(index_t index) { (*this)[index]; return (T*)arr + index ; }
			//添加一个参数值和一个字符串，并将其设置为已计算
			void AddValue(T const& t,ptr<String> s)
			{
				if (arr.IsNull())
				{
					arr = Collection::PArray<T> (1);
					barr = Collection::PArray<bool>(1);
					strs = Collection::PArray<Collection::PArray<ptr<String>> >(1);
				}
				else
				{
					arr.Resize(arr.GetLength()+1);
					barr.Resize(arr.GetLength());
					strs.Resize(arr.GetLength());
				}
				arr.LastElement() = t;
				barr.LastElement() = true;
				strs.LastElement() = Collection::PArray<ptr<String>>(2);
				strs.LastElement()[0] = Convert<ptr<String>,T>::From(t);
				strs.LastElement()[1] = s;
			}
			//获取最大的字符串数组元素数
			index_t GetMaxArrCount()const
			{
				index_t k = 0;
				for (index_t i = 0; i < (strs.IsNull() ? 0 : strs.GetLength()); ++i)
					if (!strs[i].IsNull()&&strs[i].GetLength()>k)
						k = strs[i].GetLength();
				return k;
			}
			//void*指向OperatorDebugArgs对象的与OperatorArgs兼容的默认回调方法
			static T& DefFun(void*p)
			{
				return (*((std::pair<OperatorDebugArgs<T>*, index_t>*)p)->first)[((std::pair<OperatorDebugArgs<T>*, index_t>*)p)->second];
			}
		};
		/**
		@brief 运算符对象接口类*/
		template<class T>struct IOperatorObject :public IOperatorObjects
		{
		public:
			IMETHODS(POperatorObject<T>,IOperatorObject);
			/**
			@brief 运算符名称、优先级、参数数量、属性构造，参数数量传入65535时认为可接受任意数量的参数*/
			IOperatorObject(ptr<String> const& str,unsigned short lv,unsigned short cnt, unsigned short prop)
				:IOperatorObjects(str,lv, cnt,prop){}
			//先计算一次看哪个参数没有用到
			T TestCalculate(OperatorDebugArgs<T> &args)
			{
				Collection::PArray<std::pair<OperatorDebugArgs<T>*, index_t> >vargs(args.GetLength());
				OperatorArgs<T> oargs(&OperatorDebugArgs<T>::DefFun);
				for (index_t i = 0; i < args.GetLength(); ++i)
				{
					vargs[i].first = &args;
					vargs[i].second = i;
					oargs.AddArg(((std::pair<OperatorDebugArgs<T>*, index_t>*)vargs) + i);
				}
				return Calculate(oargs);
			}
			//包装第0层参数,idx为获取每个字符串参数的层级
			ptr<String> GetExpression(OperatorDebugArgs<T> &args,index_t lv=0)
			{
				ptr<String> str = ptr<String>::GetEmpty();
				if (IOperatorObjects::IsFunctionSupport())//函数
				{
					if (args.GetLength() != count&&count != 65535)Throw<PMathException >(name + L"函数参数数目不匹配");
					if (!IOperatorObjects::IsCalculateBack())str += name;
					str += L"(";
					index_t i = 0;
					while (i < args.GetLength())
					{
						if (i != 0)str += L",";
						if (args.HasCalculated(i))
							str += args(i,lv);
						else str += L"(...)";
						++i;
					}
					str += L")";
					if (IOperatorObjects::IsCalculateBack())str += name;
				}
				else if (IOperatorObjects::IsDoubleSupport())//双目运算符
				{
					if (args.GetLength() != 2)Throw< PMathException >(L"双目操作符：" + name + L"参数数量不是2个");
					if (args.HasCalculated(0))
						str += args(0, lv);
					else str += L"(...)";
					str += name;
					if (args.HasCalculated(1))
						str += args(1, lv);
					else str += L"(...)";
				}
				else if (IOperatorObjects::IsSingleSupport())//单目操作符
				{
					if (args.GetLength() != 1)Throw< PMathException >(L"单目操作符：" + name + L"参数数量不是1个");
					if (args.HasCalculated(0))
						str = args(0, lv);
					else str = L"(...)";
					if (IOperatorObjects::IsCalculateBack())str = str + name;
					else str = name + str;
				}
				else if (IOperatorObjects::IsAloneSupport())
				{
					if (args.GetLength() != 0)Throw< PMathException >(name + L"算子不需要参数");
					str = name;//单独算子
				}
				return str;
			}
			/**
			@brief 传入参数获取多个解析层级的解析结果*/
			virtual T DebugCalculate(OperatorDebugArgs<T> &args,Collection::PArray<ptr<String>>&result)
			{//默认实现，lv~0层展开，0层为表达式代入内部参数值，-1层为表达式代入内部表达式0层...以此类推
				//先进行一次计算，以确定是不是每个值都需要
				T val = TestCalculate(args);
				index_t mlen = args.GetMaxArrCount();
				if (args.level >= 0)mlen = 1;
				else if (mlen > -args.level)mlen = 1- args.level;
				Collection::PArray<ptr<String>> arr(mlen);
				if (mlen == 0) { result = arr; return val; }
				for (index_t lv = 1-mlen; lv <= 0; ++lv)
					arr[-lv]= L"(" + IOperatorObject<T>::GetExpression(args,lv) + L")";
				result=arr;
				return val;
			}
			/**
			@brief 获取运算结果，参数是一组函数对象，用以动态计算数值*/
			virtual T Calculate(OperatorArgs<T>&)const = 0;
			//析构函数
			virtual ~IOperatorObject(){}
		};
		//运算符对象代理类
		template<class T>struct POperatorObject :public POperatorObjects
		{
		public:
			POperatorObject& operator=(IOperatorObject<T>*p){SetTo(p); return *this;}
			PIMETHODS(POperatorObject<T>, IOperatorObject<T>);
			/**
			@brief 传入参数获取多个解析层级的解析结果*/
			T DebugCalculate(OperatorDebugArgs<T> &args, Collection::PArray<ptr<String>>&result) { return Ptr()->DebugCalculate(args,result); }
			//输入参数并计算
			inline T Calculate(OperatorArgs<T> & args)const{ return Ptr()->Calculate(args); }
		};

		//默认运算符

		//&&，优先级10
		template<class T>struct IOperatorAnd :public IOperatorObject<T>
		{
		public:
			IOperatorAnd() :IOperatorObject<T>(L"&&", 10,2, IOperatorObjects::SupportDoubleProperty| IOperatorObjects::CalculatePassProperty){}
			/**
			@brief 获取运算结果*/
			T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength() != 2)Throw< PMathException >(L"双目运算符&&参数数量不对");
#ifdef LK_MATH_FLOAT_
				if(::LK::IsSame<ptr<Math::Float>,T>::V)return Convert<T,ptr<Math::Float>>::RefFrom(!((ptr<Math::Float>&)args[0]).Equals0() && !((ptr<Math::Float>&)args[1]).Equals0() ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
#endif
				return (args[0] != 0 && args[1] != 0) ? 1 : 0;
			}
		};
		//||，优先级5
		template<class T>struct IOperatorOr :public IOperatorObject<T>
		{
		public:
			IOperatorOr() :IOperatorObject<T>(L"||", 5, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty){}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength() != 2)Throw< PMathException >(L"双目运算符||参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)return Convert<T,ptr<Math::Float>>::RefFrom(!((ptr<Math::Float>&)args[0]).Equals0() || !((ptr<Math::Float>&)args[1]).Equals0() ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
#endif
				return (args[0] != 0 || args[1] != 0) ? 1 : 0;
			}
		};
		//>，优先级15
		template<class T>struct IOperatorMore :public IOperatorObject<T>
		{
		public:
			IOperatorMore() :IOperatorObject<T>(L">", 15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty){}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符>参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) > ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] > args[1] ? 1 : 0;
			}
		};
		//>=，优先级15
		template<class T>struct IOperatorNotLess :public IOperatorObject<T>
		{
		public:
			IOperatorNotLess() :IOperatorObject<T>(L">=", 15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符>=参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) >= ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] >= args[1] ? 1 : 0;
			}
		};
		//<=，优先级15
		template<class T>struct IOperatorNotMore :public IOperatorObject<T>
		{
		public:
			IOperatorNotMore() :IOperatorObject<T>(L"<=", 15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符<=参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) <= ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] <= args[1] ? 1 : 0;
			}
		};
		//<，优先级15
		template<class T>struct IOperatorLess :public IOperatorObject<T>
		{
		public:
			IOperatorLess() :IOperatorObject<T>(L"<",15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符<参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) < ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] < args[1] ? 1 : 0;
			}
		};
		//!=，优先级15
		template<class T>struct IOperatorNotEqual :public IOperatorObject<T>
		{
		public:
			IOperatorNotEqual() :IOperatorObject<T>(L"!=", 15,2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符!=参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) != ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] != args[1] ? 1 : 0;
			}
		};
		//==，优先级15
		template<class T>struct IOperatorEqual :public IOperatorObject<T>
		{
		public:
			IOperatorEqual() :IOperatorObject<T>(L"==", 15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符==参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) == ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] == args[1] ? 1 : 0;
			}
		};
		//<>，优先级15
		template<class T>struct IOperatorNotEqual2 :public IOperatorObject<T>
		{
		public:
			IOperatorNotEqual2() :IOperatorObject<T>(L"<>", 15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符<>参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom((((ptr<Math::Float>&)args[0]) < ((ptr<Math::Float>&)args[1]) || ((ptr<Math::Float>&)args[0]) > ((ptr<Math::Float>&)args[1])) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return (args[0] < args[1] || args[0] > args[1]) ? 1 : 0;
			}
		};
		//=，优先级15
		template<class T>struct IOperatorEqual2 :public IOperatorObject<T>
		{
		public:
			IOperatorEqual2() :IOperatorObject<T>(L"=",15, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符=参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]) == ((ptr<Math::Float>&)args[1]) ? ptr<Math::Float>::One() : ptr<Math::Float>::Zero());
				}
#endif
				return args[0] == args[1] ? 1 : 0;
			}
		};
		//加法，优先级20
		template<class T>struct IOperatorPlus :public IOperatorObject<T>
		{
		public:
			IOperatorPlus() :IOperatorObject<T>(L"+", 20, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符+参数数量不对");
				return  args[0] + args[1];
			}
		};
		//减法，优先级20
		template<class T>struct IOperatorMinus :public IOperatorObject<T>
		{
		public:
			IOperatorMinus() :IOperatorObject<T>(L"-", 20, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符-参数数量不对");
				T t = args[0];
				return  t - args[1];
			}
		};
		//乘法，优先级30
		template<class T>struct IOperatorMultiply :public IOperatorObject<T>
		{
		public:
			IOperatorMultiply() :IOperatorObject<T>(L"*", 30, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符*参数数量不对");
				T t = args[0];
				return  t * args[1];
			}
		};
		//除法，优先级30
		template<class T>struct IOperatorDivide :public IOperatorObject<T>
		{
		public:
			IOperatorDivide() :IOperatorObject<T>(L"/", 30, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符/参数数量不对");
				T t = args[0];
				return  t / args[1];
			}
		};
		//求余，优先级30
		template<class T>struct IOperatorMod :public IOperatorObject<T>
		{
		public:
			IOperatorMod() :IOperatorObject<T>(L"%", 30, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符%参数数量不对");
				T t = args[0];
				return std::fmod(t, args[1]);
			}
		};
		//求^，优先级35
		template<class T>struct IOperatorPow :public IOperatorObject<T>
		{
		public:
			typedef typename LK::UseT<LK::HasT<T,s8,s16,s32,s64,u8,u16,u32,u64>::V, T, double>::T f_ref;
			IOperatorPow() :IOperatorObject<T>(L"^", 35, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw< PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 2)Throw< PMathException >(L"双目运算符^参数数量不对");
				if(LK::IsSame<f_ref,double>::V)
					return Convert<T,f_ref>::RefFrom((f_ref)std::pow((f_ref)args[0], (f_ref)args[1]));
				return Convert<T,f_ref>::RefFrom((f_ref)std::pow(*(f_ref*)args.GetPointer(0), *(f_ref*)args.GetPointer(1)));
			}
		};
		//正号，优先级40
		template<class T>struct IOperatorPositive :public IOperatorObject<T>
		{
		public:
			IOperatorPositive() :IOperatorObject<T>(L"+", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符+参数数量不对");
				return args[0];
			}
		};
		//负号，优先级40
		template<class T>struct IOperatorNegative :public IOperatorObject<T>
		{
		public:
			IOperatorNegative() :IOperatorObject<T>(L"-", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return -args[0];
			}
		};
		//!，优先级40
		template<class T>struct IOperatorNot :public IOperatorObject<T>
		{
		public:
			IOperatorNot() :IOperatorObject<T>(L"!", 40,  1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)return Convert<T,ptr<Math::Float>>::RefFrom(((ptr<Math::Float>&)args[0]).Equals0());
#endif
				return args[0] == 0 ? 1 : 0;
			}
		};
		//阶乘，优先级50
		template<class T>struct IOperatorFactorial :public IOperatorObject<T>
		{
		public:
			IOperatorFactorial() :IOperatorObject<T>(L"!", 50,  1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculateBackProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength()!= 1)Throw<PMathException >(L"后置单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				if (args[0] < 0)Throw<PMathException >(L"阶乘数小于0");
				if(args[0] == args[0]+1)Throw<PMathException >(L"阶乘数过大，无法计算");
				T tmp; T i;
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<ptr<Math::Float>, T>::V)
				{
					(ptr<Math::Float>&)tmp = (ptr<Math::Float>&)i = ptr<Math::Float>::One();
					((ptr<Math::Float>&)i).SetPrecision(((ptr<Math::Float>&)args[0]).GetPrecision());
				}
				else tmp =i= 1;
#else
				tmp = i = 1;
#endif
				while (++i <= args[0])
				{
					if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
					tmp *= i;
				}
				return tmp;
			}
		};
		//求正弦，优先级40
		template<class T>struct IOperatorSin :public IOperatorObject<T>
		{
		public:
			IOperatorSin() :IOperatorObject<T>(L"sin", 40 ,1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::sin(args[0]);
			}
		};
		//求余弦，优先级40
		template<class T>struct IOperatorCos :public IOperatorObject<T>
		{
		public:
			IOperatorCos() :IOperatorObject<T>(L"cos", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::cos(args[0]);
			}
		};
		//求正切，优先级40
		template<class T>struct IOperatorTan :public IOperatorObject<T>
		{
		public:
			IOperatorTan() :IOperatorObject<T>(L"tan", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
				/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::tan(args[0]);
			}
		};
		//求余切，优先级40
		template<class T>struct IOperatorCot :public IOperatorObject<T>
		{
		public:
			IOperatorCot() :IOperatorObject<T>(L"cot", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<T, ptr<Math::Float>>::V)return Convert<T,ptr<Math::Float>>::RefFrom(ptr<Math::Float>::Cot((ptr<Math::Float>&)args[0]));
#endif
				return 1.0/std::tan(args[0]);
			}
		};
		//求反正弦，优先级40
		template<class T>struct IOperatorAsin :public IOperatorObject<T>
		{
		public:
			IOperatorAsin() :IOperatorObject<T>(L"asin", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::asin(args[0]);
			}
		};
		//求反余弦，优先级40
		template<class T>struct IOperatorAcos :public IOperatorObject<T>
		{
		public:
			IOperatorAcos() :IOperatorObject<T>(L"acos", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::acos(args[0]);
			}
		};
		//求反正切，优先级40
		template<class T>struct IOperatorAtan :public IOperatorObject<T>
		{
		public:
			IOperatorAtan() :IOperatorObject<T>(L"atan", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::atan(args[0]);
			}
		};
		//求绝对值，优先级40
		template<class T>struct IOperatorAbs :public IOperatorObject<T>
		{
		public:
			IOperatorAbs() :IOperatorObject<T>(L"abs", 40, 2, IOperatorObjects::SupportDoubleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::abs(args[0]);
			}
		};
		//向下取整，优先级40
		template<class T>struct IOperatorFloor :public IOperatorObject<T>
		{
		public:
			IOperatorFloor() :IOperatorObject<T>(L"floor", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::floor(args[0]);
			}
		};
		//向上取整，优先级40
		template<class T>struct IOperatorCeil :public IOperatorObject<T>
		{
		public:
			IOperatorCeil() :IOperatorObject<T>(L"ceil", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::ceil(args[0]);
			}
		};
		//截取整数部分，优先级40
		template<class T>struct IOperatorInt :public IOperatorObject<T>
		{
		public:
			typedef typename UseT<IsClassOrUnion<T>::V, double, T>::T t_ref;
			IOperatorInt() :IOperatorObject<T>(L"int", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				void* p = &args[0];
				if (IsSame<T, double>::V)
					return Convert<T,double>::RefFrom(*(double*)p >= 0 ? std::floor(*(double*)p) : std::ceil(*(double*)p));
				if (IsSame<T, float>::V)
					return Convert<T,float>::RefFrom(*(float*)p >= 0 ? std::floor(*(float*)p) : std::ceil(*(float*)p));
				if (IsSame<int, typename UseT<IsClassOrUnion<T>::V, int, double>::T>::V)
					return *(T*)p;
				return Convert<T,t_ref>::RefFrom(*(t_ref*)p >= 0 ? std::floor(*(t_ref*)p) : std::ceil(*(t_ref*)p));
			}
		};
		//求自然数为底的对数，优先级40
		template<class T>struct IOperatorLn :public IOperatorObject<T>
		{
		public:
			IOperatorLn() :IOperatorObject<T>(L"ln", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::log(args[0]);
			}
		};
		//求10为底的对数，优先级40
		template<class T>struct IOperatorLg :public IOperatorObject<T>
		{
		public:
			IOperatorLg() :IOperatorObject<T>(L"lg", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::log10(args[0]);
			}
		};
		//求sqrt，优先级40
		template<class T>struct IOperatorSqrt :public IOperatorObject<T>
		{
		public:
			IOperatorSqrt() :IOperatorObject<T>(L"sqrt", 40, 1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::sqrt(args[0]);
			}
		};
		//求正弦，优先级40
		template<class T>struct IOperatorSinSingle :public IOperatorObject<T>
		{
		public:
			IOperatorSinSingle() :IOperatorObject<T>(L"sin", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::sin(args[0]);
			}
		};
		//求余弦，优先级40
		template<class T>struct IOperatorCosSingle :public IOperatorObject<T>
		{
		public:
			IOperatorCosSingle() :IOperatorObject<T>(L"cos", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::cos(args[0]);
			}
		};
		//求正切，优先级40
		template<class T>struct IOperatorTanSingle :public IOperatorObject<T>
		{
		public:
			IOperatorTanSingle() :IOperatorObject<T>(L"tan", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::tan(args[0]);
			}
		};
		//求余切，优先级40
		template<class T>struct IOperatorCotSingle :public IOperatorObject<T>
		{
		public:
			IOperatorCotSingle() :IOperatorObject<T>(L"cot", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<T, ptr<Math::Float>>::V)return Convert<T,ptr<Math::Float>>::RefFrom(ptr<Math::Float>::Cot((ptr<Math::Float>&)args[0]));
#endif
				return 1.0 / std::tan(args[0]);
			}
		};
		//求反正弦，优先级40
		template<class T>struct IOperatorAsinSingle :public IOperatorObject<T>
		{
		public:
			IOperatorAsinSingle() :IOperatorObject<T>(L"asin", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::asin(args[0]);
			}
		};
		//求反余弦，优先级40
		template<class T>struct IOperatorAcosSingle :public IOperatorObject<T>
		{
		public:
			IOperatorAcosSingle() :IOperatorObject<T>(L"acos", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::acos(args[0]);
			}
		};
		//求反正切，优先级40
		template<class T>struct IOperatorAtanSingle :public IOperatorObject<T>
		{
		public:
			IOperatorAtanSingle() :IOperatorObject<T>(L"atan", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::atan(args[0]);
			}
		};
		//求绝对值，优先级40
		template<class T>struct IOperatorAbsSingle :public IOperatorObject<T>
		{
		public:
			IOperatorAbsSingle() :IOperatorObject<T>(L"abs", 40, 2, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::abs(args[0]);
			}
		};
		//向下取整，优先级40
		template<class T>struct IOperatorFloorSingle :public IOperatorObject<T>
		{
		public:
			IOperatorFloorSingle() :IOperatorObject<T>(L"floor", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::floor(args[0]);
			}
		};
		//向上取整，优先级40
		template<class T>struct IOperatorCeilSingle :public IOperatorObject<T>
		{
		public:
			IOperatorCeilSingle() :IOperatorObject<T>(L"ceil", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::ceil(args[0]);
			}
		};
		//截取整数部分，优先级40
		template<class T>struct IOperatorIntSingle :public IOperatorObject<T>
		{
		public:
			typedef typename UseT<IsClassOrUnion<T>::V, double, T>::T t_ref;
			IOperatorIntSingle() :IOperatorObject<T>(L"int", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				void* p = &args[0];
				if (IsSame<T, double>::V)
					return Convert<T,double>::RefFrom(*(double*)p >= 0 ? std::floor(*(double*)p) : std::ceil(*(double*)p));
				if (IsSame<T, float>::V)
					return Convert<T,float>::RefFrom(*(float*)p >= 0 ? std::floor(*(float*)p) : std::ceil(*(float*)p));
				if (IsSame<int, typename UseT<IsClassOrUnion<T>::V, int, double>::T>::V)
					return *(T*)p;
				return Convert<T,t_ref>::RefFrom(*(t_ref*)p >= 0 ? std::floor(*(t_ref*)p) : std::ceil(*(t_ref*)p));
			}
		};
		//求自然数为底的对数，优先级40
		template<class T>struct IOperatorLnSingle :public IOperatorObject<T>
		{
		public:
			IOperatorLnSingle() :IOperatorObject<T>(L"ln", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::log(args[0]);
			}
		};
		//求10为底的对数，优先级40
		template<class T>struct IOperatorLgSingle :public IOperatorObject<T>
		{
		public:
			IOperatorLgSingle() :IOperatorObject<T>(L"lg", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength()!= 1)Throw<PMathException >(L"单目运算符"+IOperatorObjects::ToString()+L"参数数量不对");
				return std::log10(args[0]);
			}
		};
		//求sqrt，优先级40
		template<class T>struct IOperatorSqrtSingle :public IOperatorObject<T>
		{
		public:
			IOperatorSqrtSingle() :IOperatorObject<T>(L"sqrt", 40, 1, IOperatorObjects::SupportSingleProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength() != 1)Throw<PMathException >(L"单目运算符" + IOperatorObjects::ToString() + L"参数数量不对");
				return std::sqrt(args[0]);
			}
		};
		//求pow，优先级50
		template<class T>struct IOperatorPow2 :public IOperatorObject<T>
		{
		public:
			IOperatorPow2() :IOperatorObject<T>(L"pow", 50, 2, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength() != 2)Throw<PMathException >(L"函数" + IOperatorObjects::ToString() + L"参数数量不对");
				return std::pow(args[0], args[1]);
			}
		};
		//求任意底数对数，优先级50
		template<class T>struct IOperatorLog :public IOperatorObject<T>
		{
		public:
			IOperatorLog() :IOperatorObject<T>(L"log", 50, 2, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (args.GetLength() != 2)Throw<PMathException >(L"函数" + IOperatorObjects::ToString() + L"参数数量不对");
				return std::log(args[1]) / std::log(args[0]);
			}
		};
		//if函数，优先级50
		template<class T>struct IOperatorIf :public IOperatorObject<T>
		{
		public:
			IOperatorIf() :IOperatorObject<T>(L"if", 50, 3, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength() != 3)Throw<PMathException >(L"函数" + IOperatorObjects::ToString() + L"参数数量不对");
#ifdef LK_MATH_FLOAT_
				if (::LK::IsSame<T, ptr<Math::Float>>::V)return Convert<T,ptr<Math::Float>>::RefFrom(!((ptr<Math::Float>&)args[0]).Equals0()? (ptr<Math::Float>&)args[1]: (ptr<Math::Float>&)args[2]);
#endif
				if (args[0] != 0)
					return  args[1];
				return args[2];
			}
		};
		//求最大者，优先级50
		template<class T>struct IOperatorMax :public IOperatorObject<T>
		{
		public:
			IOperatorMax() :IOperatorObject<T>(L"max", 50, -1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength() == 0)Throw<PMathException >(L"函数" + IOperatorObjects::ToString() + L"参数数量不对");
				T* imax = &args[0];
				T* t;
				for (index_t i = 1; i < args.GetLength(); ++i)if (*(t=args.GetPointer(i))>*imax)imax = t;
				return *imax;
			}
		};
		//求最小者，优先级50
		template<class T>struct IOperatorMin :public IOperatorObject<T>
		{
		public:
			IOperatorMin() :IOperatorObject<T>(L"min", 50, -1, IOperatorObjects::SupportFunctionProperty | IOperatorObjects::CalculatePassProperty) {}
			/**
			@brief 获取运算结果*/
			virtual T Calculate(OperatorArgs<T> & args)const
			{
				if (args.GetLength() == 0)Throw<PMathException >(L"函数"+IOperatorObjects::ToString()+L"参数数量不对");
				T* imin = &args[0];
				T* t;
				for (index_t i = 1; i < args.GetLength(); ++i)if (*(t=args.GetPointer(i))<*imin)imin = t;
				return *imin;
			}
		};
		/**
		@brief 表示一个运算符表接口*/
		struct IOperatorTables :BigObject
		{
			IMETHODS(POperatorTables, IOperatorTables);
			//存放运算表，实际上是一个二维数组
			ptr<BigObject> table;
			//默认的运算符，默认为operator*
			POperatorObjects DefOperator;
			//添加一个指定的运算符，返回是否发生了运算符覆盖
			bool AddOperator(POperatorObjects const& obj);
			//清理运算符表
			void Clear();
			//移除指定名称的运算符
			void RemoveOperatorByName(ptr<String> const&str);
			//移除指定名称的运算符
			void RemoveOperatorByName(Collection::PArray<ptr<String>>const&arr);
			//移除指定的运算符
			void RemoveOperator(POperatorObjects const& x);
			IOperatorTables() {}
		};
		/**
		@brief 表示一个运算符表代理*/
		struct POperatorTables :ptr<BigObject>
		{
			PIMETHODS(POperatorTables, IOperatorTables);
			/**
			@brief 清空所有运算符*/
			void Clear()const { Ptr()->Clear(); }
			//添加一个指定的运算符，返回是否发生了运算符覆盖
			bool AddOperator(POperatorObjects const& obj) { return Ptr()->AddOperator(obj); }
			//移除指定名称的运算符
			void RemoveOperatorByName(ptr<String> const&str) { Ptr()->RemoveOperatorByName(str); }
			//移除指定名称的运算符
			void RemoveOperatorByName(Collection::PArray<ptr<String>>const&arr) { Ptr()->RemoveOperatorByName(arr); }
			//移除指定的运算符
			void RemoveOperator(POperatorObjects const& x) { Ptr()->RemoveOperator(x); }
			//获取默认运算符的引用
			POperatorObjects& GetDefaultOperator()const { return Ptr()->DefOperator; }
			//获取运算符列表的引用
			Collection::PArray<Collection::PArray<POperatorObjects > > & GetOperatorTableArrayRef()const { return (Collection::PArray<Collection::PArray<POperatorObjects > >&)Ptr()->table; }

		};

		template<class T>struct OperatorTable :IOperatorTables
		{
			BMETHODS(POperatorTable<T>, OperatorTable<T>)
			//默认构造函数，添加默认的运算符
			OperatorTable() { AddDefaultOperators(); }
			//添加默认的运算符
			void AddDefaultOperators();
			//获取默认运算符的引用
			POperatorObject<T>& GetDefaultOperator()const { return DefOperator; }
		};
		template<class T>struct POperatorTable :POperatorTables
		{
			PMETHODS(POperatorTable<T>, OperatorTable<T>);
			//获取默认运算符的引用
			POperatorObject<T>& GetDefaultOperator()const { return Ptr()->GetDefaultOperator(); }
			//添加默认的运算符
			void AddDefaultOperators() { Ptr()->AddDefaultOperators(); }
			//添加运算符(以数组方式添加)
			size_t AddOperators(Collection::PArray<Collection::PArray<POperatorObject<T> > >const& objs)
			{
				size_t c = 0;
				typename Collection::PArray<Collection::PArray<POperatorObject<T> > >::const_iterator iter = objs.begin();
				typename Collection::PArray<Collection::PArray<POperatorObject<T> > >::const_iterator end = objs.end();
				while (iter != end) { c += AddOperators(*iter); ++iter; }
				return c;
			}
			//添加运算符(以数组方式添加)
			size_t AddOperators(Collection::PArray<POperatorObject<T> >const& objs)
			{
				size_t c = 0;
				typename Collection::PArray<POperatorObject<T> >::const_iterator iter = objs.begin();
				typename Collection::PArray<POperatorObject<T> >::const_iterator end = objs.end();
				while (iter != end) { if (AddOperator(*iter))++c; ++iter; }
				return c;
			}
			//默认的静态运算符(全局)
			static POperatorObject<T> DefOperatorOr;//||
			static POperatorObject<T> DefOperatorAnd;//&&
			static POperatorObject<T> DefOperatorMore;//>
			static POperatorObject<T> DefOperatorNotLess;//>=
			static POperatorObject<T> DefOperatorLess;//<
			static POperatorObject<T> DefOperatorNotMore;//<=
			static POperatorObject<T> DefOperatorEqual;//==
			static POperatorObject<T> DefOperatorNotEqual;//!=
			static POperatorObject<T> DefOperatorEqual2;//=
			static POperatorObject<T> DefOperatorNotEqual2;//<>
			static POperatorObject<T> DefOperatorNot;//!
			static POperatorObject<T> DefOperatorFactorial;//!
			static POperatorObject<T> DefOperatorPlus;//+
			static POperatorObject<T> DefOperatorPositive;//+
			static POperatorObject<T> DefOperatorMinus;//-
			static POperatorObject<T> DefOperatorNegative;//-
			static POperatorObject<T> DefOperatorMultiply;//*
			static POperatorObject<T> DefOperatorDivide;//
			static POperatorObject<T> DefOperatorMod;//%
			static POperatorObject<T> DefOperatorPow;//^
			static POperatorObject<T> DefOperatorPow2;//pow
			static POperatorObject<T> DefOperatorMax;//max
			static POperatorObject<T> DefOperatorMin;//min
			static POperatorObject<T> DefOperatorIf;//if
			static POperatorObject<T> DefOperatorLog;//log
			static POperatorObject<T> DefOperatorSin;//sin
			static POperatorObject<T> DefOperatorCos;//cos
			static POperatorObject<T> DefOperatorTan;//tan
			static POperatorObject<T> DefOperatorCot;//cot
			static POperatorObject<T> DefOperatorAsin;//asin
			static POperatorObject<T> DefOperatorAcos;//acos
			static POperatorObject<T> DefOperatorAtan;//atan
			static POperatorObject<T> DefOperatorLn;//ln
			static POperatorObject<T> DefOperatorLg;//lg
			static POperatorObject<T> DefOperatorSqrt;//sqrt
			static POperatorObject<T> DefOperatorAbs;//abs
			static POperatorObject<T> DefOperatorFloor;//floor
			static POperatorObject<T> DefOperatorCeil;//ceil
			static POperatorObject<T> DefOperatorInt;//int
		};

		//设置默认运算符
		template<class T>void OperatorTable<T>::AddDefaultOperators()
		{
			AddOperator(POperatorTable<T>::DefOperatorOr);//||
			AddOperator(POperatorTable<T>::DefOperatorAnd);//&&
			AddOperator(POperatorTable<T>::DefOperatorMore);//>
			AddOperator(POperatorTable<T>::DefOperatorNotLess);//>=
			AddOperator(POperatorTable<T>::DefOperatorLess);//<
			AddOperator(POperatorTable<T>::DefOperatorNotMore);//<=
			AddOperator(POperatorTable<T>::DefOperatorEqual);//==
			AddOperator(POperatorTable<T>::DefOperatorNotEqual);//!=
			AddOperator(POperatorTable<T>::DefOperatorEqual2);//=
			AddOperator(POperatorTable<T>::DefOperatorNotEqual2);//<>
			AddOperator(POperatorTable<T>::DefOperatorNot);//!
			AddOperator(POperatorTable<T>::DefOperatorFactorial);//!
			AddOperator(POperatorTable<T>::DefOperatorPlus);//+
			AddOperator(POperatorTable<T>::DefOperatorMinus);//-
			AddOperator(POperatorTable<T>::DefOperatorPositive);//+
			AddOperator(POperatorTable<T>::DefOperatorNegative);//-
			AddOperator(POperatorTable<T>::DefOperatorMultiply);//*
			AddOperator(POperatorTable<T>::DefOperatorDivide);//
			AddOperator(POperatorTable<T>::DefOperatorMod);//%
			AddOperator(POperatorTable<T>::DefOperatorPow);//^
			AddOperator(POperatorTable<T>::DefOperatorPow2);//pow
			AddOperator(POperatorTable<T>::DefOperatorLog);//log
			AddOperator(POperatorTable<T>::DefOperatorMax);//max
			AddOperator(POperatorTable<T>::DefOperatorMin);//min
			AddOperator(POperatorTable<T>::DefOperatorIf);//if
			AddOperator(POperatorTable<T>::DefOperatorSin);//sin
			AddOperator(POperatorTable<T>::DefOperatorCos);//cos
			AddOperator(POperatorTable<T>::DefOperatorTan);//tan
			AddOperator(POperatorTable<T>::DefOperatorCot);//cot
			AddOperator(POperatorTable<T>::DefOperatorAsin);//asin
			AddOperator(POperatorTable<T>::DefOperatorAcos);//acos
			AddOperator(POperatorTable<T>::DefOperatorAtan);//atan
			AddOperator(POperatorTable<T>::DefOperatorLn);//ln
			AddOperator(POperatorTable<T>::DefOperatorLg);//lg
			AddOperator(POperatorTable<T>::DefOperatorSqrt);//sqrt
			AddOperator(POperatorTable<T>::DefOperatorAbs);//abs
			AddOperator(POperatorTable<T>::DefOperatorFloor);//floor
			AddOperator(POperatorTable<T>::DefOperatorCeil);//ceil
			AddOperator(POperatorTable<T>::DefOperatorInt);//int
			DefOperator = POperatorTable<T>::DefOperatorMultiply;//*
		}
		//静态默认运算符实现

		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorOr = new IOperatorOr<T>();//||
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorAnd = new IOperatorAnd<T>();//&&
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMore = new IOperatorMore<T>();//>
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNotLess = new IOperatorNotLess<T>();//>=
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorLess = new IOperatorLess<T>();//<
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNotMore = new IOperatorNotMore<T>();//<=
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorEqual = new IOperatorEqual<T>();//==
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNotEqual = new IOperatorNotEqual<T>();//!=
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorEqual2 = new IOperatorEqual2<T>();//=
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNotEqual2 = new IOperatorNotEqual2<T>();//<>
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNot = new IOperatorNot<T>();//!
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorFactorial = new IOperatorFactorial<T>();//!(阶乘)
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorPlus = new IOperatorPlus<T>();//+
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorPositive = new IOperatorPositive<T>();//+
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMinus = new IOperatorMinus<T>();//-
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorNegative = new IOperatorNegative<T>();//-
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMultiply = new IOperatorMultiply<T>();//*
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorDivide = new IOperatorDivide<T>();//
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMod = new IOperatorMod<T>();//%
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorPow = new IOperatorPow<T>();//^
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorPow2 = new IOperatorPow2<T>();//pow
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorSin = new IOperatorSinSingle<T>();//sin
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorCos = new IOperatorCosSingle<T>();//cos
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorTan = new IOperatorTanSingle<T>();//tan
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorCot = new IOperatorCotSingle<T>();//cot
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorAsin = new IOperatorAsinSingle<T>();//asin
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorAcos = new IOperatorAcosSingle<T>();//acos
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorAtan = new IOperatorAtanSingle<T>();//atan
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorLn = new IOperatorLnSingle<T>();//ln
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorLg = new IOperatorLgSingle<T>();//lg
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorLog = new IOperatorLog<T>();//log
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorSqrt = new IOperatorSqrtSingle<T>();//sqrt
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMax = new IOperatorMax<T>();//max
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorMin = new IOperatorMin<T>();//min
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorIf = new IOperatorIf<T>();//if
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorAbs = new IOperatorAbsSingle<T>();//abs
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorFloor = new IOperatorFloorSingle<T>();//floor
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorCeil = new IOperatorCeilSingle<T>();//ceil
		template<class T>POperatorObject<T> POperatorTable<T>::DefOperatorInt = new IOperatorIntSingle<T>();//int

	}
}
#endif
