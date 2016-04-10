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
		/*����ͬ����2��ʾ���ֲ�����ͬʱ���У�1��ʾ���ֲ���ֻ��ִ������һ��
		�б�   ����  ����  ��д  ��д  �ع�  ��Ծ  ����  ����  ��ɾ  ��ɾ
		����      1       1       2       1        1      1       2       2       1      2
		����      1       1       1       2        1      1       2       2       2      1
		��д      2       1       1       1        1      2       1       1        1      1
		��д      1       2       1       1        2      1       1       1        1      1
		�ع�      1       1       1       2        1      1       2       2       2      1
		��Ծ      1       1       2       1        1      1       2       2       1      2
		����      2       2      1       1        2      2       1       1       1      2
		����      2       2      1       1        2      2       1       1       2      1
		��ɾ      1       2       1       1        2      1       1       2       1      2
		��ɾ      2       1       1       1        1      2       2       1       2      1
		������С�����в�������1
		�������в������������źŶ���������
		*/

		Buffer::Buffer(size_t s) :pData(NULL), state(0), threadCount(0) {
			size = Memory::GetResize(s);
			if (size < 8)size = 8;
			if (size < s)Throw<std::out_of_range>("�����±�Խ��");
			pData = new u8[size];
			Reset();
			sleepTime = DefaultSleepTime;
			if (DefaultEnableThreadSafe)state &= ~MinValues::Of((index_t*)0);
			else state |= MinValues::Of((index_t*)0);
		}
		/**
		@brief Ϊ��ʹ�ô�buffer�ļ�������n
		@note ����û������ô˺�������ôCnt����õ���ֵ��ʼ��Ϊ0*/
		void Buffer::Inc(index_t n) { mutex.lock(); threadCount += n; mutex.unlock(); }
		/**
		@brief ��ȡʹ�ô�buffer�ļ���*/
		index_t Buffer::Cnt()const { return threadCount; }
		/**
		@brief ����ָ���Ĳ���ֻ���ɵ��øú������߳̽��в�����
		@param [����]����optr��ʾҪ�����Ĳ������ݣ������Ƕ���������ݵİ�λ����
		@return ���سɹ������Ĳ������ݵİ�λ������
		@note ������������ʹ����trueҲֻ����SetThreadSafe(true)ʱ������Ч������UnlockOperator�Խ����ͷ�����
		@note �ú�����ʱ�����ΪĳЩ�������ڽ��ж���������ʧ��*/
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
		@brief ����ָ���Ĳ�����ʹ�������߳�Ҳ�ܽ��иò�����
		@param [���룬��ʡ]����optr��ʾҪ�����Ĳ������ݣ������Ƕ���������ݵİ�λ����
		@return ���سɹ������Ĳ������ݵİ�λ����
		@note �ý���������ʹ����trueҲֻ����SetThreadSafe(true)ʱ������Ч������LockOperator�Խ�������*/
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
			//���ʹ�����߳�ͬ��������Ҫ����Ƿ�������������
			if (threadSafe) {
				lck.lock();
				while (lockedThreadIds[0] != std::thread::id() && lockedThreadIds[0] != std::this_thread::get_id()) {//��������
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
		@brief ��������*/
		Buffer::~Buffer() { Reset(); delete[]pData; }
		/**
		@brief д�����ݡ��������һ����û��д�꣬��������ֱ��д��ŷ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ����д���ֽ���*/
		size_t Buffer::Write(const void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = 0;
			size_t inc;
			bool threadSafe = GetEnableThreadSafe();
			//��ȡ�ս�ʱ���
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//��
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//����д�룬ֱ��д��
			while ((tmp += (inc = WriteLittle((u8*)p + tmp, len - tmp, threadSafe?&lck:nullptr))) < len) {
				if (state&StateReset)return tmp;
				if (inc == 0) {//�ȴ�
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
		@brief д�����ݡ��������һ����û��д�꣬��������ֱ��д��ŷ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ����д���ֽ���*/
		size_t Buffer::Write(std::vector<u8> const&arr, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = arr.size();
			if (len > tmp)len = tmp;
			return Write(arr.data(), len, timeout);
		}
		/**
		@brief д�����ݡ��������һ����û��д�꣬��ֱ�ӷ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@return ����ʵ��д���ֽ���*/
		size_t Buffer::WriteLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)
				return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = plck||GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>,std::mutex&, std::defer_lock_t>(plck,mutex, std::defer_lock);
			}
			std::unique_lock<std::mutex>& lck(*plck); 
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)
				return 0;
			//���ʹ�����߳�ͬ��������Ҫ����Ƿ�������������
			if (threadSafe) {
				if(!lck.owns_lock())lck.lock();
				if (lockedThreadIds[3] != std::thread::id() && lockedThreadIds[3] != std::this_thread::get_id()) {//��������
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[3] != std::thread::id() && lockedThreadIds[3] != std::this_thread::get_id()) {//��������
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateReadingBack | StateWriting | StateWritingBack | StateSkipingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReadingBack | StateWriting | StateWritingBack | StateSkipingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//���¼��
					}
				}
				else{
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ���������д��
			state |= StateWriting;
			//д��������ܵ���backPosition�Ķ��������Ƚ��м���
			size_t limitlen = size - hasReadSize - unreadSize;
			size_t maxlen = size - unreadSize;
			if (maxlen > backMinimum)maxlen -= backMinimum;
			//maxlen��¼����д���ֽ���
			if (limitlen > maxlen)maxlen = limitlen;
			if (maxlen > len)maxlen = len;
			if (limitlen < maxlen) {
				backPosition = (maxlen - limitlen + backPosition)&(size - 1);
				hasReadSize -= maxlen - limitlen;
			}
			if (threadSafe) {
				cdv.notify_all();//����
				lck.unlock();//����
			}
			//��ʼд�룬����д��
			if (size - maxlen >= writingPosition)memcpy((u8*)pData + writingPosition, p, maxlen);
			else {
				memcpy((u8*)pData + writingPosition, p, size - writingPosition);
				memcpy(pData, (u8*)p + size - writingPosition, maxlen + writingPosition - size);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();//����
			}
			//�޸�writingPosition
			writingPosition = (writingPosition + maxlen)&(size - 1);
			unreadSize += maxlen;
			//ȡ��д����
			state &= ~StateWriting;
			if (threadSafe) {
				cdv.notify_all();//����
				if((void*)plck==&_lck)plck->~unique_lock();//����
			}
			return maxlen;
		}
		/**
		@brief д�����ݡ��������һ����û��д�꣬��ֱ�ӷ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ����д���ֽ���*/
		size_t Buffer::WriteLittle(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteLittle(arr.data(), len);
		}
		/**
		@brief д�����ݣ��Զ���������ݿռ䣩���������һ����û��д�꣬������������д���ֽ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@return ����д���ֽ���
		@note �ú���Ҫ���RemoveBack��ʹ��*/
		size_t Buffer::WriteLimit(const void*p, size_t len) {
			if (len == 0)return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����
				if (lockedThreadIds[9] != std::thread::id() && lockedThreadIds[9] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[9] != std::thread::id() && lockedThreadIds[9] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemoving | StateResize)) != 0) {
						lck.unlock();//�����
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;
				}
			}
			//δ��⵽��ͻ���������д��
			state |= StateWritingLimit;
			//����д��������ᵼ��backPosition�Ķ�
			size_t limitlen = size - unreadSize - hasReadSize;
			//limitlen��¼����д���ֽ���
			if (limitlen > len)limitlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//��ʼд�룬����д��
			if (size - limitlen >= writingPosition)memcpy((u8*)pData + writingPosition, p, limitlen);
			else {
				memcpy((u8*)pData + writingPosition, p, size - writingPosition);
				memcpy(pData, (u8*)p + size - writingPosition, limitlen + writingPosition - size);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();//����
			}
			//�޸�writingPosition
			writingPosition = (writingPosition + limitlen)&(size - 1);
			unreadSize += limitlen;
			//ȡ��д����
			state &= ~StateWritingLimit;
			if (threadSafe) {
				cdv.notify_all();
			}
			return limitlen;
		}
		/**
		@brief д�����ݣ��Զ���������ݿռ䣩���������һ����û��д�꣬������������д���ֽ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ����д���ֽ���
		@note �ú���Ҫ���RemoveBack��ʹ��*/
		size_t Buffer::WriteLimit(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteLimit(arr.data(), len);
		}
		/**
		@brief ����д������(��ĩβд��)���������һ����û��д�꣬��������ֱ��д��ŷ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ����д���ֽ���*/
		size_t Buffer::WriteBack(const void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = len;
			size_t dec;
			bool threadSafe = GetEnableThreadSafe();
			//��ȡ�ս�ʱ���
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//��
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//����д�룬ֱ��д��
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
		@brief ����д������(Դ���ݴ�ĩβд��)���������һ����û��д�꣬��������ֱ��д��ŷ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ����д���ֽ���*/
		size_t Buffer::WriteBack(std::vector<u8> const&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteBack(arr.data(), len);
		}
		/**
		@brief ����д�����ݣ�Դ���ݴ�ĩβд�𣩡��������һ����û��д�꣬��ֱ�ӷ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@return ����ʵ��д���ֽ���*/
		size_t Buffer::WriteBackLittle(const void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//��⵽�����ź�ֱ�ӷ���
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
			//�����⵽��ͻ
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
			//δ��⵽��ͻ���������д��
			state |= StateWritingBack;
			//д��������ܵ���writingPosition�Ķ��������Ƚ��м���
			size_t limitlen = size - hasReadSize - unreadSize;
			size_t maxlen = size - hasReadSize;
			if (maxlen > backMinimum)maxlen -= backMinimum;
			//maxlen��¼����д���ֽ���
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
			//��ʼ������д��
			if (maxlen <= backPosition)memcpy((u8*)pData + backPosition - maxlen, p, maxlen);
			else {
				memcpy((u8*)pData, p, backPosition);
				memcpy((u8*)pData + size - (maxlen - backPosition), (u8*)p + backPosition, maxlen - backPosition);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();
			}
			//�޸�backPosition
			backPosition = (backPosition + size - maxlen)&(size - 1);
			hasReadSize += maxlen;
			//ȡ��д����
			state &= ~StateWritingBack;
			if (threadSafe) {
				cdv.notify_all();
				if ((void*)plck == &_lck)plck->~unique_lock();
			}
			return maxlen;
		}
		/**
		@brief ����д�����ݡ��������һ����û��д�꣬��ֱ�ӷ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ����д���ֽ���*/
		size_t Buffer::WriteBackLittle(std::vector<u8> const&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return WriteBackLittle(arr.data(), len);
		}
		/**
		@brief ����д�����ݣ��Զ���������ݿռ䣩���������һ����û��д�꣬������������д���ֽ���
		@param [����]����p��ʾд������������ʼ��ַ
		@param [����]����len��ʾ��Ҫд����ֽ���
		@return ����д���ֽ���
		@note �ú���Ҫ���Remove��ʹ��*/
		size_t Buffer::WriteBackLimit(const void*p, size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock );
		START:;
			//��⵽�����ź�ֱ�ӷ���
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
			//�����⵽��ͻ
			if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateWriting | StateWritingBack | StateWritingLimit | StateWritingBackLimit | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//���¼��
					}
				}
				else {
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ���������д��
			state |= StateWritingBackLimit;
			//����д��������ᵼ��writingPosition�Ķ�
			size_t limitlen = size - unreadSize - hasReadSize;
			//limitlen��¼����д���ֽ���
			if (limitlen > len)limitlen = len;
			else p = (cu8*)p - (len - limitlen);
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//��ʼд�룬����д��
			if (limitlen <= backPosition)memcpy((u8*)pData, p, limitlen);
			else {
				memcpy(pData, (u8*)p, backPosition);
				memcpy((u8*)pData + size - backPosition, (cu8*)p + backPosition, limitlen - backPosition);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();
			}
			//�޸�backPosition
			backPosition = (backPosition + size - limitlen)&(size - 1);
			hasReadSize += limitlen;
			//ȡ��д����
			state &= ~StateWritingBackLimit;
			if (threadSafe) {
				cdv.notify_all();
			}
			return limitlen;
		}
		/**
		@brief ����д�����ݣ��Զ���������ݿռ䣩���������һ����û��д�꣬������������д���ֽ���
		@param [����]����arr��ʾ�ֽ�����
		@param [����]����len��ʾ��Ҫд����ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ����д���ֽ���
		@note �ú���Ҫ���RemoveBack��ʹ��*/
		size_t Buffer::WriteBackLimit(std::vector<u8> const&arr, size_t len) {
			if (len >(size_t)arr.size())len = arr.size();
			return WriteBackLimit(arr.data(), len);
		}
		/**
		@brief ��ȡ���ݣ���ȡλ���Զ�ƫ�ơ��������һ����û��ȡ������������ֱ����ȡ���ŷ���
		@param [����]����p��ʾĿ������������ʼ��ַ
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ���ض����ֽ���*/
		size_t Buffer::Read(void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = 0;
			size_t inc=0;
			bool threadSafe = GetEnableThreadSafe();
			//��ȡ�ս�ʱ���
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//��
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//���϶�ȡ��ֱ������
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
		@brief ��ȡָ���ֽ��������ݣ���ȡλ���Զ�ƫ�ơ��������һ����û��ȡ������������ֱ����ȡ���ŷ���
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ���ض���������*/
		std::vector<u8> Buffer::Read(size_t len, std::chrono::microseconds const& timeout) {
			std::vector<u8> arr(len);
			Read((u8*)arr.data(), len, timeout);
			return arr;
		}
		/**
		@brief ��ȡ���ݣ���ȡλ���Զ�ƫ�ơ��������һ����û��ȡ������������ֱ����ȡ���ŷ���
		@param [����]����arr��ʾĿ���ֽ�����
		@param [����]����len��ʾ��Ҫ�������ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ���ض����ֽ���*/
		size_t Buffer::Read(std::vector<u8>&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len >(size_t)arr.size())len = arr.size();
			return Read((u8*)arr.data(), len, timeout);
		}
		/**
		@brief ��ȡ���ݣ���ȡλ���Զ�ƫ�ơ��������һ����û��ȡ���������������Ѷ�ȡ�ֽ���
		@param [����]����p��ʾĿ������������ʼ��ַ
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@return ���ض����ֽ���*/
		size_t Buffer::ReadLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)
				return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//��⵽�����ź�ֱ�ӷ���
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
			//�����⵽��ͻ
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
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����������ȡ
			state |= StateReading;
			//maxlen��¼���ֶ�ȡ�ֽ���
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();
			}
			//��ʼ��ȡ������
			if (size - maxlen >= readingPosition)memcpy(p, (u8*)pData + readingPosition, maxlen);
			else {
				memcpy(p, (u8*)pData + readingPosition, size - readingPosition);
				memcpy((u8*)p + size - readingPosition, pData, maxlen + readingPosition - size);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();//�޸�����ʱ������
			}
			//�޸�readingPosition
			readingPosition = (readingPosition + maxlen)&(size - 1);
			unreadSize -= maxlen;
			hasReadSize += maxlen;
			//ȡ��д����
			state &= ~StateReading;
			if (threadSafe) {
				cdv.notify_all();
				if ((void*)plck == &_lck)plck->~unique_lock();
			}
			return maxlen;
		}
		/**
		@brief ��ȡ���ݣ���ȡλ���Զ�ƫ�ơ��������һ����û��ȡ���������������Ѷ�ȡ�ֽ���
		@param [����]����arr��ʾĿ���ֽ�����
		@param [����]����len��ʾ��Ҫ�������ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ���ض����ֽ���*/
		size_t Buffer::ReadLittle(std::vector<u8>&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return ReadLittle((u8*)arr.data(), len);
		}
		/**
		@brief �����ȡ���ݣ���ȡλ���Զ�����ƫ�ơ��������һ����û��ȡ������������ֱ����ȡ���ŷ���
		@param [����]����p��ʾĿ������������ʼ��ַ
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ���ض����ֽ���
		@note ���ݴ�ĩβ��ʼд��*/
		size_t Buffer::ReadBack(void*p, size_t len, std::chrono::microseconds const& timeout) {
			size_t tmp = len;
			size_t dec;
			bool threadSafe = GetEnableThreadSafe();
			//��ȡ�ս�ʱ���
			std::chrono::time_point<std::chrono::system_clock> timeup = std::chrono::system_clock::now() + timeout;
			//��
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
			//���϶�ȡ��ֱ������
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
		@brief �����ȡ���ݣ���ȡλ�ò��ƶ�����������ʵ�ʶ�ȡ������
		@param [����]����p��ʾĿ������������ʼ��ַ
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@return ���ض����ֽ���*/
		size_t Buffer::ReadBackStatic(void*p, size_t len) {
			if (len == 0)return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex,std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();
						goto START;//���¼��
					}
				}
				else {
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����Ƿ����ȡ
			state |= StateReadingBack;
			//maxlen��¼���ֶ�ȡ�ֽ���
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			//��ʼ��ȡ������
			if (maxlen <= readingPosition)memcpy(p, (u8*)pData + readingPosition - maxlen, maxlen);
			else {
				memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
				memcpy((u8*)p, (u8*)pData + size - (maxlen - readingPosition), maxlen - readingPosition);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();//�޸�����ʱ������
			}
			//ȡ��д����
			state &= ~StateReadingBack;
			if (threadSafe) {
				cdv.notify_all();
			}
			return maxlen;
		}
		/**
		@brief �����ȡ���ݣ���ȡλ���Զ�����ƫ�ơ��������һ����û��ȡ������������ֱ����ȡ���ŷ���
		@param [����]����arr��ʾĿ���ֽ�����
		@param [����]����len��ʾ��Ҫ�������ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@param [���룬��ѡ]����timeout��ʾд�����ĳ�ʱʱ�䣬Ĭ��Ϊ����
		@return ���ض����ֽ���
		@note ���ݴ�ĩβ��ʼд��*/
		size_t Buffer::ReadBack(std::vector<u8>&arr, size_t len, std::chrono::microseconds const& timeout) {
			if (len >(size_t)arr.size())len = arr.size();
			return ReadBack((u8*)arr.data(), len, timeout);
		}
		/**
		@brief �����ȡ���ݣ���ȡλ���Զ�����ƫ�ơ��������һ����û��ȡ���������������Ѷ�ȡ�ֽ���
		@param [����]����p��ʾĿ������������ʼ��ַ
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@return ���ض����ֽ���
		@note ���ݴ�ĩβ��ʼд��*/
		size_t Buffer::ReadBackLittle(void*p, size_t len, std::unique_lock<std::mutex>*plck) {
			if (len == 0)return 0;
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			StructOfSize<sizeof(std::unique_lock<std::mutex>)> _lck;
			if (plck == nullptr) {
				plck = (std::unique_lock<std::mutex>*)&_lck;
				placementnew<std::unique_lock<std::mutex>, std::mutex&, std::defer_lock_t>(plck, mutex, std::defer_lock);
			}
			else threadSafe = true;
			std::unique_lock<std::mutex>& lck(*plck);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				if(!lck.owns_lock())lck.lock();//����stateǰ������
				if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[2] != std::thread::id() && lockedThreadIds[2] != std::this_thread::get_id()) {
						lck.unlock();//����stateǰ������
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWriting | StateSkiping | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();//����stateǰ������
						goto START;
					}
				}
				else {
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����Ƿ����ȡ
			state |= StateReadingBack;
			//maxlen��¼���ֶ�ȡ�ֽ���
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			else p = (u8*)p + len - maxlen;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			//��ʼ��ȡ������
			if (maxlen <= readingPosition)memcpy(p, (u8*)pData + readingPosition - maxlen, maxlen);
			else {
				memcpy((u8*)p + maxlen - readingPosition, (u8*)pData, readingPosition);
				memcpy((u8*)p, (u8*)pData + size - (maxlen - readingPosition), maxlen - readingPosition);
			}
			//����д��������޸��������
			if (threadSafe) {
				lck.lock();//�޸�����ʱ������
			}
			//�޸�readingPosition
			readingPosition = (readingPosition + size - maxlen)&(size - 1);
			hasReadSize -= maxlen;
			unreadSize += maxlen;
			//ȡ��д����
			state &= ~StateReadingBack;
			if (threadSafe) {
				cdv.notify_all();
				if((void*)plck==&_lck)plck->~unique_lock();//����
			}
			return maxlen;
		}
		/**
		@brief �����ȡ���ݣ���ȡλ���Զ�����ƫ�ơ��������һ����û��ȡ���������������Ѷ�ȡ�ֽ���
		@param [����]����arr��ʾĿ���ֽ�����
		@param [����]����len��ʾ��Ҫ�������ֽ���,���len����arr->size()����arr->size()Ϊ׼
		@return ���ض����ֽ���
		@note ���ݴ�ĩβ��ʼд��*/
		size_t Buffer::ReadBackLittle(std::vector<u8>&arr, size_t len) {
			if (len > (size_t)arr.size())len = arr.size();
			return ReadBackLittle((u8*)arr.data(), len);
		}
		/**
		@brief �������ݡ������Ծλ�ó���д��λ�ã���ֻ��Ծ��д��λ��Ϊֹ���ú�����������ʵ����Ծƫ��
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@return ����ʵ�������ֽ���*/
		size_t Buffer::Skip(size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[5] != std::thread::id() && lockedThreadIds[5] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[5] != std::thread::id() && lockedThreadIds[5] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReading | StateReadingBack | StateWritingBack | StateSkipingBack | StateRemoving | StateResize)) != 0) {
						lck.unlock();
						goto START;//���¼��
					}
				}
				else {
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ�����������Ծ
			state |= StateSkiping;
			//maxlen��¼������Ծ�ֽ���
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			//�޸�readingPosition
			readingPosition = (readingPosition + maxlen)&(size - 1);
			unreadSize -= maxlen;
			hasReadSize += maxlen;
			//ȡ��д����
			state &= ~StateSkiping;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			return maxlen;
		}
		/**
		@brief ������Խ���ݡ������Ծλ�ó����Ѷ�ȡλ�ã���ֻ��Ծ���Ѷ�ȡλ��Ϊֹ���ú�����������ʵ����Ծƫ��
		@param [����]����len��ʾ��Ҫ�������ֽ���
		@return ����ʵ�������ֽ���*/
		size_t Buffer::SkipBack(size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[6] != std::thread::id() && lockedThreadIds[6] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[6] != std::thread::id() && lockedThreadIds[6] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
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
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����Ƿ�����Ծ
			state |= StateSkipingBack;
			//maxlen��¼������Ծ�ֽ���
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			//�޸�readingPosition
			readingPosition = (readingPosition + size - maxlen)&(size - 1);
			hasReadSize -= maxlen;
			unreadSize += maxlen;
			//ȡ�����
			state &= ~StateSkipingBack;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			return maxlen;
		}
		/**
		@brief �Ƴ���д�루���򣩵�һ������
		@param [����]����len��ʾҪ�Ƴ����ֽ��������len��δ��ȡ����Ҫ����ֻ��������ȡλ��
		@return ����ʵ���Ƴ����ֽ���
		@note �ò���������д��λ�û���*/
		size_t Buffer::Remove(size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[7] != std::thread::id() && lockedThreadIds[7] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[7] != std::thread::id() && lockedThreadIds[7] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
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
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ���������ɾ��
			state |= StateRemoving;
			//maxlen��¼����ɾ���ֽ���
			size_t maxlen = unreadSize;
			if (maxlen > len)maxlen = len;
			//�޸�writingPosition
			writingPosition = (writingPosition + size - maxlen)&(size - 1);
			unreadSize -= maxlen;
			//ȡ�����
			state &= ~StateRemoving;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			return maxlen;
		}
		/**
		@brief �Ƴ��Ѷ�ȡ�������򣩵�һ������
		@param [����]����len��ʾҪ�Ƴ����ֽ��������len���Ѷ�ȡ����Ҫ����ֻ��������ȡλ��
		@return ����ʵ���Ƴ����ֽ���
		@note �ò����������Ѷ�ȡλ��ǰ��*/
		size_t Buffer::RemoveBack(size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[8] != std::thread::id() && lockedThreadIds[8] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[8] != std::thread::id() && lockedThreadIds[8] != std::this_thread::get_id()) {
						lck.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
			if ((state&(StateReadingBack | StateWritingBack | StateWritingBackLimit | StateWriting | StateSkipingBack | StateRemovingBack | StateResize)) != 0) {
				if (threadSafe) {
					cdv.wait_for(lck, sleepTime);
					if ((state&(StateReadingBack | StateWritingBack | StateWritingBackLimit | StateWriting | StateSkipingBack | StateRemovingBack | StateResize)) != 0) {
						lck.unlock();//�Ƚ���
						goto START;//���¼��
					}
				}
				else {
					Sleep(1);
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����Ƿ���ɾ��
			state |= StateRemovingBack;
			//maxlen��¼����ɾ���ֽ���
			size_t maxlen = hasReadSize;
			if (maxlen > len)maxlen = len;
			//�޸�backPosition
			backPosition = (backPosition + maxlen)&(size - 1);
			hasReadSize -= maxlen;
			//ȡ�����
			state &= ~StateRemovingBack;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			return maxlen;
		}
		/**
		@brief ������������С
		@param [����]����len��ʾĿ�껺������С
		@return ����ʵ�ʻ�������С
		@note ���Ŀ�껺���������л�����С������ܵ������ݶ�ʧ�������ǣ����ȿ����Ѷ����ݣ����Ѷ����ݿ���ʱ�ٿ���δ������*/
		size_t Buffer::Resize(size_t len) {
			//��ȡ�Ƿ������̰߳�ȫ�������⵽�����ã����ڸú�����Χ�ڶ����������������в���
			bool threadSafe = GetEnableThreadSafe();
			std::unique_lock<std::mutex> lck(mutex, std::defer_lock);
		START:;
			//��⵽�����ź�ֱ�ӷ���
			if (state&StateReset)return 0;
			if (threadSafe) {
				lck.lock();//����stateǰ������
				if (lockedThreadIds[11] != std::thread::id() && lockedThreadIds[11] != std::this_thread::get_id()) {
					cdv.wait_for(lck, sleepTime);
					if (lockedThreadIds[11] != std::thread::id() && lockedThreadIds[11] != std::this_thread::get_id()) {
						mutex.unlock();
						goto START;
					}
				}
			}
			//�����⵽��ͻ
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
					goto START;//���¼��
				}
			}
			//δ��⵽��ͻ����Ǵ�С����
			state |= StateResize;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			//maxlen��¼Ŀ�������С
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
			//��������
			if (threadSafe) {
				lck.lock();//����
			}
			readingPosition = 0;
			writingPosition = unreadSize < maxlen ? unreadSize : maxlen;
			backPosition = maxlen >(unreadSize + hasReadSize) ? maxlen - hasReadSize : (maxlen > unreadSize ? unreadSize : 0);
			hasReadSize = backPosition == 0 ? 0 : maxlen - backPosition;
			unreadSize = writingPosition;
			BigObject::Swap(pData, p);
			size = maxlen;
			//ȡ��ɾ�����
			state &= ~StateResize;
			if (threadSafe) {
				cdv.notify_all();
				lck.unlock();//����
			}
			delete []p;
			return maxlen;
		}
		/**
		@brief ��ȡ������д�볤��*/
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