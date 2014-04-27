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
			src/abb.cpp

libabb_la_LIBADD=-lpthread
