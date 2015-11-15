cmake_minimum_required (VERSION 2.8)

project ("Tests")

#TESTS

file(GLOB_RECURSE _SOURCES_CPP_TESTS "*.cpp")

	
add_executable(Test ${_SOURCES_CPP_TESTS})
# linkeo con todo por las dudas, total con "-Wl,--as-needed" linkeo solo con las que uso
target_link_libraries(Test ServerLib RocksDB Zlib Snappy Bz2 Json Mongoose GMock GMock_main SSL CRYPTO -lrt)

