INCLUDES=-I./include

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