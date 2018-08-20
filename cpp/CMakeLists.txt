cmake_minimum_required(VERSION 2.6)
project(speech-demo)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/lib/linux)

add_executable(srDemo demo/speechRecognizerDemo.cpp)

target_link_libraries(srDemo nlsCppSdk ssl crypto opus uuid pthread jsoncpp)

add_executable(syDemo demo/speechSynthesizerDemo.cpp)

target_link_libraries(syDemo nlsCppSdk ssl crypto opus uuid pthread jsoncpp)

add_executable(stDemo demo/speechTranscriberDemo.cpp)

target_link_libraries(stDemo nlsCppSdk ssl crypto opus uuid pthread jsoncpp)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/demo) 

