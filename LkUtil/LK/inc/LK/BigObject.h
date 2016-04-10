#ifndef LK_BIGOBJECT_
#define LK_BIGOBJECT_
namespace LK {
	/**@brief 基类*/
	struct BigObject
	{
		//引用计数
		mutable Memory::PReferenceCount UseCount;
		/**@brief 默认构造函数*/
		BigObject() {}
		/**@brief 复制构造函数*/
		BigObject(const BigObject&obj)  {}
		/*operator&重载*/
		BigObject*operator&() { if (UseCount<0)++UseCount; return this; }
		/*operator&重载*/
		BigObject const*operator&()const { if (UseCount<0)++UseCount; return this; }
		/**@brief 赋值函数*/
		BigObject& operator=(const BigObject&) { return *this; }
		/**@brief 析构函数*/
		virtual ~BigObject() { if (UseCount != -1)UseCount.pCount->referenceCount &= MinValues::template Of<index_t>(); }
		/**@brief 判断两者是否相等*/
		virtual bool Equals(ptr<BigObject> const& obj)const;
		/**@brief 输出该对象的字符串描述*/
		virtual ::std::string ToString()const;
		/**@brief 生成该对象的一个副本指针，默认抛出异常*/
		virtual ptr<BigObject> Clone()const;
		/**@brief 当该对象正被共享时返回一个副本，其它情况直接返回该对象*/
		ptr<BigObject> CloneIfShared()const { if (UseCount > 1)return Clone(); return this; }
		/**@brief 交换两个变量的值，使用operator=*/
		template<class T>static inline typename std::enable_if<!std::is_base_of<ptr<BigObject>, T>::value>::type Swap(T& t1, T& t2){
#ifdef _HAS_RVALUE_REFERENCES
			T t = (T&&)t1; t1 = (T&&)t2; t2 = (T&&)t;
#else
			T t = t1; t1 = t2; t2 = t;
#endif
		}
		template<class T>static inline typename std::enable_if<std::is_base_of<ptr<BigObject>, T>::value  >::type Swap(T& t1, T& t2);
		/**@brief 判断两个代理对象是否引用了同一个对象*/
		static bool ReferenceEqual(ptr<BigObject> t1, ptr<BigObject> t2);
		/**
		@brief 判断两个代理对象是否相等*/
		static bool Equals(ptr<BigObject> t1, ptr<BigObject> t2);
	protected:
		static void ChangePtr(ptr<BigObject>&r, BigObject*p);
	private:
		friend struct ptr<BigObject >;
		friend struct weakptr<BigObject >;
		template<class TT, bool is_base_of_pobj, bool is_convertible_to_pobj>friend struct VariableConstructMethods;
	};
	//typedef ptr<BigObject> PObject;

	inline bool operator==(ptr<BigObject>&t1, ptr<BigObject>&t2) { if (t1.pObject == t2.pObject)return true; if (t1.pObject || t2.pObject)return false; return t1.pObject->Equals(t2); }
	inline bool operator!=(ptr<BigObject>&t1, ptr<BigObject>&t2) { if (t1.pObject == t2.pObject)return false; if (t1.pObject || t2.pObject)return true; return !t1.pObject->Equals(t2); }

	template<class T>inline typename std::enable_if<std::is_base_of<ptr<BigObject>, T>::value >::type BigObject::Swap(T& t1, T& t2) {
		//要求T的大小必须跟ptr<BigObject>一样
		typename std::enable_if<sizeof(T) == sizeof(ptr<BigObject >), StructOfSize<sizeof(ptr<BigObject>) >> :: type
		 t = (StructOfSize<sizeof(ptr<BigObject>) >&)t1;
		((StructOfSize<sizeof(ptr<BigObject>) >&)t1) = (StructOfSize<sizeof(ptr<BigObject>) >&)t2;
		((StructOfSize<sizeof(ptr<BigObject>) >&)t2) = t;
	}

}
#endif
