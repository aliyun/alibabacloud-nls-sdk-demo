cmake_minimum_required(VERSION 2.4)
project(speech-demo)

if(UNIX)
    message("this operation platform is UNIX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI -lpthread -D_GNU_ -ldl")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/openssl)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(speechRecognizerFileDemo demo/speechRecognizerFileDemo.cpp)

if(UNIX)
	target_link_libraries(speechRecognizerFileDemo
				${CMAKE_SOURCE_DIR}/lib/linux/libnlscppsdk.so
				${CMAKE_SOURCE_DIR}/lib/linux/libssl.so 
				${CMAKE_SOURCE_DIR}/lib/linux/libcrypto.so
				${CMAKE_SOURCE_DIR}/lib/linux/libopus.so)
endif()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/demo) 

