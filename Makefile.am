INCLUDES=-I./include -I./src

AM_LDFLAGS=-fPIC
AM_CPPFLAGS= -fPIC 
#-march=i686
lib_LTLIBRARIES=libabb.la

libabb_la_SOURCES=src/abb/base/buffer.cpp\
			src/abb/base/log.cpp\
			src/abb/base/signal.cpp\
			src/abb/base/thread.cpp\
			src/abb/base/thread_pool.cpp\
			src/abb/net/socket.cpp\
			src/abb/net/acceptor.cpp\
			src/abb/net/connector.cpp\
			src/abb/net/connection.cpp\
			src/abb/net/event_loop_group.cpp\
			src/abb/net/event_loop.cpp\
			src/abb/net/poller.cpp\
			src/abb/net/timer_set.cpp\
			src/abb/net/event_io.cpp\
			src/abb/net/ip_addr.cpp\
			src/abb/net/singler.cpp\
			src/abb/net/tcp_server.cpp\
			src/abb/net/connection_ref.cpp\
			src/abb/net/tcp_client.cpp\
			src/abb/http/http_header.cpp\
			src/abb/http/http_request.cpp\
			src/abb/http/http_responce.cpp\
			src/abb/http/http_responce_writer.cpp\
			src/abb/http/http_decoder.cpp\
			src/abb/http/http_server.cpp\
			src/abb/http/http_client.cpp\
			src/abb/url/url.cpp

export INCLUDES
libabb_la_LIBADD=-lpthread -lrt

bin_PROGRAMS = echo_server
echo_server_LDADD = -labb
echo_server_SOURCES = examples/echo_server.cpp

bin_PROGRAMS += http_hello_service
http_hello_service_LDADD = -labb
http_hello_service_SOURCES = examples/http_hello_service.cpp

bin_PROGRAMS += http_hello_client
http_hello_client_LDADD = -labb
http_hello_client_SOURCES = examples/http_hello_client.cpp

bin_PROGRAMS += echo_client
echo_client_LDADD = -labb
echo_client_SOURCES = examples/echo_client.cpp


#EXTRA_DIST=./include
baseincludedir=$(includedir)/abb/base
baseinclude_HEADERS=$(top_srcdir)/include/abb/base/buffer.hpp\
					$(top_srcdir)/include/abb/base/callback.hpp\
					$(top_srcdir)/include/abb/base/define.hpp\
					$(top_srcdir)/include/abb/base/log.hpp\
					$(top_srcdir)/include/abb/base/ref_object.hpp\
					$(top_srcdir)/include/abb/base/thread_pool.hpp\
					$(top_srcdir)/include/abb/base/signal.hpp\
					$(top_srcdir)/include/abb/base/thread.hpp\
					$(top_srcdir)/include/abb/base/block_queue.hpp

netincludedir=$(includedir)/abb/net
netinclude_HEADERS=$(top_srcdir)/include/abb/net/connection_ref.hpp\
					$(top_srcdir)/include/abb/net/event_loop_group.hpp\
					$(top_srcdir)/include/abb/net/event_handler.hpp\
					$(top_srcdir)/include/abb/net/event_loop.hpp\
					$(top_srcdir)/include/abb/net/event_io.hpp\
					$(top_srcdir)/include/abb/net/ip_addr.hpp\
					$(top_srcdir)/include/abb/net/listener.hpp\
					$(top_srcdir)/include/abb/net/socket.hpp\
					$(top_srcdir)/include/abb/net/tcp_client.hpp\
					$(top_srcdir)/include/abb/net/tcp_server.hpp
httpincludedir=$(includedir)/abb/http
httpinclude_HEADERS=$(top_srcdir)/include/abb/http/http_client.hpp\
					$(top_srcdir)/include/abb/http/http_const.hpp\
					$(top_srcdir)/include/abb/http/http_header.hpp\
					$(top_srcdir)/include/abb/http/http_responce.hpp\
					$(top_srcdir)/include/abb/http/http_request.hpp\
					$(top_srcdir)/include/abb/http/http_responce_writer.hpp\
					$(top_srcdir)/include/abb/http/http_server.hpp

urlincludedir=$(includedir)/abb/url
urlinclude_HEADERS=$(top_srcdir)/include/abb/url/url.hpp