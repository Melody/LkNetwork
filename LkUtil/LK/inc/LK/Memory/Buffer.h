#ifndef LK_MEMORY_BUFFER_
#define LK_MEMORY_BUFFER_
#include<condition_variable>
namespace LK
{
	namespace Memory
	{
		/**@brief
		表示一个字节缓存块*/
		struct Buffer {
		private:
			/**
			@brief 线程计数（通过Inc函数进行多线程同步操作）*/
			volatile size_t threadCount;
			/**
			@brief 操作数量，共operatorCounts种*/
			static const size_t operatorCounts = 12;
			/**
			@brief 记录块总字节数*/
			size_t size;
			/**
			@brief 内存区*/
			u8* pData;
			/**
			@brief 记录未读内容字节数*/
			size_t unreadSize;
			/**
			@brief 记录已读内容字节数*/
			size_t hasReadSize;
			/**
			@brief 记录当前出数据位置相对偏移*/
			size_t readingPosition;
			/**
			@brief 记录当前进数据位置相对偏移*/
			size_t writingPosition;
			/**
			@brief 记录可回滚位置相对偏移*/
			size_t backPosition;
			/**
			@brief 反向预留字节数（默认为0）*/
			size_t backMinimum;
			/**
			@brief 同步信号，最高位非0表示禁用线程同步*/
			volatile size_t state;
			/**@brief 不允许复制*/
			Buffer(Buffer const&);
			/**@brief 不允许赋值*/
			Buffer&operator=(Buffer const&);
		public:
			/**
			@brief 为在使用此buffer的计数增加n
			@note 如果用户不调用此函数，那么Cnt所获得的数值将始终为0*/
			void Inc(index_t n);
			/**
			@brief 获取使用此buffer的计数*/
			index_t Cnt()const;
			/**
			@brief 默认的等待时限*/
			static std::chrono::microseconds DefaultTimeOut;
			/**
			@brief 重置操作标记*/
			static const size_t StateReset = 0x1;
			/**
			@brief 读标记*/
			static const size_t StateReading=0x2;
			/**
			@brief 反向读标记*/
			static const size_t StateReadingBack = 0x4;
			/**
			@brief 写标记*/
			static const size_t StateWriting = 0x8;
			/**
			@brief 反向写标记*/
			static const size_t StateWritingBack = 0x10;
			/**
			@brief 跳跃标记*/
			static const size_t StateSkiping = 0x20;
			/**
			@brief 反向跳跃标记*/
			static const size_t StateSkipingBack = 0x40;
			/**
			@brief 移除标记*/
			static const size_t StateRemoving = 0x80;
			/**
			@brief 反向移除标记*/
			static const size_t StateRemovingBack = 0x100;
			/**
			@brief 写标记（不覆盖正向已读）*/
			static const size_t StateWritingLimit = 0x200;
			/**
			@brief 反向写标记（不覆盖正向未读）*/
			static const size_t StateWritingBackLimit = 0x400;
			/**
			@brief 调整大小操作*/
			static const size_t StateResize = 0x800;
			/**
			@brief 同步对象*/
			std::mutex mutex;
			/**
			@brief 条件变量*/
			std::condition_variable cdv;
			/**
			@brief 锁定线程id*/
			std::thread::id lockedThreadIds[operatorCounts];
			/**
			@brief 自旋等待时间（单位ms）*/
			std::chrono::milliseconds sleepTime;
			/**
			@brief 重置参数，清空缓存，缓存大小不变，线程安全性不变*/
			void Reset();
			/**
			@brief 获取是否启用线程安全
			@return 返回true表示启用线程安全，否则未启用*/
			inline bool GetEnableThreadSafe()const { return (index_t)state >= 0; }
			/**
			@brief 设置指示该缓存是否线程安全*/
			inline void SetEnableThreadSafe(bool v) { if (v != GetEnableThreadSafe()) {state^= MinValues::template Of<index_t>(); } }
			/**
			@brief 设置回退预留空间大小*/
			inline void SetBackMinimum(size_t s) { backMinimum = s; }
			/**
			@brief 获取回退预留空间大小*/
			inline size_t GetBackMinimum()const { return backMinimum; }
			/**
			@brief 获取缓存实际空间大小*/
			inline size_t GetLength()const { return size; }
			/**
			@brief 使用一个表示容量的数值来初始化，实际的容量总是大于或等于s
			@param [传入，可选]参数s表示需要构造的缓存块大小*/
			Buffer(size_t s= DefaultSize);
			/**
			@brief 析构函数*/
			~Buffer();
			/**
			@brief 锁定指定的操作只能由调用该函数的线程进行操作。
			@param [传入]参数optr表示要锁定的操作内容，可以是多个操作内容的按位与结果
			@return 返回成功锁定的操作内容的按位与结果。
			@note 该锁定操作即使返回true也只有在SetThreadSafe(true)时才能生效。调用UnlockOperator以进行释放锁定
			@note 该函数有时候会因为某些操作正在进行而部分锁定失败*/
			size_t LockOperator(size_t optr);
			/**
			@brief 解锁指定的操作，使得其它线程也能进行该操作。
			@param [传入，可省]参数optr表示要解锁的操作内容，可以是多个操作内容的按位与结果
			@return 返回成功解锁的操作内容的按位与结果
			@note 该解锁操作即使返回true也只有在SetThreadSafe(true)时才能生效。调用LockOperator以进行锁定*/
			size_t UnlockOperator(size_t optr=~(0));
			/**
			@brief 写入数据。如果内容单次没法写完，将阻塞，直到写完才返回
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示需要写入的字节数
			@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
			@return 返回写入字节数*/
			size_t Write(const void*p, size_t len,std::chrono::microseconds const& timeout=DefaultTimeOut);
			/**
			@brief 写入数据。如果内容单次没法写完，将阻塞，直到写完才返回
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回写入字节数*/
			size_t Write(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL), std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 写入数据（自动填充无数据空间）。如果内容单次没法写完，将立即返回已写入字节数
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示准备写入字节数
			@return 返回写入字节数
			@note 该函数要配合RemoveBack来使用*/
			size_t WriteLimit(const void*p, size_t len);
			/**
			@brief 写入数据（自动填充无数据空间）。如果内容单次没法写完，将立即返回已写入字节数
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回写入字节数
			@note 该函数要配合RemoveBack来使用*/
			size_t WriteLimit(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 写入数据（自动覆盖位置前的内容）。如果内容单次没法写完，则直接返回已写入字节数
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示需要写入的字节数
			@param [传入，可选]参数plck指向外部互斥锁，默认为空
			@return 返回实际写入字节数*/
			size_t WriteLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck = nullptr);
			/**
			@brief 写入数据（自动覆盖读取位置前的内容）。如果内容单次没法写完，则直接返回已写入字节数
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回写入字节数*/
			size_t WriteLittle(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 反向写入数据(从末尾写起)。如果内容单次没法写完，将阻塞，直到写完才返回
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示需要写入的字节数
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回写入字节数*/
			size_t WriteBack(const void*p, size_t len , std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 反向写入数据。如果内容单次没法写完，将阻塞，直到写完才返回
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回写入字节数*/
			size_t WriteBack(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL), std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 反向写入数据（自动填充无数据空间）。如果内容单次没法写完，将立即返回已写入字节数
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示需要写入的字节数
			@return 返回写入字节数
			@note 该函数要配合RemoveBack来使用*/
			size_t WriteBackLimit(const void*p, size_t len);
			/**
			@brief 反向写入数据（自动填充无数据空间）。如果内容单次没法写完，将立即返回已写入字节数
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回写入字节数
			@note 该函数要配合RemoveBack来使用*/
			size_t WriteBackLimit(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 反向写入数据（自动覆盖位置前的内容）。如果内容单次没法写完，则直接返回已写入字节数
			@param [传入]参数p表示写入数据内容起始地址
			@param [传入]参数len表示需要写入的字节数
			@param [传入，可选]参数plck指向外部互斥锁，默认为空
			@return 返回实际写入字节数*/
			size_t WriteBackLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck = nullptr);
			/**
			@brief 反向写入数据（自动覆盖读取位置前的内容）。如果内容单次没法写完，则直接返回已写入字节数
			@param [传入]参数arr表示字节数组
			@param [传入]参数len表示需要写入的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回写入字节数*/
			size_t WriteBackLittle(std::vector<u8> const&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 读取数据，读取位置自动偏移。如果内容单次没读取够，将阻塞，直到读取够才返回
			@param [传入]参数p表示目标数据内容起始地址
			@param [传入]参数len表示需要读出的字节数
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回读出字节数*/
			size_t Read(void*p, size_t len,std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 读取指定字节数的数据，读取位置自动偏移。如果内容单次没读取够，将阻塞，直到读取够才返回
			@param [传入]参数len表示需要读出的字节数
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回读出的数组*/
			std::vector<u8> Read(size_t len,std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 读取数据，读取位置自动偏移。如果内容单次没读取够，将阻塞，直到读取够才返回
			@param [传入]参数arr表示目标字节数组
			@param [传入]参数len表示需要读出的字节数,如果len大于arr.size()则以arr.size()为准
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回读出字节数*/
			size_t Read(std::vector<u8>&arr, size_t len = MaxValues::Of((size_t*)NULL),std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 读取数据，读取位置自动偏移。如果内容单次没读取够，将立即返回已读取字节数
			@param [传入]参数p表示目标数据内容起始地址
			@param [传入]参数len表示需要读出的字节数
			@param [传入，可选]参数plck指向外部互斥锁，默认为空
			@return 返回读出字节数*/
			size_t ReadLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck = nullptr);
			/**
			@brief 读取数据，读取位置自动偏移。如果内容单次没读取够，将立即返回已读取字节数
			@param [传入]参数arr表示目标字节数组
			@param [传入]参数len表示需要读出的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回读出字节数*/
			size_t ReadLittle(std::vector<u8>&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 反向读取数据，读取位置自动反向偏移。如果内容单次没读取够，将阻塞，直到读取够才返回
			@param [传入]参数p表示目标数据内容起始地址
			@param [传入]参数len表示需要读出的字节数
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回读出字节数
			@note 数据从末尾开始写出*/
			size_t ReadBack(void*p, size_t len, std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 反向读取数据，读取位置不移动。立即返回实际读取数据量
			@param [传入]参数p表示目标数据内容起始地址
			@param [传入]参数len表示需要读出的字节数
			@return 返回读出字节数*/
			size_t ReadBackStatic(void*p, size_t len);
			/**
			@brief 反向读取数据，读取位置自动反向偏移。如果内容单次没读取够，将阻塞，直到读取够才返回
			@param [传入]参数arr表示目标字节数组
			@param [传入]参数len表示需要读出的字节数,如果len大于arr.size()则以arr.size()为准
			@param [传入，可选]参数timeout表示操作的超时时间，默认为无限
			@return 返回读出字节数
			@note 数据从末尾开始写出*/
			size_t ReadBack(std::vector<u8>&arr, size_t len = MaxValues::Of((size_t*)NULL), std::chrono::microseconds const& timeout = DefaultTimeOut);
			/**
			@brief 反向读取数据，读取位置自动反向偏移。如果内容单次没读取够，将立即返回已读取字节数
			@param [传入]参数p表示目标数据内容起始地址
			@param [传入]参数len表示需要读出的字节数
			@param [传入，可选]参数plck指向外部互斥锁，默认为空
			@return 返回读出字节数
			@note 数据从末尾开始写出*/
			size_t ReadBackLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck = nullptr);
			/**
			@brief 反向读取数据，读取位置自动反向偏移。如果内容单次没读取够，将立即返回已读取字节数
			@param [传入]参数arr表示目标字节数组
			@param [传入]参数len表示需要读出的字节数,如果len大于arr.size()则以arr.size()为准
			@return 返回读出字节数
			@note 数据从末尾开始写出*/
			size_t ReadBackLittle(std::vector<u8>&arr, size_t len = MaxValues::Of((size_t*)NULL));
			/**
			@brief 跳过数据。如果跳跃位置超过写入位置，则只跳跃到写入位置为止。该函数立即返回实际跳跃偏移
			@param [传入]参数len表示需要跳过的字节数
			@return 返回实际跳过字节数*/
			size_t Skip(size_t len);
			/**
			@brief 反向跳越数据。如果跳跃位置超过已读取位置，则只跳跃到已读取位置为止。该函数立即返回实际跳跃偏移
			@param [传入]参数len表示需要跳过的字节数
			@return 返回实际跳过字节数*/
			size_t SkipBack(size_t len);
			/**
			@brief 移除刚写入（正向）的一段数据
			@param [传入]参数len表示要移除的字节数，如果len比未读取的量要大，则只操作到读取位置
			@return 返回实际移除的字节数
			@note 该操作将导致写入位置回退*/
			size_t Remove(size_t len);
			/**
			@brief 移除已读取过（正向）的一段数据
			@param [传入]参数len表示要移除的字节数，如果len比已读取的量要大，则只操作到读取位置
			@return 返回实际移除的字节数
			@note 该操作将导致已读取位置前进*/
			size_t RemoveBack(size_t len);
			/**
			@brief 调整缓存区大小
			@param [传入]参数len表示目标缓存区大小
			@return 返回实际缓存区大小
			@note 如果目标缓存区比现有缓存区小，则可能导致数据丢失。策略是：优先砍掉已读数据，当已读数据砍完时再砍掉未读数据*/
			size_t Resize(size_t len);
			/**
			@brief 获取写入地址*/
			u8* GetWritePositionPtr() { return pData + writingPosition; }
			/**
			@brief 获取可连续写入长度*/
			size_t GetWriteSizeContinuity();
			/**
			@brief 获取可正向读取量（可跳跃字节数）*/
			size_t GetUnreadSize()const { return unreadSize; }
			/**
			@brief 获取可反向读取量（可反向跳跃字节数）*/
			size_t GetHasReadSize()const { return hasReadSize; }
			/**
			@brief 默认启用线程同步*/
			static bool DefaultEnableThreadSafe;
			/**
			@brief 默认反向预留大小*/
			static size_t DefaultBackMinimum;
			/**
			@brief 默认缓存大小*/
			static size_t DefaultSize;
			/**
			@brief 默认自旋等待时间*/
			static std::chrono::milliseconds DefaultSleepTime;
		};
		/**
		@brief 获取一个最接近s且不小于s的2^n值*/
		inline size_t GetResize(size_t s) {
			if (s == 0)return 0;
			s -= 1;
			s |= s >> 1;
			s |= s >> 2;
			s |= s >> 4;
			s |= s >> 8;
			s |= s >> 16;
#ifdef _X64
			s |= s >> 32;
#endif
			return s + 1;
		}
	}
}
#endif