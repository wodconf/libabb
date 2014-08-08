#ifndef AD_CLOUD_THREAD_HPP_
#define AD_CLOUD_THREAD_HPP_

#include <pthread.h>
#include "define.hpp"
namespace abb{

	class Mutex{
	public:
		struct Locker{
			Locker(Mutex&mtx):refmtx_(mtx){
				refmtx_.Lock();
			}
			~Locker(){
				refmtx_.UnLock();
			}
			Mutex& refmtx_;
		};
	public:
		Mutex(){
			pthread_mutex_init(&mtx_,NULL);
		};
		~Mutex(){
			pthread_mutex_destroy(&mtx_);
		}
		void Lock(){
			pthread_mutex_lock(&mtx_);
		}
		void UnLock(){
			pthread_mutex_unlock(&mtx_);
		}
	private:
		friend class Cond;
		pthread_mutex_t mtx_;
		ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Mutex);
	};

	class RWLock {
public:
	struct RLocker{
		RLocker(RWLock&mtx):refmtx_(mtx){
			refmtx_.RLock();
		}
		~RLocker(){
			refmtx_.RUnLock();
		}
		RWLock& refmtx_;
	};
	struct WLocker{
		WLocker(RWLock&mtx):refmtx_(mtx){
			refmtx_.WLock();
		}
		~WLocker(){
			refmtx_.WUnLock();
		}
		RWLock& refmtx_;
	};
public:
	RWLock(){
		pthread_rwlock_init(&rw_,NULL);
	}
	~RWLock(){
		pthread_rwlock_destroy(&rw_);
	}
	void RLock(){
		pthread_rwlock_rdlock(&rw_);
	}
	void RUnLock(){
		pthread_rwlock_unlock(&rw_);
	}
	void WLock(){
		pthread_rwlock_wrlock(&rw_);
	}
	void WUnLock(){
		pthread_rwlock_unlock(&rw_);
	}
private:
	pthread_rwlock_t rw_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(RWLock);
};

class Cond {
public:
	Cond(){
		pthread_condattr_t attr;
		pthread_condattr_init(&attr);
		pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
		pthread_cond_init(&cond_,&attr);
	}
	~Cond(){
		pthread_cond_destroy(&cond_);
	}
	int WaitTimeout(Mutex& mtx,int timeout);
	void Wait(Mutex& mtx){
		pthread_cond_wait(&cond_,&mtx.mtx_);
	}
	void Signal(){
		pthread_cond_signal(&cond_);
	}
	void Broadcast(){
		pthread_cond_broadcast(&cond_);
	}
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
		void Start(thread_fn fn,void*arg);
		void Wait();
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
