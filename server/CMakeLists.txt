cmake_minimum_required (VERSION 2.8)

project ("Server")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	#set(warnings "-Wall -Wextra")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread") #tal vez incluir pthread como las demas, no hardcodeada

# "-Wl,--as-needed" hace que solo se linkee con librerias que se usan en el ejecutable"
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed -L/usr/lib")

#SERVER
file(GLOB_RECURSE _SOURCES_CPP_SERVER "src/*.cpp")
list(REMOVE_ITEM _SOURCES_CPP_SERVER "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
file(GLOB_RECURSE _HEADERS_SERVER "src/*.h")

set(_INCLUDE_DIRS_SERVER "")
foreach(_headerFile ${_HEADERS_SERVER})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS_SERVER ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS_SERVER)
include_directories(${_INCLUDE_DIRS_SERVER})

# includes .h
include_directories("include")
add_definitions(-DNS_ENABLE_THREADS)

add_library(ServerLib STATIC ${_SOURCES_CPP_SERVER})

# RocksDB
#include_directories(RocksDB)
add_library(RocksDB STATIC IMPORTED)
set_property(TARGET RocksDB PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/RocksDB/librocksdb.a")

# Json
#include_directories(Json)
add_library(Json STATIC IMPORTED)
set_property(TARGET Json PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Json/libjson.a")

# Mongoose
#include_directories(Mongoose)
add_library(Mongoose STATIC IMPORTED)
set_property(TARGET Mongoose PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Mongoose/libmongoose.a")

# Zlib
#include_directories(Zlib)
add_library(Zlib STATIC IMPORTED)
set_property(TARGET Zlib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Zlib/libz.a")

# Snappy
#include_directories(Snappy)
add_library(Snappy STATIC IMPORTED)
set_property(TARGET Snappy PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Snappy/libsnappy.a")

# Bz2
#include_directories(Bz2)
add_library(Bz2 STATIC IMPORTED)
set_property(TARGET Bz2 PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Bz2/libbz2.a")

#ssl
add_library(SSL STATIC IMPORTED)
set_property(TARGET SSL PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Openssl/libssl.a")

#crypto
add_library(CRYPTO STATIC IMPORTED)
set_property(TARGET CRYPTO PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Openssl/libcrypto.a")

add_executable(Server src/main.cpp)
target_link_libraries(Server ServerLib RocksDB Zlib Snappy Bz2 Json Mongoose SSL CRYPTO -lrt)


#TESTS

# GTest y GMock
add_library(GMock STATIC IMPORTED)
set_property(TARGET GMock PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/GMock/libgmock.a")

add_library(GMock_main STATIC IMPORTED)
set_property(TARGET GMock_main PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/GMock/libgmock_main.a")

set(_dir "${CMAKE_CURRENT_BINARY_DIR}/unit_tests/")
file(MAKE_DIRECTORY ${_dir})

add_subdirectory(unit_tests unit_tests)
