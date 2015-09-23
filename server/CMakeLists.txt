cmake_minimum_required (VERSION 2.8)

project ("Server")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(warnings "-Wall -Wextra")
endif()
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread") #tal vez incluir pthread como las demas, no hardcodeada

# "-Wl,--as-needed" hace que solo se linkee con librerias que se usan en el ejecutable"
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")

#SERVER
file(GLOB_RECURSE _SOURCES_CPP_SERVER "src/*.cpp")
file(GLOB_RECURSE _SOURCES_C_SERVER "src/*.c")
file(GLOB_RECURSE _HEADERS_SERVER "src/*.h")

set(_INCLUDE_DIRS_SERVER "")
foreach(_headerFile ${_HEADERS_SERVER})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS_SERVER ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS_SERVER)
include_directories(${_INCLUDE_DIRS_SERVER})

add_executable(Server ${_SOURCES_CPP_SERVER} ${_SOURCES_C_SERVER})

# includes .h
include_directories("include")

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

target_link_libraries(Server RocksDB Zlib Snappy Bz2 Json Mongoose)
#target_link_libraries(Server RocksDB Json Mongoose)


#TESTS

# GTest - no linkeo con libgtest_main.a, no se si es necesario
add_library(GTest STATIC IMPORTED)
set_property(TARGET GTest PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/GTest/libgtest.a")

# GMock - no linkeo con libgmock_main.a, no se si es necesario
add_library(GMock STATIC IMPORTED)
set_property(TARGET GMock PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/GMock/libgmock.a")

file(GLOB_RECURSE _SOURCES_CPP_TESTS "tests/*.cpp")
set(_dir "${CMAKE_CURRENT_BINARY_DIR}/tests/")
file(MAKE_DIRECTORY ${_dir})

set(_SOURCES_CPP_CON_TESTS "")
foreach(_testSource ${_SOURCES_CPP_TESTS}) # por cada test en tests/ compilo un ejecutable con todo el codigo del server
	set(_SOURCES_CPP_SERVER_SIN_MAIN ${_SOURCES_CPP_SERVER})
	list(REMOVE_ITEM _SOURCES_CPP_SERVER_SIN_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
	get_filename_component(_test_name ${_testSource} NAME_WE)	
	set(_test_name "tests/${_test_name}")
	add_executable(${_test_name} ${_testSource} ${_SOURCES_CPP_SERVER_SIN_MAIN} ${_SOURCES_C_SERVER_SIN_MAIN})
	# linkeo con todo por las dudas, total con "-Wl,--as-needed" linkeo solo con las que uso
	target_link_libraries(${_test_name} RocksDB Zlib Snappy Bz2 Json Mongoose GTest GMock)
endforeach()
