#include "stdafx.h"
#ifdef LK_MATH_CALCULATION_
namespace LK
{
	namespace Math
	{
		/**
		@brief 获取类型标记的引用*/
		signed char& PCalculationNodes::GetFlagRef()const{ return Ptr()->flag; }
		/**
		@brief 获取序号的引用*/
		int& PCalculationNodes::GetIndexRef()const{ return Ptr()->index; }
		/**
		@brief 获取是否已计算标记的引用*/
		bool& PCalculationNodes::GetHasCalculatedRef()const{ return Ptr()->hasCalculated; }
		/**
		@brief 获取外部节点的引用*/
		PCalculationNodes& PCalculationNodes::GetOutNodeRef()const{ return Ptr()->outNode; }
		/**
		@brief 获取内部节点的引用*/
		PCalculationNodes& PCalculationNodes::GetInnerNodeRef()const{ return Ptr()->innerNode; }
		/**
		@brief 获取前一个节点*/
		PCalculationNodes& PCalculationNodes::GetHeadNodeRef()const{ return Ptr()->headNode; }
		/**
		@brief 获取后一个节点的引用*/
		PCalculationNodes& PCalculationNodes::GetNextNodeRef()const{ return Ptr()->nextNode; }
		/**
		@brief 获取与内部节点计算的运算符*/
		POperatorObjects& PCalculationNodes::GetInnerOperatorRef()const{ return Ptr()->innerOperator; }
		/**
		@brief 设置内部节点，并将该内部节点的外节点设置为本节点*/
		void PCalculationNodes::SetInnerNode(PCalculationNodes const&n)const { return Ptr()->SetInnerNode(n); }
		/**
		@brief 设置前节点，并将该内部节点的后节点设置为本节点*/
		void PCalculationNodes::SetHeadNode(PCalculationNodes const&n)const { return Ptr()->SetHeadNode(n); }
		/**
		@brief 设置后节点，并将该内部节点的前节点设置为本节点*/
		void PCalculationNodes::SetNextNode(PCalculationNodes const&n)const { return Ptr()->SetNextNode(n); }
		/**
		@brief 设置外节点，并将该内部节点的内节点设置为本节点*/
		void PCalculationNodes::SetOutNode(PCalculationNodes const&n)const { return Ptr()->SetOutNode(n); }
		/**
		@brief 清理节点关系*/
		void PCalculationNodes::Clear() { Ptr()->Clear(); }
		/**
		@brief 获取内一层节点数*/
		int PCalculationNodes::GetInnerNodeCount()const { return Ptr()->GetInnerNodeCount(); }
		/**
		@brief 获取可能的运算符重载*/
		Collection::PArray<POperatorObjects>& PCalculationNodes::GetOverloadsRef()const
		{
			return Ptr()->overloads;
		}
		/**
		@brief 检查函数可能重载中的单独算子*/
		POperatorObjects PCalculationNodes::OverloadAlone()const{return Ptr()->OverloadAlone();}
		/**
		@brief 检查函数可能重载中的单目运算符*/
		POperatorObjects PCalculationNodes::OverloadSingle()const{return Ptr()->OverloadSingle();}
		/**
		@brief 检查函数可能重载中后置单目运算符*/
		POperatorObjects PCalculationNodes::OverloadBackSingle()const{return Ptr()->OverloadBackSingle();}
		/**
		@brief 检查函数可能重载中双目运算符*/
		POperatorObjects PCalculationNodes::OverloadDouble()const{return Ptr()->OverloadDouble();}
		/**
		@brief 检查函数可能重载中指定参数数量函数运算符*/
		POperatorObjects PCalculationNodes::OverloadFunction(index_t c)const{return Ptr()->OverloadFunction(c);}
		/**
		@brief 检查函数可能重载中指定参数数量后置函数运算符*/
		POperatorObjects PCalculationNodes::OverloadBackFunction(index_t c)const{return Ptr()->OverloadBackFunction(c);}
		/**
		@brief 使用指定的字符串来设置数值*/
		void PCalculationNodes::SetValueWithString(std::string str) { Ptr()->SetValueWithString(str); }
		//设置当前算式字符串
		void PCalculations::SetString(std::string const& str) { Ptr()->SetString(str); }
		//获取分析后的字符串
		std::string PCalculations::GetAnalyzedString() { return Ptr()->GetAnalyzedString(); }
		//设置参数(如果原来有相同的名称，那原来的序号将会被替换)
		void PCalculations::AddVariable(Collection::PArray<std::string >argStrings) { if (!argStrings.IsNull())Ptr()->AddVariable(argStrings); }
		//移除指定的变量名
		void PCalculations::RemoveVariable(std::string & name) { Ptr()->RemoveVariable(name); }
		//获取变量名数组
		Collection::PArray<std::string > PCalculations::GetVariableArray()const { return Ptr()->GetVariableArray(); }
		//移除所有变量名
		void PCalculations::ClearVariable() { Ptr()->ClearVariable(); }
		//获取当前函数列表
		POperatorTables PCalculations::GetOperatorTable()const { return Ptr()->GetOperatorTable(); }
		//设置函数列表
		void PCalculations::SetOperatorTable(POperatorTables const&tbl) { Ptr()->SetOperatorTable(tbl); }
		//移除解析结果
		void PCalculations::ClearAnalyse() { Ptr()->ClearAnalyse(); }
		//获取树形结点中与指定operator相等（BigObject::ReferenceEqual）的运算符数量,如果baseNode为NULL则返回0
		index_t PCalculations::GetCountOfOperatorInNodes(POperatorObjects const& x)const {
			return Ptr()->GetCountOfOperatorInNodes(x);
		}
		/**
		@brief 设置内部节点*/
		void ICalculationNodes::SetInnerNode(PCalculationNodes n)
		{
			if (n.GetPtr() == this)Throw<PMathException >(L"不能将自身设置为内部节点");
			PCalculationNodes t = this;//防止操作过程中被release
			if (innerNode.GetPtr() != n.GetPtr()) {
				if (!innerNode.IsNull())innerNode.GetOutNodeRef().SetToNull();
				innerNode = n;
			}
			if (!n.IsNull()&& n.GetOutNodeRef().GetPtr()!=this)
			{
				if(!n.GetOutNodeRef().IsNull())n.GetOutNodeRef().GetInnerNodeRef().SetToNull();
				n.GetOutNodeRef() = this;
			}
		}
		/**
		@brief 设置前节点*/
		void ICalculationNodes::SetHeadNode(PCalculationNodes n)
		{
			if (n.GetPtr() == this)Throw<PMathException >(L"不能将自身设置为前节点");
			PCalculationNodes t = this;//防止操作过程中被release
			if (headNode.GetPtr() != n.GetPtr()) {
				if (!headNode.IsNull())headNode.GetNextNodeRef().SetToNull();
				headNode = n;
			}
			if (!n.IsNull() && n.GetNextNodeRef().GetPtr() != this)
			{
				if (!n.GetNextNodeRef().IsNull())n.GetNextNodeRef().GetHeadNodeRef().SetToNull();
				n.GetNextNodeRef() = this;
			}
		}
		/**
		@brief 设置后节点*/
		void ICalculationNodes::SetNextNode(PCalculationNodes n)
		{
			if (n.GetPtr() == this)Throw<PMathException >(L"不能将自身设置为后节点");
			PCalculationNodes t = this;//防止操作过程中被release
			if (nextNode.GetPtr() != n.GetPtr()) {
				if (!nextNode.IsNull())nextNode.GetHeadNodeRef().SetToNull();
				nextNode = n;
			}
			if (!n.IsNull() && n.GetHeadNodeRef().GetPtr() != this)
			{
				if (!n.GetHeadNodeRef().IsNull())n.GetHeadNodeRef().GetNextNodeRef().SetToNull();
				n.GetHeadNodeRef() = this;
			}
		}
		/**
		@brief 设置外节点*/
		void ICalculationNodes::SetOutNode(PCalculationNodes n)
		{
			if (n.GetPtr() == this)Throw<PMathException >(L"不能将自身设置为外节点");
			PCalculationNodes t = this;//防止操作过程中被release
			if (outNode.GetPtr() != n.GetPtr()) {
				if (!outNode.IsNull())outNode.GetInnerNodeRef().SetToNull();
				outNode = n;
			}
			if (!n.IsNull() && n.GetInnerNodeRef().GetPtr() != this)
			{
				if (!n.GetInnerNodeRef().IsNull())n.GetInnerNodeRef().GetOutNodeRef().SetToNull();
				n.GetInnerNodeRef() = this;
			}
		}
		//设置参数(如果原来有相同的名称，那原来的序号将会被替换)
		void ICalculation::AddVariable(Collection::PArray<std::string >argStrings)
		{
			for (index_t i = 0; i < argStrings.GetLength(); ++i)
				AddVariable(argStrings[i], i);
		}
		//获取变量名数组
		Collection::PArray<std::string > ICalculation::GetVariableArray()const
		{
			if (variables.IsNull())return Collection::PArray<std::string >((index_t)0);
			Collection::PArray<std::string >arr(variables.size());
			for (index_t i = 0; i < variables.size(); ++i)
				arr[variables[i].second] = variables[i].first;
			return arr;
		}
		//添加参数与序号的关联(如果原来有相同的名称，那原来的序号将会被替换)
		void ICalculation::AddVariable(const std::string & name, index_t index)
		{
			ClearAnalyse();
			if (variables.IsNull())variables = Collection::PArray<std::pair<std::string , index_t> >((index_t)0);
			index_t i = 0;
			while (i < variables.GetLength())
			{
				if (variables[i].first.size() < name.size())
				{
					variables.Insert(i, std::pair<std::string , index_t>(name, index));
					return;
				}
				else if (variables[i].first == name)
				{
					variables[i].second = i;
					return;
				}
				++i;
			}
			variables.Append(std::pair<std::string , index_t>(name, index));
		}
		//移除指定的变量名
		void ICalculation::RemoveVariable(std::string & name)
		{
			if (variables.IsNull())return;
			for (index_t i = 0; i < variables.GetLength(); ++i)
				if (name == variables[i].first)
				{
					ClearAnalyse();
					variables.Remove(i); break;
				}
		}
		//移除所有变量名
		void ICalculation::ClearVariable()
		{
			if (!variables.IsNull())
			{
				variables.Remove(0);
				ClearAnalyse();
			}
		}
		void ICalculation::Analyse(PCalculationNodes& node)
		{
			//先预解析
			if (baseNode.IsNull())PreAnalyse();
			if (baseNode.IsNull())return;
			PCalculationNodes n = node;
			//循环确定性解析
			while (SureAnalyse(node));
			//双目节点紧缩
			n = node;
			PCalculationNodes d2, d3;
			if (!n.GetNextNodeRef().IsNull())
			{
				if (node.GetFlagRef() == -1 && node.GetInnerOperatorRef().IsDoubleSupport() && n.GetInnerNodeRef().IsNull())//第一个结点不允许是未处理双目
				{
					Throw<PMathException >(L"双目运算符：\"" + node->ToString() + L"\"前面无内容。");
					/*
					node.GetHeadNodeRef()=(new VCalculationNode<T>(4));
					node.GetHeadNodeRef().GetNextNodeRef()=node;
					node.GetHeadNodeRef().GetOutNodeRef()=(node.GetOutNodeRef());
					if (!node.GetOutNodeRef().IsNull())node.GetOutNodeRef().GetInnerNodeRef()=(node.GetHeadNodeRef());
					node.GetOutNodeRef().SetToNull();
					node = node.GetHeadNodeRef();
					*/
				}
				for (;;)//循环求解
				{
					if (n.IsNull() || (n.GetFlagRef() == -1 && n.GetInnerOperatorRef().IsDoubleSupport() && n.GetInnerNodeRef().IsNull()))//检测到未处理的双目运算符或已结束
					{
						if (!n.IsNull())
						{
							if (n.GetNextNodeRef().IsNull())
								Throw<PMathException >(L"双目运算符：\"" + n->ToString() + L"\"后面无内容");
							PCalculationNodes m = n.GetNextNodeRef();
							if (m.GetFlagRef() == -1 && m.GetInnerOperatorRef().IsDoubleSupport() && m.GetInnerNodeRef().IsNull())
								Throw<PMathException >(L"连续两个双目运算符：" + n->ToString() + m->ToString());
						}
						if (d3.IsNull())
						{
							if (n.IsNull())//没有双目
								break;
							d3 = n;//记录第一个遇到的双目节点
						}
						else if (d2.IsNull())
						{
							if (n.IsNull())//只有一个双目
							{
								PCalculationNodes out = node.GetOutNodeRef();
								//d3添加两个内部参数节点
								d3.SetInnerNode(MakeCalculationNode(2));
								d3.GetInnerNodeRef().SetNextNode(MakeCalculationNode(2));
								//d3左端内容移入参数1
								d3.GetInnerNodeRef().SetInnerNode(node);
								//断绝与前节点的关系
								d3.SetHeadNode(NULL);
								//d3右端内容移入参数2
								d3.GetInnerNodeRef().GetNextNodeRef().SetInnerNode(d3.GetNextNodeRef());
								d3.SetNextNode(NULL);
								//外部节点与d3建立连接
								d3.SetOutNode(out);
								node = d3;
								break;
							}
							//如果前一个节点优先级大于等于后一个节点，则进行紧缩
							if (d3.GetInnerOperatorRef().GetOperatingLevel() >= n.GetInnerOperatorRef().GetOperatingLevel())
							{
								PCalculationNodes out = node.GetOutNodeRef();
								//d3添加两个内部参数节点
								d3.GetInnerNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetOutNodeRef() = (d3);
								d3.GetInnerNodeRef().GetNextNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetNextNodeRef().GetHeadNodeRef() = (d3.GetInnerNodeRef());
								//d3左端内容移入参数
								d3.GetInnerNodeRef().GetInnerNodeRef() = (node);
								node.GetOutNodeRef() = (d3.GetInnerNodeRef());
								if (!d3.GetHeadNodeRef().IsNull())d3.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								d3.GetHeadNodeRef().SetToNull();
								//d3右端内容移入参数
								d3.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef() = (d3.GetNextNodeRef());
								d3.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								d3.GetNextNodeRef().GetOutNodeRef() = (d3.GetInnerNodeRef().GetNextNodeRef());
								//紧缩到d3
								d3.GetNextNodeRef() = (n);
								if (!n.GetHeadNodeRef().IsNull())n.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								n.GetHeadNodeRef() = (d3);
								//外部节点与d3建立连接
								d3.GetOutNodeRef() = (out);
								if (!d3.GetOutNodeRef().IsNull())d3.GetOutNodeRef().GetInnerNodeRef() = (d3);
								node = d3;
							}
							else
								d2 = d3;
							d3 = n;
						}
						else
						{
							if (n.IsNull())
							{
								//d3添加两个内部参数节点
								d3.GetInnerNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetOutNodeRef() = (d3);
								d3.GetInnerNodeRef().GetNextNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetNextNodeRef().GetHeadNodeRef() = (d3.GetInnerNodeRef());
								//d3左端内容移入参数1
								d3.GetInnerNodeRef().GetInnerNodeRef() = (d2.GetNextNodeRef());
								d2.GetNextNodeRef().GetOutNodeRef() = (d3.GetInnerNodeRef());
								d2.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								d3.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								d3.GetHeadNodeRef() = (d2);
								d2.GetNextNodeRef() = (d3);
								//d3右端内容移入参数2
								d3.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef() = (d3.GetNextNodeRef());
								d3.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								d3.GetNextNodeRef().GetOutNodeRef() = (d3.GetInnerNodeRef().GetNextNodeRef());
								d3.GetNextNodeRef().SetToNull();
								d3 = d2;
								d2 = d2.GetHeadNodeRef();
								while (!d2.IsNull() && (d2.GetFlagRef() != -1 || !d2.GetInnerNodeRef().IsNull() || !d2.GetInnerOperatorRef().IsDoubleSupport()))
									d2 = d2.GetHeadNodeRef();
								continue;
							}
							else if (d3.GetInnerOperatorRef().GetOperatingLevel() > d2.GetInnerOperatorRef().GetOperatingLevel() && d3.GetInnerOperatorRef().GetOperatingLevel() >= n.GetInnerOperatorRef().GetOperatingLevel())//紧缩d3
							{
								//d3添加两个内部参数节点
								d3.GetInnerNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetOutNodeRef() = (d3);
								d3.GetInnerNodeRef().GetNextNodeRef() = MakeCalculationNode(2);
								d3.GetInnerNodeRef().GetNextNodeRef().GetHeadNodeRef() = (d3.GetInnerNodeRef());
								//d3左端内容移入参数1
								d3.GetInnerNodeRef().GetInnerNodeRef() = (d2.GetNextNodeRef());
								d2.GetNextNodeRef().GetOutNodeRef() = (d3.GetInnerNodeRef());
								d2.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								d3.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								d3.GetHeadNodeRef() = (d2);
								d2.GetNextNodeRef() = (d3);
								//d3右端内容移入参数2
								d3.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef() = (d3.GetNextNodeRef());
								d3.GetNextNodeRef().GetOutNodeRef() = (d3.GetInnerNodeRef().GetNextNodeRef());
								d3.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								n.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								n.GetHeadNodeRef() = (d3);
								d3.GetNextNodeRef() = (n);
								d3 = n;
							}
							else if (d2.GetInnerOperatorRef().GetOperatingLevel() >= d3.GetInnerOperatorRef().GetOperatingLevel())
							{
								//d2添加两个内部参数节点
								d2.GetInnerNodeRef() = MakeCalculationNode(2);
								d2.GetInnerNodeRef().GetOutNodeRef() = (d2);
								d2.GetInnerNodeRef().GetNextNodeRef() = MakeCalculationNode(2);
								d2.GetInnerNodeRef().GetNextNodeRef().GetHeadNodeRef() = (d2.GetInnerNodeRef());
								PCalculationNodes out = node.GetOutNodeRef();
								//d2左端内容移入参数1
								d2.GetInnerNodeRef().SetInnerNode(node);
								//断绝与前节点的关系
								d2.SetHeadNode(NULL);
								//外部节点与d2建立连接
								d2.SetOutNode(out);
								node = d2;
								//d2右端内容移入参数2
								d2.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef() = (d2.GetNextNodeRef());
								d2.GetNextNodeRef().GetOutNodeRef() = (d2.GetInnerNodeRef().GetNextNodeRef());
								d2.GetNextNodeRef().GetHeadNodeRef().SetToNull();
								d3.GetHeadNodeRef().GetNextNodeRef().SetToNull();
								d3.GetHeadNodeRef() = (d2);
								d2.GetNextNodeRef() = (d3);
								d2 = d3;
								d3 = n;
							}
							else
							{
								d2 = d3;
								d3 = n;
							}
						}
					}
					n = n.GetNextNodeRef();
				}
			}
			//深层内容的解析（递归）
			n = node;
			while (!n.IsNull())
			{
				if (!n.GetInnerNodeRef().IsNull())Analyse(n.GetInnerNodeRef());
				n = n.GetNextNodeRef();
			}
			//具体内容解析
			n = node;
			while (!n.IsNull())
			{
				switch (n.GetFlagRef())
				{
				case -1://函数
					if (n.GetInnerOperatorRef().IsNull())Throw<PMathException >(L"空函数异常");
					if (n.GetInnerOperatorRef().IsSingleSupport() && n.GetInnerNodeRef().IsNull())
					{
						if (n.GetHeadNodeRef().IsNull() && n.GetInnerOperatorRef().IsCalculateBack())
							Throw<PMathException >(L"后置单目运算符" + n.GetInnerOperatorRef()->ToString() + L"前面无内容");
						if (n.GetNextNodeRef().IsNull() && !n.GetInnerOperatorRef().IsCalculateBack())
							Throw<PMathException >(L"单目运算符" + n.GetInnerOperatorRef()->ToString() + L"后面无内容");
					}
					if (n.GetInnerOperatorRef().IsAloneSupport())goto Five;//算子当做数字处理
																		   //函数和单目部分留到括号、数字部分处理
					break;
				case 0://不确定运算符
				{
					std::string str = L"无法为运算符\"" + n.GetInnerOperatorRef()->ToString() + L"\"确定重载版本，可能是：";
					for (Collection::PArray<POperatorObjects >::iterator i = n.GetOverloadsRef().begin(); i != n.GetOverloadsRef().end(); ++i)
					{
						if (i != n.GetOverloadsRef().begin())str += L" 或 ";
						if (i->IsCalculateBack())str += L"后置";
						else if (i->IsFunctionSupport() || i->IsSingleSupport())str += L"前置";
						if (i->IsFunctionSupport())
						{
							str += L"函数(";
							if (i->GetParametersCount() == 65535)str += L"可变";
							else str += std::string (i->GetParametersCount()) + L"个";
							str += L"参数)";
						}
						if (i->IsSingleSupport())str += L"单目运算符";
						if (i->IsDoubleSupport())str += L"双目运算符";
						if (i->IsAloneSupport())str += L"单独算子";
					}
					Throw<PMathException >(str);
				}
				case 2://逗号
					break;
				case 3://空格
				{
					if (n.Equals(node))
					{
						node.GetOutNodeRef().GetInnerNodeRef() = (n.GetNextNodeRef());
						node = n.GetNextNodeRef();
						if (!node.IsNull())node.GetHeadNodeRef().SetToNull();
						break;
					}
					//移除空格
					if (!n.GetHeadNodeRef().IsNull())n.GetHeadNodeRef().GetNextNodeRef() = (n.GetNextNodeRef());
					if (!n.GetNextNodeRef().IsNull())n.GetNextNodeRef().GetHeadNodeRef() = (n.GetHeadNodeRef());
					n = n.GetHeadNodeRef();
				}
				break;
				case 1://括号
				{
					index_t c = n.GetInnerNodeRef().IsNull() ? 0 : n.GetInnerNodeRef().GetFlagRef() == 2 ? n.GetInnerNodeCount() : 1;//获取参数
																																	 //检查前面的运算符
					POperatorObjects opb;
					if (!n.GetHeadNodeRef().IsNull() &&
						n.GetHeadNodeRef().GetFlagRef() == -1 &&
						n.GetHeadNodeRef().GetInnerNodeRef().IsNull() &&
						!n.GetHeadNodeRef().GetInnerOperatorRef().IsNull() &&
						(n.GetHeadNodeRef().GetInnerOperatorRef().IsFunctionSupport() || n.GetHeadNodeRef().GetInnerOperatorRef().IsSingleSupport()) &&
						!n.GetHeadNodeRef().GetInnerOperatorRef().IsCalculateBack() &&
						(n.GetHeadNodeRef().GetInnerOperatorRef().GetParametersCount() == 65535 || n.GetHeadNodeRef().GetInnerOperatorRef().GetParametersCount() == c))
						opb = n.GetHeadNodeRef().GetInnerOperatorRef();
					//检查后面的运算符
					POperatorObjects ope;
					if (!n.GetNextNodeRef().IsNull() &&
						n.GetNextNodeRef().GetFlagRef() == -1 &&
						n.GetNextNodeRef().GetInnerNodeRef().IsNull() &&
						!n.GetNextNodeRef().GetInnerOperatorRef().IsNull() &&
						(n.GetNextNodeRef().GetInnerOperatorRef().IsFunctionSupport() || n.GetNextNodeRef().GetInnerOperatorRef().IsSingleSupport()) &&
						n.GetNextNodeRef().GetInnerOperatorRef().IsCalculateBack() &&
						(n.GetNextNodeRef().GetInnerOperatorRef().GetParametersCount() == 65535 || n.GetNextNodeRef().GetInnerOperatorRef().GetParametersCount() == c))
						ope = n.GetNextNodeRef().GetInnerOperatorRef();
					if (opb.IsNull() && ope.IsNull())break;
					if (!opb.IsNull() && !ope.IsNull())//优先级比较
					{
						if (opb.GetOperatingLevel() >= ope.GetOperatingLevel())ope.SetToNull();
						else opb.SetToNull();
					}
					PCalculationNodes h = n.GetHeadNodeRef();
					PCalculationNodes b = n.GetNextNodeRef();
					PCalculationNodes i = n.GetInnerNodeRef();
					PCalculationNodes o = n.GetOutNodeRef();
					if (ope.IsNull())//采用前面的运算符
					{
						h.GetInnerNodeRef() = (i);
						if (!i.IsNull())i.GetOutNodeRef() = (h);
						if (!b.IsNull())b.GetHeadNodeRef() = (h);
						h.GetNextNodeRef() = (b);
						n = h;
					}
					else//采用后面的运算符
					{
						b.GetInnerNodeRef() = (i);
						if (!i.IsNull())i.GetOutNodeRef() = (b);
						b.GetOutNodeRef() = (n.GetOutNodeRef());
						if (!o.IsNull())o.GetInnerNodeRef() = (b);
						if (!h.IsNull())h.GetNextNodeRef() = (b);
						b.GetHeadNodeRef() = (h);
						if (baseNode.Equals(n))baseNode = b;
						n = b;
					}
				}//接下来整体作为数字看待
				case 4:
				case 5:
				{
				Five:;
					for (;;)
					{
						//检查前面的运算符
						POperatorObjects opb;
						if (!n.GetHeadNodeRef().IsNull() &&
							n.GetHeadNodeRef().GetFlagRef() == -1 &&
							n.GetHeadNodeRef().GetInnerNodeRef().IsNull() &&
							!n.GetHeadNodeRef().GetInnerOperatorRef().IsNull() &&
							n.GetHeadNodeRef().GetInnerOperatorRef().IsSingleSupport() &&
							!n.GetHeadNodeRef().GetInnerOperatorRef().IsCalculateBack())
							opb = n.GetHeadNodeRef().GetInnerOperatorRef();
						//检查后面的运算符
						POperatorObjects ope;
						if (!n.GetNextNodeRef().IsNull() &&
							n.GetNextNodeRef().GetFlagRef() == -1 &&
							n.GetNextNodeRef().GetInnerNodeRef().IsNull() &&
							!n.GetNextNodeRef().GetInnerOperatorRef().IsNull() &&
							n.GetNextNodeRef().GetInnerOperatorRef().IsSingleSupport() &&
							n.GetNextNodeRef().GetInnerOperatorRef().IsCalculateBack())
							ope = n.GetNextNodeRef().GetInnerOperatorRef();
						if (opb.IsNull() && ope.IsNull())break;
						if (!opb.IsNull() && !ope.IsNull())//优先级比较
						{
							if (opb.GetOperatingLevel() >= ope.GetOperatingLevel())ope.SetToNull();
							else opb.SetToNull();
						}
						PCalculationNodes h = n.GetHeadNodeRef();
						PCalculationNodes b = n.GetNextNodeRef();
						PCalculationNodes i = n.GetInnerNodeRef();
						PCalculationNodes o = n.GetOutNodeRef();
						if (ope.IsNull())//采用前面的运算符
						{
							if (n.GetFlagRef() == 1)//括号
							{
								h.GetInnerNodeRef() = (i);
								if (!i.IsNull())i.GetOutNodeRef() = (h);
							}
							else
							{
								h.GetInnerNodeRef() = (n);
								n.GetOutNodeRef() = (h);
							}
							if (!b.IsNull())
								b.GetHeadNodeRef() = (h);
							h.GetNextNodeRef() = (b);
							n.GetHeadNodeRef().SetToNull();
							n.GetNextNodeRef().SetToNull();
							n = h;
						}
						else//采用后面的运算符
						{
							if (n.GetFlagRef() == 1)//括号
							{
								b.GetInnerNodeRef() = (i);
								if (!i.IsNull())i.GetOutNodeRef() = (b);
							}
							else
							{
								b.GetInnerNodeRef() = (n);
								n.GetOutNodeRef() = (b);
							}
							b.GetOutNodeRef() = (o);
							if (!o.IsNull())
								o.GetInnerNodeRef() = (b);
							if (!h.IsNull())
								h.GetNextNodeRef() = (b);
							b.GetHeadNodeRef() = (h);
							n.GetHeadNodeRef().SetToNull();
							n.GetNextNodeRef().SetToNull();
							if (baseNode.Equals(n))baseNode = b;
							n = b;
						}
					}
					break;
				}
				default:
					break;
				}
				n = n.GetNextNodeRef();
			}
			//添加缺省双目运算符
			if (node.GetFlagRef() != 2)while (!node.GetNextNodeRef().IsNull())
			{
				//添加默认运算符节点
				n = MakeCalculationNode(-1);
				n.GetInnerOperatorRef() = operatorTable.GetDefaultOperator();
				//n添加两个内部参数节点
				n.GetInnerNodeRef() = MakeCalculationNode(2);
				n.GetInnerNodeRef().GetOutNodeRef() = n;
				n.GetInnerNodeRef().GetNextNodeRef() = MakeCalculationNode(2);
				n.GetInnerNodeRef().GetNextNodeRef().GetHeadNodeRef() = n.GetInnerNodeRef();
				//node外部节点与n建立连接
				n.GetOutNodeRef() = node.GetOutNodeRef();
				//node移入参数1
				n.GetInnerNodeRef().GetInnerNodeRef() = node;
				node.GetOutNodeRef() = (n.GetInnerNodeRef());
				//node右端内容移入参数2
				n.GetInnerNodeRef().GetNextNodeRef().GetInnerNodeRef() = (node.GetNextNodeRef());
				node.GetNextNodeRef().GetHeadNodeRef().SetToNull();
				node.GetNextNodeRef().GetOutNodeRef() = (n.GetInnerNodeRef().GetNextNodeRef());
				n.GetNextNodeRef() = (node.GetNextNodeRef().GetNextNodeRef());
				if (!node.GetNextNodeRef().GetNextNodeRef().IsNull())node.GetNextNodeRef().GetNextNodeRef().GetHeadNodeRef() = (n);
				node.GetNextNodeRef().GetNextNodeRef().SetToNull();
				node.GetNextNodeRef().SetToNull();
				node = n;
				if (!n.GetOutNodeRef().IsNull())n.GetOutNodeRef().GetInnerNodeRef() = n;
			}
			if (!node.GetOutNodeRef().IsNull() && node.GetOutNodeRef().GetFlagRef() == 2 && node.GetFlagRef() == 1 && node.GetNextNodeRef().IsNull())
			{
				if (!node.GetInnerNodeRef().IsNull())node.GetInnerNodeRef().SetOutNode(node.GetOutNodeRef());
			}
		}
		//清理所有内容，若运算符表仅被该对象引用，则自动调用运算符表的Clear()
		void ICalculation::Clear()
		{
			ClearAnalyse();
			if (operatorTable.RefCount == 1)operatorTable.Clear();//未被引用过的运算符表
		}
		//预解析
		void ICalculation::PreAnalyse()
		{
			if (operatorTable.IsNull())operatorTable = MakeOperatorTable();
			std::string  str = sourceString;
			//-1已确定函数，0未确定重载，1括号内容，2逗号，3空格，4数值，5变量
			PCalculationNodes lastnode;
			PCalculationNodes node;
			//分析方法：括号、变量、运算符 各占一个节点
			signed char lflag;//上一次的标记
			for (index_t istr = 0; istr < str.size(); ++istr)
			{
				wchar_t ch = str[istr];
				switch (ch)
				{
				case L'(':
					node = MakeCalculationNode(1);
					AddNodeByFlag(lastnode, node, lflag);
					continue;
				case L')':
					if (baseNode.IsNull()) { lastnode = baseNode = MakeCalculationNode(1); }
					else if (lflag != 1)
					{
						node = lastnode;
						for (;;)
						{
							if (node.GetFlagRef() == 2)//增加最后一个参数
							{
								PCalculationNodes n = MakeCalculationNode(2);
								if (!node.GetNextNodeRef().IsNull()) { node.GetNextNodeRef().GetOutNodeRef() = (n); n.GetInnerNodeRef() = (node.GetNextNodeRef()); }
								node.GetNextNodeRef() = (n); n.GetHeadNodeRef() = (node);
								break;
							}
							if (node.GetHeadNodeRef().IsNull())break;
							node = node.GetHeadNodeRef();
						}
						while (!node.GetHeadNodeRef().IsNull())node = node.GetHeadNodeRef();
						if (node.Equals(baseNode)) { baseNode.GetOutNodeRef() = (MakeCalculationNode(1)); lastnode = baseNode = baseNode.GetOutNodeRef(); baseNode.GetInnerNodeRef() = (node); }
						else lastnode = node.GetOutNodeRef();
					}
					lflag = -2;
					continue;
				case L',':
					node = MakeCalculationNode(2);
					if (baseNode.IsNull() || lflag == 1 || lflag == 2) { AddNodeByFlag(lastnode, node, lflag); }//起始位置添加括号
					else
					{
						PCalculationNodes n = lastnode;
						for (;;)
						{
							PCalculationNodes h = n.GetHeadNodeRef();
							if (h.IsNull())
							{
								if (n.Equals(baseNode)) { node.GetInnerNodeRef() = (n); baseNode = node; n.GetOutNodeRef() = (node); }
								else { node.GetOutNodeRef() = (n.GetOutNodeRef()); n.GetOutNodeRef().GetInnerNodeRef() = (node); node.GetInnerNodeRef() = (n); n.GetOutNodeRef() = (node); }
								break;
							}
							else if (h.GetFlagRef() == 2)
							{
								node.GetHeadNodeRef() = (h); h.GetNextNodeRef() = (node); node.GetInnerNodeRef() = (n); n.GetOutNodeRef() = (node); n.GetHeadNodeRef().SetToNull();
								break;
							}
							n = h;
						}
					}
					lastnode = node;
					lflag = 2;
					continue;
				case L' ':
					if (istr == 0 || str[istr - 1] == L' ')continue;
					node = MakeCalculationNode(3);
					AddNodeByFlag(lastnode, node, lflag);
					continue;
				}
				//如果是数字，则将字符串转换为数值
				if ((ch <= L'9' && ch >= L'0') || ch == L'.')
				{
					node = MakeCalculationNode(4);
					index_t end = istr;
					for (; end < str.size(); ++end)
					{
						if (str[end] == L'+' || str[end] == L'-')
						{
							if ((str[end - 1] <= L'9' && str[end - 1] >= L'0' && str[end - 1] != L'e') || str[end - 1] == L'.')break;
						}
						else if (str[end] != L'e' && str[end] != L'.' && (str[end]<L'0' || str[end]>L'9'))
							break;
					}
					node.SetValueWithString(str.Substring(istr, end - istr));
					istr = end - 1;
					AddNodeByFlag(lastnode, node, lflag);
					continue;
				}
				node = MakeCalculationNode(5);
				//变量优先识别
				Collection::PArray<std::pair<std::string , index_t> >::iterator it;
				Collection::PArray<std::pair<std::string , index_t> >::iterator itend;
				if (!variables.IsNull() && variables.GetLength() != 0)
				{
					it = variables.begin();
					itend = variables.end();
					while (it != itend)
					{
						if (ch == it->first[0])
						{
							index_t j = 0;
							LPCWSTR pstr = it->first.ToUnicode();
							while (pstr[j] && str[istr + j] && pstr[j] == str[istr + j])++j;
							if (pstr[j] != 0)continue;
							node.GetIndexRef() = int(it->second);
							AddNodeByFlag(lastnode, node, lflag);
							istr += j - 1;
							break;
						}
						++it;
					}
				}
				if (!variables.IsNull() && it != itend)continue;
				//自定义运算符或常量
				node.GetFlagRef() = (0);
				index_t i = 0;
				for (; i < operatorTable.GetOperatorTableArrayRef().GetLength(); ++i)
				{
					if (ch == (operatorTable.GetOperatorTableArrayRef()[i][0].GetName())[0])
					{
						index_t j = 0;
						LPCWSTR pstr = operatorTable.GetOperatorTableArrayRef()[i][0].GetName().ToUnicode();
						while (pstr[j] && str[istr + j] && pstr[j] == str[istr + j])++j;
						if (pstr[j] != 0)continue;
						node.GetInnerOperatorRef() = (operatorTable.GetOperatorTableArrayRef()[i][0]);//暂时选择第一个重载版本
						if (operatorTable.GetOperatorTableArrayRef()[i].GetLength() == 1)
						{
							node.GetIndexRef() = (-1);//如果只有一个版本则直接标记为(-1)已确定，否则标记函数索引号
							node.GetFlagRef() = (-1);
						}
						else
						{
							node.GetIndexRef() = int(i);
							node.GetOverloadsRef() = (operatorTable.GetOperatorTableArrayRef()[i]->Clone());
						}
						AddNodeByFlag(lastnode, node, lflag);
						istr += j - 1;
						break;
					}
				}
				if (i < operatorTable.GetOperatorTableArrayRef().GetLength())continue;
				if (i == operatorTable.GetOperatorTableArrayRef().GetLength())
				{
					ClearAnalyse();
					Throw<PMathException >(L"解析遇到未知变量名或操作符" + str.Remove(0, istr));
				}
			}
			/*
			if (!lastnode.IsNull()) {
			node = lastnode;
			for (;;)
			{
			if (node.GetFlagRef() == 2)//增加最后一个参数
			{
			XCalculationNode<T> n = new VCalculationNode<T>(2);
			if (!node.GetNextNodeRef().IsNull()) { node.GetNextNodeRef().GetOutNodeRef()=(n); n.GetInnerNodeRef()=(node.GetNextNodeRef()); }
			node.GetNextNodeRef()=(n); n.GetHeadNodeRef()=(node);
			break;
			}
			if (node.GetHeadNodeRef().IsNull())
			{
			break;
			}
			else node = node.GetHeadNodeRef();
			}
			}
			*/
			//移除多余括号
			RemoveLR(baseNode);
		}
		//单个节点的确定解析
		bool ICalculation::MakeSure(PCalculationNodes& node)
		{
			if (node.IsNull())return false;
			Collection::PArray<POperatorObjects >arr = node.GetOverloadsRef();
			if (arr.IsNull())return false;
			index_t lastlen = arr.GetLength();
			/*可能出现内容：-1已确定函数，0未确定重载，1括号内容，3空格，4数值，5变量*/
			/*重载变化范围：算子、单目（后置）、函数（后置）、双目*/
			POperatorObjects s;
			index_t cnts[6] = { -1,-1,-1,-1,-1,-1 };//-1表示未进行数量统计，0：前置单目，1：后置单目，2：前置函数、3：后值函数、4：双目、5：算子
			MakeSureDouble(node, arr, s, cnts);
			MakeSureSingle(node, arr, s, cnts);
			MakeSureSingleBack(node, arr, s, cnts);
			MakeSureFun(node, arr, s, cnts);
			MakeSureFunBack(node, arr, s, cnts);
			MakeSureAlone(node, arr, s, cnts);
			if (s.IsNull() && !node.GetOverloadsRef().IsNull() && node.GetOverloadsRef().GetLength() == 1)s = node.GetOverloadsRef().FirstElement();
			if (!s.IsNull())//得到确认
			{
				node.GetFlagRef() = (-1);
				node.GetInnerOperatorRef() = (s);
				node.GetOverloadsRef() = (Collection::PArray<POperatorObjects >());
				return true;
			}
			if (node.GetOverloadsRef().IsNull())Throw<PMathException >(L"无法为运算符" + operatorTable.GetOperatorTableArrayRef()[node.GetIndexRef()][0]->ToString() + L"选择重载版本");
			return lastlen != arr.GetLength();
		}

		//确定性解析
		bool ICalculation::SureAnalyse(PCalculationNodes& node)
		{
			PCalculationNodes *p = &node;
			bool hasChanged = false;
			while (!p->IsNull())
			{
				if (p->GetFlagRef() == 0)
					hasChanged = MakeSure(*p) || hasChanged;
				if ((p->GetFlagRef() == 4 || p->GetFlagRef() == 5 || (p->GetFlagRef() == -1 && p->GetInnerOperatorRef().IsAloneSupport())))
				{//变量与算子之间无空格时优先使用缺省运算符
					PCalculationNodes nn = *p;
					while (!nn.GetNextNodeRef().IsNull() && (nn.GetNextNodeRef().GetFlagRef() == 4 || nn.GetNextNodeRef().GetFlagRef() == 5 || (nn.GetNextNodeRef().GetFlagRef() == -1 && nn.GetNextNodeRef().GetInnerOperatorRef().IsAloneSupport())))
						nn = nn.GetNextNodeRef();
					if (p == &node && nn.GetNextNodeRef().IsNull()) { break; }//该层全是顺序双目
					if (!nn.Equals(*p)) {//跳过
						hasChanged = true;
						PCalculationNodes n = *p;
						//添加括号
						PCalculationNodes t = MakeCalculationNode(1);
						//n外部节点与t建立连接
						t.SetOutNode(n.GetOutNodeRef());
						//n前节点与t建立连接
						t.SetHeadNode(n.GetHeadNodeRef());
						//n移入参数1
						t.SetInnerNode(n);
						//nn右端内容衔接
						t.SetNextNode(nn.GetNextNodeRef());
						*p = t;
					}
				}
				p = &p->GetNextNodeRef();
			}
			return hasChanged;
		}
		//运算符重载中的前置单目运算符肯定分析
		void ICalculation::MakeSureSingle(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			if (!h.IsNull() && h.GetFlagRef() == 3)h = h.GetHeadNodeRef();//忽略空格
			PCalculationNodes n = node.GetNextNodeRef();
			if (!n.IsNull() && n.GetFlagRef() == 3)n = n.GetNextNodeRef();//忽略空格
			index_t lc = cnts[0];
			cnts[0] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择，只可能拥有一个前置单目
			{
				if (arr[i].IsSingleSupport() && !arr[i].IsCalculateBack())//前置单目
				{
					cnts[0] = 1;
					POperatorObjects op = arr[i];
					//肯定是单目的情况：
					//1.后面是数字、变量、算子，而且前面是空、非后置
					//2.后面是括号而且前置函数可能数为0，而且前面是空、非后置
					if (!n.IsNull() && ((n.GetFlagRef() == 4 || n.GetFlagRef() == 5 || (n.GetFlagRef() == -1 && n.GetInnerOperatorRef().IsAloneSupport()))
						|| (n.GetFlagRef() == 1 && cnts[2] == 0)
						) && (h.IsNull() || h.GetFlagRef()==2 || (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && !h.GetInnerOperatorRef().IsCalculateBack())))
						s = op;
					//不可能的情况：后面是后置函数、后置单目、双目、空格
					else if (n.IsNull() || n.GetFlagRef() == 2 || (n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && n.GetInnerOperatorRef().IsCalculateBack() && !n.GetInnerOperatorRef().IsAloneSupport())) { arr.RemoveAt(i); cnts[0] = 0; }
					break;
				}
			}
			if (s.IsNull() && lc != cnts[0])//关联解析
			{
				if (cnts[4] != 0)MakeSureDouble(node, arr, s, cnts);
			}
		}
		//运算符重载中的后置单目运算符肯定分析
		void ICalculation::MakeSureSingleBack(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			if (!h.IsNull() && h.GetFlagRef() == 3)h = h.GetHeadNodeRef();//忽略空格
			PCalculationNodes n = node.GetNextNodeRef();
			if (!n.IsNull() && n.GetFlagRef() == 3)n = n.GetNextNodeRef();//忽略空格
			index_t lc = cnts[1];
			cnts[1] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择,只可能有一个后置单目
			{
				if (arr[i].IsSingleSupport() && arr[i].IsCalculateBack())
				{
					cnts[1] = 1;
					POperatorObjects op = arr[i];
					//肯定是后置单目的情况：
					//1.前面是数字、变量、算子，而且后面是空、后置
					//2.前面是括号，而且其后置函数无匹配，后面是空、后置
					if (!h.IsNull() && ((h.GetFlagRef() == 4 || h.GetFlagRef() == 5 || (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && h.GetInnerOperatorRef().IsAloneSupport()))
						|| (h.GetFlagRef() == 1 && cnts[3] == 0))
						&& (n.IsNull() || (n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && n.GetInnerOperatorRef().IsCalculateBack())))
						s = op;
					//不可能的情况：前面是函数、单目、双目、空
					else if (h.IsNull() || (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && !h.GetInnerOperatorRef().IsCalculateBack() && !h.GetInnerOperatorRef().IsAloneSupport())) { arr.RemoveAt(i); cnts[1] = 0; }
					break;
				}
			}
			if (s.IsNull() && lc != cnts[1])//关联解析
			{
				if (cnts[4] != 0)MakeSureDouble(node, arr, s, cnts);
			}
		}
		//运算符重载中的前置函数运算符肯定分析
		void ICalculation::MakeSureFun(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			if (!h.IsNull() && h.GetFlagRef() == 3)h = h.GetHeadNodeRef();//忽略空格
			PCalculationNodes n = node.GetNextNodeRef();
			if (!n.IsNull() && n.GetFlagRef() == 3)n = n.GetNextNodeRef();//忽略空格
			index_t paramc = -1;
			if (!n.IsNull() && n.GetFlagRef() == 1)
			{
				if (n.GetInnerNodeRef().IsNull())paramc = 0;
				else if (n.GetInnerNodeRef().GetFlagRef() == 2)paramc = n.GetInnerNodeCount();
				else paramc = 1;
			}
			index_t lc = cnts[2];
			cnts[2] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择
			{
				if (arr[i].IsFunctionSupport() && !arr[i].IsCalculateBack())//前置函数
				{
					++cnts[2];
					POperatorObjects op = arr[i];
					index_t c = op.GetParametersCount();
					//排除的情况：后面是不是括号，或者参数数量对不上
					if (paramc == -1 || (c != 65535 && c != paramc)) { arr.RemoveAt(i--); --cnts[2]; }
					//肯定是前置函数的情况：后面是括号，而且参数数量匹配，前面是空、双目、前置
					else if (h.IsNull() || (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && (h.GetInnerOperatorRef().IsDoubleSupport() || (!h.GetInnerOperatorRef().IsCalculateBack() && (h.GetInnerOperatorRef().IsSingleSupport() || h.GetInnerOperatorRef().IsFunctionSupport())))))
					{
						s = op;
					}
				}
			}
			if (s.IsNull() && lc != cnts[2])//关联解析
			{
				if (cnts[0] != 0)MakeSureSingle(node, arr, s, cnts);
				if (cnts[4] != 0)MakeSureDouble(node, arr, s, cnts);
			}
		}
		//运算符重载中的后置函数运算符肯定分析
		void ICalculation::MakeSureFunBack(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			if (!h.IsNull() && h.GetFlagRef() == 3)h = h.GetHeadNodeRef();//忽略空格
			PCalculationNodes n = node.GetNextNodeRef();
			if (!n.IsNull() && n.GetFlagRef() == 3)n = n.GetNextNodeRef();//忽略空格
			index_t paramc = -1;
			if (!h.IsNull() && h.GetFlagRef() == 1)
			{
				if (h.GetInnerNodeRef().IsNull())paramc = 0;
				else if (h.GetInnerNodeRef().GetFlagRef() == 2)paramc = h.GetInnerNodeCount();
				else paramc = 1;
			}
			index_t lc = cnts[3];
			cnts[3] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择
			{
				if (arr[i].IsFunctionSupport() && arr[i].IsCalculateBack())//后置函数
				{
					++cnts[3];
					POperatorObjects op = arr[i];
					index_t c = op.GetParametersCount();
					//排除的情况：前面是不是括号，或者参数数量对不上
					if (paramc == -1 || (c != 65536 && c != paramc)) { arr.RemoveAt(i--); --cnts[3]; }
					//肯定是后置函数的情况：前面是括号，而且参数数量匹配，后面是空、双目、后置
					else if (n.IsNull() || (n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && (n.GetInnerOperatorRef().IsDoubleSupport() || (n.GetInnerOperatorRef().IsCalculateBack() && (n.GetInnerOperatorRef().IsSingleSupport() || n.GetInnerOperatorRef().IsFunctionSupport())))))
					{
						s = op;
					}
				}
			}
			if (s.IsNull() && lc != cnts[3])//关联解析
			{
				if (cnts[1] != 0)MakeSureSingleBack(node, arr, s, cnts);
				if (cnts[4] != 0)MakeSureDouble(node, arr, s, cnts);
			}
		}
		//运算符重载中的双目肯定分析
		void ICalculation::MakeSureDouble(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			if (!h.IsNull() && h.GetFlagRef() == 3)h = h.GetHeadNodeRef();//忽略空格
			PCalculationNodes n = node.GetNextNodeRef();
			if (!n.IsNull() && n.GetFlagRef() == 3)n = n.GetNextNodeRef();//忽略空格
			cnts[4] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择，只可能拥有一个双目
			{
				if (arr[i].IsDoubleSupport())//双目
				{
					cnts[4] = 1;
					POperatorObjects op = arr[i];
					//肯定是双目的情况：
					//前面是数字、变量、算子、括号（后置函数和单目不匹配），而且后面是数字、变量、算子、括号（函数和单目不匹配）
					if (!h.IsNull() && ((h.GetFlagRef() == 4 || h.GetFlagRef() == 5 || (h.GetFlagRef() == -1 && h.GetInnerOperatorRef().IsAloneSupport()))
						|| (h.GetFlagRef() == 1 && cnts[3] == 0 && cnts[1] == 0)
						|| (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && h.GetInnerOperatorRef().IsCalculateBack())
						) && !n.IsNull() && ((n.GetFlagRef() == 4 || n.GetFlagRef() == 5 || (n.GetFlagRef() == -1 && n.GetInnerOperatorRef().IsAloneSupport()))
							|| (n.GetFlagRef() == 1 && cnts[2] == 0 && cnts[1] == 0)
							|| (n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && !n.GetInnerOperatorRef().IsCalculateBack())
							))
						s = op;
					//不可能的情况：前面或后面是双目、前面是单目或函数、后面是后置单目或后置函数
					else if (h.IsNull() || (h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && (h.GetInnerOperatorRef().IsDoubleSupport() || ((h.GetInnerOperatorRef().IsSingleSupport() || h.GetInnerOperatorRef().IsFunctionSupport()) && !h.GetInnerOperatorRef().IsCalculateBack())))
						|| n.IsNull() || (n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && (n.GetInnerOperatorRef().IsDoubleSupport() || ((n.GetInnerOperatorRef().IsSingleSupport() || n.GetInnerOperatorRef().IsFunctionSupport()) && n.GetInnerOperatorRef().IsCalculateBack())))) {
						arr.RemoveAt(i); cnts[4] = 0;
					}
					break;
				}
			}
		}
		//运算符重载中的算子肯定分析
		void ICalculation::MakeSureAlone(PCalculationNodes& node, Collection::PArray<POperatorObjects >arr, POperatorObjects &s, index_t cnts[])
		{
			PCalculationNodes h = node.GetHeadNodeRef();
			PCalculationNodes n = node.GetNextNodeRef();
			cnts[4] = 0;
			for (index_t i = 0; s.IsNull() && i < arr.GetLength() && arr.GetLength() != 1; ++i)//根据当前节点所拥有重载进行选择，只可能拥有一个算子
			{
				if (arr[i].IsAloneSupport())//算子
				{
					cnts[5] = 1;
					POperatorObjects op = arr[i];
					//肯定是算子的情况：
					//前面是双目、前置单目，而且后面是双目、后置单目
					if (!h.IsNull() && h.GetFlagRef() == -1 && !h.GetInnerOperatorRef().IsNull() && (h.GetInnerOperatorRef().IsDoubleSupport() || (h.GetInnerOperatorRef().IsSingleSupport() && !h.GetInnerOperatorRef().IsCalculateBack())) &&
						!n.IsNull() && n.GetFlagRef() == -1 && !n.GetInnerOperatorRef().IsNull() && (n.GetInnerOperatorRef().IsDoubleSupport() || (n.GetInnerOperatorRef().IsSingleSupport() && n.GetInnerOperatorRef().IsCalculateBack())))
						s = op;
					//不可能的情况：没有
					break;
				}
			}
		}
		void ICalculation::RemoveLR(PCalculationNodes n)
		{
			while (!n.IsNull())
			{
				if (n.GetFlagRef() == 2)//补充参数
				{
					PCalculationNodes nn = n.GetNextNodeRef();
					while (!nn.IsNull() && nn.GetFlagRef() == 2)nn = nn.GetNextNodeRef();
					if (!nn.IsNull())
					{
						PCalculationNodes no = MakeCalculationNode(2);
						no.GetInnerNodeRef() = (nn);
						nn.GetOutNodeRef() = (no);
						no.GetHeadNodeRef() = (nn.GetHeadNodeRef());
						no.GetHeadNodeRef().GetNextNodeRef() = (no);
						nn.GetHeadNodeRef().SetToNull();
					}
				}
				if (!n.GetInnerNodeRef().IsNull())
				{
					if (n.GetInnerNodeRef().GetNextNodeRef().IsNull() && n.GetInnerNodeRef().GetFlagRef() == 1 && (n.GetFlagRef() == 1 || n.GetFlagRef() == 2))
					{
						n.GetInnerNodeRef() = (n.GetInnerNodeRef().GetInnerNodeRef());
						if (!n.GetInnerNodeRef().IsNull())n.GetInnerNodeRef().GetOutNodeRef() = (n);
						continue;
					}
					RemoveLR(n.GetInnerNodeRef());
				}
				n = n.GetNextNodeRef();
			}
		}
		index_t ICalculation::GetCountOfOperatorInNodes(POperatorObjects const& x)const
		{
			if (baseNode.IsNull())return 0;
			return baseNode.GetCountOfOperatorInNodes(x);
		}
	}
}
#endif