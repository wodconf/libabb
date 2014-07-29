#ifndef _ABB_NET_SINGLER_HPP_
#define _ABB_NET_SINGLER_HPP_

namespace abb{
namespace net{
class Singler{
public:
	Singler();
	~Singler();
	int GetReadFd(){
		return rfd_;
	}
	void Write();
	void Read();
private:
	int rfd_;
	int wfd_;

};
}
}

#endif
