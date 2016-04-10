#include "stdafx.h"
#ifdef LK_BIGOBJECT_
namespace LK{
	/**@brief 判断两者是否相等*/
	bool BigObject::Equals(ptr<BigObject> const& obj)const {
		return this == obj.get();
	}
	/**@brief 输出该对象的字符串描述*/
	::std::string BigObject::ToString()const {
		return RThrow<::std::string ,std::exception>("该对象不支持ToString方法");
	}
	/**@brief 生成该对象的一个副本指针，默认抛出异常*/
	ptr<BigObject> BigObject::Clone()const {
		return RThrow<ptr<BigObject>, std::exception>("该对象不支持Clone方法");
	}
	/**@brief 判断两个代理对象是否引用了同一个对象*/
	bool BigObject::ReferenceEqual(ptr<BigObject> t1, ptr<BigObject> t2){
		return t1.get() == t2.get();
	}
	/**
	@brief 判断两个代理对象是否相等*/
	bool BigObject::Equals(ptr<BigObject> t1, ptr<BigObject> t2) {
		if (t1.get() == t2.get())return true;
		if (t1 == nullptr || t2==nullptr)return false;
		return t1.get()->Equals(t2);
	}
	void BigObject::ChangePtr(ptr<BigObject>&r, BigObject*p) {
		r.SetTo(p);
	}
}
#endif