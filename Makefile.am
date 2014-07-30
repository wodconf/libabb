INCLUDES=-I./include -I./src

AM_LDFLAGS=-fPIC
AM_CPPFLAGS= -fPIC 
#-march=i686
lib_LTLIBRARIES=libabb.la

libabb_la_SOURCES=src/abb/base/buffer.cpp\
			src/abb/base/log.cpp\
			src/abb/base/thread_pool.cpp\
			src/abb/net/socket.cpp\
			src/abb/net/acceptor.cpp\
			src/abb/net/connector.cpp\
			src/abb/net/connection.cpp\
			src/abb/net/event_loop_group.cpp\
			src/abb/net/event_loop.cpp\
			src/abb/net/poller.cpp\
			src/abb/net/ip_addr.cpp\
			src/abb/net/singler.cpp\
			src/abb/net/tcp_server.cpp\
			src/abb/net/connection_ref.cpp\
			src/abb/net/tcp_client.cpp

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
						$(top_srcdir)/include/abb/net/event_loop_group.hpp\
						$(top_srcdir)/include/abb/net/event_handler.hpp\
						$(top_srcdir)/include/abb/net/event_loop.hpp\
						$(top_srcdir)/include/abb/net/io_event.hpp\
						$(top_srcdir)/include/abb/net/ip_addr.hpp\
						$(top_srcdir)/include/abb/net/listener.hpp\
						$(top_srcdir)/include/abb/net/socket.hpp\
						$(top_srcdir)/include/abb/net/tcp_client.hpp\
						$(top_srcdir)/include/abb/net/tcp_server.hpp
						