#include "stdafx.h"
#ifdef LK_MEMORY_WEAKPOINTER_
namespace LK{
	//构造
	weakptr<BigObject>::weakptr(BigObject const* p) : pObject((BigObject*)p) { 
		if (p) {
			p->UseCount.Create();
			UseCount = p->UseCount;
		}
	}
	weakptr<BigObject>::weakptr(const ptr<BigObject>& w) : pObject(w.pObject), UseCount(w.UseCount){}
	weakptr<BigObject>::weakptr(const weakptr<BigObject>& w) : pObject(w.pObject), UseCount(w.UseCount){}
	//赋值
	weakptr<BigObject>& weakptr<BigObject>::operator=(const weakptr&w) {
		UseCount = w.UseCount;
		pObject = w.pObject; 
		return *this;
	}
	bool operator==(weakptr<BigObject> w1, weakptr<BigObject> w2) { ptr<BigObject>x1 = w1.lock(); ptr<BigObject>x2= w2.lock();  return x1==x2; }
	weakptr<BigObject>& weakptr<BigObject>::operator=(std::nullptr_t) {
		UseCount.ChangeRef(nullptr); pObject = nullptr; return *this;
	}
	weakptr<BigObject>& weakptr<BigObject>::operator=(const ptr<BigObject>& w) {
		UseCount = w.UseCount;
		pObject = w.pObject;
		return *this;
	}
	//析构
	weakptr<BigObject>::~weakptr() {
		UseCount.ChangeRef(nullptr);
	}
	//获取共享引用
	ptr<BigObject> weakptr<BigObject>::lock() {
		ptr<BigObject> t;
		if (!UseCount.pCount)return t;
		else {
			if (UseCount.pCount->GetEnableThreadSafe()) {
				UseCount.pCount->Lock();
				if (*UseCount.pCount != 0) { ++*UseCount.pCount; t.pObject = pObject; }
				UseCount.pCount->UnLock();
			}
			else {
				if (*UseCount.pCount != 0) { ++*UseCount.pCount; t.pObject = pObject; }
			}
		}
		if (t.pObject) {
			t.UseCount = t.pObject->UseCount;
		}
		return t;
	}

}
#endif