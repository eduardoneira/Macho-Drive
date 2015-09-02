cmake_minimum_required (VERSION 2.8)

project ("Test")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(warnings "-Wall -Wextra")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread") #tal vez incluir pthread como las demas, no hardcodeada

file(GLOB_RECURSE _SOURCES_CPP "src/*.cpp")
file(GLOB_RECURSE _SOURCES_C "src/*.c")
file(GLOB_RECURSE _HEADERS "src/*.h")

set(_INCLUDE_DIRS "")
foreach(_headerFile ${_HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS)

include_directories(${_INCLUDE_DIRS})

add_executable(Test ${_SOURCES_CPP} ${_SOURCES_C})

# includes .h
include_directories("include")

# RocksDB
include_directories(RocksDB)
add_library(RocksDB STATIC IMPORTED)
set_property(TARGET RocksDB PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/RocksDB/librocksdb.a")

# Zlib
include_directories(Zlib)
add_library(Zlib STATIC IMPORTED)
set_property(TARGET Zlib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Zlib/libz.a")

# Snappy
include_directories(Snappy)
add_library(Snappy STATIC IMPORTED)
set_property(TARGET Snappy PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Snappy/libsnappy.a")

# Bz2
include_directories(Bz2)
add_library(Bz2 STATIC IMPORTED)
set_property(TARGET Bz2 PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Bz2/libbz2.a")

target_link_libraries(Test RocksDB Zlib Snappy Bz2)
