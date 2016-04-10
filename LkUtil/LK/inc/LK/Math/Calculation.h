#ifndef LK_MATH_CALCULATION_
#define LK_MATH_CALCULATION_
#include<map>
namespace LK
{
	namespace Math
	{
		/**
		@brief 解析节点对象代理*/
		struct PCalculationNodes :ptr<BigObject>
		{
			PIMETHODS(PCalculationNodes, ICalculationNodes);
			/**
			@brief 获取类型标记的引用:-1已确定函数，0未确定重载，1括号内容，2逗号，3空格，4数值，5变量*/
			signed char& GetFlagRef()const;
			/**
			@brief 获取序号的引用*/
			int& GetIndexRef()const;
			/**
			@brief 获取是否已计算标记的引用*/
			bool& GetHasCalculatedRef()const;
			/**
			@brief 获取外部节点的引用*/
			PCalculationNodes& GetOutNodeRef()const;
			/**
			@brief 获取内部节点的引用*/
			PCalculationNodes& GetInnerNodeRef()const;
			/**
			@brief 获取前一个节点的引用*/
			PCalculationNodes& GetHeadNodeRef()const;
			/**
			@brief 获取后一个节点的引用*/
			PCalculationNodes& GetNextNodeRef()const;
			/**
			@brief 获取与内部节点计算的运算符的引用*/
			POperatorObjects& GetInnerOperatorRef()const;
			/**
			@brief 设置内部节点，并将内节点的外节点设置为本节点，断绝原内先结点与本节点的关系*/
			void SetInnerNode(PCalculationNodes const&)const;
			/**
			@brief 设置前节点，并将前节点的后节点设置为本节点，断绝原先前结点与本节点的关系*/
			void SetHeadNode(PCalculationNodes const&)const;
			/**
			@brief 设置后节点，并将后节点的前节点设置为本节点，断绝原先后结点与本节点的关系*/
			void SetNextNode(PCalculationNodes const&)const;
			/**
			@brief 设置外节点，并将外节点的内节点设置为本节点，断绝原先外结点与本节点的关系*/
			void SetOutNode(PCalculationNodes const&)const;
			/**
			@brief 清理节点关系*/
			void Clear();
			/**
			@brief 获取内一层节点数*/
			int GetInnerNodeCount()const;
			/**
			@brief 获取可能的运算符重载的引用*/
			Collection::PArray<POperatorObjects>& GetOverloadsRef()const;
			/**
			@brief 检查函数可能重载中的单独算子*/
			POperatorObjects OverloadAlone()const;
			/**
			@brief 检查函数可能重载中的单目运算符*/
			POperatorObjects OverloadSingle()const;
			/**
			@brief 检查函数可能重载中后置单目运算符*/
			POperatorObjects OverloadBackSingle()const;
			/**
			@brief 检查函数可能重载中双目运算符*/
			POperatorObjects OverloadDouble()const;
			/**
			@brief 检查函数可能重载中指定参数数量函数运算符*/
			POperatorObjects OverloadFunction(index_t c)const;
			/**
			@brief 检查函数可能重载中指定参数数量后置函数运算符*/
			POperatorObjects OverloadBackFunction(index_t c)const;
			/**
			@brief 使用指定的字符串来设置数值*/
			void SetValueWithString(ptr<String> str);
			//获取该节点以及子节点中包含指定operator的数量
			index_t GetCountOfOperatorInNodes(POperatorObjects const& x)const
			{
				index_t i = 0;
				if (!GetInnerNodeRef().IsNull())i += GetInnerNodeRef().GetCountOfOperatorInNodes(x);
				if (!GetNextNodeRef().IsNull())i += GetNextNodeRef().GetCountOfOperatorInNodes(x);
				return BigObject::ReferenceEqual(GetInnerOperatorRef(), x) ? (i + 1) : i;
			}
		};
		/**
		@brief 解析节点对象*/
		struct ICalculationNodes :BigObject
		{
			IMETHODS(PCalculationNodes, ICalculationNodes);
			/**
			@brief 外部节点*/
			PCalculationNodes outNode;
			/**
			@brief 内部节点*/
			PCalculationNodes innerNode;
			/**
			@brief 前节点*/
			PCalculationNodes headNode;
			/**
			@brief 后节点*/
			PCalculationNodes nextNode;
			/**
			@brief 内部计算方式*/
			POperatorObjects innerOperator;
			/**
			@brief 可能的运算符重载*/
			Collection::PArray<POperatorObjects> overloads;
			/**
			@brief 节点类型标记：-1已确定函数，0未确定重载，1括号内容，2逗号，3空格，4数值，5变量*/
			signed char flag;
			/**
			@brief 是否已计算过*/
			bool hasCalculated;
			/**
			@brief 变量序号或函数序号或参数个数*/
			int index;
			/**
			@brief 获取字符串表示*/
			virtual ptr<String> ToString()const = 0;
			/**
			@brief 设置内部节点*/
			void SetInnerNode(PCalculationNodes);
			/**
			@brief 设置前节点*/
			void SetHeadNode(PCalculationNodes);
			/**
			@brief 设置后节点*/
			void SetNextNode(PCalculationNodes);
			/**
			@brief 设置外节点*/
			void SetOutNode(PCalculationNodes);
			/**
			@brief 清理节点关系*/
			void Clear()
			{
				if (!innerNode.IsNull() && innerNode.GetOutNodeRef().Equals(this)) { innerNode.GetOutNodeRef().SetToNull(); innerNode.Clear(); }
				if (!outNode.IsNull() && outNode.GetInnerNodeRef().Equals(this)) { outNode.GetInnerNodeRef().SetToNull(); outNode.Clear(); }
				if (!headNode.IsNull() && headNode.GetNextNodeRef().Equals(this)) { headNode.GetNextNodeRef().SetToNull(); headNode.Clear(); }
				if (!nextNode.IsNull() && nextNode.GetHeadNodeRef().Equals(this)) { nextNode.GetHeadNodeRef().SetToNull(); nextNode.Clear(); }
			}
			/**
			@brief 使用一个类别符号来构造*/
			ICalculationNodes(signed char f) : flag(f), hasCalculated(false) {}
			/**
			@brief 析构函数*/
			~ICalculationNodes() { }
			/**
			@brief 获取内一层节点数*/
			int GetInnerNodeCount()const { int c = 0; PCalculationNodes n = innerNode; while (!n.IsNull()) { ++c; n = n.GetNextNodeRef(); }return c; }
			/**
			@brief 检查函数可能重载中的单独算子*/
			POperatorObjects OverloadAlone()const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)if (overloads[i].IsAloneSupport())return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 检查函数可能重载中的单目运算符*/
			POperatorObjects OverloadSingle()const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)if (overloads[i].IsSingleSupport() && !overloads[i].IsCalculateBack())return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 检查函数可能重载中后置单目运算符*/
			POperatorObjects OverloadBackSingle()const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)if (overloads[i].IsSingleSupport() && overloads[i].IsCalculateBack())return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 检查函数可能重载中双目运算符*/
			POperatorObjects OverloadDouble()const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)if (overloads[i].IsDoubleSupport())return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 检查函数可能重载中指定参数数量函数运算符*/
			POperatorObjects OverloadFunction(index_t c)const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)
					if (overloads[i].IsFunctionSupport() &&
						!overloads[i].IsCalculateBack() &&
						(overloads[i].GetParametersCount() == c ||
							overloads[i].GetParametersCount() == 65535))return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 检查函数可能重载中指定参数数量后置函数运算符*/
			POperatorObjects OverloadBackFunction(index_t c)const
			{
				if (!overloads.IsNull())
					for (index_t i = 0; i < overloads.GetLength(); ++i)
					if (overloads[i].IsFunctionSupport() &&
						overloads[i].IsCalculateBack() &&
						(overloads[i].GetParametersCount() == c ||
							overloads[i].GetParametersCount() == 65535))return overloads[i];
				return POperatorObjects();
			}
			/**
			@brief 使用指定的字符串来设置数值*/
			virtual void SetValueWithString(ptr<String> str) = 0;
		};
		template<class T>struct CalculationCallBack;//用以传入回调函数中以进行调用
		template<class T>struct CalculationDebugCallBack;//用以传入回调函数中以进行调用

		/**
		@brief 具体类型的解析节点对象*/
		template<class T>struct VCalculationNode :ICalculationNodes
		{
			BMETHODS(PCalculationNode<T>, VCalculationNode<T>)
				/**
				@brief 使用一个类别符号来构造*/
				VCalculationNode(signed char f) :ICalculationNodes(f), value(0) {}
			//节点值
			T value;
			/**
			@brief 将该式子转换到字符串表示*/
			ptr<String> ToString()const;
			/**
			@brief 使用指定的字符串来设置数值*/
			void SetValueWithString(ptr<String> str)
			{
				typedef typename UseT<IsClassOrUnion<T>::V, ptr<String>, T>::T Obj;
				//如果T为double 类型
				if (IsSame<T, double>::V)
					value = (Convert<T, double>::RefFrom(str.ToDouble()));
				//如果T为float类型
				else if (IsSame<T, float>::V)
					value = (Convert<T, float>::RefFrom((float)str.ToDouble()));
				//如果T为其它基本数据类型(整形)
				else if (!IsClassOrUnion<T>::V)
					value = (Convert<T, u64>::RefFrom(str.ToUInt64()));
				//默认的未知类型将调用其构造函数，比如Float256等
				else value = (Convert<T, Obj>::RefFrom(Obj(str)));
			}
		};

		/**
		@brief 具体类型的解析节点对象代理*/
		template<class T>struct PCalculationNode :PCalculationNodes
		{
			PMETHODS(PCalculationNode, VCalculationNode<T>);
			/**
			@brief 获取数值引用*/
			T & GetValueRef()const { return Ptr()->value; }
			/**
			@brief 获取外部节点的引用*/
			PCalculationNode& GetOutNodeRef()const { return Convert<PCalculationNode, PCalculationNodes>::RefFrom(PCalculationNodes::GetOutNodeRef()); }
			/**
			@brief 获取内部节点的引用*/
			PCalculationNode& GetInnerNodeRef()const { return Convert<PCalculationNode, PCalculationNodes>::RefFrom(PCalculationNodes::GetInnerNodeRef()); }
			/**
			@brief 获取前一个节点的引用*/
			PCalculationNode& GetHeadNodeRef()const { return Convert<PCalculationNode, PCalculationNodes>::RefFrom(PCalculationNodes::GetHeadNodeRef()); }
			/**
			@brief 获取后一个节点的引用*/
			PCalculationNode& GetNextNodeRef()const { return Convert<PCalculationNode, PCalculationNodes>::RefFrom(PCalculationNodes::GetNextNodeRef()); }
			/**
			@brief 获取与内部节点计算的运算符的引用*/
			POperatorObject<T>& GetInnerOperatorRef()const { return Convert<POperatorObject<T>, ptr<BigObject> >::RefFrom(PCalculationNodes::GetInnerOperatorRef()); }
			/**
			@brief 获取可能的运算符重载*/
			Collection::PArray<POperatorObject<T> >& GetOverloadsRef()const { return Convert<Collection::PArray<POperatorObject<T> >, ptr<BigObject> >::RefFrom(PCalculationNodes::GetOverloadsRef()); }
			/**
			@brief 检查函数可能重载中的单独算子*/
			POperatorObject<T> OverloadAlone()const { return Convert<POperatorObject<T>, ptr<BigObject>>::RefFrom(PCalculationNodes::OverloadAlone()); }
			/**
			@brief 检查函数可能重载中的单目运算符*/
			POperatorObject<T> OverloadSingle()const { return Convert<POperatorObject<T>, ptr<BigObject>>::RefFrom(PCalculationNodes::OverloadSingle()); }
			/**
			@brief 检查函数可能重载中后置单目运算符*/
			POperatorObject<T> OverloadBackSingle()const { return Convert<POperatorObject<T>, ptr<BigObject> >::RefFrom(PCalculationNodes::OverloadBackSingle()); }
			/**
			@brief 检查函数可能重载中双目运算符*/
			POperatorObject<T> OverloadDouble()const { return Convert<POperatorObject<T>, ptr<BigObject> >::RefFrom(PCalculationNodes::OverloadDouble()); }
			/**
			@brief 检查函数可能重载中指定参数数量函数运算符*/
			POperatorObject<T> OverloadFunction(index_t c)const { return Convert<POperatorObject<T>, ptr<BigObject>>::RefFrom(PCalculationNodes::OverloadFunction(c)); }
			/**
			@brief 检查函数可能重载中指定参数数量后置函数运算符*/
			POperatorObject<T> OverloadBackFunction(index_t c)const { return Convert<POperatorObject<T>, ptr<BigObject>>::RefFrom(PCalculationNodes::OverloadBackFunction(c)); }
			/**
			@brief 传入变量参数以计算值*/
			void Calculate(OperatorArgs<T>& args);
			T& Calculate(T* p, index_t len);
			/**
			@brief 算值函数*/
			static T& Calculate(void*ptr);
			//与获取字符串相关的算值函数
			static T & StringCalculate(void*, Collection::PArray<ptr<String>>&);
			/**
			@brief 获取字符串表示*/
			ptr<String> ToString()const { return (*(ptr<BigObject>*)this)->ToString(); }
			/**
			@brief 传入参数获取多个解析层级的解析结果*/
			Collection::PArray<ptr<String>> GetCalculationString(OperatorDebugArgs<T> &args);
		};

		template<class T>struct CalculationCallBack//用以传入回调函数中以进行调用
		{
			PCalculationNode<T> node;
			OperatorArgs<T>* param;
		};
		template<class T>struct CalculationDebugCallBack//用以传入回调函数中以进行调用
		{
			PCalculationNode<T> node;
			OperatorDebugArgs<T>* param;
		};

		//转换到字符串
		template<class T>ptr<String> VCalculationNode<T>::ToString()const
		{
			ptr<String> allstr = ptr<String>::GetEmpty();
			PCalculationNode<T> node = (VCalculationNode<T>*)this;
			do
			{
				//-1已确定函数，0未确定重载，1括号内容，2逗号，3空格，4数值，5变量
				ptr<String> str = ptr<String>::GetEmpty();
				bool back = false;
				bool dbl = false;
				switch (node.GetFlagRef())
				{
				case 5:
					str = L"(var[" + ptr<String>(node.GetIndexRef()) + L"])";
					break;
				case 4:
					str = Convert<ptr<String>, T>::From(node.GetValueRef());
					break;
				case 3:
					str = L" ";
					break;
				case 2:
					if (!node.Equals((VCalculationNode<T>*)this))str = L",";
					break;
				case 1:
					break;
				case 0:
					if (!node.GetInnerOperatorRef().IsNull())
						str = node.GetInnerOperatorRef().ToString();
					break;
				case -1:
					if (!node.GetInnerOperatorRef().IsNull()) {
						str = node.GetInnerOperatorRef().ToString();
						back = node.GetInnerOperatorRef().IsCalculateBack();
						dbl = node.GetInnerOperatorRef().IsDoubleSupport();
						if (node.GetInnerOperatorRef().IsFunctionSupport() &&
							(back ? (node.GetHeadNodeRef().IsNull() || node.GetHeadNodeRef().GetFlagRef() != 1) :
								(node.GetNextNodeRef().IsNull() || node.GetNextNodeRef().GetFlagRef() != 1)) &&
							node.GetInnerNodeRef().IsNull()&&!node.GetInnerOperatorRef().IsSingleSupport())
							str += L"()";
					}

					break;
				default:
					str = L"(未知类型节点)";
				}
				if (!node.GetInnerNodeRef().IsNull())
				{
					if (dbl)
					{
						str = L"(" + node.GetInnerNodeRef().GetInnerNodeRef().ToString() + str + node.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef().ToString() + L")";
					}
					else
					{
						ptr<String> tstr = node.GetInnerNodeRef().ToString();
						if (back)str = L"(" + tstr + L")" + str;
						else str += L"(" + tstr + L")";
					}
				}
				else if (node.GetFlagRef() == 1)str = L"()";
				allstr += str;
			} while (!(node = node.GetNextNodeRef()).IsNull());
			return allstr;
		}
		//转换到字符串数组
		template<class T>Collection::PArray<ptr<String>> PCalculationNode<T>::GetCalculationString(OperatorDebugArgs<T>&args)
		{
			Collection::PArray<ptr<String>>arr;
			if (GetFlagRef() == 2 || GetFlagRef() == 1)//逗号或者括号获取内部内容（等lv传递获取）
			{
				if (!GetInnerNodeRef().IsNull())
				{
					arr = GetInnerNodeRef().GetCalculationString(args);
					GetValueRef() = GetInnerNodeRef().GetValueRef();
				}
				else
				{
					arr = Collection::PArray<ptr<String>>(1, L"0");
					GetValueRef() = 0;
				}
				return arr;
			}
			else arr = Collection::PArray<ptr<String>>(1);
			OperatorDebugArgs<T> inargs(&PCalculationNode<T>::StringCalculate, args.deep - 1, args.level + 1);//内部参数
			Collection::PArray<CalculationDebugCallBack<T> >vargs; //回调
			Collection::PArray<ptr<String>> instrs;
			if (args.level < 0)switch (GetFlagRef())//根据节点类型获取节点字符串
			{
			case 5://变量可继续内部展开。
				GetValueRef() = args[GetIndexRef()];
				instrs = args(GetIndexRef()).Clone();
				if (!instrs.IsNull() && instrs.GetLength() > 0)instrs.RemoveAt(0);
				break;
			case 4://数字无内部展开
				break;
			default:
				//函数或者双目之类要处理内部参数
				if (!GetInnerOperatorRef().IsNull())//存在内部函数
				{
					if (!GetInnerNodeRef().IsNull())//非空内部函数
					{
						if (GetInnerNodeRef().GetFlagRef() != 2)//不是逗号，表明只有一个参数
						{
							vargs = Collection::PArray<CalculationDebugCallBack<T> >(1);
							vargs[0].node = GetInnerNodeRef();
							vargs[0].param = &args;
							inargs.AddArg((CalculationDebugCallBack<T>*)vargs);
						}
						else
						{
							vargs = Collection::PArray<CalculationDebugCallBack<T> >((index_t)0);
							PCalculationNode<T> in = GetInnerNodeRef();
							while (!in.IsNull())
							{
								CalculationDebugCallBack<T> t;
								t.node = in;
								t.param = &args;
								vargs.Append(t);
								in = in.GetNextNodeRef();
							}
							for (index_t i = 0; i < vargs.GetLength(); ++i)
								inargs.AddArg((CalculationDebugCallBack<T>*)vargs + i);
						}
					}
					GetValueRef() = GetInnerOperatorRef().DebugCalculate(inargs, instrs);
				}
				break;
			}
			if (!instrs.IsNull())arr.AppendArray(instrs);
			if (args.level <= 0 && 1 - args.level < arr.GetLength())
				arr.Resize(1 - args.level);
			arr[0] = Convert< ptr<String>, T>::From(GetValueRef());//0层表示结果，-1层开始展开计算步骤
			return arr;
		}
		/**
		@brief 传入回调变量参数以计算值*/
		template<class T>inline void PCalculationNode<T>::Calculate(OperatorArgs<T>& args)
		{
			//如果已计算（比如数值无需计算）则直接返回
			if (GetHasCalculatedRef())return;
			if (GetFlagRef() == 5)//变量
			{
				GetValueRef() = args[GetIndexRef()]; return;
			}
			if ((GetFlagRef() == 2 || GetFlagRef() == 1) && !GetInnerNodeRef().IsNull())//逗号或括号
			{
				GetInnerNodeRef().Calculate(args);
				GetValueRef() = GetInnerNodeRef().GetValueRef();
				return;
			}
			//函数节点
			if (!GetInnerOperatorRef().IsNull())//如果与内部节点间存在函数
			{
				if (GetInnerNodeRef().IsNull())//无内部节点
				{
					GetValueRef() = GetInnerOperatorRef().Calculate(OperatorArgs<T>::Default);//空参数
					GetHasCalculatedRef() = GetInnerOperatorRef().IsCalculateOnce();//如果只需计算一次，那么以后就再也不需要计算了
				}
				else//有内部节点，进行参数组织操作
				{
					OperatorArgs<T> argsPass(&PCalculationNode<T>::Calculate);//传入参数处理
					CalculationCallBack<T> vals[8];//缓存8个参数，不够时使用varr
					Collection::PArray<CalculationCallBack<T> > varr;
					CalculationCallBack<T>* p;//参数指针
					PCalculationNode<T> n = GetInnerNodeRef();//
					index_t c = 0;
					if (n.GetFlagRef() == 2)//参数不止一个
					{
						c = GetInnerNodeCount();
						if (c > 8)
						{
							varr = Collection::PArray<CalculationCallBack<T> >(c);
							p = varr;
						}
						else
							p = vals;
						for (index_t i = 0; i < c; ++i)
						{
							p[i].node = n;
							p[i].param = &args;
							argsPass.AddArg(p + i);
							n = n.GetNextNodeRef();
						}
						GetValueRef() = GetInnerOperatorRef().Calculate(argsPass);
						bool hascalculate = true;
						n = GetInnerNodeRef();
						for (index_t i = 0; i < c; ++i)
						{
							hascalculate = hascalculate&& n.GetHasCalculatedRef();
							n = n.GetNextNodeRef();
						}
						GetHasCalculatedRef() = ((hascalculate&&GetInnerOperatorRef().IsCalculatePass()) || GetInnerOperatorRef().IsCalculateOnce());
					}
					else
					{
						vals->param = &args;
						vals->node = n;
						argsPass.AddArg(vals);
						GetValueRef() = (GetInnerOperatorRef().Calculate(argsPass));
						GetHasCalculatedRef() = (n.GetHasCalculatedRef() && GetInnerOperatorRef().IsCalculatePass());//传递表明当参数不变时，结果不变
					}
				}
			}
		}
		template<class T>inline T & PCalculationNode<T>::Calculate(T * p, index_t len)
		{
			OperatorArgs<T> argsPass;
			for (index_t i = 0; i < len; ++i)argsPass.AddValue(p[i]);
			Calculate(argsPass);
			return (T&)GetValueRef();
		}
		template<class T>T& PCalculationNode<T>::Calculate(void*ptr)
		{
			if (ptr) {
				if (((CalculationCallBack<T>*)ptr)->param)((CalculationCallBack<T>*)ptr)->node.Calculate(*((CalculationCallBack<T>*)ptr)->param);
				return (T&)((CalculationCallBack<T>*)ptr)->node.GetValueRef();
			}
			Throw<PMathException >(L"传参回调函数错误：无效的回调类型");
		}
		template<class T>T & PCalculationNode<T>::StringCalculate(void*ptr, Collection::PArray<ptr<String>>&arr)
		{
			if (ptr) {
				if (((CalculationDebugCallBack<T>*)ptr)->param)arr = ((CalculationDebugCallBack<T>*)ptr)->node.GetCalculationString(*((CalculationDebugCallBack<T>*)ptr)->param);
				return (T&)((CalculationDebugCallBack<T>*)ptr)->node.GetValueRef();
			}
			Throw<PMathException >(L"传参回调函数错误：无效的回调类型");
		}
		/**
		@brief 算式接口*/
		struct ICalculation :public ::LK::BigObject
		{
		protected:
			//当前字符串获取过计算结果的次数
			int getResultCount;
			//计算式字符串
			ptr<String> sourceString;
			//制作一个新的节点
			virtual PCalculationNodes MakeCalculationNode(signed char flag) = 0;
			//制作一个空的运算符表
			virtual POperatorTables MakeOperatorTable()=0;
		public:
			IMETHODS(PCalculations, ICalculation);
			//默认构造函数
			ICalculation() :getResultCount(0) {}
			//转换到字符串表示
			ptr<String> ToString()const { if (baseNode.IsNull())((ICalculation*)this)->Analyse(((ICalculation*)this)->baseNode); if (baseNode.IsNull())return ptr<String>::GetEmpty(); return baseNode->ToString(); }
			//基节点
			PCalculationNodes baseNode;
			//运算符表
			POperatorTables operatorTable;
			//当前的变量名到变量值序号的映射数组
			Collection::PArray<std::pair<ptr<String>, index_t> >variables;
			//预解析某个节点，将字符串内容转换到节点列表
			void PreAnalyse();
			//确定单个节点
			bool MakeSure(PCalculationNodes& node);
			//运算符重载中的前置单目运算符肯定分析
			void MakeSureSingle(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//运算符重载中的后置单目运算符肯定分析
			void MakeSureSingleBack(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//运算符重载中的前置函数运算符肯定分析
			void MakeSureFun(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//运算符重载中的后置函数运算符肯定分析
			void MakeSureFunBack(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//运算符重载中的算子肯定分析
			void MakeSureAlone(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//运算符重载中的双目肯定分析
			void MakeSureDouble(PCalculationNodes& node, Collection::PArray<POperatorObjects >ops, POperatorObjects &s, index_t cnts[]);
			//确定性分析
			bool SureAnalyse(PCalculationNodes& node);
			//移除某个节点的多余括号
			void RemoveLR(PCalculationNodes node);
			//根据标记添加节点
			void AddNodeByFlag(PCalculationNodes&lastnode, PCalculationNodes newNode, signed char& lastfalg)
			{
				if (baseNode.IsNull())baseNode = newNode;
				else
				{
					if (lastfalg == 1) { lastnode.GetInnerNodeRef() = (newNode); newNode.GetOutNodeRef() = (lastnode); }
					else { lastnode.GetNextNodeRef() = (newNode); newNode.GetHeadNodeRef() = (lastnode); }
				}
				lastnode = newNode;
				lastfalg = newNode.GetFlagRef();
			}
			//解析某个节点
			void Analyse(PCalculationNodes& node);
			//移除解析结果
			void ClearAnalyse()
			{
				if (!baseNode.IsNull())
					baseNode.Clear();
				baseNode.SetToNull();
				getResultCount = 0;
			}
			//获取分析后的字符串
			ptr<String> GetAnalyzedString() {
				if (baseNode.IsNull()) {
					Analyse(baseNode);
				}
				return baseNode.IsNull() ? ptr<String>::GetEmpty() : baseNode->ToString();
			}
			//获取当前函数列表
			POperatorTables GetOperatorTable()const { return operatorTable; }
			//设置函数列表
			void SetOperatorTable(POperatorTables const&tbl)
			{
				if (BigObject::ReferenceEqual(operatorTable, tbl))return;
				if (operatorTable.RefCount == 1)operatorTable.Clear();
				operatorTable = tbl;
				ClearAnalyse();
			}
			//获取当前算式字符串
			ptr<String> GetString()const { return sourceString; }
			//设置当前算式字符串
			void SetString(ptr<String> const& str) { if (str.IsNull() || sourceString.IsNull() || str != sourceString) { getResultCount = 0; sourceString = str; ClearAnalyse(); } }
			//设置参数(如果原来有相同的名称，那原来的序号将会被替换)
			void AddVariable(Collection::PArray<ptr<String>>argStrings);
			//添加参数与序号的关联(如果原来有相同的名称，那原来的序号将会被替换)
			void AddVariable(const ptr<String>& name, index_t index);
			//获取变量名数组
			Collection::PArray<ptr<String>> GetVariableArray()const;
			//移除指定的变量名
			void RemoveVariable(ptr<String>& name);
			//移除所有变量名
			void ClearVariable();
			//获取树形结点中与指定operator相等（BigObject::ReferenceEqual）的运算符数量,如果baseNode为NULL则返回0
			index_t GetCountOfOperatorInNodes(POperatorObjects const& x)const;
			//清理所有内容
			void Clear();
			//获取相邻搭配的可能性（1为可能，0为不可能）,
			//传入的数值代表各种符号：0无内容,1括号或逗号,2数值变量或算子,3前置单目运算符,4后置单目运算符,5前置函数,6后置函数,7双目
			static bool GetPosible(int head, int curent, int next)
			{
				static const unsigned long long result[8] =
				{//000-077
					0x000002002ebfff00,
					//100-177
					   0x2ebf02bf2ebfff00,
					//200-277
					   0x2e0002bf2ebfff00,
					//300-377
					   0x000002002ebfff00,
					//400-477
					   0x2e0002bf2ebfff00,
					//500-577
					   0x000000000000ff00,
					//600-677
					   0x2e0002bf2ebfff00,
					//700-777
					   0x000002002ebfff00
				};
				return Bits::Get(result + head, curent * 8 + next);
			}
		};
		/**
		@brief 算式接口代理（本类型可能会出现循环引用现象，因此当不再使用该类型对象时，应调用Clear成员函数以清除循环引用）*/
		struct PCalculations : ::LK::ptr<BigObject>
		{
		public:
			PIMETHODS(PCalculations, ICalculation);
			//获取当前算式字符串
			ptr<String> GetString()const { return Ptr()->GetString(); }
			//清理所有内容，若运算符表仅被该对象引用，则自动调用运算符表的Clear()
			void Clear() { Ptr()->Clear(); }
			//析构函数
			~PCalculations() {}
			//设置当前算式字符串
			void SetString(ptr<String> const& str);
			//获取分析后的字符串
			ptr<String> GetAnalyzedString();
			//设置参数(如果原来有相同的名称，那原来的序号将会被替换)
			void AddVariable(Collection::PArray<ptr<String>>argStrings);
			//移除指定的变量名
			void RemoveVariable(ptr<String>& name);
			//获取变量名数组
			Collection::PArray<ptr<String>> GetVariableArray()const;
			//移除所有变量名
			void ClearVariable();
			//获取当前函数列表
			POperatorTables GetOperatorTable()const;
			//设置函数列表
			void SetOperatorTable(POperatorTables const&tbl);
			//移除解析结果
			void ClearAnalyse();
			//获取树形结点中与指定operator相等（BigObject::ReferenceEqual）的运算符数量,如果baseNode为NULL则返回0
			index_t GetCountOfOperatorInNodes(POperatorObjects const& x)const;
		};
		/**
		@brief 代表一个算式，其中T表示数值类型，比如double、Float256等*/
		template<class T>class Calculation :public ICalculation
		{
		protected:
			//上一次运算结果
			T lastValue;
			//制作一个新的节点
			virtual PCalculationNodes MakeCalculationNode(signed char flag) { return new VCalculationNode<T>(flag); }
			//制作一个空的运算符表
			virtual POperatorTables MakeOperatorTable() { return new OperatorTable<T>(); }
		public:
			//通用实体类算法
			BMETHODS(PCalculation<T>, Calculation)
			//默认构造函数
			Calculation();
			/**
			@brief 使用一个字符串来初始化算式*/
			Calculation(ptr<String> const&str);
			//析构函数，清理解析、运算符、变量
			~Calculation()
			{
				ClearAnalyse();
				ClearVariable();
			}
			//动态计算并返回结果
			const T&Calculate(OperatorArgs<T>&args)
			{
				lastValue = baseNode.IsNull() ? T(0) : ((PCalculationNode<T>&)baseNode).GetValueRef();
				if (baseNode.IsNull())Analyse(baseNode);
				if (baseNode.IsNull())Throw<PMathException >(L"算式为空，无法计算值");
				++getResultCount;
				((PCalculationNode<T>&)baseNode).Calculate(args);
				return ((PCalculationNode<T>&)baseNode).GetValueRef();
			}
			//传入参数并获取结果
			const T& GetResult(T *args = 0)
			{
				lastValue = baseNode.IsNull() ? 0 : ((PCalculationNode<T>&)baseNode).GetValueRef();
				if (baseNode.IsNull())Analyse(baseNode);
				if (baseNode.IsNull())Throw<PMathException >(L"算式根节点为空");
				++getResultCount;
				((PCalculationNode<T>&)baseNode).Calculate(args, variables.IsNull() ? 0 : variables.GetLength());
				return ((PCalculationNode<T>&)baseNode).GetValueRef();
			}
			//传入参数值并获取结果
			inline const T& GetResult(Collection::PArray<T>  const&args) { return GetResult((T const*)args); }
			//获取上一次的计算结果
			inline T const& GetLastResult()const { return lastValue; }
			//获取最近一次的计算结果
			inline T const& GetCurrentResult()const { if (baseNode.IsNull())Throw<PMathException >(L"数值尚未计算"); return ((PCalculationNode<T>&)baseNode).GetValueRef(); }
			//获取内部解析结果
			Collection::PArray<ptr<String>> GetCalculationString(OperatorDebugArgs<T>&args)
			{
				if (baseNode.IsNull())
					Analyse(baseNode);
				if (baseNode.IsNull())return Collection::PArray<ptr<String>>();
				return ((PCalculationNode<T>&)baseNode).GetCalculationString(args);
			}
			//解析结果(level~0)
			Collection::PArray<ptr<String>> GetAnalyzedString(index_t level, index_t deep, T *args);
		};
		//表示一个计算式的代理类
		template<class T>class PCalculation :public PCalculations
		{
		public:
			//基类通用方法
			PMETHODS(PCalculation, Calculation<T>);
			/**
			@brief 使用一个字符串来初始化算式*/
			PCalculation(ptr<String> const& str) :PCalculations(new Calculation<T>(str)) {}
			//传入参数并获取结果
			const T& GetResult(T *args = 0) { return Ptr()->GetResult(args); }
			//传入参数值并获取结果
			inline const T& GetResult(Collection::PArray<T>  const&args) { return Ptr()->GetResult((T const*)args); }
			//获取上一次的计算结果
			inline T const& GetLastResult()const { return Ptr()->lastValue; }
			//获取最近一次的计算结果
			inline T const& GetCurrentResult()const { return Ptr()->GetCurrentResult(); }
			//动态计算并返回结果
			const T&Calculate(OperatorArgs<T>&args) { return Ptr()->Calculate(args); }
			//获取解析数组
			Collection::PArray<ptr<String>> GetCalculationString(OperatorDebugArgs<T>&args) { return Ptr()->GetCalculationString(args); }
			//获取指定参数和步骤的解析结果(level~0)
			Collection::PArray<ptr<String>> GetAnalyzedString(index_t level, index_t deep, T *args) { return Ptr()->GetAnalyzedString(level, deep, args); }
		};
		/**
		@brief 动态函数算子，默认优先级100*/
		template<class T>class IDynamicFunctionOperator :public IOperatorObject<T>
		{
			mutable std::map<index_t, size_t> mp;
			mutable Mutex mt;
			typedef PDynamicFunctionOperator<T> xDynamicFunctionOperator;
		public:
			IMETHODS(PDynamicFunctionOperator<T>,IDynamicFunctionOperator<T>)
			/**
			@brief 从一个函数(变量)表达式字符串中获取函数(变量)名，并从args参数中写出参数数组，在pt中输出函数类型：0单目，1后置单目，2函数，3后置函数，4双目，5算子*/
			static ptr<String> GetNameFromExpression(ptr<String> const& str, Collection::PArray<ptr<String>>& args, int*pt)
			{
				if (str.IsNullOrEmpty())Throw<PMathException >(L"表达式为空");
				index_t index = str.IndexOf(L'=');//=前为函数声明，后为表达式
				if (index < 0)Throw<PMathException >(L"表达式非法");
				ptr<String> name = str.Substring(0, index);//.SelfReplace(L" ",ptr<String>::GetEmpty());//移除空格
				if (name.IsNullOrEmpty())Throw<PMathException >(L"表达式无名称");
				//支持以下类型声明
				//函数：f(a,b)=
				//后置函数：(a,b)f=
				//双目：(a)f(b)=
				//算子：f=
				//前置单目：f(a=
				//后置单目：a)f=
				index_t lindex = name.LastIndexOf(L'(');//左括号位置
				index_t rindex = name.IndexOf(L')');//右括号位置
				if (lindex == -1 && rindex == -1)//算子
				{
					if (name.IndexOf(L",") >= 0)Throw<PMathException >(L"函数定义\"" + name + L"\"不明确，看起来像算子但具有多个参数");
					if (pt)*pt = 5;
					return name;
				}
				if (lindex == -1)//后置单目或函数
				{
					ptr<String> s = name.Substring(rindex + 1);
					if (s.IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"不明确，看起来像后置单目或函数，但是 \")\" 后面找不到函数名");
					if (s.IndexOf(L",") >= 0)Throw<PMathException >(L"函数定义\"" + name + L"\"不明确，看起来像后置单目或函数，但函数名中包含\",\"");
					if (rindex != 0)
					{
						args = name.Substring(0, rindex).Split(L",");
						for (index_t i = 0; i < args.GetLength(); ++i)
							if (args[i].IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"存在无参数名参数");
							else if (args[i][0] <= L'9'&&args[i][0] >= L'0')
								Throw<PMathException >(L"函数定义\"" + name + L"\"存在参数名以数字开头");
					}
					else args.SetToNull();
					if (pt)*pt = (!args.IsNull() && args.GetLength() == 1) ? 1 : 3;
					return s;
				}
				if (rindex == -1)
				{
					ptr<String> s = name.Substring(0, lindex);
					if (s.IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"不明确，看起来像单目或函数，但是 \"(\" 前面找不到函数名");
					if (s.IndexOf(L",") >= 0)Throw<PMathException >(L"函数定义\"" + name + L"\"不明确，看起来像单目或函数，但函数名中包含\",\"");
					if (lindex + 1 != name.GetLength())
					{
						args = name.Substring(lindex + 1).Split(L",");
						for (index_t i = 0; i < args.GetLength(); ++i)
							if (args[i].IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"存在无参数名参数");
							else if (args[i][0] <= L'9'&&args[i][0] >= L'0')
								Throw<PMathException >(L"函数定义\"" + name + L"\"存在参数名以数字开头");
					}
					else args.SetToNull();
					if (pt)*pt = (!args.IsNull() && args.GetLength() == 1) ? 0 : 2;
					return s;
				}
				if (lindex < rindex)//函数或后置函数
				{
					ptr<String> s1 = name.Substring(0, lindex);
					ptr<String> s2 = name.Substring(rindex+1);
					if (s1.IsNull() && s2.IsNull())
						Throw<PMathException >(L"函数定义\"" + name + L"\"非法");
					if (rindex != lindex + 1)
					{
						args = name.Substring(lindex + 1, rindex - lindex - 1).Split(L",");
						for (index_t i = 0; i < args.GetLength(); ++i)
							if (args[i].IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"存在无参数名参数");
							else if (args[i][0] <= L'9'&&args[i][0] >= L'0')
								Throw<PMathException >(L"函数定义\"" + name + L"\"存在参数名以数字开头");
					}
					else args.SetToNull();
					if (pt)*pt = s1.IsNullOrEmpty() ? 3 : 2;
					return s1.IsNullOrEmpty() ? s2 : s1;
				}
				//双目
				ptr<String> s = name.Substring(rindex + 1, lindex - rindex - 1);
				if (s.IsNullOrEmpty())
					Throw<PMathException >(L"双目运算符定义\"" + name + L"\"无名称");
				args = Collection::PArray<ptr<String> >(2);
				args[0] = name[0] == L'(' ? name.Substring(1, rindex - 1) : name.Substring(0, rindex);
				args[1] = name[name.GetLength() - 1] == L')' ? name.Substring(lindex + 1, name.GetLength() - lindex - 2) : name.Substring(lindex + 1);
				for (index_t i = 0; i < args.GetLength(); ++i)
					if (args[i].IsNullOrEmpty())Throw<PMathException >(L"函数定义\"" + name + L"\"存在无参数名参数");
					else if (args[i][0] <= L'9'&&args[i][0] >= L'0')
						Throw<PMathException >(L"函数定义\"" + name + L"\"存在参数名以数字开头");
				if (pt)*pt = 4;
				return s;
			}
			/**
			@brief 计算式*/
			mutable PCalculation<T> calculation;
			/**
			@brief 最大递归调用次数,默认为1000*/
			static size_t MaxReCall;
			/**
			@brief 判断该对象是否引用了某个运算符列表*/
			virtual bool IsUsing(BigObject const* t)const
			{
				if (calculation.IsNull())return false;
				return  t == calculation.GetOperatorTable().GetPtr();
			}
			/**
			@brief 使用函数式子和参数进行构造，式子比如f(x)=a+bx*/
			IDynamicFunctionOperator(ptr<String> const& funstr, unsigned short lv = 100)
				:IOperatorObject<T>(ptr<String>::GetEmpty(), lv, 65535, IOperatorObjects::CalculatePassProperty)
			{
				SetFunction(funstr);
			}
			void SetFunction(ptr<String> const& str)
			{
				Collection::PArray<ptr<String>>args;
				int tp;
				IOperatorObjects::name = GetNameFromExpression(str, args, &tp);
				switch (tp)
				{
				case 1:IOperatorObjects::prop |= IOperatorObjects::CalculateBackProperty;
				case 0:IOperatorObjects::prop |= IOperatorObjects::SupportSingleProperty;
					break;
				case 3:IOperatorObjects::prop |= IOperatorObjects::CalculateBackProperty;
				case 2:IOperatorObjects::prop |= IOperatorObjects::SupportFunctionProperty;
					break;
				case 4:IOperatorObjects::prop |= IOperatorObjects::SupportDoubleProperty;
					break;
				case 5:IOperatorObjects::prop |= IOperatorObjects::SupportAloneProperty;
				default:
					break;
				}
				IOperatorObjects::count = args.IsNull() ? 0 : (unsigned short)args.GetLength();
				if (calculation.IsNull())calculation = new Calculation<T>(str.Substring(str.IndexOf(L"=") + 1));
				else calculation.SetString(str.Substring(str.IndexOf(L"=") + 1));
				calculation.ClearVariable();
				if (!args.IsNull())calculation.AddVariable(args);
			}
			/**
			@brief 传入参数获取多个解析层级的解析结果*/
			virtual T DebugCalculate(OperatorDebugArgs<T> &args, Collection::PArray<ptr<String>>&result)
			{
				if (IOperatorObjects::count != 65535 && IOperatorObjects::count != args.GetLength())Throw<PMathException >(L"传入函数\"" + IOperatorObjects::name + L"\"的参数数量不对");
				if (args.level == 0)return IOperatorObject<T>::DebugCalculate(args, result);
				if (args.deep < 0)//不进行展开
				{
					T val = IOperatorObject<T>::TestCalculate(args);
					index_t mlen = args.GetMaxArrCount();
					if (args.level >= 0)mlen = 1;
					else if (mlen > -args.level)mlen = 1 - args.level;
					Collection::PArray<ptr<String>> arr(mlen);
					if (mlen == 0) { result = arr; return val; }
					for (index_t i = 0; i < mlen; ++i)
						arr[i] = L"(" + IOperatorObject<T>::GetExpression(args, -i) + L")";
					result = arr;
					return val;
				}
				result = calculation.GetCalculationString(args);
				if (!result.IsNull() && result.GetLength()>0)
					result[0] = L"(" + IOperatorObject<T>::GetExpression(args) + L")";
				return calculation.GetCurrentResult();
			}
			/**
			@brief 获取运算结果，参数是一组函数对象，用以动态计算数值*/
			virtual T Calculate(OperatorArgs<T>&args)const
			{
				if (IOperatorObjects::StopCalculating)Throw<PMathException >(L"运算过程被终止");
				if (IOperatorObjects::count != 65535 && IOperatorObjects::count != args.GetLength())Throw<PMathException >(L"传入函数\"" + IOperatorObjects::name + L"\"的参数数量不对");
				mt.Lock();
				if (mp.find(mt.GetID()) == mp.end())
					mp[mt.GetID()] = 1;
				else if (++mp[mt.GetID()] > MaxReCall) { mp[mt.GetID()] = 0; mt.UnLock(); Throw<PMathException >(IOperatorObjects::name + L"的递归调用次数超过" + ptr<String>(MaxReCall) + L"次"); }
				mt.UnLock();
				const T&rt = calculation.Calculate(args);
				mt.Lock();
				--mp[mt.GetID()];
				mt.UnLock();
				return rt;
			}
			/**
			@brief 清理*/
			virtual void Clear()
			{
				PCalculations s;
				BigObject::SwapReference(s, calculation);
				if (!s.IsNull()) { s.Clear(); }
			}
			/**
			@brief 设置运算符列表*/
			void SetOperatorTable(POperatorTable<T> tbl)
			{
				calculation.SetOperatorTable(tbl);
			}
		};
		template<class T>size_t IDynamicFunctionOperator<T>::MaxReCall = 200;
		/**
		@brief 动态函数算子的代理*/
		template<class T>struct PDynamicFunctionOperator :POperatorObject<T>
		{
			//通用方法
			PMETHODS(PDynamicFunctionOperator<T>, IDynamicFunctionOperator<T>);
			/**
			@brief 使用函数式子和参数进行构造，式子比如f(x)=a+bx*/
			PDynamicFunctionOperator(ptr<String> const& funstr, short lv = 100) :POperatorObject<T>(new IDynamicFunctionOperator<T>(funstr, lv)) {}
			/**
			@brief 获取内部计算式的引用*/
			PCalculation<T> & GetCalculationsRef() { return Ptr()->calculation; }
			/**
			@brief 设置运算符列表*/
			void SetOperatorTable(POperatorTable<T> tbl)const { return Ptr()->SetOperatorTable(tbl); }
			/**
			brief 使用指定的函数式子来设置函数*/
			void SetFunction(ptr<String> const& funstr)
			{
				Ptr()->SetFunction(funstr);
			}
			//传入参数并计算
			T Calculate(OperatorArgs<T>&args)const
			{
				return Ptr()->Calculate(args);
			}
			//析构函数
			~PDynamicFunctionOperator() {}
		};

		//默认构造函数
		template<class T>Calculation<T>::Calculation() {}
		template<class T>Calculation<T>::Calculation(ptr<String> const&str)
		{
			//设置算式
			SetString(str);
		}
		//获取指定参数和步骤的解析结果(level~0)
		template<class T>Collection::PArray<ptr<String>> Calculation<T>::GetAnalyzedString(index_t level, index_t deep, T *pargs)
		{
			OperatorDebugArgs<T> args(NULL, deep, level);
			for (index_t i = 0; i < (variables.IsNull() ? 0 : variables.GetLength()); ++i)
			{
				for (index_t j = 0; j < variables.GetLength(); ++j)
					if (variables[j].second == i) {
						args.AddValue(pargs[i], variables[j].first); break;
					}
			}
			return GetCalculationString(args);
		}
	}
}
#endif
