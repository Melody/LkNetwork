#ifndef LK_MEMORY_SHAREDPOINTER_
#define LK_MEMORY_SHAREDPOINTER_
namespace LK {
	/**@brief 引用类型基类*/
	template<>struct ptr<BigObject>
	{
		/**
		@brief 用以推断被代理类型*/
		typedef BigObject type;
		/**
		@brief 默认构造函数*/
		inline ptr() :pObject(nullptr) {}
		/**
		@brief 从空指针构造*/
		inline ptr(std::nullptr_t) : pObject(nullptr) {}
		/**
		@brief 析构函数*/
		~ptr();
		/**
		@brief 复制构造函数*/
		ptr(ptr const&t);
		/**
		@brief 从除了PObject外的任意其它类型对象构造
		@param [模板]参数T不需要指定
		@note 构造规则：
		@note 1.如果对象是PObject的派生类，则直接引用增加；
		@note 2.如果对象可以隐式转换到BigObject*，那么将直接将BigObject*指向的对象进行引用；
		@note 3.如果对象是其它类型将进行Variable封箱操作*/
		template<typename TT>inline ptr(TT t);
		/*operator&重载*/
		BigObject& operator*()const;
		/**
		@brief 赋值*/
		ptr & operator=(ptr const&r);
		/**
		@brief 置nullptr*/
		ptr & operator=(std::nullptr_t);
		/**
		@brief 获取对象指针，并在对象指针为空时抛出异常*/
		BigObject* chk()const;
		/**
		@brief 获取对象指针，但不进行检查*/
		BigObject* get()const;
		/**
		@brief 重载operator->*/
		inline BigObject* operator->()const { return chk(); }
		/**@brief 引用计数*/
		Memory::PReferenceCount UseCount;
		friend bool operator==(ptr<BigObject>&t1, ptr<BigObject>&t2);
		friend bool operator!=(ptr<BigObject>&t1, ptr<BigObject>&t2);
		friend bool operator==(std::nullptr_t, ptr<BigObject>&p2) { return nullptr == p2.pObject; }
		friend bool operator!=(std::nullptr_t, ptr<BigObject>&p2) { return nullptr != p2.pObject; }
		bool operator==(std::nullptr_t)const { return pObject == nullptr; }
		bool operator!=(std::nullptr_t)const { return pObject != nullptr; }
	protected:
		/**
		@brief 将该引用设置为另一个指针，但必须小心保证这个指针是可用的*/
		void SetTo(BigObject const*p);
	private:
		//被代理对象指针
		mutable BigObject* pObject;
		/**
		@brief 从代理类对象构造方法*/
		void RefCtor(ptr<BigObject> const&r);
		friend BigObject;
		friend weakptr<BigObject>;
		template<class TT, bool is_base_of_pobj, bool is_convertible_to_pobj>friend struct VariableConstructMethods;
	};
	//智能指针模板
	template<class T>struct ptr :
		std::enable_if<std::is_base_of<BigObject, T>::value,
		ptr<BigObject> >::type {
		typedef T type;
		/**@brief 复制构造函数*/
		ptr(ptr const&t) {
			ptr<BigObject>::operator=(t);
		}
		/**@brief 从对象指针构造*/
		ptr(T const* p) { ptr<BigObject>::SetTo(StaticCastPointer(p)); }
		/**@brief 从空指针*/
		ptr(std::nullptr_t) {}
		/**@brief 默认构造空引用*/
		ptr() {}
		/**@brief 赋值*/
		ptr& operator=(ptr const&r) { ptr<BigObject>::operator=(r); return *this; }
		/**@brief 置空*/
		ptr& operator=(std::nullptr_t) { ptr<BigObject>::SetTo(nullptr); return *this; }
		bool operator==(std::nullptr_t)const { return ptr<BigObject>::operator==(nullptr); }
		bool operator!=(std::nullptr_t)const { return ptr<BigObject>::operator!=(nullptr); }
		/**@brief 获取对象指针，并在对象指针为空时抛出异常*/
		T* chk()const { return CheckPointer<T>(ptr<BigObject>::get()); }
		/**@brief 获取对象指针*/
		T* get()const { return CastPointer<T>(ptr<BigObject>::get()); }
		/**@brief 重载operator->*/
		T* operator->()const { return chk(); }
		/*@brief 从其他智能指针构造（代理对象存在继承关系）*/
		template<class OT>inline ptr(OT const&t,
			typename std::enable_if <
			std::is_base_of < T, typename std::conditional<
			std::is_base_of<ptr<BigObject>, OT>::value,
			OT, ptr<BigObject >> ::type::type>::V>::T*p = nullptr) {
			ptr<BigObject>::operator=(t);
		}
	};
#if _HAS_TEMPLATE_ALIAS
	namespace Memory {
		template<class T>using SharedPointer = ptr<T>;
	}
#endif
}
//实体类方法
#define IMETHODS(pobj,obj)\
/*operator&重载*/\
obj*operator&() { BigObject::operator&(); return this; }\
obj const*operator&()const { BigObject::operator&(); return this; }\

#define BMETHODS(pobj,obj)\
/*operator&重载*/\
obj*operator&() { ((BigObject const*)this)->operator&(); return this; }\
obj const*operator&()const {  ((BigObject const*)this)->operator&(); return this; }\


//代理类方法
#define PIMETHODS_NODEFCTOR(pobj,obj)\
/**@brief 复制构造函数*/\
inline pobj(pobj const&t){ptr<BigObject>::operator=(t);}\
/**@brief 从对象指针构造*/\
inline pobj(obj const* p){(*(ptr<BigObject>*)this) = p; }\
/**@brief 赋值*/\
inline pobj& operator=(pobj const&r) { ptr<BigObject>::operator=(r); return *this; }\
/**@brief 获取对象指针，并在对象指针为空时抛出异常*/\
inline obj* chk()const { return CheckPointer<obj>(ptr<BigObject>::get()); }\
/**@brief 获取对象指针*/\
inline obj* get()const {return CastPointer<obj>(ptr<BigObject>::get());}\
/**@brief 重载operator->*/\
inline obj* operator->()const { return chk(); }\


#define PIMETHODS(pobj,obj)\
PIMETHODS_NODEFCTOR(pobj,obj)\
/**@brief 默认构造函数*/\
inline pobj(){}

#define PMETHODS_NODEFCTOR(pobj,obj)\
PIMETHODS_NODEFCTOR(pobj,obj)

#define PMETHODS(pobj,obj)\
PIMETHODS_NODEFCTOR(pobj,obj)\
/**@brief 默认构造函数*/\
inline pobj(){}

#endif