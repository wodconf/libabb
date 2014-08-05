#ifndef __ABB_BASE_NOTIFYCATION_HPP__
#define __ABB_BASE_NOTIFYCATION_HPP__

#include <pthread.h>
#include "abb/base/define.hpp"
namespace abb{
namespace base{

class Notification {
public:
	Notification();
	~Notification();
	void Wait();
	void Notify();
private:
	bool notifyed_;
	pthread_mutex_t mtx_;
	pthread_cond_t cond_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Notification);
};

}
}
#endif