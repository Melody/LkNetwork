#ifndef LK_MEMORY_WEAKPOINTER_
#define LK_MEMORY_WEAKPOINTER_
namespace LK {
	/**@brief 弱引用类型基类*/
	template<>struct weakptr<BigObject>
	{
		/**
		@brief 用以推断被代理类型*/
		typedef BigObject type;
		//默认构造空引用
		inline weakptr() :pObject(nullptr) {}
		//默认构造空引用
		inline weakptr(std::nullptr_t) : pObject(nullptr) {}
		//构造，并不会导致对象引用计数变化
		weakptr(BigObject const* p);
		//构造，并不会导致对象引用计数变化
		weakptr(const ptr<BigObject>& w);
		//复制构造函数，并不会导致对象引用计数变化
		weakptr(const weakptr&w);
		//赋值，置空
		weakptr& operator=(const weakptr&w);
		//赋值，并不会导致对象引用计数变化
		weakptr& operator=(std::nullptr_t);
		//构造，并不会导致对象引用计数变化
		weakptr& operator=(const ptr<BigObject>& w);
		//构造，并不会导致对象引用计数变化
		friend bool operator==(weakptr w1, weakptr w2);
		//析构，并不会导致对象引用计数变化
		~weakptr();
		//获取共享引用
		ptr<BigObject> lock();
		/**@brief 引用计数*/
		Memory::PReferenceCount UseCount;

	private:
		//被代理对象指针
		mutable BigObject* pObject;
	};
	/**@brief 代理类的弱引用*/
	template<class T>struct weakptr :
		std::enable_if<std::is_base_of<BigObject, T>::value,
		weakptr<BigObject> >::type
	{
		typedef T type;
		//默认构造空引用
		weakptr() {}
		//默认构造空引用
		weakptr(std::nullptr_t) {}
		//构造，并不会导致对象引用计数变化
		weakptr(T const* p) :weakptr<BigObject>(p) {}
		//构造，并不会导致对象引用计数变化
		weakptr(const ptr<T>& w) :weakptr<BigObject>(w) {}
		//复制构造函数，并不会导致对象引用计数变化
		weakptr(const weakptr&w) :weakptr<BigObject>(w) {}
		//从其他类型弱引用构造
		template<class AnyT>weakptr(weakptr<AnyT>const& w, typename std::enable_if<std::is_base_of<T, AnyT>::value>::T*p = NULL) : weakptr<BigObject>(w) {}
		//赋值，置空
		weakptr& operator=(const weakptr&w) { (*(weakptr<BigObject>*)this) = w; return *this; }
		//赋值，并不会导致对象引用计数变化
		weakptr& operator=(std::nullptr_t) { (*(weakptr<BigObject>*)this) = nullptr; return *this;}
		//构造，并不会导致对象引用计数变化
		weakptr& operator=(const ptr<T>& w) { (*(weakptr<BigObject>*)this) = w; return *this;}
		//析构，并不会导致对象引用计数变化
		~weakptr() {}
		//获取共享引用
		ptr<T> lock() { ptr<BigObject> t = weakptr<BigObject>::lock(); return (ptr<T>&)t; }
	};
#if _HAS_TEMPLATE_ALIAS
	namespace Memory {
		template<class T>using WeakPointer = weakptr<T>;
	}
#endif
}
#endif