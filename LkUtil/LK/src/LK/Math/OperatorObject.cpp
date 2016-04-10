#include"stdafx.h"
#ifdef LK_MATH_OPERATOROBJECT_
namespace LK
{
	namespace Math
	{
		unsigned short IOperatorObjects::DefaultProperty = IOperatorObjects::CalculatePassProperty |
			IOperatorObjects::SupportFunctionProperty;
		bool IOperatorObjects::StopCalculating = false;
		unsigned short& POperatorObjects::DefaultProperty = IOperatorObjects::DefaultProperty;
		bool& POperatorObjects::StopCalculating = IOperatorObjects::StopCalculating;

		void IOperatorTables::Clear()
		{
			Collection::PArray<Collection::PArray<POperatorObjects > >tbl;
			BigObject::SwapReference(tbl, table);
			if (tbl.IsNull())return;
			for (index_t i = 0; i < tbl.size(); ++i)
			{
				Collection::PArray<POperatorObjects >  a=tbl[i];
				for (index_t j = 0;j < a.size(); ++j)
				{
					POperatorObjects t;
					BigObject::SwapReference(t, a[j]);
					if (!t.IsNull())t.Clear();
				}
			}
		}
		//添加运算符(逐个)
		bool IOperatorTables::AddOperator(POperatorObjects const& obj)
		{
			Collection::PArray<Collection::PArray<POperatorObjects > > &operators = (Collection::PArray<Collection::PArray<POperatorObjects > > &)table;
			if (operators.IsNull()) { operators = Collection::PArray<Collection::PArray<POperatorObjects > >((index_t)0); }
			for (index_t i = 0; i < operators.GetLength(); ++i)
			{
				if (operators[i][0].GetName() == obj.GetName())//重载或覆盖
				{
					bool cover = false;
					int j = 0;
					for (j = 0; j < operators[i].GetLength(); ++j)//忽略运算符优先级进行比较
						if ((obj.GetComparingNumber() & 0x0000ffffffffffff) == (operators[i][j].GetComparingNumber() & 0x0000ffffffffffff))
						{
							operators[i].RemoveAt(j);
							cover = true;
							break;
						}
					for (j = 0; j < operators[i].GetLength(); ++j)
						if (obj.GetComparingNumber() < operators[i][j].GetComparingNumber())break;
					operators[i].Insert(j, obj);
					return cover;
				}
				if (operators[i][0].GetName().size() < obj.GetName().size())
				{
					operators.Insert(i, Collection::PArray<POperatorObjects >(1, obj));
					return false;
				}
			}
			operators.Append(Collection::PArray<POperatorObjects >(1, obj));
			return false;
		}
		//移除指定名称的运算符
		void IOperatorTables::RemoveOperatorByName(std::string const&str)
		{
			Collection::PArray<Collection::PArray<POperatorObjects > > &operators = (Collection::PArray<Collection::PArray<POperatorObjects > > &)table;
			if (operators.IsNull() || str.IsNullOrEmpty() || operators.GetLength() == 0)return;
			for (index_t i = 0; i<operators.GetLength(); ++i)
				if (operators[i][0].GetName() == str)
				{
					Collection::PArray<POperatorObjects >& a = operators[i];
					for (index_t j = 0; j < a.size(); ++j)
						if (!a[j].IsNull())a[j].Clear();
					operators.RemoveAt(i);
					break;
				}
		}
		//移除指定名称的运算符
		void IOperatorTables::RemoveOperatorByName(Collection::PArray<std::string >const&arr)
		{
			Collection::PArray<Collection::PArray<POperatorObjects > > &operators = (Collection::PArray<Collection::PArray<POperatorObjects > > &)table;
			if (operators.IsNull() || arr.IsNull() || operators.GetLength() == 0 || arr.GetLength() == 0)return;
			index_t last = operators.GetLength();
			for (index_t i = 0; i<last;)
			{
				for (index_t j = 0; j<arr.GetLength(); ++j)
				{
					if (operators[i][0].GetName() == arr[j])
					{
						Collection::PArray<POperatorObjects >& a = operators[i];
						for (index_t k = 0; k < a.size(); ++k)
							if (!a[k].IsNull())a[k].Clear();
						BigObject::SwapReference(operators[i], operators[--last]);
						--i;
						break;
					}
				}
				++i;
			}
			if (last<operators.GetLength())
			{
				operators.Resize(last);
			}
		}
		//移除指定的运算符
		void IOperatorTables::RemoveOperator(POperatorObjects const& x)
		{
			Collection::PArray<Collection::PArray<POperatorObjects > > &operators = (Collection::PArray<Collection::PArray<POperatorObjects > > &)table;
			if (operators.IsNull())return;
			for (index_t i = 0; i < operators.GetLength(); ++i)
			{
				for (index_t j = 0; j < operators[i].GetLength(); ++j)
					if (operators[i][j].Equals(x))
					{
						operators[i][j].Clear();
						operators[i].RemoveAt(j);
						if (operators[i].IsNull())
							operators.RemoveAt(i);
						return;
					}
			}
		}

	}
}
#endif