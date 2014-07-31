#ifndef __ABB_HTTP_CLIENT_HPP__
#define __ABB_HTTP_CLIENT_HPP__

#include <string>
namespace abb{
namespace http{

bool POST(std::string& url,const std::string& body_type,void* body,int size);
bool GET(std::string& url,const std::string& body_type,void* body,int size);
bool Do(Request req);
}}
#endif