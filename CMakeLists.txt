cmake_minimum_required(VERSION 2.8.3)

project(zmq-communication)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -pthread ")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# top-level targets to depend on
add_custom_target(build_libraries)
add_custom_target(copy_headers)

#########
# libzmq 
#########
add_custom_target(TGT_libzmq_build_libraries)
add_custom_target(TGT_libzmq_copy_headers)

set(LIBZMQ_DIR            "${PROJECT_SOURCE_DIR}/contrib/libzmq")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_BUILD_TYPE      Release)

set(ZMQ_BUILD_TESTS OFF CACHE BOOL "Build the tests for ZeroMQ")
add_subdirectory("${LIBZMQ_DIR}" EXCLUDE_FROM_ALL)

set_property(TARGET libzmq-static APPEND_STRING PROPERTY COMPILE_FLAGS " -fPIC ")

add_dependencies(TGT_libzmq_build_libraries libzmq-static)

add_custom_target(TGT_copy_libzmq_headers COMMAND ${CMAKE_COMMAND} -E
                  copy_directory "${LIBZMQ_DIR}/include" "${PROJECT_BINARY_DIR}/include/libzmq")
include_directories(SYSTEM "${PROJECT_BINARY_DIR}/include/libzmq")
add_dependencies(TGT_libzmq_copy_headers TGT_copy_libzmq_headers)

#depend on top-level target
add_dependencies(build_libraries TGT_libzmq_build_libraries)
add_dependencies(copy_headers TGT_libzmq_copy_headers)

#########
# cppzmq 
#########
add_custom_target(TGT_cppzmq_build_libraries)
add_custom_target(TGT_cppzmq_copy_headers)

set(CPPZMQ_DIR            "${PROJECT_SOURCE_DIR}/contrib/cppzmq")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_BUILD_TYPE      Release)

add_subdirectory("${CPPZMQ_DIR}" EXCLUDE_FROM_ALL)

add_dependencies(TGT_cppzmq_build_libraries cppzmq-static)

add_custom_target(TGT_copy_cppzmq_headers COMMAND ${CMAKE_COMMAND} -E
                  copy_directory "${CPPZMQ_DIR}" "${PROJECT_BINARY_DIR}/include/cppzmq")
include_directories(SYSTEM "${PROJECT_BINARY_DIR}/include/cppzmq")
add_dependencies(TGT_cppzmq_copy_headers TGT_copy_cppzmq_headers)

#depend on top-level target
add_dependencies(build_libraries TGT_cppzmq_build_libraries)
add_dependencies(copy_headers TGT_cppzmq_copy_headers)

##########
# msgpack 
##########
add_custom_target(TGT_msgpack_build_libraries)
add_custom_target(TGT_msgpack_copy_headers)

set(MSGPACK_DIR           "${PROJECT_SOURCE_DIR}/contrib/msgpack-c")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_BUILD_TYPE      Release)

add_subdirectory("${MSGPACK_DIR}" EXCLUDE_FROM_ALL)

set_property(TARGET msgpackc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -fPIC ")

add_dependencies(TGT_msgpack_build_libraries msgpackc-static)

add_custom_target(TGT_copy_msgpack_headers COMMAND ${CMAKE_COMMAND} -E
                  copy_directory "${MSGPACK_DIR}/include" "${PROJECT_BINARY_DIR}/include/msgpack")
include_directories(SYSTEM "${PROJECT_BINARY_DIR}/include/msgpack")
add_dependencies(TGT_msgpack_copy_headers TGT_copy_msgpack_headers)

#depend on top-level target
add_dependencies(build_libraries TGT_msgpack_build_libraries)
add_dependencies(copy_headers TGT_msgpack_copy_headers)

########
## main 
########

set(LIBS_TO_LINK_WITH "libzmq-static" "cppzmq-static" "msgpackc-static")

add_subdirectory(src)
