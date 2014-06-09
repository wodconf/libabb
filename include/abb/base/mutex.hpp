
#ifndef ABB_BASE_MUTEX_HPP_
#define ABB_BASE_MUTEX_HPP_
#include "define.hpp"
#include <pthread.h>
namespace abb{ namespace base{

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
}}



#endif /* MUTEX_HPP_ */
