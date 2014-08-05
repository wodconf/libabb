
#ifndef __ABB_BASE_RW_LOCK_HPP__
#define __ABB_BASE_RW_LOCK_HPP__

#include <pthread.h>
#include "define.hpp"

namespace abb {
namespace base {

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


}
}

#endif 

