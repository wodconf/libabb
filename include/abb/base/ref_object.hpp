

#ifndef __ABB_BASE_REF_OBJECT_HPP__
#define __ABB_BASE_REF_OBJECT_HPP__

namespace abb {
namespace base {

class RefObject {
public:
	RefObject():ref_(1){}
	virtual ~RefObject(){};
	RefObject *Ref() {
		__sync_add_and_fetch(&ref_,1);
		return this;
	}
	void UnRef() {
		if (__sync_sub_and_fetch(&ref_,1) == 0)
			delete this;
	}
private:
	int ref_;
};

}
}

#endif
