#include "stdafx.h"
#ifdef LK_MEMORY_SHAREDPOINTER_
namespace LK {
		/**
		@brief 析构函数*/
		ptr<BigObject>::~ptr() { if (--UseCount == 0)delete pObject; }
		/**
		@brief 复制构造函数*/
		ptr<BigObject>::ptr(ptr const&t)
			:UseCount(t.UseCount),
			pObject((BigObject*)t.pObject) {
			if (UseCount <= 0)
				pObject = nullptr;
			if (pObject)
				++UseCount;
		}

		/*operator&重载*/
		BigObject& ptr<BigObject>::operator*()const {
			if (UseCount < 0)
				++UseCount;
			return *chk();
		}
		/**
		@brief 赋值*/
		ptr<BigObject> & ptr<BigObject>::operator=(ptr const&r) {
			if (r.pObject == pObject)return*this;
			BigObject* p;
			if (!r.UseCount.pCount)p = nullptr;
			else {
				if (r.UseCount.pCount->GetEnableThreadSafe()) {
					r.UseCount.pCount->Lock();
					if (*r.UseCount.pCount == 0)p = nullptr;
					else { ++*r.UseCount.pCount; p = r.pObject; }
					r.UseCount.pCount->UnLock();
				}
				else {
					if (*r.UseCount.pCount == 0)p = nullptr;
					else { ++*r.UseCount.pCount; p = r.pObject; }
				}
			}
			if (UseCount > 0) {
				index_t cnt = --UseCount;
				if (cnt == 0)
					delete pObject;
				else
					UseCount.ChangeRef(nullptr);
			}
			if (p) {
				UseCount = p->UseCount;
			}
			pObject = p;
			return *this;
		}
		/**
		@brief 置nullptr*/
		ptr<BigObject> & ptr<BigObject>::operator=(std::nullptr_t) {
			if (UseCount > 0) {
				index_t cnt = --UseCount;
				if (cnt == 0)
					delete pObject;
				else
					UseCount.ChangeRef(nullptr);
			}
			pObject = nullptr;
			return *this;
		}
		/**
		@brief 获取对象指针，并在对象指针为空时抛出异常*/
		BigObject* ptr<BigObject>::chk()const { if (UseCount <= 0 || !pObject)Throw<std::runtime_error>("空引用"); return pObject; }
		/**
		@brief 获取对象指针，但不进行检查*/
		BigObject* ptr<BigObject>::get()const { if (UseCount <= 0)return pObject = nullptr; return pObject; }
		/**
		@brief 将该引用设置为另一个指针，但必须小心保证这个指针是可用的*/
		void ptr<BigObject>::SetTo(BigObject const*p) {
			if (p == pObject)return;
			if (UseCount > 0) {
				index_t cnt = --UseCount;
				if (cnt == 0)
					delete pObject;
				else
					UseCount.ChangeRef(nullptr);
			}
			if (p) {
				if (p->UseCount == -1 && UseCount.GetWeakRefCount() == 1) {//对象从未被引用而且引用计数可重用
					++UseCount;
					p->UseCount = UseCount;
				}
				else {
					++(p->UseCount);
					UseCount = p->UseCount;
				}
			}
			pObject = (BigObject*)p;
		}
		/**
		@brief 从代理类对象构造方法*/
		void ptr<BigObject>::RefCtor(ptr<BigObject> const&r) {
			if (!r.UseCount.pCount)pObject = NULL;
			else {
				if (r.UseCount.pCount->GetEnableThreadSafe()) {
					r.UseCount.pCount->Lock();
					if (*r.UseCount.pCount == 0)pObject = NULL;
					else { ++*r.UseCount.pCount; pObject = r.pObject; }
					r.UseCount.pCount->UnLock();
				}
				else {
					if (*r.UseCount.pCount == 0)pObject = NULL;
					else { ++*r.UseCount.pCount; pObject = r.pObject; }
				}
			}
			if (pObject) {
				UseCount = pObject->UseCount;
			}
		}
	

}
#endif