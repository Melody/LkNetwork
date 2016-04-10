#ifndef LK_REFERENCECOUNT_
#define LK_REFERENCECOUNT_
namespace LK {
	namespace Memory {
		/**
		@brief 表示一个记录引用次数的对象*/
		struct ReferenceCount {
			/**
			@brief 获取该引用计数是否启用原子增减
			@return 返回值为true时表示该引用计数启用了原子增减*/
			inline bool GetEnableThreadSafe()const { return (index_t)referenceCount < 0; }
			/**
			@brief 设置该引用计数是否使用原子增减
			@param [传入]参数b为true时表示启用原子增减*/
			inline void SetEnableThreadSafe(bool b) { if (b)referenceCount |= MinValues::Of((index_t*)NULL); else referenceCount &= MaxValues::Of<index_t>(); }
			//对象引用计数
			mutable volatile size_t referenceCount;
			//被引用计数
			mutable volatile size_t weakReferenceCount;
			//同步对象
			mutable std::mutex mutex;
			/**
			@brief 默认是否默认启用原子增减*/
			static bool& GetDefaultEnableThreadSafe() { static bool en = false; return en; }
			/**
			@brief 初始化为0*/
			inline ReferenceCount() :referenceCount(GetDefaultEnableThreadSafe() ? (size_t)MinValues::Of((index_t*)NULL) : 0), weakReferenceCount(1) {}
			/**
			@brief 初始化
			@param [传入]参数v表示对象引用次数
			@param [传入]参数w表示本身引用次数
			@param [传入,可选]参数en表示是否启用互斥*/
			inline ReferenceCount(size_t v, index_t w, bool en = GetDefaultEnableThreadSafe()) : referenceCount(GetDefaultEnableThreadSafe() ? (MinValues::Of((index_t*)NULL) | v) : (MaxValues::template Of<index_t>() &v)), weakReferenceCount(w) { SetEnableThreadSafe(en); }
			/**
			@brief 转换成数值*/
			inline operator size_t()const { return referenceCount&MaxValues::Of<index_t>(); }
			/**
			@brief 弱引用计数+1*/
			inline size_t AddRef()const {
				return ++weakReferenceCount;
			}
			/**
			@brief 弱引用计数-1*/
			inline size_t Release()const {
				return --weakReferenceCount;
			}
			/**
			@brief 计数+1*/
			inline size_t operator++()const {
				return (++referenceCount)&MaxValues::Of<index_t>();
			}
			/**
			@brief 计数-1*/
			inline size_t operator--()const {
				return (--referenceCount)&MaxValues::Of<index_t>();
			}
			inline void Lock()const { mutex.lock(); }
			inline void UnLock()const { mutex.unlock(); }
		private:
			//禁止复制和赋值
			ReferenceCount(const ReferenceCount&);
			ReferenceCount&operator=(ReferenceCount const&);
		};
		/**
		@brief 表示RefCount的引用*/
		struct PReferenceCount
		{
			//是否启用引用计数的原子增减，同RefCount::GetDefaultEnableThreadSafe
			static inline bool& GetDefaultEnableThreadSafe() {
				return ReferenceCount::GetDefaultEnableThreadSafe();
			}
			//创造引用计数对象，如果引用计数已存在，则直接返回
			inline bool Create(size_t rc = 0)const {
				bool flag = false;
				if (!pCount) {
					getMutexRef().lock();
					if (!pCount) { pCount = new ReferenceCount(rc, 1); flag = true; }
					getMutexRef().unlock();
				}
				return flag;
			}
			inline index_t operator++()const {
				index_t result;
				if (!pCount && Create(1))
					result = 1;
				else {
					if (pCount->GetEnableThreadSafe())
					{
						pCount->Lock();
						result = ++*pCount;
						pCount->UnLock();
					}
					else result = ++*pCount;
				}
				return result;
			}
			inline index_t operator--()const
			{
				index_t result;
				if (pCount) {
					if (pCount->GetEnableThreadSafe())
					{
						pCount->Lock();
						result = --*pCount;
						pCount->UnLock();
					}
					else result = --*pCount;
				}
				else return -1;
				return result;
			}
			//转换为对象引用计数，如果没有计数则返回-1
			operator index_t()const { return pCount ? (size_t)*pCount : -1; }
			//构造
			PReferenceCount() :pCount(NULL) {}
			//复制构造
			PReferenceCount(PReferenceCount const&r) :pCount(r.pCount) {
				if (pCount) {
					if (pCount->GetEnableThreadSafe()) {
						pCount->Lock();
						pCount->AddRef();
						pCount->UnLock();
					}
					else pCount->AddRef();
				}
			}
			//赋值构造
			PReferenceCount& operator=(PReferenceCount const&r)
			{
				ChangeRef(r.pCount);
				return *this;
			}
			//改变RefCount引用
			void ChangeRef(ReferenceCount* p)
			{
				if (pCount == p)return;
				if (pCount) {
					index_t val;
					if (pCount->GetEnableThreadSafe()) {
						pCount->Lock();
						val = pCount->Release();
						pCount->UnLock();
					}
					else val = pCount->Release();
					if (val == 0)delete pCount;
				}
				if (p != NULL) {
					if (p->GetEnableThreadSafe()) {
						p->Lock();
						p->AddRef();
						p->UnLock();
					}
					else p->AddRef();
				}
				pCount = p;
			}
			//获取弱引用计数
			index_t GetWeakRefCount()const { return pCount ? pCount->weakReferenceCount : -1; }
			//析构
			~PReferenceCount()
			{
				if (pCount) {
					index_t val;
					if (pCount->GetEnableThreadSafe()) {
						pCount->Lock();
						val = pCount->Release();
						pCount->UnLock();
					}
					else val = pCount->Release();
					if (val == 0)delete pCount;
				}
			}
			/**
			@brief 获取该引用计数是否启用原子增减
			@return 返回值为true时表示该引用计数启用了原子增减
			@note 如果该引用为空将会抛出空指针异常*/
			inline bool GetEnableThreadSafe()const { if (!pCount)Create(); return CheckPointer<ReferenceCount>(pCount)->GetEnableThreadSafe(); }
			/**
			@brief 设置该引用计数是否使用原子增减
			@param [传入]参数b为true时表示启用原子增减
			@note 如果该引用为空将会抛出空指针异常*/
			inline void SetEnableThreadSafe(bool b) { if (!pCount)Create(); CheckPointer<ReferenceCount>(pCount)->SetEnableThreadSafe(b); }
			//指针
			mutable ReferenceCount* pCount;
		private:
			static std::mutex& getMutexRef()
			{
				static std::mutex* p;
				if (p)return *p;
				Sleep(1);
				if (!p)
				{
					static std::mutex m;
					p = &m;
				}
				return *p;
			}
		};
	}
}
#endif