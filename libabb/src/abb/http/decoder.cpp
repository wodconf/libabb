#include "abb/http/decoder.hpp"
#include "abb/http/const.hpp"
#include "abb/base/log.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
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
	std::string::size_type pre_pos = 0;
	while(true){
		std::string::size_type pos = str.find(sep,pre_pos);
		if( pos != std::string::npos ){

			s_arr.push_back(str.substr(pre_pos,pos-pre_pos));
			pre_pos = pos+1;
		}else{
			s_arr.push_back(str.substr(pre_pos));
			break;
		}
	}
}
static bool GetLine(Buffer& buf,std::string& str){
	int sz = FindCRLF((char*)buf.ReadPtr(),buf.ReadSize());
	if(sz < 0){
		return false;
	}
	if(sz == 0) str = "";
	else str = std::string((char*)buf.ReadPtr(),sz);
	buf.GaveRead(sz+2);
	return true;
}
RequestDecoder::RequestDecoder():req_(NULL),state_(FIRST_HEAD),len_(0){

}
RequestDecoder::~RequestDecoder(){

}
bool RequestDecoder::Decode(Buffer& buf){
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
			state_ = STATE_HEAD;
		}else{
			LOG(WARN) << "parse first head fail";
			return false;
		}
	}
	if(state_ == STATE_HEAD){
		while(GetLine(buf,line)){ 
			if(line.empty()){
				std::string val;
				req_->GetHeader().Get(header::HOST,req_->GetURL().Host);
				if(req_->GetHeader().Get(header::CONTENT_LENGTH,val)){
					len_ = atoi(val.c_str());
					if(len_ >0){
						this->state_ = STATE_BODY;
					}else{
						this->state_ = STATE_COMPLETE;
					}
				}else if(req_->GetHeader().Get(header::TRANSFER_ENCODING,val)){
					this->state_ = STATE_CHUNK_SIZE;
				}else{
					this->state_ = STATE_COMPLETE;
				}
				break;
			}
			std::string::size_type pos = line.find(":");
			if(pos == std::string::npos){
				continue;
			}
			if(line.size() > pos + 2){
				req_->GetHeader().Set(line.substr(0,pos),line.substr(pos+2));
			}else{
				return false;
			}
			
		}

	}
	if(state_ == STATE_BODY){
		if((int)buf.ReadSize() >= len_){
			req_->Body().Write(buf.ReadPtr(),len_);
			buf.GaveRead(len_);
			this->state_ = STATE_COMPLETE;
		}
	}
	while(state_ == STATE_CHUNK_SIZE || state_ == STATE_CHUNK_BODY){
		if(state_ == STATE_CHUNK_SIZE){
			if( GetLine(buf,line) ){
				if(line.empty()){
					chunk_len_ = 0;
				}else{
					std::istringstream in(line);
					in >> std::hex >> chunk_len_;
				}
				if(chunk_len_ == 0){
					this->state_ = STATE_COMPLETE;
					break;
				}
				state_ = STATE_CHUNK_BODY;
			}else{
				break;
			}
		}
		if(state_ == STATE_CHUNK_BODY){
			if((int)buf.ReadSize() >= chunk_len_){
				req_->Body().Write(buf.ReadPtr(),chunk_len_);
				buf.GaveRead(chunk_len_);
				state_ = STATE_CHUNK_SIZE;
			}
		}
	}
	return true;
}

ResponceDecoder::ResponceDecoder():rsp_(NULL),state_(FIRST_HEAD),len_(0){

}
ResponceDecoder::~ResponceDecoder(){

}
bool ResponceDecoder::Decode(Buffer& buf){
	std::string line;
	if(state_ == FIRST_HEAD){
		if(! GetLine(buf,line) ){
			return true;
		}
		std::vector<std::string> s_arr;
		Split(line," ",s_arr);
		if(s_arr.size() < 2){
			LOG(WARN) << "parse first head fail";
			return false;
		}else{
			std::string reasonPhrase="";
			if(s_arr.size() >= 3){
				reasonPhrase = s_arr[2];
			}
			rsp_ = new Responce(s_arr[0]);
			rsp_->SetStatusCode(atoi(s_arr[1].c_str()));
			state_ = STATE_HEAD;
		}
	}
	if(state_ == STATE_HEAD){
		while(GetLine(buf,line)){
			if(line == ""){
				std::string val;
				if(rsp_->GetHeader().Get(header::CONTENT_LENGTH,val)){
					len_ = atoi(val.c_str());
					if(len_ >0){
						this->state_ = STATE_BODY;
					}else{
						this->state_ = STATE_COMPLETE;
					}
				}else if(rsp_->GetHeader().Get(header::TRANSFER_ENCODING,val)){
					this->state_ = STATE_CHUNK_SIZE;
				}else{
					this->state_ = STATE_COMPLETE;
				}
				break;
			}
			std::string::size_type pos = line.find(":");
			if(pos == std::string::npos){
				continue;
			}
			if(line.size() > pos + 2){
				rsp_->GetHeader().Set(line.substr(0,pos),line.substr(pos+2));
			}else{
				return false;
			}
		}

	}
	if(state_ == STATE_BODY){
		if((int)buf.ReadSize() >= len_){
			rsp_->Body().Write(buf.ReadPtr(),len_);
			buf.GaveRead(len_);
			this->state_ = STATE_COMPLETE;
		}
	}
	while(state_ == STATE_CHUNK_SIZE || state_ == STATE_CHUNK_BODY){
		if(state_ == STATE_CHUNK_SIZE){
			if( GetLine(buf,line) ){
				if(line.empty()){
					chunk_len_ = 0;
				}else{
					std::istringstream in(line);
					in >> std::hex >> chunk_len_;
				}
				if(chunk_len_ == 0){
					this->state_ = STATE_COMPLETE;
					break;
				}
				state_ = STATE_CHUNK_BODY;
			}else{
				break;
			}
		}
		if(state_ == STATE_CHUNK_BODY){
			if((int)buf.ReadSize() >= chunk_len_){
				rsp_->Body().Write(buf.ReadPtr(),chunk_len_);
				buf.GaveRead(chunk_len_);
				state_ = STATE_CHUNK_SIZE;
			}
		}
	}
	
	return true;
}

}}
