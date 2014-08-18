#ifndef __ABB_BASE_THREAD_HPP__
#define __ABB_BASE_THREAD_HPP__

#include <pthread.h>
#include "define.hpp"
namespace abb{

class Mutex{
public:
	struct Locker{
		Locker(Mutex&mtx);
		~Locker();
		Mutex& refmtx_;
	};
public:
	Mutex();
	~Mutex();
	void Lock();
	void UnLock();
private:
	friend class Cond;
	pthread_mutex_t mtx_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Mutex);
};

class RWLock {
public:
struct RLocker{
	RLocker(RWLock&mtx);
	~RLocker();
	RWLock& refmtx_;
};
struct WLocker{
	WLocker(RWLock&mtx);
	~WLocker();
	RWLock& refmtx_;
};
public:
RWLock();
~RWLock();
void RLock();
void RUnLock();
void WLock();
void WUnLock();
private:
pthread_rwlock_t rw_;
ABB_BASE_DISALLOW_COPY_AND_ASSIGN(RWLock);
};

class Cond {
public:
	Cond();
	~Cond();
	int WaitTimeout(Mutex& mtx,int timeout);
	void Wait(Mutex& mtx);
	void Signal();
	void Broadcast();
private:
	pthread_cond_t cond_;
};

class Notification {
public:
	Notification();
	~Notification();
	void Wait();
	bool WaitTimeout(int timeout);
	void Notify();
private:
	bool bsingle_;
	Cond cond_;
	Mutex mtx_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Notification);
};

class Thread{
public:
	Thread();
	~Thread();
	typedef void* (*thread_fn)(void*arg);
	void Start(thread_fn fn,void*arg,int stacksize = 1024*1024);
	void Wait();
	void Detach();
	pthread_t ID(){
		return tid_;
	}
private:
	pthread_t tid_;
	bool bstart_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Thread);
};
}
#endif
