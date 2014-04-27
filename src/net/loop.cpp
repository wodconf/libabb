
#include "loop.hpp"
using namespace abb::net;
Loop::Loop():stop_(false) {

}

Loop::~Loop() {
}
void Loop::loop(){
	while(!stop_){
		poller_.Poll(1000);
	}
}
