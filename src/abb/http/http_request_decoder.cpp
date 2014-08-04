#include "http_request_decoder.hpp"


namespace abb{
namespace http{	

	int FindCRLF(char* buf,int size){
		for(int i=0;i<size-1;i++){
			if(buf[i] = '\r' && buf[i+1] = '\n'){
				return i;
			}
		}
		return -1;
	}
	void Split(const std::string&str,const std::string& sep,std::vector<std::string>& s_arr){
		int pre_pos = 0;
		while(true){
			int pos = str.find(sep,pre_pos);
			if( pos != std::string::npos ){
				pre_pos = pos+1;
				s_arr.push_back(str.substr(0,pos));
			}else{
				s_arr.push_back(str.substr(pre_pos));
				break;
			}
		}
	}
	RequestDecoder::RequestDecoder(){

	}
	RequestDecoder::~RequestDecoder(){

	}
	bool GetLine(base::Buffer& buf,std::string& str){
		int sz = FindCRLF(buf.Data(),buf.Size());
		if(sz < 0){
			return false;
		}
		str = std::string(buf.Data(),sz);
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
					return false;
				}
				req_->URL().Scheme = "http";
			}else{
				return false;
			}
		}
		if(state_ == STATE_HEAD){
			while(GetLine(buf,line)){
				if(line == ""){
					std::string val;
					req_->Header().Get(header::HOST,req_->URL().Host);
					if(req_->Header().Get(header::CONTENT_LENGTH,val)){
						len_ = atoi(val.c_str());
						if(len_ >0){
							this->state_ == STATE_BODY;
						}else{
							this->state_ == STATE_COMPLETE;
						}
					}
				}
				int pos = line.find(":");
				if(pos == std::string::npos){
					continue;
				}
				req_->Header().Set(str.substr(0,pos),str.substr(pos+2));
			}
			
		}
		if(state_ == STATE_BODY){
			if(buf.Size() >= len_){
				this->state_ == STATE_COMPLETE;
			}
		}
		return true;
	}

}}