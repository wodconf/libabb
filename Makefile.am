INCLUDES=-I./include

AM_LDFLAGS=-fPIC
AM_CPPFLAGS= -fPIC 
#-march=i686
lib_LTLIBRARIES=libabb.la

libabb_la_SOURCES=src/base/buffer.cpp\
			src/base/log.cpp\
			src/base/thread_pool.cpp\
			src/net/socket.cpp\
			src/net/acceptor.cpp\
			src/net/connector.cpp\
			src/net/connection.cpp\
			src/net/context.cpp\
			src/net/poller.cpp\
			src/net/loop.cpp\
			src/net/ip_addr.cpp\
			src/net/singler.cpp\
			src/net/tcp_server.cpp\
			src/net/connection_ref.cpp\
			src/net/tcp_client.cpp
export INCLUDES
libabb_la_LIBADD=-lpthread

bin_PROGRAMS = echo_server
echo_server_LDADD = -labb
echo_server_SOURCES = examples/echo_server.cpp

bin_PROGRAMS += echo_client
echo_client_LDADD = -labb
echo_client_SOURCES = examples/echo_client.cpp


#EXTRA_DIST=./include
baseincludedir=$(includedir)/abb/base
baseinclude_HEADERS=$(top_srcdir)/include/abb/base/buffer.hpp\
						$(top_srcdir)/include/abb/base/callback.hpp\
						$(top_srcdir)/include/abb/base/cond.hpp\
						$(top_srcdir)/include/abb/base/define.hpp\
						$(top_srcdir)/include/abb/base/log.hpp\
						$(top_srcdir)/include/abb/base/mutex.hpp\
						$(top_srcdir)/include/abb/base/ref_object.hpp\
						$(top_srcdir)/include/abb/base/rw_lock.hpp\
						$(top_srcdir)/include/abb/base/thread_pool.hpp

netincludedir=$(includedir)/abb/net
netinclude_HEADERS=$(top_srcdir)/include/abb/net/connection_ref.hpp\
						$(top_srcdir)/include/abb/net/context.hpp\
						$(top_srcdir)/include/abb/net/ip_addr.hpp\
						$(top_srcdir)/include/abb/net/listener.hpp\
						$(top_srcdir)/include/abb/net/socket.hpp\
						$(top_srcdir)/include/abb/net/tcp_client.hpp\
						$(top_srcdir)/include/abb/net/tcp_server.hpp