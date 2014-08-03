#ifndef __ABB_NET_HTTP_RESPONCE_HPP__
#define __ABB_NET_HTTP_RESPONCE_HPP__

#include <string>
#include <abb/base/buffer.hpp>
#include <abb/url/url.hpp>
#include <abb/http/http_header.hpp>
#include "abb/base/ref_object.hpp"

namespace abb
{
	namespace http
	{
		class Responce
		{
		public:
			Responce();
			~Responce();
			void SetStatusCode(int code);
		private:
			Header header_;
			int code_;
			int content_length_;
		};
	}
}

#endif
