#include "abb/http/http_request_decoder.hpp"
#include <string>
#include <vector>
#include "abb/http/http_const.hpp"
#include <stdlib.h>
namespace abb{
namespace http{	

static int FindCRLF(char* buf,int size){
	for(int i=0;i<size-1;i++){
		if(buf[i] == '\r' && buf[i+1] == '\n'){
			return i;
		}
	}
	return -1;
}
static void Split(const std::string&str,const std::string& sep,std::vector<std::string>& s_arr){
	int pre_pos = 0;
	while(true){
		int pos = str.find(sep,pre_pos);
		if( pos != std::string::npos ){

			s_arr.push_back(str.substr(pre_pos,pos-pre_pos));
			pre_pos = pos+1;
		}else{
			s_arr.push_back(str.substr(pre_pos));
			break;
		}
	}
}
RequestDecoder::RequestDecoder():req_(NULL),state_(FIRST_HEAD),len_(0){

}
RequestDecoder::~RequestDecoder(){

}
static bool GetLine(base::Buffer& buf,std::string& str){
	int sz = FindCRLF((char*)buf.Data(),buf.Size());
	if(sz < 0){
		return false;
	}
	str = std::string((char*)buf.Data(),sz);
	buf.GaveRd(sz+2);
	return true;
}
bool RequestDecoder::Decode(base::Buffer& buf){
	std::string line;
	if(state_ == FIRST_HEAD){
		if(! GetLine(buf,line) ){
			return true;
		}
		std::vector<std::string> s_arr;
		Split(line," ",s_arr);
		if(s_arr.size() == 3){
			req_ = new Request(s_arr[0],s_arr[2]);
			if( !req_->SetUrl(s_arr[1]) ){
				LOG(WARN) << "set url fail";
				return false;
			}
			req_->GetURL().Scheme = "http";
			LOG(INFO) << s_arr[0] << "   " << s_arr[1] << "  " << s_arr[2];
			LOG(INFO) << req_->GetURL().Scheme << " "
					<<req_->GetURL().Fragment << " "
					<<  req_->GetURL().Opaque << " "
					<<  req_->GetURL().Path << " "
					<<  req_->GetURL().RawQuery << " ";
			state_ = STATE_HEAD;
		}else{
			LOG(WARN) << "parse first head fail";
			return false;
		}
	}
	if(state_ == STATE_HEAD){
		while(GetLine(buf,line)){
			if(line == ""){
				std::string val;
				req_->GetHeader().Get(header::HOST,req_->GetURL().Host);
				LOG(INFO) <<req_->GetURL().Host;
				if(req_->GetHeader().Get(header::CONTENT_LENGTH,val)){
					len_ = atoi(val.c_str());
					if(len_ >0){
						this->state_ = STATE_BODY;
					}else{
						this->state_ = STATE_COMPLETE;
					}
				}else{
					this->state_ = STATE_COMPLETE;
				}
			}
			int pos = line.find(":");
			if(pos == std::string::npos){
				continue;
			}
			req_->GetHeader().Set(line.substr(0,pos),line.substr(pos+2));
		}

	}
	if(state_ == STATE_BODY){
		if(buf.Size() >= len_){
			this->state_ = STATE_COMPLETE;
		}
	}
	return true;
}

}}
