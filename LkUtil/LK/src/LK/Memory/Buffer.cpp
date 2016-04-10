#include"stdafx.h"
#ifdef LK_MEMORY_BUFFER_
namespace LK
{
	namespace Memory {
		bool Buffer::DefaultEnableThreadSafe = true;
		size_t Buffer::DefaultBackMinimum = 0;
		size_t Buffer::DefaultSize = 1024 * 16;
		std::chrono::milliseconds Buffer::DefaultSleepTime = std::chrono::milliseconds(500);
		std::chrono::microseconds Buffer::DefaultTimeOut(0);
		/*操作同步：2表示两种操作可同时进行，1表示两种操作只能执行其中一种
		列表   正读  反读  正写  反写  回滚  跳跃  正限  反限  正删  反删
		正读      1       1       2       1        1      1       2       2       1      2
		反读      1       1       1       2        1      1       2       2       2      1
		正写      2       1       1       1        1      2       1       1        1      1
		反写      1       2       1       1        2      1       1       1        1      1
		回滚      1       1       1       2        1      1       2       2       2      1
		跳跃      1       1       2       1        1      1       2       2       1      2
		正限      2       2      1       1        2      2       1       1       1      2
		反限      2       2      1       1        2      2       1       1       2      1
		正删      1       2       1       1        2      1       1       2       1      2
		反删      2       1       1       1        1      2       2       1       2      1
		调整大小跟所有操作都是1
		上述所有操作遇到重置信号都立即返回
		*/

		Buffer::Buffer(size_t s) :pData(NULL), state(0), threadCount(0) {
			size = Memory::GetResize(s);
			if (size < 8)size = 8;
			if (size < s)Throw<std::out_of_range>("数组下标越界");
			pData = new u8[size];
			Reset();
			sleepTime = DefaultSleepTime;
			if (DefaultEnableThreadSafe)state &= ~MinValues::Of((index_t*)0);
			else state |= MinValues::Of((index_t*)0);
		}
		/**
		@brief 为在使用此buffer的计数增加n
		@note 如果用户不调用此函数，那么Cnt所获得的数值将始终为0*/
		void Buffer::Inc(index_t n) { mutex.lock(); threadCount += n; mutex.unlock(); }
		/**
		@brief 获取使用此buffer的计数*/
		index_t Buffer::Cnt()const { return threadCount; }
		/**
		@brief 锁定指定的操作只能由调用该函数的线程进行操作。
		@param [传入]参数optr表示要锁定的操作内容，可以是多个操作内容的按位与结果
		@return 返回成功锁定的操作内容的按位与结果。
		@note 该锁定操作即使返回true也只有在SetThreadSafe(true)时才能生效。调用UnlockOperator以进行释放锁定
		@note 该函数有时候会因为某些操作正在进行而部分锁定失败*/
		size_t Buffer::LockOperator(size_t optr) {
			std::thread::id id = std::this_thread::get_id();
			size_t mask = 1;
			size_t result = 0;
			bool threadSafe = GetEnableThreadSafe();
			if (threadSafe)mutex.lock();
			for (index_t i = 0; i < operatorCounts; ++i, mask <<= 1) {
				if ((mask&optr) && (lockedThreadIds[i] == std::thread::id() || lockedThreadIds[i] == id)) {
					lockedThreadIds[i] = id;
					result |= mask;
				}
			}
			if (threadSafe)mutex.unlock();
			return result;
		}
		/**
		@brief 解锁指定的操作，使得其它线程也能进行该操作。
		@param [传入，可省]参数optr表示要解锁的操作内容，可以是多个操作内容的按位与结果
		@return 返回成功解锁的操作内容的按位与结果
		@note 该解锁操作即使返回true也只有在SetThreadSafe(true)时才能生效。调用LockOperator以进行锁定*/
		size_t Buffer::UnlockOperator(size_t optr) {
			std::thread::id id = std::this_thread::get_id();
			size_t mask = 1;
			size_t result = 0;
			bool threadSafe = GetEnableThreadSafe();
			if (threadSafe)mutex.lock();
			for (index_t i = 0; i < operatorCounts; ++i, mask <<= 1) {
				if ((mask&optr) && (lockedThreadIds[i] == std::thread::id() || lockedThreadIds[i] == id)) {
					lockedThreadIds[i] = std::thread::id();
					result |= mask;
				}
			}
			if (threadSafe)mutex.unlock();
			return result;
		}
		void Buffer::Reset() {
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//如果使用了线程同步，则需要检测是否有其它操作在
			if (threadSafe) {
				lck.lock();
				while (lockedThreadIds[0] != std::thread::id() && lockedThreadIds[0] != std::this_thread::get_id()) {//操作锁定
					cdv.wait(lck);
				}
				state |= StateReset;
				while ((state&(~(StateReset | MinValues::template Of<index_t>()))) != 0) {
					cdv.notify_all();
					cdv.wait(lck);
				}
			}
			unreadSize = 0;
			hasReadSize = 0;
			readingPosition = 0;
			writingPosition = 0;
			backPosition = 0;
			backMinimum = DefaultBackMinimum;
			state &= MinValues::template Of<index_t>();
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
		}
		/**
		@brief 析构函数*/
		Buffer::~Buffer() { Reset(); delete[]pData; }
		/**
		@brief 写入数据。如果内容一下子没法写完，将阻塞，直到写完才返回
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回写入字节数*/
		size_t Buffer::Write(const void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = 0;
			size_t inc;
			bool threadSafe = GetEnableThreadSafe();
			//获取终结时间点
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//锁
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//不断写入，直至写完
			while ((tmp += (inc = WriteLittle((u8*)p + tmp, len - tmp, threadSafe?&lck:nullptr))) < len) {
				if (state&StateReset)return tmp;
				if (inc == 0) {//等待
					if (threadSafe&&lck.owns_lock()) {
						if (unreadSize + backMinimum>=size && unreadSize+hasReadSize>=size) {
							if (timeout.count() == 0)cdv.wait(lck);
							else if (cdv.wait_for(lck, timeup - std::chrono::system_clock::now()) == std::cv_status::timeout)break;
						}
					}
					else
					{
						if (timeout.count() != 0 && std::chrono::system_clock::now() > timeup)break;
						Sleep(1);
					}
				}
			}
			return len;
		}
		/**
		@brief 写入数据。如果内容一下子没法写完，将阻塞，直到写完才返回
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回写入字节数*/
		size_t Buffer::Write(std::vector<u8> const&arr, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = arr.size();
			if (len > tmp)len = tmp;
			return Write(arr.data(), len, timeout);
		}
		/**
		@brief 写入数据。如果内容一下子没法写完，则直接返回
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@return 返回实际写入字节数*/
		size_t Buffer::WriteLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)
				return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = plck||GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>,std::mutex&, std::defer_lock_t>(plck,mutex, std::defer_lock);
			}
			std::unique_lock<std::mutex>& lck(*plck); 
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)
				return 0;
			//如果使用了线程同步，则需要检测是否有其它操作在
			if (threadSafe) {
				if(!lck.owns_lock())lck.lock();
				if (lockedThreadIds[3] != std::thread::id() && lockedThreadIds[3] != std::this_thread::get_id()) {//操作锁定
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[3] != std::thread::id() && lockedThreadIds[3] != std::this_thread::get_id()) {//操作锁定
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReadingBack | StateWriting | StateWritingBack | StateSkipingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReadingBack | StateWriting | StateWritingBack | StateSkipingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//重新检测
					}
				}
				else{
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记正向写入
			state |= StateWriting;
			//写入操作可能导致backPosition改动，这里先进行计算
			size_t limitlen = size - hasReadSize - unreadSize;
			size_t maxlen = size - unreadSize;
			if (maxlen > backMinimum)maxlen -= backMinimum;
			//maxlen记录本轮写入字节数
			if (limitlen > maxlen)maxlen = limitlen;
			if (maxlen > len)maxlen = len;
			if (limitlen < maxlen) {
				backPosition = (maxlen - limitlen + backPosition)&(size - 1);
				hasReadSize -= maxlen - limitlen;
			}
			if (threadSafe) {
				cdv.notify_all();//提醒
				lck.unlock();//解锁
			}
			//开始写入，复制写入
			if (size - maxlen >= writingPosition)memcpy((u8*)pData + writingPosition, p, maxlen);
			else {
				memcpy((u8*)pData + writingPosition, p, size - writingPosition);
				memcpy(pData, (u8*)p + size - writingPosition, maxlen + writingPosition - size);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();//上锁
			}
			//修改writingPosition
			writingPosition = (writingPosition + maxlen)&(size - 1);
			unreadSize += maxlen;
			//取消写入标记
			state &= ~StateWriting;
			if (threadSafe) {
				cdv.notify_all();//提醒
				if((void*)plck==&_lck)plck->~unique_lock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 写入数据。如果内容一下子没法写完，则直接返回
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回写入字节数*/
		size_t Buffer::WriteLittle(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteLittle(arr.data(), len);
		}
		/**
		@brief 写入数据（自动填充无数据空间）。如果内容一下子没法写完，将立即返回已写入字节数
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@return 返回写入字节数
		@note 该函数要配合RemoveBack来使用*/
		size_t Buffer::WriteLimit(const void*p, size_t len) {
			if (len == 0)return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//上锁
				if (lockedThreadIds[9] != std::thread::id() && lockedThreadIds[9] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[9] != std::thread::id() && lockedThreadIds[9] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateResize)) != 0) {
						lck.unlock();//后解锁
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;
				}
			}
			//未检测到冲突，标记正向写入
			state |= StateWritingLimit;
			//限制写入操作不会导致backPosition改动
			size_t limitlen = size - unreadSize - hasReadSize;
			//limitlen记录本轮写入字节数
			if (limitlen > len)limitlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//开始写入，复制写入
			if (size - limitlen >= writingPosition)memcpy((u8*)pData + writingPosition, p, limitlen);
			else {
				memcpy((u8*)pData + writingPosition, p, size - writingPosition);
				memcpy(pData, (u8*)p + size - writingPosition, limitlen + writingPosition - size);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();//上锁
			}
			//修改writingPosition
			writingPosition = (writingPosition + limitlen)&(size - 1);
			unreadSize += limitlen;
			//取消写入标记
			state &= ~StateWritingLimit;
			if (threadSafe) {
				cdv.notify_all();
			}
			return limitlen;
		}
		/**
		@brief 写入数据（自动填充无数据空间）。如果内容一下子没法写完，将立即返回已写入字节数
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回写入字节数
		@note 该函数要配合RemoveBack来使用*/
		size_t Buffer::WriteLimit(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteLimit(arr.data(), len);
		}
		/**
		@brief 反向写入数据(从末尾写起)。如果内容一下子没法写完，将阻塞，直到写完才返回
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回写入字节数*/
		size_t Buffer::WriteBack(const void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = len;
			size_t dec;
			bool threadSafe = GetEnableThreadSafe();
			//获取终结时间点
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//锁
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//不断写入，直至写完
			while ((tmp -= (dec = WriteBackLittle((u8*)p, tmp,threadSafe?&lck:nullptr))) != 0) {
				if (state&StateReset)break;
				if (dec == 0){
					if (threadSafe&&lck.owns_lock()) {
						if (hasReadSize + backMinimum >= size && unreadSize + hasReadSize >= size) {
							if (timeout.count() == 0)
								cdv.wait(lck);
							else if (cdv.wait_for(lck, timeup - std::chrono::system_clock::now()) == std::cv_status::timeout)break;
						}
					}
					else {
						if (timeout.count() != 0 && std::chrono::system_clock::now() > timeup)break;
						Sleep(1);
					}
				}
			}
			return len-tmp;
		}
		/**
		@brief 反向写入数据(源数据从末尾写起)。如果内容一下子没法写完，将阻塞，直到写完才返回
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回写入字节数*/
		size_t Buffer::WriteBack(std::vector<u8> const&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteBack(arr.data(), len);
		}
		/**
		@brief 反向写入数据（源数据从末尾写起）。如果内容一下子没法写完，则直接返回
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@return 返回实际写入字节数*/
		size_t Buffer::WriteBackLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				if (!lck.owns_lock())lck.lock();
				if (lockedThreadIds[4] != std::thread::id() && lockedThreadIds[4] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[4] != std::thread::id() && lockedThreadIds[4] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateWriting | StateWritingBack | StateSkiping | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateWriting | StateWritingBack | StateSkiping | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;
				}
			}
			//未检测到冲突，标记正向写入
			state |= StateWritingBack;
			//写入操作可能导致writingPosition改动，这里先进行计算
			size_t limitlen = size - hasReadSize - unreadSize;
			size_t maxlen = size - hasReadSize;
			if (maxlen > backMinimum)maxlen -= backMinimum;
			//maxlen记录本轮写入字节数
			if (limitlen > maxlen)maxlen = limitlen;
			if (maxlen > len)maxlen = len;
			if (limitlen < maxlen) {
				writingPosition = (size + writingPosition - maxlen - limitlen)&(size - 1);
				unreadSize -= maxlen - limitlen;
			}
			if (len > limitlen) { p = (cu8*)p + len - limitlen; }
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//开始，复制写入
			if (maxlen <= backPosition)memcpy((u8*)pData + backPosition - maxlen, p, maxlen);
			else {
				memcpy((u8*)pData, p, backPosition);
				memcpy((u8*)pData + size - (maxlen - backPosition), (u8*)p + backPosition, maxlen - backPosition);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();
			}
			//修改backPosition
			backPosition = (backPosition + size - maxlen)&(size - 1);
			hasReadSize += maxlen;
			//取消写入标记
			state &= ~StateWritingBack;
			if (threadSafe) {
				cdv.notify_all();
				if ((void*)plck == &_lck)plck->~unique_lock();
			}
			return maxlen;
		}
		/**
		@brief 反向写入数据。如果内容一下子没法写完，则直接返回
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回写入字节数*/
		size_t Buffer::WriteBackLittle(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteBackLittle(arr.data(), len);
		}
		/**
		@brief 反向写入数据（自动填充无数据空间）。如果内容一下子没法写完，将立即返回已写入字节数
		@param [传入]参数p表示写入数据内容起始地址
		@param [传入]参数len表示需要写入的字节数
		@return 返回写入字节数
		@note 该函数要配合Remove来使用*/
		size_t Buffer::WriteBackLimit(const void*p, size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock );
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();
				if (lockedThreadIds[10] != std::thread::id() && lockedThreadIds[10] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[10] != std::thread::id() && lockedThreadIds[10] != std::this_thread::get_id()){
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//重新检测
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记正向写入
			state |= StateWritingBackLimit;
			//限制写入操作不会导致writingPosition改动
			size_t limitlen = size - unreadSize - hasReadSize;
			//limitlen记录本轮写入字节数
			if (limitlen > len)limitlen = len;
			else p = (cu8*)p - (len - limitlen);
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//开始写入，复制写入
			if (limitlen <= backPosition)memcpy((u8*)pData, p, limitlen);
			else {
				memcpy(pData, (u8*)p, backPosition);
				memcpy((u8*)pData + size - backPosition, (cu8*)p + backPosition, limitlen - backPosition);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();
			}
			//修改backPosition
			backPosition = (backPosition + size - limitlen)&(size - 1);
			hasReadSize += limitlen;
			//取消写入标记
			state &= ~StateWritingBackLimit;
			if (threadSafe) {
				cdv.notify_all();
			}
			return limitlen;
		}
		/**
		@brief 反向写入数据（自动填充无数据空间）。如果内容一下子没法写完，将立即返回已写入字节数
		@param [传入]参数arr表示字节数组
		@param [传入]参数len表示需要写入的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回写入字节数
		@note 该函数要配合RemoveBack来使用*/
		size_t Buffer::WriteBackLimit(std::vector<u8> const&arr, size_t len) {
			if (len >(size_t)arr.size())len = arr.size();
			return WriteBackLimit(arr.data(), len);
		}
		/**
		@brief 读取数据，读取位置自动偏移。如果内容一下子没读取够，将阻塞，直到读取够才返回
		@param [传入]参数p表示目标数据内容起始地址
		@param [传入]参数len表示需要读出的字节数
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回读出字节数*/
		size_t Buffer::Read(void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = 0;
			size_t inc=0;
			bool threadSafe = GetEnableThreadSafe();
			//获取终结时间点
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//锁
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//不断读取，直至读够
			while ((tmp += (inc = ReadLittle((u8*)p + tmp, len - tmp,threadSafe?&lck:nullptr))) < len) {
				if (state&StateReset)return tmp;
				if (inc == 0&& unreadSize==0){
					if (threadSafe&&lck.owns_lock()) {
						if (timeout.count() == 0)
							cdv.wait(lck);
						else if (cdv.wait_for(lck, timeup - std::chrono::system_clock::now()) == std::cv_status::timeout)break;
					}
					else
					{
						if (timeout.count() != 0&&std::chrono::system_clock::now() > timeup)break;
						Sleep(1);
					}
				}
			}
			return len;
		}
		/**
		@brief 读取指定字节数的数据，读取位置自动偏移。如果内容一下子没读取够，将阻塞，直到读取够才返回
		@param [传入]参数len表示需要读出的字节数
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回读出的数组*/
		std::vector<u8> Buffer::Read(size_t len, std::chrono::microseconds const& timeout) {
			std::vector<u8> arr(len);
			Read((u8*)arr.data(), len, timeout);
			return arr;
		}
		/**
		@brief 读取数据，读取位置自动偏移。如果内容一下子没读取够，将阻塞，直到读取够才返回
		@param [传入]参数arr表示目标字节数组
		@param [传入]参数len表示需要读出的字节数,如果len大于arr->size()则以arr->size()为准
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回读出字节数*/
		size_t Buffer::Read(std::vector<u8>&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len >(size_t)arr.size())len = arr.size();
			return Read((u8*)arr.data(), len, timeout);
		}
		/**
		@brief 读取数据，读取位置自动偏移。如果内容一下子没读取够，将立即返回已读取字节数
		@param [传入]参数p表示目标数据内容起始地址
		@param [传入]参数len表示需要读出的字节数
		@return 返回读出字节数*/
		size_t Buffer::ReadLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)
				return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)
				return 0;
			if (threadSafe) {
				if(!lck.owns_lock())lck.lock();
				if (lockedThreadIds[1] != std::thread::id() && lockedThreadIds[1] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[1] != std::thread::id() && lockedThreadIds[1] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
						lck.unlock();
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记正向读取
			state |= StateReading;
			//maxlen记录本轮读取字节数
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//开始读取，复制
			if (size - maxlen >= readingPosition)memcpy(p, (u8*)pData + readingPosition, maxlen);
			else {
				memcpy(p, (u8*)pData + readingPosition, size - readingPosition);
				memcpy((u8*)p + size - readingPosition, pData, maxlen + readingPosition - size);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();//修改内容时先上锁
			}
			//修改readingPosition
			readingPosition = (readingPosition + maxlen)&(size - 1);
			unreadSize -= maxlen;
			hasReadSize += maxlen;
			//取消写入标记
			state &= ~StateReading;
			if (threadSafe) {
				cdv.notify_all();
				if ((void*)plck == &_lck)plck->~unique_lock();
			}
			return maxlen;
		}
		/**
		@brief 读取数据，读取位置自动偏移。如果内容一下子没读取够，将立即返回已读取字节数
		@param [传入]参数arr表示目标字节数组
		@param [传入]参数len表示需要读出的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回读出字节数*/
		size_t Buffer::ReadLittle(std::vector<u8>&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return ReadLittle((u8*)arr.data(), len);
		}
		/**
		@brief 反向读取数据，读取位置自动反向偏移。如果内容一下子没读取够，将阻塞，直到读取够才返回
		@param [传入]参数p表示目标数据内容起始地址
		@param [传入]参数len表示需要读出的字节数
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回读出字节数
		@note 数据从末尾开始写出*/
		size_t Buffer::ReadBack(void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = len;
			size_t dec;
			bool threadSafe = GetEnableThreadSafe();
			//获取终结时间点
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//锁
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//不断读取，直至读够
			while ((tmp -= (dec = ReadBackLittle((u8*)p, tmp,threadSafe?&lck:nullptr))) != 0) {
				if (state&StateReset)break;
				if (dec == 0&& hasReadSize==0){
					if (threadSafe&&lck.owns_lock()) {
						if (timeout.count() == 0)cdv.wait(lck);
						else if (cdv.wait_for(lck, timeup - std::chrono::system_clock::now()) == std::cv_status::timeout)break;
					}
					else{
						if (timeout.count() != 0 && std::chrono::system_clock::now() > timeup)break;
						Sleep(1);
					}
				}
			}
			return len-tmp;
		}
		/**
		@brief 反向读取数据，读取位置不移动。立即返回实际读取数据量
		@param [传入]参数p表示目标数据内容起始地址
		@param [传入]参数len表示需要读出的字节数
		@return 返回读出字节数*/
		size_t Buffer::ReadBackStatic(void*p, size_t len) {
			if (len == 0)return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//重新检测
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记反向读取
			state |= StateReadingBack;
			//maxlen记录本轮读取字节数
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			//开始读取，复制
			if (maxlen <= readingPosition)memcpy(p, (u8*)pData + readingPosition - maxlen, maxlen);
			else {
				memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
				memcpy((u8*)p, (u8*)pData + size - (maxlen - readingPosition), maxlen - readingPosition);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();//修改内容时先上锁
			}
			//取消写入标记
			state &= ~StateReadingBack;
			if (threadSafe) {
				cdv.notify_all();
			}
			return maxlen;
		}
		/**
		@brief 反向读取数据，读取位置自动反向偏移。如果内容一下子没读取够，将阻塞，直到读取够才返回
		@param [传入]参数arr表示目标字节数组
		@param [传入]参数len表示需要读出的字节数,如果len大于arr->size()则以arr->size()为准
		@param [传入，可选]参数timeout表示写操作的超时时间，默认为无限
		@return 返回读出字节数
		@note 数据从末尾开始写出*/
		size_t Buffer::ReadBack(std::vector<u8>&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len >(size_t)arr.size())len = arr.size();
			return ReadBack((u8*)arr.data(), len, timeout);
		}
		/**
		@brief 反向读取数据，读取位置自动反向偏移。如果内容一下子没读取够，将立即返回已读取字节数
		@param [传入]参数p表示目标数据内容起始地址
		@param [传入]参数len表示需要读出的字节数
		@return 返回读出字节数
		@note 数据从末尾开始写出*/
		size_t Buffer::ReadBackLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)return 0;
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				if(!lck.owns_lock())lck.lock();//访问state前先上锁
				if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
						lck.unlock();//访问state前先上锁
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();//访问state前先上锁
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记反向读取
			state |= StateReadingBack;
			//maxlen记录本轮读取字节数
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			else p = (u8*)p + len - maxlen;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			//开始读取，复制
			if (maxlen <= readingPosition)memcpy(p, (u8*)pData + readingPosition - maxlen, maxlen);
			else {
				memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
				memcpy((u8*)p, (u8*)pData + size - (maxlen - readingPosition), maxlen - readingPosition);
			}
			//本轮写入结束，修改相关数据
			if (threadSafe) {
				lck.lock();//修改内容时先上锁
			}
			//修改readingPosition
			readingPosition = (readingPosition + size - maxlen)&(size - 1);
			hasReadSize -= maxlen;
			unreadSize += maxlen;
			//取消写入标记
			state &= ~StateReadingBack;
			if (threadSafe) {
				cdv.notify_all();
				if((void*)plck==&_lck)plck->~unique_lock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 反向读取数据，读取位置自动反向偏移。如果内容一下子没读取够，将立即返回已读取字节数
		@param [传入]参数arr表示目标字节数组
		@param [传入]参数len表示需要读出的字节数,如果len大于arr->size()则以arr->size()为准
		@return 返回读出字节数
		@note 数据从末尾开始写出*/
		size_t Buffer::ReadBackLittle(std::vector<u8>&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return ReadBackLittle((u8*)arr.data(), len);
		}
		/**
		@brief 跳过数据。如果跳跃位置超过写入位置，则只跳跃到写入位置为止。该函数立即返回实际跳跃偏移
		@param [传入]参数len表示需要跳过的字节数
		@return 返回实际跳过字节数*/
		size_t Buffer::Skip(size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[5] != std::thread::id() && lockedThreadIds[5] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[5] != std::thread::id() && lockedThreadIds[5] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
						lck.unlock();
						goto START;//重新检测
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记正向跳跃
			state |= StateSkiping;
			//maxlen记录本轮跳跃字节数
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			//修改readingPosition
			readingPosition = (readingPosition + maxlen)&(size - 1);
			unreadSize -= maxlen;
			hasReadSize += maxlen;
			//取消写入标记
			state &= ~StateSkiping;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 反向跳越数据。如果跳跃位置超过已读取位置，则只跳跃到已读取位置为止。该函数立即返回实际跳跃偏移
		@param [传入]参数len表示需要跳过的字节数
		@return 返回实际跳过字节数*/
		size_t Buffer::SkipBack(size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[6] != std::thread::id() && lockedThreadIds[6] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[6] != std::thread::id() && lockedThreadIds[6] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记反向跳跃
			state |= StateSkipingBack;
			//maxlen记录本轮跳跃字节数
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			//修改readingPosition
			readingPosition = (readingPosition + size - maxlen)&(size - 1);
			hasReadSize -= maxlen;
			unreadSize += maxlen;
			//取消标记
			state &= ~StateSkipingBack;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 移除刚写入（正向）的一段数据
		@param [传入]参数len表示要移除的字节数，如果len比未读取的量要大，则只操作到读取位置
		@return 返回实际移除的字节数
		@note 该操作将导致写入位置回退*/
		size_t Buffer::Remove(size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[7] != std::thread::id() && lockedThreadIds[7] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[7] != std::thread::id() && lockedThreadIds[7] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReading | StateWriting | StateWritingLimit | StateWritingBack | StateSkiping | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateWriting | StateWritingLimit | StateWritingBack | StateSkiping | StateRemoving | StateResize)) != 0) {
						lck.unlock();
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记正向删除
			state |= StateRemoving;
			//maxlen记录本轮删除字节数
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			//修改writingPosition
			writingPosition = (writingPosition + size - maxlen)&(size - 1);
			unreadSize -= maxlen;
			//取消标记
			state &= ~StateRemoving;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 移除已读取过（正向）的一段数据
		@param [传入]参数len表示要移除的字节数，如果len比已读取的量要大，则只操作到读取位置
		@return 返回实际移除的字节数
		@note 该操作将导致已读取位置前进*/
		size_t Buffer::RemoveBack(size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[8] != std::thread::id() && lockedThreadIds[8] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[8] != std::thread::id() && lockedThreadIds[8] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if ((state&(StateReadingBack | StateWritingBack | StateWritingBackLimit | StateWriting | StateSkipingBack | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReadingBack | StateWritingBack | StateWritingBackLimit | StateWriting | StateSkipingBack | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();//先解锁
						goto START;//重新检测
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记反向删除
			state |= StateRemovingBack;
			//maxlen记录本轮删除字节数
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			//修改backPosition
			backPosition = (backPosition + maxlen)&(size - 1);
			hasReadSize -= maxlen;
			//取消标记
			state &= ~StateRemovingBack;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			return maxlen;
		}
		/**
		@brief 调整缓存区大小
		@param [传入]参数len表示目标缓存区大小
		@return 返回实际缓存区大小
		@note 如果目标缓存区比现有缓存区小，则可能导致数据丢失。策略是：优先砍掉已读数据，当已读数据砍完时再砍掉未读数据*/
		size_t Buffer::Resize(size_t len) {
			//获取是否启用线程安全，如果检测到已启用，则在该函数范围内都按照已启用来进行操作
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//检测到重置信号直接返回
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//访问state前先上锁
				if (lockedThreadIds[11] != std::thread::id() && lockedThreadIds[11] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[11] != std::thread::id() && lockedThreadIds[11] != std::this_thread::get_id()) {
						mutex.unlock();
						goto START;
					}
				}
			}
			//如果检测到冲突
			if (state != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if (state != 0) {
						mutex.unlock();
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//重新检测
				}
			}
			//未检测到冲突，标记大小调整
			state |= StateResize;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			//maxlen记录目标调整大小
			size_t maxlen = Memory::GetResize(len);
			u8*p = NULL;
			if (maxlen != size) {
				p =new u8[maxlen];
				if (maxlen >= unreadSize + hasReadSize) {
					if (backPosition< readingPosition)
						memcpy((u8*)p + maxlen - hasReadSize, (u8*)pData + backPosition, hasReadSize);
					else if (hasReadSize != 0) {
						memcpy((u8*)p + maxlen - hasReadSize, (u8*)pData + backPosition, size - backPosition);
						memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
					}
				}
				if (maxlen >unreadSize) {
					if (maxlen < unreadSize + hasReadSize) {
						if (maxlen - unreadSize < readingPosition)
							memcpy((u8*)p + unreadSize, (u8*)pData + readingPosition - (maxlen - unreadSize), maxlen - unreadSize);
						else if (hasReadSize != 0) {
							memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
							memcpy((u8*)p + unreadSize, (u8*)pData + size - (maxlen - unreadSize - readingPosition), maxlen - unreadSize < readingPosition);
						}
					}
					if (writingPosition > readingPosition)
						memcpy(p, (u8*)pData + readingPosition, unreadSize);
					else if (unreadSize != 0) {
						memcpy(p, (u8*)pData + readingPosition, size - readingPosition);
						memcpy((u8*)p + size - readingPosition, pData, writingPosition);
					}
				}
				else {
					if (size >= readingPosition + maxlen)
						memcpy((u8*)p, (u8*)pData + readingPosition, maxlen);
					else if (unreadSize != 0) {
						memcpy((u8*)p, (u8*)pData + readingPosition, size - readingPosition);
						memcpy((u8*)p + size - readingPosition, (u8*)pData, maxlen - (size - readingPosition));
					}
				}
			}
			//操作结束
			if (threadSafe) {
				lck.lock();//解锁
			}
			readingPosition = 0;
			writingPosition = unreadSize < maxlen ? unreadSize : maxlen;
			backPosition = maxlen >(unreadSize + hasReadSize) ? maxlen - hasReadSize : (maxlen > unreadSize ? unreadSize : 0);
			hasReadSize = backPosition == 0 ? 0 : maxlen - backPosition;
			unreadSize = writingPosition;
			BigObject::Swap(pData, p);
			size = maxlen;
			//取消删除标记
			state &= ~StateResize;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//解锁
			}
			delete []p;
			return maxlen;
		}
		/**
		@brief 获取可连续写入长度*/
		size_t Buffer::GetWriteSizeContinuity() {
			bool threadSafe = GetEnableThreadSafe();
			if (threadSafe)mutex.lock();
			size_t limitlen = size - unreadSize - hasReadSize;
			size_t maxlen = size - unreadSize;
			if (maxlen > backMinimum)maxlen -= backMinimum;
			if (maxlen < limitlen)maxlen = limitlen;
			if (maxlen + writingPosition>size)maxlen = size - writingPosition;
			if (threadSafe)mutex.unlock();
			return maxlen;
		}
	}

}
#endif