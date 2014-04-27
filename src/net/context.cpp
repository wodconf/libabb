

#include "context.hpp"

namespace abb {
namespace net {

Context::Context():num_io_thread(1),threads(NULL),brund(true) {

}
Context::~Context() {
	this->WaitAndStop();
}
void Context::Run(){
	if(threads)return;
	threads = new IOThread[this->num_io_thread];
	for(int i=0;i<this->num_io_thread;i++){
		threads[i].Start();
	}
}
void Context::WaitAndStop(){
	if(threads == NULL) return;
	for(int i=0;i<this->num_io_thread;i++){
		threads[i].StopAndWait();
	}
	delete[] threads;
	threads = NULL;
}
} /* namespace translate */
} /* namespace adcloud */
