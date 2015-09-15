cmake_minimum_required (VERSION 2.8)

project ("Test")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(warnings "-Wall -Wextra")
endif()
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread") #tal vez incluir pthread como las demas, no hardcodeada

#COMMON
file(GLOB_RECURSE _SOURCES_CPP_COMMON "src/common/*.cpp")
file(GLOB_RECURSE _SOURCES_C_COMMON "src/common/*.c")
file(GLOB_RECURSE _HEADERS_COMMON "src/common/*.h")

set(_INCLUDE_DIRS_COMMON "")
foreach(_headerFile ${_HEADERS_COMMON})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS_COMMON ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS_COMMON)
include_directories(${_INCLUDE_DIRS_COMMON})

#SERVER
file(GLOB_RECURSE _SOURCES_CPP_SERVER "src/server/*.cpp")
file(GLOB_RECURSE _SOURCES_C_SERVER "src/server/*.c")
file(GLOB_RECURSE _HEADERS_SERVER "src/server/*.h")

set(_INCLUDE_DIRS_SERVER "")
foreach(_headerFile ${_HEADERS_SERVER})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS_SERVER ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS_SERVER)
include_directories(${_INCLUDE_DIRS_SERVER})

add_executable(Server ${_SOURCES_CPP_SERVER} ${_SOURCES_C_SERVER} ${_SOURCES_CPP_COMMON} ${_SOURCES_C_COMMON})

# includes .h
include_directories("include")

# RocksDB
include_directories(RocksDB)
add_library(RocksDB STATIC IMPORTED)
set_property(TARGET RocksDB PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/RocksDB/librocksdb.a")

# Json
include_directories(Json)
add_library(Json STATIC IMPORTED)
set_property(TARGET Json PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/Json/libjson.a")

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

target_link_libraries(Server RocksDB Zlib Snappy Bz2 Json)

#CLIENT
file(GLOB_RECURSE _SOURCES_CPP_CLIENT "src/client/*.cpp")
file(GLOB_RECURSE _SOURCES_C_CLIENT "src/client/*.c")
file(GLOB_RECURSE _HEADERS_CLIENT "src/client/*.h")

set(_INCLUDE_DIRS_CLIENT "")
foreach(_headerFile ${_HEADERS_CLIENT})
	get_filename_component(_dir ${_headerFile} PATH)
	list(APPEND _INCLUDE_DIRS_CLIENT ${_dir})
endforeach()
list(REMOVE_DUPLICATES _INCLUDE_DIRS_CLIENT)
include_directories(${_INCLUDE_DIRS_CLIENT})

add_executable(Client ${_SOURCES_CPP_CLIENT} ${_SOURCES_C_CLIENT} ${_SOURCES_CPP_COMMON} ${_SOURCES_C_COMMON})
