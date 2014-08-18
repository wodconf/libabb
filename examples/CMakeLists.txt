ADD_EXECUTABLE(echo_client echo_client.cpp)
TARGET_LINK_LIBRARIES(echo_client libabb.so pthread rt)

ADD_EXECUTABLE(echo_server echo_server.cpp)
TARGET_LINK_LIBRARIES(echo_server libabb.a pthread rt)


ADD_EXECUTABLE(http_hello_client http_hello_client.cpp)
TARGET_LINK_LIBRARIES(http_hello_client libabb.a pthread rt )

ADD_EXECUTABLE(http_hello_service http_hello_service.cpp)
TARGET_LINK_LIBRARIES(http_hello_service libabb.a pthread rt)