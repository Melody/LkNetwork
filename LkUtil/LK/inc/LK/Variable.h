#ifndef LK_VARIABLE_
#define LK_VARIABLE_
namespace LK {
	/**@brief 引用类型基类*/
	template<>struct ptr<Variable>:ptr<BigObject>
	{
		//从任意对象构造
		template<class T>ptr(T t);
		//获取变量名
		std::string GetName()const;
		//获取指定名称的属性
		var Get(std::string const&name);
		//获取一个添加了属性（同名时覆盖）的var副本。如果var为空，则直接返回本身
		var Add(var const&v)const;
		//获取一个移除指定名称的属性的var副本
		var Del(std::string const&name);
		//传参并对该变量执行调用，返回调用结果
		var Call(std::vector<var> const&vars = std::vector<var>());
	};
	/**@brief 变量*/
	struct Variable:BigObject{
		std::string name;//名称
		std::vector<var> properties;//属性列表
	};
	template<class TT, bool is_base_of_pobj, bool is_convertible_to_pobj>struct VariableConstructMethods { static ptr<BigObject> Make(TT t) { return var(t); } };
	template<class TT, bool val>struct VariableConstructMethods<TT, true, val> { static ptr<BigObject> Make(TT t) { ptr<BigObject> v; BigObject::ChangePtr(v, t.pObject); return v; } };
	template<class TT, bool val>struct VariableConstructMethods<TT,  val,true> { static ptr<BigObject> Make(TT t) { ptr<BigObject> v; BigObject::ChangePtr(v, CastPointer<BigObject>(t)); return v; } };
	template<typename TT>inline ptr<BigObject>::ptr(TT t) :pObject(nullptr) { *this = VariableConstructMethods < TT, std::is_base_of<ptr, typename std::remove_all_extents<TT>::type>::value, std::is_convertible<TT, BigObject const*>::value >::Make(t); }

	template<typename T>inline ptr<Variable>::ptr(T t){
		
	}

}
#endif