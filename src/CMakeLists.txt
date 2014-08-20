SET( ABB_SRC
	abb/base/buffer.cpp
	abb/base/log.cpp
	abb/base/signal.cpp
	abb/base/thread.cpp
	abb/base/thread_pool.cpp
	abb/net/socket.cpp
	abb/net/acceptor.cpp
	abb/net/connector.cpp
	abb/net/connection.cpp
	abb/net/event_loop_group.cpp
	abb/net/event_loop.cpp
	abb/net/poller.cpp
	abb/net/timer_set.cpp
	abb/net/event_io.cpp
	abb/net/ip_addr.cpp
	abb/net/singler.cpp
	abb/net/tcp_server.cpp
	abb/net/connection_ref.cpp
	abb/net/tcp_client.cpp
	abb/http/http_header.cpp
	abb/http/http_request.cpp
	abb/http/http_responce.cpp
	abb/http/http_responce_writer.cpp
	abb/http/http_decoder.cpp
	abb/http/http_server.cpp
	abb/http/http_client.cpp
	abb/url/url.cpp
	abb/base/date.cpp
	)

ADD_LIBRARY(abb SHARED ${ABB_SRC})

ADD_LIBRARY(abb_static STATIC ${ABB_SRC})
SET_TARGET_PROPERTIES(abb_static PROPERTIES OUTPUT_NAME "abb")
SET_TARGET_PROPERTIES(abb PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(abb_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)



TARGET_LINK_LIBRARIES(abb pthread;rt)
TARGET_LINK_LIBRARIES(abb_static pthread;rt)


INSTALL( TARGETS abb;abb_static
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
)
