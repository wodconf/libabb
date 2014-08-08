
#ifndef __ABB_BASE_BlockQueue_HPP__
#define __ABB_BASE_BlockQueue_HPP__

#include <time.h>
#include "define.hpp"
#include <errno.h>
#include "thread.hpp"
namespace abb {
template <typename T>
class BlockQueue {
public:
	BlockQueue(){
		que_ = new T[1];
		has_data_ = false;
		size_ = 1;
		this->start_ = 0;
		this->end_ = 0;
	}
	explicit BlockQueue(unsigned int size){
		que_ = new T[size];
		has_data_ = false;
		size_ = size;
		this->start_ = 0;
		this->end_ = 0;
	}
	~BlockQueue(){
		delete []que_;
	}
	int Size(){
		Mutex::Locker l(mtx_);
		if(this->has_data_){
			if(this->start_ == this->end_){
				return this->size_;
			}else if(this->start_ > this->end_){
				return this->end_+ this->size_ - this->start_;
			}else{
				return this->end_ - this->start_;
			}
		}
		return 0;
	}
	void Push(T t){
		Mutex::Locker l(mtx_);
		while(this->start_ == this->end_ && this->has_data_){//full
			cond_.Wait(mtx_);
		}
		this->que_[this->end_] = t;
		this->end_ += 1;
		if(this->end_ == this->size_){
			this->end_ = 0;
		}
		if(!this->has_data_){
			this->has_data_ = true;
			cond_.Broadcast();
		}
	}
	bool PollTimeout(int timeout , T* ot){
		Mutex::Locker l(mtx_);
		while(this->start_ == this->end_ && !this->has_data_){//empty
			if(timeout <= 0){
				return false;
			}
			if(cond_.WaitTimeout(mtx_,timeout) == ETIMEDOUT){
				return false;
			}
		}
		bool bfull = (this->start_ == this->end_ && this->has_data_);
		*ot = this->que_[this->start_];
		this->start_ += 1;
		if(this->start_ == this->size_){
			this->start_ = 0;
		}
		if(this->start_ == this->end_){
			this->has_data_ = false;
		}
		if(bfull){
			cond_.Broadcast();
		}
		return true;
	}
	T Poll(){
		Mutex::Locker l(mtx_);
		while(this->start_ == this->end_ && !this->has_data_){//empty
			cond_.Wait(this->mtx_);
		}
		bool bfull = (this->start_ == this->end_ && this->has_data_);
		T t = this->que_[this->start_];
		this->start_ += 1;
		if(this->start_ == this->size_){
			this->start_ = 0;
		}
		if(this->start_ == this->end_){
			this->has_data_ = false;
		}
		if(bfull){
			cond_.Broadcast();
		}
		return t;
	}
private:
	unsigned int size_;
	unsigned int start_;
	unsigned int end_;
	bool has_data_;
	T * que_;
	Mutex mtx_;
	Cond cond_;

	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(BlockQueue);
};
}
#endif /* BlockQueue_H_ */
