#pragma once
#ifdef _CLR
#define _MUTEX_
#else
#ifdef _VC
#if _VC<1700
#define _MUTEX_
#endif
#endif
#endif

#include <vector>
#ifndef _MUTEX_
#include <mutex>
#else
#define STD_MUTEX_
#include <chrono>
#include <functional>
namespace std {

#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

	// MUTUAL EXCLUSION
	class _Mutex_base
	{	// base class for all mutex types
	public:
		_Mutex_base() throw ()
		{	// construct with _Flags
		}

		~_Mutex_base() throw ()
		{	// clean up
		}

		_Mutex_base(const _Mutex_base&) = delete;
		_Mutex_base& operator=(const _Mutex_base&) = delete;

		typedef size_t native_handle_type;

	private:
		friend class condition_variable;
		friend class condition_variable_any;
	};

	class mutex
		: public _Mutex_base
	{	// class for mutual exclusion
		native_handle_type id;
	public:
		/* _CONST_FUN */ mutex() throw ()	// TRANSITION
			: _Mutex_base()
		{	// default construct
			id=0;
		}
		void lock()
		{	// lock the mutex
			int cnt = 0;
			while (!try_lock()) {
				Sleep(cnt >> 4);
				if (cnt < 256)cnt++;
			}
		}
		bool try_lock()
		{	// try to lock the mutex
			return InterlockedCompareExchange(&(size_t&)id, GetCurrentThreadId(),0) == 0;
		}
		void unlock()
		{	// unlock the mutex
			id = 0;
		}
		native_handle_type native_handle() {
			return id;
		}
		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;
	};

	class recursive_mutex
		: public _Mutex_base
	{	// class for recursive mutual exclusion
		CRITICAL_SECTION cs;
	public:
		recursive_mutex()
			: _Mutex_base()
		{	// default construct
			InitializeCriticalSection(&cs);
		}
		void lock()
		{	// lock the mutex
			EnterCriticalSection(&cs);
		}
		bool try_lock()
		{	// try to lock the mutex
			return TryEnterCriticalSection(&cs) != 0;
		}

		void unlock()
		{	// unlock the mutex
			LeaveCriticalSection(&cs);
		}

		native_handle_type native_handle() {
			return (native_handle_type)cs.OwningThread;
		}

		recursive_mutex(const recursive_mutex&) = delete;
		recursive_mutex& operator=(const recursive_mutex&) = delete;
	};

	// LOCK PROPERTIES
	struct adopt_lock_t
	{	// indicates adopt lock
	};

	struct defer_lock_t
	{	// indicates defer lock
	};

	struct try_to_lock_t
	{	// indicates try to lock
	};
	const static adopt_lock_t adopt_lock;
	const static defer_lock_t defer_lock;
	const static try_to_lock_t try_to_lock;
	// LOCKS
	template<class _Mutex>
	class lock_guard
	{	// class with destructor that unlocks mutex
	public:
		typedef _Mutex mutex_type;

		explicit lock_guard(_Mutex& _Mtx)
			: _MyMutex(_Mtx)
		{	// construct and lock
			_MyMutex.lock();
		}

		lock_guard(_Mutex& _Mtx, adopt_lock_t)
			: _MyMutex(_Mtx)
		{	// construct but don't lock
		}

		~lock_guard() throw ()
		{	// unlock
			_MyMutex.unlock();
		}

		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;
	private:
		_Mutex& _MyMutex;
	};

	template<class _Mutex>
	class unique_lock
	{	// whizzy class with destructor that unlocks mutex
	public:
		typedef unique_lock<_Mutex> _Myt;
		typedef _Mutex mutex_type;

		// CONSTRUCT, ASSIGN, AND DESTROY
		unique_lock() throw ()
			: _Pmtx(0), _Owns(false)
		{	// default construct
		}

		explicit unique_lock(_Mutex& _Mtx)
			: _Pmtx(&_Mtx), _Owns(false)
		{	// construct and lock
			_Pmtx->lock();
			_Owns = true;
		}

		unique_lock(_Mutex& _Mtx, adopt_lock_t)
			: _Pmtx(&_Mtx), _Owns(true)
		{	// construct and assume already locked
		}

		unique_lock(_Mutex& _Mtx, defer_lock_t) throw ()
			: _Pmtx(&_Mtx), _Owns(false)
		{	// construct but don't lock
		}

		unique_lock(_Mutex& _Mtx, try_to_lock_t)
			: _Pmtx(&_Mtx), _Owns(_Pmtx->try_lock())
		{	// construct and try to lock
		}

		template<class _Rep,
		class _Period>
			unique_lock(_Mutex& _Mtx,
				const chrono::duration<_Rep, _Period>& _Rel_time)
			: _Pmtx(&_Mtx), _Owns(_Pmtx->try_lock_for(_Rel_time))
		{	// construct and lock with timeout
		}

		template<class _Clock,
		class _Duration>
			unique_lock(_Mutex& _Mtx,
				const chrono::time_point<_Clock, _Duration>& _Abs_time)
			: _Pmtx(&_Mtx), _Owns(_Pmtx->try_lock_until(_Abs_time))
		{	// construct and lock with timeout
		}

		unique_lock(_Mutex& _Mtx, const xtime *_Abs_time)
			: _Pmtx(&_Mtx), _Owns(false)
		{	// try to lock until _Abs_time
			_Owns = _Pmtx->try_lock_until(_Abs_time);
		}

		unique_lock(unique_lock&& _Other) throw ()
			: _Pmtx(_Other._Pmtx), _Owns(_Other._Owns)
		{	// destructive copy
			_Other._Pmtx = 0;
			_Other._Owns = false;
		}

		unique_lock& operator=(unique_lock&& _Other)
		{	// destructive copy
			if (this != &_Other)
			{	// different, move contents
				if (_Owns)
					_Pmtx->unlock();
				_Pmtx = _Other._Pmtx;
				_Owns = _Other._Owns;
				_Other._Pmtx = 0;
				_Other._Owns = false;
			}
			return (*this);
		}

		~unique_lock() throw ()
		{	// clean up
			if (_Owns)
				_Pmtx->unlock();
		}

		unique_lock(const unique_lock&) = delete;
		unique_lock& operator=(const unique_lock&) = delete;

		// LOCK AND UNLOCK
		void lock()
		{	// lock the mutex
			_Validate();
			_Pmtx->lock();
			_Owns = true;
		}

		bool try_lock()
		{	// try to lock the mutex
			_Validate();
			_Owns = _Pmtx->try_lock();
			return (_Owns);
		}

		template<class _Rep,
		class _Period>
			bool try_lock_for(const chrono::duration<_Rep, _Period>& _Rel_time)
		{	// try to lock mutex for _Rel_time
			_Validate();
			_Owns = _Pmtx->try_lock_for(_Rel_time);
			return (_Owns);
		}

		template<class _Clock,
		class _Duration>
			bool try_lock_until(
				const chrono::time_point<_Clock, _Duration>& _Abs_time)
		{	// try to lock mutex until _Abs_time
			_Validate();
			_Owns = _Pmtx->try_lock_until(_Abs_time);
			return (_Owns);
		}

		bool try_lock_until(const xtime *_Abs_time)
		{	// try to lock the mutex until _Abs_time
			_Validate();
			_Owns = _Pmtx->try_lock_until(_Abs_time);
			return (_Owns);
		}

		void unlock()
		{	// try to unlock the mutex
			if (!_Pmtx || !_Owns)
				throw std::exception("操作未经许可");

			_Pmtx->unlock();
			_Owns = false;
		}

		// MUTATE
		void swap(unique_lock& _Other) throw ()
		{	// swap with _Other
			_STD swap(_Pmtx, _Other._Pmtx);
			_STD swap(_Owns, _Other._Owns);
		}

		_Mutex *release() throw ()
		{	// disconnect
			_Mutex *_Res = _Pmtx;
			_Pmtx = 0;
			_Owns = false;
			return (_Res);
		}

		// OBSERVE
		bool owns_lock() const throw ()
		{	// return true if this object owns the lock
			return (_Owns);
		}

		explicit operator bool() const throw ()
		{	// return true if this object owns the lock
			return (_Owns);
		}

		_Mutex *mutex() const throw ()
		{	// return pointer to managed mutex
			return (_Pmtx);
		}

	private:
		_Mutex *_Pmtx;
		bool _Owns;

		void _Validate() const
		{	// check if the mutex can be locked
			if (!_Pmtx)
				throw std::exception("操作未经许可");

			if (_Owns)
				throw std::exception("将发生资源死锁");
		}
	};

	// SWAP
	template<class _Mutex>
	void swap(unique_lock<_Mutex>& _Left,
		unique_lock<_Mutex>& _Right) throw ()
	{	// swap _Left and _Right
		_Left.swap(_Right);
	}

	// MULTIPLE LOCKS
	template<class _Lock0> inline
		int _Try_lock(_Lock0& _Lk0)
	{	// try to lock one mutex
		if (!_Lk0.try_lock())
			return (0);
		else
			return (-1);
	}


	template<class _Lock0,
	class _Lock1,
	class... _LockN> inline
		int _Try_lock(_Lock0& _Lk0, _Lock1& _Lk1, _LockN&... _LkN)
	{	// try to lock n-1 mutexes
		int _Res;
		if (!_Lk0.try_lock())
			return (0);
		_TRY_BEGIN
			// handle exceptions from tail lock
			if ((_Res = try_lock(_Lk1, _LkN...)) != -1)
			{	// tail lock failed
				_Lk0.unlock();
				++_Res;
			}
		_CATCH_ALL
			// tail lock threw exception
			_Lk0.unlock();
		throw;
		_CATCH_END
			return (_Res);
	}

	template<class _Lock0,
	class... _LockN> inline
		int try_lock(_Lock0& _Lk0, _LockN&... _LkN)
	{	// try to lock n-1 mutexes
		return (_Try_lock(_Lk0, _LkN...));
	}

	template<class _Lock0,
	class _Lock1,
	class... _LockN> inline
		void lock(_Lock0& _Lk0, _Lock1& _Lk1, _LockN&... _LkN)
	{	// lock N mutexes
		int _Res = 0;
		while (_Res != -1)
			_Res = _Try_lock(_Lk0, _Lk1, _LkN...);
	}


	// STRUCT once_flag
	struct once_flag
	{	// opaque data structure for call_once()
		once_flag() throw ()
			: _Opaque(0)
		{	// default construct
		}

		once_flag(const once_flag&) = delete;
		once_flag& operator=(const once_flag&) = delete;

		void *_Opaque;
	};

	// TEMPLATE FUNCTION call_once
	//typedef int(__stdcall *_Lambda_fp_t)(void *, void *, void **);
	// int  _Execute_once(
	//	once_flag& _Flag, _Lambda_fp_t _Lambda_fp, void *_Pv) throw ();

	//void _XGetLastError();

	template<class _Tuple,
		size_t... _Ix> inline
		void _Invoke_once(_Tuple& _Tup, integer_sequence<size_t, _Ix...>)
	{	// unpack a tuple for call_once()
		_STD invoke(_STD get<_Ix>(_STD move(_Tup))...);
	}

	template<class _Fn,
	class... _Args> inline
		void (call_once)(once_flag& _Flag, _Fn&& _Fx, _Args&&... _Ax)
	{	// call _Fx(_Ax...) once
		typedef tuple<_Fn&&, _Args&&..., _XSTD exception_ptr&> _Tuple;
		typedef make_integer_sequence<size_t, 1 + sizeof...(_Args)> _Seq;

		_XSTD exception_ptr _Exc;
		_Tuple _Tup(_STD forward<_Fn>(_Fx), _STD forward<_Args>(_Ax)..., _Exc);

		auto _Lambda = [](void *, void *_Pv, void **)
		{	// adapt call_once() to callback API
			_Tuple *_Ptup = static_cast<_Tuple *>(_Pv);

			_TRY_BEGIN
				_Invoke_once(*_Ptup, _Seq());
			_CATCH_ALL
				auto& _Ref = _STD get<1 + sizeof...(_Args)>(*_Ptup);
			_Ref = _XSTD current_exception();
			return (0);
			_CATCH_END

				return (1);
		};

		if (_Execute_once(_Flag, _Lambda, _STD addressof(_Tup)) != 0)
			return;

		if (_Exc)
			_XSTD rethrow_exception(_Exc);

		_XGetLastError();
	}

	enum class cv_status {	// names for wait returns
		no_timeout,
		timeout
	};

	typedef cv_status _Cv_status;

	class condition_variable
	{	// class for waiting for conditions
	public:
		typedef size_t native_handle_type;

		condition_variable()
		{	// construct
		}

		~condition_variable() throw ()
		{	// destroy
		}

		condition_variable(const condition_variable&) = delete;
		condition_variable& operator=(const condition_variable&) = delete;

		void notify_one() throw ()
		{	// wake up one waiter
			mt.lock();
			native_handle_type id = handles.back();
			handles.resize(handles.size() - 1);
			mt.unlock();
			*(bool*)id = false;
		}

		void notify_all() throw ();

		void wait(unique_lock<mutex>& _Lck)
		{	// wait for signal
			if (_Lck.mutex()->native_handle() != GetCurrentThreadId())
				throw std::exception("lock a mutex unown");
			bool signal=true;
			int cnt = 0;
			_Lck.unlock();
			mt.lock();
			handles.push_back((native_handle_type)&signal);
			mt.unlock();
			while (signal) {
				Sleep(cnt >> 3);
				if (cnt < 256)cnt++;
			}
			_Lck.lock();
		}

		template<class _Predicate>
		void wait(unique_lock<mutex>& _Lck, _Predicate _Pred)
		{	// wait for signal and test predicate
			while (!_Pred())
				wait(_Lck);
		}

		template<class _Rep,
		class _Period>
			_Cv_status wait_for(
				unique_lock<mutex>& _Lck,
				const chrono::duration<_Rep, _Period>& _Rel_time)
		{	// wait for duration
			stdext::threads::xtime _Tgt = _To_xtime(_Rel_time);
			return (wait_until(_Lck, &_Tgt));
		}

		template<class _Rep,
		class _Period,
		class _Predicate>
			bool wait_for(
				unique_lock<mutex>& _Lck,
				const chrono::duration<_Rep, _Period>& _Rel_time,
				_Predicate _Pred)
		{	// wait for signal with timeout and check predicate
			stdext::threads::xtime _Tgt = _To_xtime(_Rel_time);
			return (wait_until(_Lck, &_Tgt, _Pred));
		}

		template<class _Clock,
		class _Duration>
			_Cv_status wait_until(
				unique_lock<mutex>& _Lck,
				const chrono::time_point<_Clock, _Duration>& _Abs_time)
		{	// wait until time point
			typename chrono::time_point<_Clock, _Duration>::duration
				_Rel_time = _Abs_time - _Clock::now();
			return (wait_for(_Lck, _Rel_time));
		}

		template<class _Clock,
		class _Duration,
		class _Predicate>
			bool wait_until(
				unique_lock<mutex>& _Lck,
				const chrono::time_point<_Clock, _Duration>& _Abs_time,
				_Predicate _Pred)
		{	// wait for signal with timeout and check predicate
			typename chrono::time_point<_Clock, _Duration>::duration
				_Rel_time = _Abs_time - _Clock::now();
			return (wait_for(_Lck, _Rel_time, _Pred));
		}

		_Cv_status wait_until(
			unique_lock<mutex>& _Lck,
			const xtime *_Abs_time)
		{	// wait for signal with timeout
			if (_Lck.mutex()->native_handle() != GetCurrentThreadId())
				throw std::exception("lock a mutex unown");
			bool signal = true;
			int cnt = 0;
			_Lck.unlock();
			mt.lock();
			handles.push_back((native_handle_type)&signal);
			mt.unlock();
			while (signal) {
				xtime x = _To_xtime(chrono::system_clock::now().time_since_epoch());
				if (x.sec < _Abs_time->sec || (x.sec == _Abs_time->sec&&x.nsec < _Abs_time->nsec)){
					int len = (int)(_Abs_time->sec - x.sec) * 1000 + (int)((_Abs_time->nsec - x.nsec) / 1000000);
					if (cnt < 256)cnt++;
					if (len < (cnt >> 3))cnt = len << 3;
					Sleep(cnt>>3);
				}
				else {
					_Lck.lock();
					return cv_status::timeout;
				}
			}
			return cv_status::no_timeout;
		}

		template<class _Predicate>
		bool wait_until(
			unique_lock<mutex>& _Lck,
			const xtime *_Abs_time,
			_Predicate _Pred)
		{	// wait for signal with timeout and check predicate
			while (!_Pred())
				if (wait_until(_Lck, _Abs_time) == cv_status::timeout)
					return (_Pred());
			return (true);
		}

		native_handle_type native_handle()
		{	// return condition variable handle
			return mt.native_handle();
		}

	private:
		std::vector<native_handle_type> handles;
		mutex mt;
	};

	class timed_mutex
	{	// class for timed mutual exclusion
	public:
		timed_mutex() throw ()
			: _My_locked(0)
		{	// default construct
		}

		timed_mutex(const timed_mutex&) = delete;
		timed_mutex& operator=(const timed_mutex&) = delete;

		void lock()
		{	// lock the mutex
			unique_lock<mutex> _Lock(_My_mutex);
			while (_My_locked != 0)
				_My_cond.wait(_Lock);
			_My_locked = UINT_MAX;
		}

		bool try_lock() throw ()
		{	// try to lock the mutex
			lock_guard<mutex> _Lock(_My_mutex);
			if (_My_locked != 0)
				return (false);
			else
			{
				_My_locked = UINT_MAX;
				return (true);
			}
		}

		void unlock()
		{	// unlock the mutex
			{
				// The lock here is necessary
				lock_guard<mutex> _Lock(_My_mutex);
				_My_locked = 0;
			}
			_My_cond.notify_one();
		}

		template<class _Rep,
		class _Period>
			bool try_lock_for(const chrono::duration<_Rep, _Period>& _Rel_time)
		{	// try to lock for duration
			return (try_lock_until(chrono::steady_clock::now() + _Rel_time));
		}

		template<class _Time>
		bool _Try_lock_until(_Time _Abs_time)
		{	// try to lock the mutex with timeout
			unique_lock<mutex> _Lock(_My_mutex);
			if (!_My_cond.wait_until(_Lock, _Abs_time,
				[this] { return (_My_locked == 0); }))
				return (false);
			_My_locked = UINT_MAX;
			return (true);
		}

		template<class _Clock,
		class _Duration>
			bool try_lock_until(
				const chrono::time_point<_Clock, _Duration>& _Abs_time)
		{	// try to lock the mutex with timeout
			return (_Try_lock_until(_Abs_time));
		}

		bool try_lock_until(const xtime *_Abs_time)
		{	// try to lock the mutex with timeout
			return (_Try_lock_until(_Abs_time));
		}

	private:
		mutex _My_mutex;
		condition_variable _My_cond;
		unsigned int _My_locked;
	};

	class recursive_timed_mutex
	{	// class for recursive timed mutual exclusion
	public:
		recursive_timed_mutex() throw ()
			: _My_locked(0)
		{	// default construct
		}

		recursive_timed_mutex(const recursive_timed_mutex&) = delete;
		recursive_timed_mutex& operator=(const recursive_timed_mutex&) = delete;

		void lock()
		{	// lock the mutex
			size_t _Tid = (size_t)GetCurrentThreadId();

			unique_lock<mutex> _Lock(_My_mutex);

			if (_Tid == _My_owner)
			{
				if (_My_locked < UINT_MAX)
					++_My_locked;
				else
					throw std::exception("设备正忙或资源不足");
			}
			else
			{
				while (_My_locked != 0)
					_My_cond.wait(_Lock);
				_My_locked = 1;
				_My_owner = _Tid;
			}
		}

		bool try_lock() throw ()
		{	// try to lock the mutex
			size_t _Tid = (size_t)GetCurrentThreadId();

			lock_guard<mutex> _Lock(_My_mutex);

			if (_Tid == _My_owner)
			{
				if (_My_locked < UINT_MAX)
					++_My_locked;
				else
					return (false);
			}
			else
			{
				if (_My_locked != 0)
					return (false);
				else
				{
					_My_locked = 1;
					_My_owner = _Tid;
				}
			}
			return (true);
		}

		void unlock()
		{	// unlock the mutex
			bool _Do_notify = false;

			{
				lock_guard<mutex> _Lock(_My_mutex);
				--_My_locked;
				if (_My_locked == 0)
				{
					_Do_notify = true;
					_My_owner = (size_t)GetCurrentThreadId();
				}
			}

			if (_Do_notify)
				_My_cond.notify_one();
		}

		template<class _Rep,
		class _Period>
			bool try_lock_for(const chrono::duration<_Rep, _Period>& _Rel_time)
		{	// try to lock for duration
			return (try_lock_until(chrono::steady_clock::now() + _Rel_time));
		}

		template<class _Time>
		bool _Try_lock_until(_Time _Abs_time)
		{	// try to lock the mutex with timeout
			size_t _Tid = GetCurrentThreadId();

			unique_lock<mutex> _Lock(_My_mutex);

			if (_Tid == _My_owner)
			{
				if (_My_locked < UINT_MAX)
					++_My_locked;
				else
					return (false);
			}
			else
			{
				if (!_My_cond.wait_until(_Lock, _Abs_time,
					[this] { return (_My_locked == 0); }))
					return (false);
				_My_locked = 1;
				_My_owner = _Tid;
			}
			return (true);
		}

		template<class _Clock,
		class _Duration>
			bool try_lock_until(
				const chrono::time_point<_Clock, _Duration>& _Abs_time)
		{	// try to lock the mutex with timeout
			return (_Try_lock_until(_Abs_time));
		}

		bool try_lock_until(const xtime *_Abs_time)
		{	// try to lock the mutex with timeout
			return (_Try_lock_until(_Abs_time));
		}

	private:
		mutex _My_mutex;
		condition_variable _My_cond;
		unsigned int _My_locked;
		size_t _My_owner;
	};
#pragma pop_macro("new")
#pragma warning(pop)
#pragma pack(pop)
 /* RC_INVOKED */
}
#endif