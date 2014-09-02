

#include <abb/base/log.hpp>
#include "abb/net/event_loop.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_responce.hpp"
#include "abb/http/http_const.hpp"
#include "abb/http/http_client.hpp"
#include <string.h>
using namespace abb::net;
using namespace abb;

void sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
int main(){
		int error;
		http::Responce* rsp = http::SyncGet("http://www.baidu.com",&error,5000);
		if(rsp){
			LOG(DEBUG) << std::string((char *)rsp->Body().ReadPtr(),rsp->Body().ReadSize());
		}else{
			LOG(DEBUG) << strerror(error);
		}
		sleep(50); 

	//loop.Loop();
}
