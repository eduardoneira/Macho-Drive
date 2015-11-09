cmake_minimum_required (VERSION 2.8)

project ("Tests")

#TESTS

file(GLOB_RECURSE _SOURCES_CPP_TESTS "*.cpp")

set(_SOURCES_CPP_CON_TESTS "")
foreach(_testSource ${_SOURCES_CPP_TESTS}) # por cada test en unit tests/ compilo un ejecutable con todo el codigo del server
	get_filename_component(_test_name ${_testSource} NAME_WE)	
	add_executable(${_test_name} ${_testSource})
	# linkeo con todo por las dudas, total con "-Wl,--as-needed" linkeo solo con las que uso
	target_link_libraries(${_test_name} ServerLib RocksDB Zlib Snappy Bz2 Json Mongoose GMock GMock_main SSL CRYPTO)
endforeach()