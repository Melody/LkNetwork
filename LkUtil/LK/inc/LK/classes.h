/*@brief LK命名空间*/
namespace LK{

	//大对象基类
	struct BigObject;
	//动态对象类型
	struct Variable;
	//类型信息类
	struct Type;
	//ptr模板
	template<class T>struct ptr;
	//weakptr模板
	template<class T>struct weakptr;

	//变量
	typedef ptr<Variable> var;
	typedef weakptr<Variable> weakvar;
}